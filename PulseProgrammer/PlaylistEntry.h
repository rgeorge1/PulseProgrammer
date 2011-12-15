//
//  PlaylistEntry.h
//  PulseProgrammer
//
//  Created by Richard George on 09/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#ifndef PulseProgrammer_PlaylistEntry_h
#define PulseProgrammer_PlaylistEntry_h

#include "PulseBuffer.h"

class PlaylistEntry {
    
public:
    PlaylistEntry();
    PlaylistEntry(PulseBuffer *);
    PlaylistEntry(int);
    
    PulseBuffer *pb;
    int sample_length;
};

#endif
