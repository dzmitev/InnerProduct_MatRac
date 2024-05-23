//MasterSecretKey.h

#pragma once

#include <vector>
#include <crypto++/integer.h>


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
        // not needed -- nothing is allocated dynamically
        //~MasterSecretKey();

    public:
        const vector<Integer>& getPrivateKey() const;
        void printPrivateKey() const;
};