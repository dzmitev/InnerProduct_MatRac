//
// NewKey.h
//
#pragma once
#ifndef IP2_KEYPAIR_H
#define IP2_KEYPAIR_H


#include "MasterPublicKey.h"
#include "MasterSecretKey.h"

class KeyPair
{
private:
    MasterPublicKey mpk;
    MasterSecretKey msk;
    int length;

public:
    KeyPair(int length, int bits, Integer p);
    KeyPair(int length, int bits);

    const MasterPublicKey &getMasterPublicKey() const;

    const MasterSecretKey &getMasterSecretKey() const;

    int getLength() const;
};


#endif //IP2_KEYPAIR_H
