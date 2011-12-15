//
//  PulseData.h
//  PulseProgrammer
//
//  Created by Richard George on 09/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#ifndef PulseProgrammer_PulseData_h
#define PulseProgrammer_PulseData_h

#include <map>
#include <string>

#include "AWG.h"
#include "PulseBuffer.h"

class PulseData {
    
private:
    
    std::string envelope;
    double start_time,duration,frequency,amplitude,phase;
    std::map<std::string,double> properties;
    
public:
    
    int this_pulse_number;

    PulseData(const char *envelope, double start_time, double duration, double fq, double amp, double ph);
    
    void describePulse();
    
    double getFirstSynthesisTime() const;
    double getLastSynthesisTime() const;
    
    void renderToBuffer(const AWG &a,PulseBuffer *pb) const;
    
    ~PulseData();
    
};

#endif
