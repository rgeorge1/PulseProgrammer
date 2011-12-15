//
//  MD5Hash.cpp
//  PulseProgrammer
//
//  Created by Richard George on 09/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#include <iostream>

#include "md5.h"
#include "MD5Hash.h"

md5_byte_t *MD5Hash::getDigest()
{
    return &digest[0];
}

bool operator==(const MD5Hash &l,const MD5Hash &r)
{
    int i;
    for (i=0;i<16;i++)
    {
        if (l.digest[i] != r.digest[i]) return false;
    }
    
    return true;
}


MD5Hash &MD5Hash::operator=(const MD5Hash &other)
{
    int i;
    for (i=0;i<16;i++) digest[i] = other.digest[i];
    return *this;
}

bool operator<=(const MD5Hash &l,const MD5Hash &r)
{
    int i;
    for (i=0;i<16;i++)
    {
        if (l.digest[i] > r.digest[i]) return false;
    }
    
    return true;
}

bool operator<(const MD5Hash &l,const MD5Hash &r)
{
    int i;
    for (i=0;i<16;i++)
    {
        if (l.digest[i] >= r.digest[i]) return false;
    }
    
    return true;
}

bool operator>(const MD5Hash &l,const MD5Hash &r)
{
    int i;
    for (i=0;i<16;i++)
    {
        if (l.digest[i] <= r.digest[i]) return false;
    }
    
    return true;
}


bool operator>=(const MD5Hash &l,const MD5Hash &r)
{
    int i;
    for (i=0;i<16;i++)
    {
        if (l.digest[i] < r.digest[i]) return false;
    }
    
    return true;
}

MD5Hash::MD5Hash(const MD5Hash &h)
{
    int i;
    for (i=0;i<16;i++) digest[i] = h.digest[i];
}

MD5Hash::MD5Hash()
{
    int i;
    for (i=0;i<16;i++) digest[i] = 0;
}

std::string MD5Hash::getHash() const
{
    std::string result;

    int i;
    
    for (i=0;i<16;i++)
    {
        char buf[4];
        sprintf(buf,"%02X",digest[i]);
        result += buf;
    }
    
    return result;
}

void MD5Hash::reportHash() const
{
    std::cout << getHash().c_str();
}