//
//  PulseSequence.h
//  PulseProgrammer
//
//  Created by Richard George on 08/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#ifndef PulseProgrammer_PulseSequence_h
#define PulseProgrammer_PulseSequence_h

#include <map>
#include <set>

class DigitalPulseSequence {
    
private:
    std::map<double,int> edges;
    
public:
    
    DigitalPulseSequence();
    ~DigitalPulseSequence();
    
    void addRisingEdge(double time);
    void addFallingEdge(double time);
    
    void getPulseEdgeTimes(std::set<double > &) const;
    
};

class FlatDigitalPulseSequence {
	
private:
    std::map<double,bool> sequence;
	
public:
	
    FlatDigitalPulseSequence(const DigitalPulseSequence &input);
	
    bool isPulseActiveAfterTime(double t) const;
    bool isPulseActiveBeforeTime(double t) const;
    
    void getPulseEdgeTimes(std::set<double > &) const;
    
};


#endif
