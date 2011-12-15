//
//  PulseData.cpp
//  PulseProgrammer
//
//  Created by Richard George on 09/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#include <iostream>
#include <cmath>

#include "PulseData.h"
#include "AWG.h"


extern bool verbose;

int pulse_counter = 0;

// Pulse Data Class

PulseData::PulseData(const char *env, double st, double dur, double fq, double amp, double ph)
{
    envelope = env;
    start_time = st;
    duration = dur;
    frequency = fq;
    amplitude = amp;
    phase = ph;
    
    this_pulse_number = pulse_counter++;
    
    if (verbose) describePulse();
    if (verbose) std::cout << "Created pulse #" << this_pulse_number << std::endl;
}

void PulseData::describePulse()
{
    std::cout << "Pulse with envelope `" << envelope << "', start_time = " << start_time << " ns";
    std::cout << ", duration = " << duration << "ns, frequency = " << frequency << " MHz, amplitude = " << amplitude;
    std::cout << ", phase = " << phase << std::endl;
}

PulseData::~PulseData()
{
    if (verbose) std::cout << "Deleting pulse #" << this_pulse_number << std::endl;
}

double PulseData::getFirstSynthesisTime() const
{
    return start_time;
}

double PulseData::getLastSynthesisTime() const
{
    return start_time+duration;
}

void PulseData::renderToBuffer(const AWG &a,PulseBuffer *pb) const
{
    
    double *I_samples;
    double *Q_samples;
    
    int first_sample;
    int length;
    
    pb->updateBuffers(I_samples,Q_samples,first_sample,length);
    
    int i;
    
    if (verbose) std::cout << "Buffer covers samples " << first_sample << " to " << (first_sample + length) << std::endl;

    int b = a.getSampleBeforeTime(start_time);
    int c = a.getSampleAfterTime(start_time+duration);
    
    if ((b>first_sample) && (c<(first_sample+length)))
    {
        if (verbose) std::cout << " --> Pulse " << this_pulse_number << " is in the buffer" << std::endl;
        
        double angular_frequency = M_2_PI * frequency;
        double phase_offset = ((double)phase) * M_PI / 180.0;
        double t;
        
        for (i=(b-first_sample);i<(length);i++)
        {
     
            t = pb->getTimeForIndex(a,i);
            
            I_samples[i] += amplitude*cos(angular_frequency * t + phase_offset);
            Q_samples[i] += amplitude*sin(angular_frequency * t + phase_offset);
            
        }
    }
    else
    {
        if (verbose) std::cout << " --> Pulse " << this_pulse_number << " is not in the buffer" << std::endl;        
    }
    
}

