//
//  GenericPulseProgram.h
//  PulseProgrammer
//
//  Created by Richard George on 09/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#ifndef PulseProgrammer_GenericPulseProgram_h
#define PulseProgrammer_GenericPulseProgram_h

#include "PulseData.h"
#include "PlaylistEntry.h"

#include <map>
#include <vector>

class GenericPulseProgram {
    
private:
    
    std::map<std::pair<int,int>,std::vector<PulseData *> > pulses;
    
    int current_point;
    int current_phase_cycle_step;
    
    int max_points;
    int max_phase_cycle_steps;
    
public:
    
    GenericPulseProgram();
    
    // Methods to add pulses
    void setPointNumber(int);
    void setPhaseCycleStep(int);
    
    void addPulse(const char *envelope,double start_time,double duration,double fq,double amp,double ph);
    
    // Methods to retrieve pulses
    void getPulses(int,int,std::vector<PulseData *> &);
    void generatePlaylist(const AWG &,int,int,std::vector<PlaylistEntry> &);
    
    // Count points
    int getPointCount() const;
    int getPhaseCycleStepCount() const;
    
    ~GenericPulseProgram();    
};

#endif
