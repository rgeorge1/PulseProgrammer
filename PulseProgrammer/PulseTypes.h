//
//  PulseTypes.h
//  PulseProgrammer
//
//  Created by Richard George on 08/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#ifndef PulseProgrammer_PulseTypes_h
#define PulseProgrammer_PulseTypes_h

#include <assert.h>
#include <set>
#include <map>
#include <string>
#include <cmath>

#include "AWG.h"
#include "PulseSequence.h"

class PulseBuffer {
    
private:
    
    int first_sample;
    int last_sample;
    
    int buffer_length;
    
    int *I_samples;
    int *Q_samples;
    
    void allocateBufferForSamples(int first_sample,int last_sample);
    
public:
    
    PulseBuffer();    
    ~PulseBuffer();
    
    int getFirstSample() const;
    int getLastSample() const;
    
    void retrieveBuffers(int *&i, int *&q,int &f);
    
};

class SparsePulseBuffer {
    
public:
    
    SparsePulseBuffer(const FlatDigitalPulseSequence &times);
    ~SparsePulseBuffer();
    
};


class GenericPulse {
    
protected:
    
    double start_time;
    double length;
    double start_increment;
    double length_increment;
    
    std::map<std::string,double > properties;
    
public:
    
    virtual double getEarliestSynthesisTime() const;
    virtual double getLatestSynthesisTime() const;
    
    virtual bool addPulseToBuffer(PulseBuffer &) const;
    
    
    ~GenericPulse();
};

double read_property(const std::map<std::string,double> &,const char *n,double d);

class SquarePulse : protected GenericPulse {
    
protected:
    
    double frequency;
    double amplitude;
    double phase;
    
public:
    
    SquarePulse(double start_time,double l,double start_inc,double l_inc,const std::map<std::string,double> properties);
    
    virtual double getEarliestSynthesisTime() const;
    virtual double getLatestSynthesisTime() const;
    
    virtual bool addPulseToBuffer(const AWG &,PulseBuffer &) const;
    
};

double read_property(const std::map<std::string,double> &p,const char *n,double def)
{
    std::map<std::string,double>::const_iterator it = p.find(std::string(n));
    
    if (it!=p.end())
    {
        return it->second;
    }
    
    return def;
}

SquarePulse::SquarePulse(double start,double lng,double start_inc,double length_inc,const std::map<std::string,double> &properties)
{
    start_time = start;
    length = lng;
    start_increment = start_inc;
    length_increment = length_inc;
    
    frequency = read_property(properties,"frequency",0.0);
    amplitude = read_property(properties,"amplitude",1.0);
    phase     = read_property(properties,"phase",0.0);
}

double SquarePulse::getEarliestSynthesisTime() const
{
    return start_time;
}

double SquarePulse::getLatestSynthesisTime() const
{
    return start_time + length;
}

bool SquarePulse::addPulseToBuffer(const AWG &a,PulseBuffer &b) const
{
    int i;

    int j=a.getSampleBeforeTime(getEarliestSynthesisTime());
    int k=a.getSampleAfterTime(getLatestSynthesisTime());

    
    int *I_samples;
    int *Q_samples;
    
    int earliest_sample;
    double t,t_0;
    
    b.retrieveBuffers(I_samples,Q_samples,earliest_sample);
    
    t_0 = a.timeForSample(earliest_sample);
                
    for (i=(j - earliest_sample);i<=(k - earliest_sample);i++)
    {
        t = a.timeForSample(i);
        
        I_samples[i] += amplitude*cos( angular_frequency*(t_0 + t) );
        Q_samples[i] += amplitude*sin( angular_frequency*(t_0 + t) );
    }
    
    return true;
}

class PulseSequence {
    
private:
    
    std::set<GenericPulse *> pulses;
    
public:
    
    PulseSequence();
    
    void addPulse(GenericPulse *);
    
    ~PulseSequence();
    
};

PulseSequence::PulseSequence()
{
}

void PulseSequence::addPulse(GenericPulse *g)
{
    pulses.insert(g);
}

class PulseExperiment {
    
private:

    std::set<GenericPulse *> pulses;
    
public:
    PulseExperiment();
    void addPulse();
    ~PulseExperiment();
};


#endif
