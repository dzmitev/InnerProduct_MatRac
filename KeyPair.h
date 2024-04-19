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

class KeyPair
{
    private:
        int l; // dimension of the vectors, key length
        Integer p; // safe prime
        Integer g; // generator of the quadratic residue subgroup
        vector<Integer> publicKey;
        vector<Integer> privateKey;

    public:
        //constructors
        KeyPair(int length, int bits);
        KeyPair(int length, int bits, Integer p1); //if we already have a safe prime found, we can use it again
        
        //destructor
        ~KeyPair();

        int getLength() const;
        const Integer& getSafePrime() const;
        const Integer& getGenerator() const;
        const vector<Integer>& getPublicKey() const;
        const vector<Integer>& getPrivateKey() const;
        Integer getSubPrime() const;
    
};