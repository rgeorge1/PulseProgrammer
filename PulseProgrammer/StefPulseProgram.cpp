//
//  StefPulseProgram.cpp
//  PulseProgrammer
//
//  Created by Richard George on 09/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

#include "StefPulseProgram.h"

extern bool verbose;

// Class to parse Stef's text files

StefPulseProgram::StefPulseProgram(const char *filename,int version)
{
    char buffer[BUFFER_SIZE];
    
    std::cout << "Reading a Stef Pulse Program from file `" << filename << "'" << std::endl;
    
    std::ifstream ifs(filename);
    
    ifs.getline(buffer,BUFFER_SIZE);
    sscanf(buffer,"%d,%d,%d,%d",&x_steps1,&averages,&shots_per_point,&total_afq_file_size);
    
    if (verbose) std::cout << "x_steps = " << x_steps1 << " averages = " << averages << " shots_per_point = " << shots_per_point << " total_afq_file_size = " << total_afq_file_size << std::endl;
    
    ifs.getline(buffer,BUFFER_SIZE);
    sscanf(buffer,"%d,%d,%d,%d",&x_steps2,&pulses_per_shot,&sampling_freq,&longest_sequence);
    
    if (verbose) std::cout << "x_steps = " << x_steps2 << " pulses_per_shot = " << pulses_per_shot << " sampling_freq = " << sampling_freq << " longest_sequence = " << longest_sequence << std::endl;
    
    // We are expecting x_steps shots with pulses_per_shot pulses each
    
    int i,j;
    
    for (i=0;i<x_steps1;i++)
    {
        if (verbose) std::cout << "-->  Creating point #" << i << std::endl;
        
        setPointNumber(i);
        setPhaseCycleStep(0);

        double current_time = 0.0;
        
        for (j=0;j<pulses_per_shot;j++)
        {
            ifs.getline(buffer,BUFFER_SIZE);
            
            double this_frequency = 0.0;
            double this_phase = 0;
            double this_amplitude = 0.0;
            double this_duration = 0.0;
            
            double this_start_time = 0.0;
            
            switch (version)
            {
                case 1:
                    sscanf(buffer,"%lg,%lg,%lg,%lg",&this_frequency,&this_phase,&this_amplitude,&this_duration);
                    this_start_time = current_time;
                    break;
                    
                case 2:
                    sscanf(buffer,"%lg,%lg,%lg,%lg,%lg",&this_frequency,&this_phase,&this_amplitude,&this_duration,&this_start_time);
                    break;
            }
            
            
            if (fabs(this_amplitude)>MIN_AMPLITUDE)
            {
                    if (verbose) std::cout << "Point " << i << " gets a pulse starting at time " << this_start_time << " ns  with f = " << this_frequency;
                    if (verbose) std::cout << ", a = " << this_amplitude << ", phi = " << this_phase << ", with length " << this_duration;
                    if (verbose) std::cout << " ns" << std::endl;
                
                addPulse("square",this_start_time,this_duration,this_frequency,this_amplitude,this_phase);
            }
            else
            {
                    if (verbose) std::cout << "Point " << i << " gets a pause starting at time " << this_start_time << " ns, of duration ";
                    if (verbose) std::cout << this_duration << " ns" << std::endl;
            }
            
            if (version == 1)
            {
                current_time += this_duration;
                if (verbose) std::cout << "-->  Time is now " << current_time << " ns into point #" << i << std::endl;
            }
            
        }
        
        if (verbose) std::cout << "-->  Ending point #" << i << std::endl << std::endl;
    }
    
}

StefPulseProgram::~StefPulseProgram()
{
}