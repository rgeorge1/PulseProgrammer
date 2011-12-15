//
//  AWG.cpp
//  PulseProgrammer
//
//  Created by Richard George on 08/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <string>

#include <assert.h>

#include "PulseBuffer.h"
#include "AWG.h"

extern bool change_endian;

AWG::AWG(double cr,const char *n) :clockrate(cr * 1e-3 ), sample_period(1.0 / (clockrate * 1e-3) ), name(n)
{
    bytes_per_sample = 4;
    blockbits = 8;
}

int AWG::getBlockBitCount() const
{
    return blockbits;
}

int AWG::getChunkyBlockSize() const
{
    return (1 << blockbits);
}

int AWG::getSampleBeforeTime(double time) const
{
    return floor(time / clockrate);
}

int AWG::getSampleAfterTime(double time) const
{
    return ceil(time / clockrate);
}

double AWG::getTimeForSample(int idx) const
{
    return (((double)idx)+0.5) * sample_period;
}

double AWG::getClockRate() const
{
    return clockrate;
}

int AWG::getStorageForBuffer(PulseBuffer *pb) const
{
    int samples = pb->getSampleCount();
    return samples * bytes_per_sample;
}

void AWG::prepareBinaryBlock(PulseBuffer *pb,uint8_t **output,int &output_length) const
{
    assert(pb!=0);
    
    output_length = getStorageForBuffer(pb);
    *output = new uint8_t[output_length];
    
    uint16_t *uint_buffer = (uint16_t *)*output;
    
    int i;
    
    int sample_count;
    int first_sample;
    const double *I_samples=0;
    const double *Q_samples=0;
    
    pb->getBuffers(I_samples,Q_samples,first_sample,sample_count);
    
    assert(I_samples!=0);
    assert(Q_samples!=0);
    
    for (i=0;i<sample_count;i++)
    {
        uint_buffer[2*i] = scaleSample(I_samples[i]);
        uint_buffer[2*i+1] = scaleSample(Q_samples[i]);
    }
}

uint16_t AWG::scaleSample(double s) const
{
    double v = 65535*(s + 0.5);
    v = (v < 0.0) ? 0.0 : v;
    v = (v > 65535.0) ? 65535.0 : v;
    
    uint16_t i = (uint16_t) v;
    
    if (change_endian)
    {
        v = ((( i & 0x00ff) << 8 ) | ((i & 0xff00) >> 8));
    }
        
    return i;
}

AWG::~AWG()
{
}