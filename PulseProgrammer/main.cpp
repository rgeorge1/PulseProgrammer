//
//  main.cpp
//  PulseProgrammer
//
//  Created by Richard George on 08/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <ctime>

#include "AWG.h"
#include "Sequence.h"
#include "PulseBuffer.h"
#include "StefPulseProgram.h"

extern bool verbose;
extern bool change_endian;

extern std::map<int,int> zero_entries;

extern int buffer_count;
extern int merged_buffer_count;

void explainUsage(const char *);
void dumb_message();

void dumb_message()
{
    int number = (int) ((int)time(NULL) % 10);
    
    switch (number) {
                        
        case 1:    
        {
            std::cout << "Caution: Use of this program may inform you about the ultimate nature of reality." << std::endl;
            break;
        }
                        
        case 3:
        {
            std::cout << "Why wait? Dial 1-800-EVERETT now! Speak with a sales representitive in your branch of the wavefunction." << std::endl;      
            break;
        }
            
        default: 
            return;

    }
    
    std::cout << std::endl;
}

void explainUsage(const char *n)
{
    std::cout << "Usage: PulseProgrammer current.txt output.txt output.dat" << std::endl << std::endl;
    std::cout << "Convert current.txt into a segment list `output.txt' and binary data `output.dat'" << std::endl << std::endl;
}

