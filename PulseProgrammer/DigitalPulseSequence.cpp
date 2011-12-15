//
//  DigitalPulseSequence.cpp
//  PulseProgrammer
//
//  Created by Richard George on 08/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include "PulseTypes.h"

void DigitalPulseSequence::addRisingEdge(double time)
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

void DigitalPulseSequence::addFallingEdge(double time)
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

void DigitalPulseSequence::getPulseEdgeTimes(std::set<double > &times) const
{
    times.clear();
    std::transform(edges.begin(),edges.end(),std::back_inserter(times), RetrieveKey() );
}
