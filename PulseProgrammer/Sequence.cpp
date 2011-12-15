//
//  Sequence.cpp
//  PulseProgrammer
//
//  Created by Richard George on 09/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#include <iostream>
#include <cmath>

#include "Sequence.h"

extern bool verbose;

Sequence::Sequence()
{    
}

void Sequence::addRisingEdge(double time)
{
	if (edges.count(time) == 0 )
    {
		edges[time] = 1;
	}
	else
	{
		edges[time] += 1;
	}
}

void Sequence::addFallingEdge(double time)
{
	if (edges.count(time) == 0 )
  	{
		edges[time] = -1;
	}
	else
	{
		edges[time] -= 1;
	}
}

struct RetrieveKey
{
    template<typename T> typename T::first_type operator()(T keyValuePair) const
    {
        return keyValuePair.first;
    }
};

void Sequence::getPulseEdgeTimes(std::set<double > &times) const
{
    times.clear();
    std::transform(edges.begin(),edges.end(),std::inserter(times,times.end()), RetrieveKey() );
}


Sequence::Sequence(const Sequence &other)
{
    Sequence sq;
    
    int current_edge_count = (int) other.edges.size();
    
    int new_total = 0, total = 0;
    
    // Iterate over edges on the input sequence
    for (std::map<double,int>::const_iterator it=other.edges.begin();it!=other.edges.end();it++)
    {
        new_total = total + (*it).second;
        
        if (( new_total > 0 ) && ( total <= 0 )) sq.addRisingEdge((*it).first);
        if (( new_total <= 0 ) && ( total > 0 )) sq.addFallingEdge((*it).first);
        
    }
    
    edges.swap(sq.edges);
    
    if (verbose) std::cout << "flatten() changed from " << current_edge_count << " to " << edges.size() << " edges." << std::endl;
}

double Sequence::round_down(double e,int t)
{
    return floor(e / ((double) t))*((double)t);
}

double Sequence::round_up(double e,int t)
{
    return ceil(e / ((double) t))*((double)t);
}

Sequence::Sequence(const Sequence &other,int round)
{
    Sequence sq;
    
    rounding = round;
    
    int resolution = rounding;
    
    if (verbose) std::cout << "Creating new Sequence with coarsness of " << resolution << " samples" << std::endl;
    
    int current_edge_count = (int) other.edges.size();
    
    int new_total = 0, total = 0;
    
    for (std::map<double,int>::const_iterator it=other.edges.begin();it!=other.edges.end();it++)
    {
        total = new_total;
        if (verbose) std::cout << (*it).first << " : " << (*it).second << " new_total = " << new_total << std::endl;
        new_total = total + (*it).second;
        
        if (( new_total > 0 ) && ( total <= 0 )) sq.addRisingEdge( round_down((*it).first,resolution) );
        if (( new_total <= 0 ) && ( total > 0 )) sq.addFallingEdge( round_up((*it).first,resolution) );
        
    }
    
    edges.swap(sq.edges);
    
    if (verbose) std::cout << "flatten() changed from " << current_edge_count << " to " << edges.size() << " edges." << std::endl;
}


void Sequence::flatten()
{
    Sequence sq;
    
    int current_edge_count = (int) edges.size();
    
    int new_total = 0, total = 0;
    
    for (std::map<double,int>::iterator it=edges.begin();it!=edges.end();it++)
    {
        total = new_total;
        if (verbose) std::cout << (*it).first << " : " << (*it).second << " new_total = " << new_total << std::endl;
        new_total = total + (*it).second;
        
        if (( new_total > 0 ) && ( total <= 0 )) { if (verbose) { std::cout << "Rise" << std::endl; } sq.addRisingEdge((*it).first); continue; }
        if (( new_total <= 0 ) && ( total > 0 )) { if (verbose) { std::cout << "Fall" << std::endl; } sq.addFallingEdge((*it).first); continue; }
        
    }
    
    edges.swap(sq.edges);
    
    if (verbose) std::cout << "flatten() changed from " << current_edge_count << " to " << edges.size() << " edges." << std::endl;
}

void Sequence::describeSequence()
{
    for (std::map<double,int>::iterator it=edges.begin();it!=edges.end();it++)
    {
        std::cout << "Time " << (*it).first << "ns : " << (*it).second << std::endl;
    }
}

Sequence::~Sequence()
{
}
