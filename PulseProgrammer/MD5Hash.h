//
//  MD5Hash.h
//  PulseProgrammer
//
//  Created by Richard George on 09/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#ifndef PulseProgrammer_MD5Hash_h
#define PulseProgrammer_MD5Hash_h

class MD5Hash {
    
protected:
    
    md5_byte_t digest[16];    
    
public:
    
    friend bool operator==(const MD5Hash &,const MD5Hash&);
    friend bool operator<=(const MD5Hash &,const MD5Hash&);
    friend bool operator>=(const MD5Hash &,const MD5Hash&);
    friend bool operator<(const MD5Hash &,const MD5Hash&);
    friend bool operator>(const MD5Hash &,const MD5Hash&);
    MD5Hash &operator =(const MD5Hash &rhs);
    
    md5_byte_t *getDigest();
    
    void reportHash() const;
    std::string getHash() const;
    
    MD5Hash(const MD5Hash &);
    MD5Hash();
};

#endif
