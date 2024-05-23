#include <iostream>
#include <crypto++/integer.h>
#include <vector>
#include "MasterPublicKey.h"

using namespace std;
using namespace CryptoPP;

// random_device rd;
// mt19937_64 gen(rd());
// AutoSeededRandomPool rng;

MasterPublicKey::MasterPublicKey(Integer p, Integer g, vector<Integer> publicKey)
{
    //this->l = length;
    this->l = publicKey.size();
    this->p = p;
    this->q = Integer((p-1) / 2);
    this->g = g;
    this->publicKey = publicKey;
}
MasterPublicKey::MasterPublicKey()
{
    this->l = -1;
    this->p = Integer(-1);
    this->q = Integer(-1);
    this->g = Integer(-1);
    this->publicKey = vector<Integer>();
}

const Integer& MasterPublicKey::getSafePrime() const
{
    return this->p;
}

const Integer& MasterPublicKey::getGenerator() const
{
    return this->g;
}

const vector<Integer>& MasterPublicKey::getPublicKey() const
{
    return this->publicKey;
}

int MasterPublicKey::getLength() const
{
    return this->l;
}
const Integer& MasterPublicKey::getSubPrime() const
{
    return this->q;
}

void MasterPublicKey::printPublicKey() const
{
    for(int i = 0; i < this->l; i++)
    {
        //cout << "i = " << i << "; ";
        cout<<this->publicKey[i] << endl;
    }
}


