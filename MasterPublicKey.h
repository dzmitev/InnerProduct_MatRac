//MasterPublicKey.h

#pragma once

#include <iostream>
#include <iomanip>
#include <bitset>
#include <vector>
#include <random>
#include <crypto++/nbtheory.h>
#include <crypto++/integer.h>
#include <crypto++/osrng.h>
#include <sodium.h>
#include <sstream>

using namespace std;
using namespace CryptoPP;

class MasterPublicKey
{
    friend class KeyPair;

    private:
        int l; // dimension of the vectors, key length
        Integer p; // safe prime
        Integer q; // sub prime
        Integer g; // generator of the quadratic residue subgroup
        vector<Integer> publicKey;
        


    private:
        //constructors

        // they are private to prevent someone creating a
        // MasterPublicKey and MasterPrivateKey seperately:
        // they have to be created together in order to be appropriate
        // key pair
        MasterPublicKey\
        (Integer p, Integer g, vector<Integer> publicKey);

        MasterPublicKey();

    //destructor
        //no need for it, nothing is allocated dynamically
        //~MasterPublicKey();

    public:
        int getLength() const;
        const Integer& getSafePrime() const;
        const Integer& getGenerator() const;
        const vector<Integer>& getPublicKey() const;
        
        const Integer& getSubPrime() const;
        void printPublicKey() const;
};