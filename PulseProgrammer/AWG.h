//
//  AWG.h
//  PulseProgrammer
//
//  Created by Richard George on 08/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#ifndef PulseProgrammer_AWG_h
#define PulseProgrammer_AWG_h

typedef struct tagSamplePair {
    uint16_t i;
    uint16_t q;
} SamplePair;

class PulseBuffer;

class AWG {
    
private:
    
    double clockrate;
    double sample_period;
    
    int bytes_per_sample;
    
    int blockbits;
    
    std::string name;
    
public:
    
    AWG(double clock,const char *);
    ~AWG();
    
    int getSampleBeforeTime(double time) const;
    int getSampleAfterTime(double time) const;
    double getTimeForSample(int s) const;
    
    double getClockRate() const;
    
    int getChunkyBlockSize() const;
    int getBlockBitCount() const;
    
    int getStorageForBuffer(PulseBuffer *) const;
    void prepareBinaryBlock(PulseBuffer *pb,uint8_t **output,int &output_length) const;
    uint16_t scaleSample(double v) const;
    
};

#endif
