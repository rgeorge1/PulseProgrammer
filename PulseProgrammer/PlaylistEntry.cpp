//
//  PlaylistEntry.cpp
//  PulseProgrammer
//
//  Created by Richard George on 09/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#include <iostream>
#include "PlaylistEntry.h"

#include <map>

std::map<int,int> zero_entries;


PlaylistEntry::PlaylistEntry()
{
    pb = 0;
    sample_length = 0;
}

PlaylistEntry::PlaylistEntry(PulseBuffer *p)
{
    pb = p;
    sample_length = pb->getSampleCount();
}

PlaylistEntry::PlaylistEntry(int c)
{
    pb = 0;
    sample_length = c;    
    
    std::map<int,int>::iterator it = zero_entries.find(c);
    
    if (it==zero_entries.end())
    {
        zero_entries[c] = 1;
    }
    else
    {
        zero_entries[c] += 1;
    }
}

