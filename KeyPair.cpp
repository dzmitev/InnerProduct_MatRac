// KeyPair.cpp

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

#include "KeyPair.h"
#include "my_utilities.h"
using namespace std;
using namespace CryptoPP;

// random_device rd;
// mt19937_64 gen(rd());
// AutoSeededRandomPool rng;


KeyPair::KeyPair(int length, int bits, Integer p) 
{
    // Initialize publicKey and privateKey arrays
    vector<Integer> publicKey;
    vector<Integer> privateKey;
    Integer g = generatorOfQuadraticResidues(p);
    Integer q = (p-1) / 2;
    //check that p is indeed a safe prime
    /*cout << "first probable prime check" << endl;
    if(isProbablePrime(p, 64) == false)
    {
        throw invalid_argument("The input is not prime!");
    }
    cout << "second probable prime check" << endl;
    if(isProbablePrime( ((p-1) / 2), 64) == false)
    {
        throw invalid_argument("The input is not safe prime!");
    }*/

    cout << "constructing keys" << endl;
    for (int i = 0; i < length; i++) 
    {
        cout << "i = " << i << endl;
        privateKey.push_back(generateCryptoSecureRandomInteger(bits).Modulo(q));
        publicKey.push_back(a_exp_b_mod_c(g, privateKey[i], p));
        //cout << privateKey[i] << ";;;" << publicKey[i] << endl;
    }
    //publicKey.resize(l);
    //privateKey.resize(l);
    
    cout << "setting the attributes" << endl;

    this->l = length;
    this->p = p;
    this->g = g;
    this->publicKey = publicKey;
    this->privateKey = privateKey;
}

KeyPair::KeyPair(int length, int bits) 
{
    // generate a safe prime and use the previous constructor
    //AutoSeededRandomPool rng;
    cout << "generating a prime!" << endl;
    PrimeAndGenerator pg(1, rng, bits);
    Integer p = pg.Prime();
    //cout << "before calling the first constructor" << endl;
    *this = KeyPair(length, bits, p);
}

//destructor
KeyPair::~KeyPair()
{
    // Release dynamically allocated resources
    // Example: if publicKey and privateKey are dynamically allocated vectors
    // delete them in the destructor

    // delete[] publicKey;
    // delete[] privateKey;
}


// getter methods
int KeyPair::getLength() const
{
    return this->l;
}

const Integer& KeyPair::getSafePrime() const
{
    return this->p;
}

const Integer& KeyPair::getGenerator() const
{
    return this->g;
}

const vector<Integer>& KeyPair::getPublicKey() const
{
    return this->publicKey;
}

const vector<Integer>& KeyPair::getPrivateKey() const 
{
    return this->privateKey;
}

Integer KeyPair::getSubPrime() const
{
    Integer q = (this->p - 1) / 2;
    return q;
}