//
//  PulseBuffer.cpp
//  PulseProgrammer
//
//  Created by Richard George on 09/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#include <iostream>

#include "AWG.h"
#include "PulseBuffer.h"
#include "MD5Hash.h"
#include <map>

extern bool verbose;

int buffer_count = 0;
int merged_buffer_count = 0;

std::map<std::string,std::pair<double *,double *> > all_buffers;
std::map<int,bool> buffer_disposition;


PulseBuffer::PulseBuffer(int len)
{
    first_sample = -1;
    buffer_length = len;
    
    I_samples = new double[buffer_length];
    Q_samples = new double[buffer_length];
    
    int i;
    
    // Zero the contents of the buffer
    for (i=0;i<buffer_length;i++) 
    {
        I_samples[i] = 0.0;
        Q_samples[i] = 0.0;
    }
    
    md5_valid = false;
    finalised = false;
    
    this_buffer_number = buffer_count ++;
}


PulseBuffer::PulseBuffer(int fs,int ls)
{
    first_sample = fs;
    buffer_length = ls - fs;
    
    I_samples = new double[buffer_length];
    Q_samples = new double[buffer_length];

    int i;
    
    for (i=0;i<buffer_length;i++) 
    {
        I_samples[i] = 0.0;
        Q_samples[i] = 0.0;
    }
    
    md5_valid = false;
    finalised = false;
    
    this_buffer_number = buffer_count ++;
}

void PulseBuffer::updateBuffers(double *&is,double *&qs,int &fs,int &l)
{   
    if (finalised == false)
    {
        is = I_samples;
        qs = Q_samples;
        l = buffer_length;
        fs = first_sample;
    
        md5_valid = false;
    }
    else
    {
        std::cerr << "Attempting to update a finalised buffer" << std::endl;
        exit(1);
    }
}

void PulseBuffer::getBuffers(const double *&is, const double *&qs,int &fs, int &l) 
{
    if (finalised == false)
    {
        std::cerr << "getBuffers() can only be called on a finalised buffer" << std::endl;
        exit(1);
    }
    
    l = buffer_length;
    fs = first_sample;
    
    std::map<std::string,std::pair<double *,double *> >::const_iterator it = all_buffers.find(our_hash);
    
    if (it!=all_buffers.end() )
    {
        is = (*it).second.first;
        qs = (*it).second.second;
    }
    else
    {
        std::cerr << "getBuffers() could not find the hashed buffer! we should not get here..." << std::endl;
        exit(1);
    }
}

void PulseBuffer::hash()
{
    md5_state_t state;
    
    if (md5_valid == false)
    {
        
        md5_init(&state);
        
        md5_append(&state, (const md5_byte_t *)I_samples, sizeof(double) * buffer_length );
        md5_append(&state, (const md5_byte_t *)Q_samples, sizeof(double) * buffer_length );
        
        md5_finish(&state, md5hash.getDigest() );
            
        md5_valid = true;
        
        our_hash = md5hash.getHash();
    }
}

void PulseBuffer::reportMD5()
{
    if (!md5_valid) hash();
    
    std::cout << "MD5(buffer#" << this_buffer_number <<") = ";
    
    md5hash.reportHash();
    
    std::cout << std::endl << std::endl;

}

void PulseBuffer::finalise()
{
    // Calculate a hash for this buffer
    
    finalised = true;
    hash();
    
    // Check if our hash is globally unique
    
    our_hash = md5hash.getHash();
    std::map<std::string,std::pair<double *,double *> >::const_iterator it = all_buffers.find(our_hash);
    
    if (it == all_buffers.end())
    {
        // This is the first time we have encountered this buffer
        all_buffers[our_hash] = std::pair<double *,double *>(I_samples,Q_samples);
        buffer_disposition[this_buffer_number] = true;
    }
    else
    {
        if (verbose) std::cout << "...oooOOO : We have a buffer collision! Two buffers are identical. We have saved some space : OOOooo..." << std::endl;
        
        // We made a duplicate buffer, so give back the memory now
        dispose_our_buffers();
        buffer_disposition[this_buffer_number] = false;
        merged_buffer_count++;
    }
}

void PulseBuffer::dispose_our_buffers()
{
    if (I_samples!=0) { delete[] I_samples; I_samples = 0; }
    if (Q_samples!=0) { delete[] Q_samples; Q_samples = 0; }
}

PulseBuffer::~PulseBuffer()
{
    dispose_our_buffers();
}

double PulseBuffer::getTimeForIndex(const AWG &a,int idx) const
{
    return a.getTimeForSample(idx + first_sample);
}

void PulseBuffer::clipSamples()
{
    int i;
    
    for (i=0;i<buffer_length;i++)
    {
        I_samples[i] = (I_samples[i] > 1.0) ? 1.0 : ( (I_samples[i]< -1.0 ) ? -1.0 : I_samples[i] );
        Q_samples[i] = (Q_samples[i] > 1.0) ? 1.0 : ( (Q_samples[i]< -1.0 ) ? -1.0 : Q_samples[i] );
    }
}

int PulseBuffer::getSampleCount() const
{
    return buffer_length;
}

std::string PulseBuffer::getBufferHash() 
{
    if (!finalised)
    {
        if (!md5_valid) hash();
        return our_hash;
    }
    else
    {
        return our_hash;
    }
}
