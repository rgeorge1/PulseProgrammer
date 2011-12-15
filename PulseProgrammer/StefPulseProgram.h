//
//  StefPulseProgram.h
//  PulseProgrammer
//
//  Created by Richard George on 09/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#ifndef PulseProgrammer_StefPulseProgram_h
#define PulseProgrammer_StefPulseProgram_h

#define BUFFER_SIZE 256
#define MIN_AMPLITUDE 1e-12

#include <map>
#include <vector>

#include "GenericPulseProgram.h"

class StefPulseProgram : public GenericPulseProgram {

private:
    
    int x_steps1;
    int averages;
    int shots_per_point;
    int total_afq_file_size; // Number of bytes for AFQ
    
    int x_steps2;
    int pulses_per_shot;
    int sampling_freq;
    int longest_sequence;
    
public:
    
    StefPulseProgram(const char *,int version);
    ~StefPulseProgram();
    
};


#endif
