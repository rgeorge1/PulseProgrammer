//
//  PulseBuffers.cpp
//  PulseProgrammer
//
//  Created by Richard George on 08/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#include <iostream>

#include "PulseTypes.h"

SparsePulseBuffer::SparsePulseBuffer(const FlatDigitalPulseSequence &times)
{
    
}

PulseBuffer::PulseBuffer()
{
    I_samples = 0;
    Q_samples = 0;
    
    first_sample = 0;
    last_sample = 0;
    buffer_length = 0;
}

void PulseBuffer::allocateBufferForSamples(int fs,int ls)
{
    if (I_samples!=0) delete[] I_samples;
    if (Q_samples!=0) delete[] Q_samples;
    
    first_sample = fs;
    last_sample = ls;
    
    buffer_length = last_sample - first_sample + 1;
    
    assert(buffer_length > 0);
    
    I_samples = new int[buffer_length];
    Q_samples = new int[buffer_length];
    
    int i;
    
    for (i=0;i<buffer_length;i++) 
    {
        I_samples[i] = 0.0;
        Q_samples[i] = 0.0;
    }
}

PulseBuffer::~PulseBuffer()
{
    if (I_samples!=0) delete[] I_samples;
    if (Q_samples!=0) delete[] Q_samples;
}

void PulseBuffer::retrieveBuffers(int *&i, int *&q, int &f)
{
    i = I_samples;
    q = Q_samples;
    f = first_sample;
}
