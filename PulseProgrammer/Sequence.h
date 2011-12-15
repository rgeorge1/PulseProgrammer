//
//  Sequence.h
//  PulseProgrammer
//
//  Created by Richard George on 09/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#ifndef PulseProgrammer_Sequence_h
#define PulseProgrammer_Sequence_h

#include <map>
#include <set>
#include <vector>

class Sequence {
    
protected:
        
    int rounding;
    
    double round_down(double,int);
    double round_up(double,int);
    
public:

    std::map<double,int> edges;

    Sequence();
    Sequence(const Sequence &sq);    
    Sequence(const Sequence &sq,int rounding);
    
    void addRisingEdge(double time);
    void addFallingEdge(double time);
    void addPulse(double time,double duration);
    void getPulseEdgeTimes(std::set<double > &times) const;
    void describeSequence();
    
    void flatten();
    void merge(double);
    
    ~Sequence();
    
};

#endif
