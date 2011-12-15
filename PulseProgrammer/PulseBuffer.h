//
//  PulseBuffer.h
//  PulseProgrammer
//
//  Created by Richard George on 09/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#ifndef PulseProgrammer_PulseBuffer_h
#define PulseProgrammer_PulseBuffer_h

#include "AWG.h"
#include "md5.h"
#include "MD5Hash.h"

class PulseBuffer {

protected:
    
    double *I_samples;
    double *Q_samples;
    
    bool md5_valid;
    bool finalised;
    
    int first_sample;
    int buffer_length;

    void hash();
    MD5Hash md5hash;
    std::string our_hash;
    
public:
    
    int this_buffer_number;
    
    PulseBuffer(int len);
    PulseBuffer(int fs,int len);

    void updateBuffers(double *&I_samples,double *&Q_samples,int &fs,int &l);
    void getBuffers(const double *&I_samples,const double *&Q_samples,int &fs,int &l);
    
    double getTimeForIndex(const AWG &a,int idx) const;
    
    void dispose_our_buffers();
    void finalise();    
    void reportMD5();
    void clipSamples();
    int getSampleCount() const;
    
    std::string getBufferHash();
    
    ~PulseBuffer();
    
};


#endif