int main (int argc, const char * argv[])
{
    AWG a(600.0,"Agilent81180A");
    
    std::string output_text_file,output_data_file,input_text_file;
    
    int zero_buffer_number;
    int zero_buffer_block_size = a.getChunkyBlockSize();
    
    std::cout << "Richard's Optimising Pulse Compiler (c) 2011" << std::endl;
    std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl << std::endl;
    
    dumb_message();
    
    switch (argc)
    {
        case 1:
            explainUsage(argv[0]);
            
            std::cout << "No arguments supplied! Using defaults." << std::endl << std::endl;
        
            output_text_file = "/Users/richard/Dropbox/Share_StefRich/output.txt";
            output_data_file = "/Users/richard/Dropbox/Share_StefRich/output.dat";
            input_text_file = "/Users/richard/Dropbox/Share_StefRich/current.txt";

            break;
            
        case 4:
            input_text_file = argv[1];
            output_text_file = argv[2];
            output_data_file = argv[3];
            break;
            
        default:
            explainUsage(argv[0]);
            return 1;
            break;
    }
    

    if (change_endian)
    {
        std::cout << "Running with reverse byte order" << std::endl << std::endl;
    }
    else
    {
        std::cout << "Running with normal byte order" << std::endl << std::endl;
    }
    // Report arguments
    
    std::cout << "input_text_file = `" << input_text_file << "'" << std::endl;
    std::cout << "output_text_file = `" << output_text_file << "'" << std::endl;
    std::cout << "output_data_file = `" << output_data_file << "'" << std::endl;
    std::cout << std::endl;
    
    // Parse one of Stef's text files
    
    StefPulseProgram spp(input_text_file.c_str(),2);
    
    // Describe the pulse program briefly

    std::cout << std::endl;    
    std::cout << "Experiment has " << spp.getPointCount() << " point(s) and " << spp.getPhaseCycleStepCount()+1 << " phase cycle step(s)" << std::endl;
    
    // Generate sequence lists for the different points in the experiment
    
    if (verbose) std::cout << "Generating " << spp.getPointCount() << " points" << std::endl;

    int i,j;
    std::map<std::pair<int,int> ,std::vector<PlaylistEntry> > playlists;
    
    for (i=0;i<=spp.getPointCount();i++)
    {
        // Loop over each x-point
        if (verbose) std::cout << "Generating point " << i << " of " << spp.getPointCount() << std::endl;
        
        // Loop over each phase cycle step
        for (j=0;j<=spp.getPhaseCycleStepCount();j++)
        {
            // Generate the sequence list for point (i,j) using AWG a
            std::vector<PlaylistEntry> playlist;
            spp.generatePlaylist(a,i,j,playlist);
    
            // store the result
            playlists[std::pair<int,int>(i,j)].swap(playlist);
        }
    }
    
    // Describe the Playlists we just generated
    /*
    if (verbose)
    {
       std::vector<PlaylistEntry>::iterator it=playlist1.begin();
    
       for (;it!=playlist1.end();it++)
       {
          if ((it->pb)==0)
          {
             std::cout << it->sample_length << " zeros" << std::endl;
          }
          else
          {
             std::cout << "A buffer with " << it->pb->getSampleCount() << " samples" << std::endl;
          } 
       }
    }
    */
    
    // Report the results of hashing the data
    
    std::cout << "(Merged " << merged_buffer_count << " of " << buffer_count << " segments)" << std::endl << std::endl;
    std::cout << "Pulse program now comprises:" << std::endl << std::endl;
    std::cout << " -- " << (buffer_count - merged_buffer_count) << " distinct pulse shape(s)" << std::endl;
    
    std::map<int,int>::iterator it3 = zero_entries.begin();
    for (;it3!=zero_entries.end();it3++)
    {
        std::cout << " -- " << it3->second << " run(s) of " << it3->first << " nil samples (" << ((double)it3->first / (zero_buffer_block_size)) << " blocks)" << std::endl;
    }
    
    std::cout << std::endl;
    
    // Generate an ordered list of the buffers we actually make use of

    // Number the buffers in order of access
    std::cout << "Collecting useful segments" << std::endl << std::endl;
    
    int ordered_buffer_count=0;
    std::map<std::pair<int,int>,std::vector<PlaylistEntry> >::iterator it5 = playlists.begin();
    std::map<std::string,int> ordered_buffers;
    std::map<int,PulseBuffer *> buffer_pointer;
    std::map<int,std::string> buffer_usage;
    
    // For each point in the experiment
    for (;it5!=playlists.end();it5++)
    {
        // Loop over each buffer that will be output
        std::vector<PlaylistEntry> &pl = (*it5).second;
        std::vector<PlaylistEntry>::iterator it6 = pl.begin();
        
        // For each buffer:
        for (;it6!=pl.end();it6++)
        {
            // If the step involves a pulse
            if (it6->pb!=0)
            {
                // Get the hash of the pulse
                std::string this_hash = it6->pb->getBufferHash();
                
                // If it is new, assign it a number
                std::map<std::string,int>::iterator it7 = ordered_buffers.find(this_hash);
                if (it7 == ordered_buffers.end()) 
                {
                    // This is the first time we've seen this hash
                    ordered_buffer_count++;
                    buffer_usage[ordered_buffer_count] = this_hash;
                    buffer_pointer[ordered_buffer_count] = it6->pb;
                    ordered_buffers[this_hash] = ordered_buffer_count;
                }
            }
        }
    }

    // Now store a zero buffer

    zero_buffer_number = ++ordered_buffer_count;
    PulseBuffer *pb_zeros = new PulseBuffer(zero_buffer_block_size);
    pb_zeros->finalise();

    std::string zero_hash = pb_zeros->getBufferHash();
    buffer_usage[zero_buffer_number] = zero_hash;
    buffer_pointer[zero_buffer_number] = pb_zeros;
    ordered_buffers[zero_hash] = zero_buffer_number;

    // We know which buffers to write out, and how to number them
    
    std::cout << "Writing the segment contents to `" << output_data_file << "'" << std::endl;
    
    std::ofstream ofs1(output_data_file.c_str());
        
    // Now we have an ordered list of buffers 
    
    for (i=1;i<=ordered_buffer_count;i++)
    {
        uint8_t *this_buffer;
        int this_buffer_length;
        
        a.prepareBinaryBlock(buffer_pointer[i],&this_buffer,this_buffer_length);
        
        ofs1.write((const char *)this_buffer,this_buffer_length);
        
        
        delete [] this_buffer;
    }
    
    // We write that to a file

    std::cout << "Writing the segment description and sequence tables to `" << output_text_file << "'" << std::endl;
    
    // Write the buffers to the disk
    std::ofstream ofs(output_text_file.c_str());

    ofs << "# This file generated from:" << std::endl;
    ofs << input_text_file << std::endl;
    ofs << "# Binary data describing segments is in:" << std::endl;
    ofs << output_data_file << std::endl;
    ofs << "# Segment description follows:" << std::endl;
    ofs << "# Segment count" << std::endl;
    ofs << ordered_buffer_count << std::endl;
    
    ofs << "# Segment number, start index into binary data file (zero-based), segment length (bytes)" << std::endl;
    
    int buffer_address = 0,this_buffer_length = 0;
    
    for (i=1;i<=ordered_buffer_count;i++)
    {
        this_buffer_length = a.getStorageForBuffer(buffer_pointer[i]);
        ofs << i << "," << buffer_address << "," << this_buffer_length << std::endl;
        buffer_address += this_buffer_length;
    }
    
    ofs << "# Sequence table description follows:" << std::endl;
    ofs << "# number of points, number of phase cycling steps" << std::endl;
    
    ofs << spp.getPointCount() << "," << spp.getPhaseCycleStepCount()+1 << std::endl;
    
    int n=1;
    
    for (i=0;i<=spp.getPointCount();i++)
    {
        for (j=0;j<=spp.getPhaseCycleStepCount();j++)
        {
            std::vector<PlaylistEntry> &playlist = playlists[std::pair<int,int>(i,j)];
            
            ofs << "# Sequence number, point number, phase cycle step number, number of steps in sequence" << std::endl;
            ofs << n << "," << i << "," << j << "," << playlist.size() << std::endl;
            ofs << "# Segment number, repeat count" << std::endl;
            
            std::vector<PlaylistEntry>::iterator it4 = playlist.begin();
            
            for (;it4!=playlist.end();it4++)
            {
                if (it4->pb!=0)
                {
                    ofs << ordered_buffers[it4->pb->getBufferHash()] << ",1" << std::endl; 
                }
                else
                {
                    ofs << zero_buffer_number << "," << (it4->sample_length / zero_buffer_block_size) << std::endl;
                }
            }
            
            n++;
        }
    }
    
    ofs << "# End of file." << std::endl;
    
    std::cout << std::endl << "Done." << std::endl << std::endl;
    
    return 0;
}

