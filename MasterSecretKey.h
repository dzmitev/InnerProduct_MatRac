//MasterSecretKey.h

#pragma once

#include <iostream>
//#include <iomanip>
//#include <bitset>
#include <vector>
//#include <random>
//#include <crypto++/nbtheory.h>
//#include <crypto++/integer.h>
//#include <crypto++/osrng.h>
//#include <sodium.h>
//#include <sstream>

using namespace std;
using namespace CryptoPP;

class MasterSecretKey
{
    friend class KeyPair;

    private:
        int l;
        vector<Integer> privateKey;

    private:
        //constructors
        MasterSecretKey(vector<Integer>);
        MasterSecretKey();
        //destructor
        //~MasterSecretKey();

    public:
        const vector<Integer>& getPrivateKey() const;
        void printPrivateKey() const;
};