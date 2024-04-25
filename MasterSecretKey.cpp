// MasterSecretKey.cpp

#include <iostream>
//#include <iomanip>
//#include <bitset>
#include <vector>
//#include <random>
//#include <crypto++/nbtheory.h>
#include <crypto++/integer.h>
//#include <crypto++/osrng.h>
//#include <sodium.h>
//#include <sstream>

#include "MasterSecretKey.h"
//#include "my_utilities.h"
using namespace std;
using namespace CryptoPP;

// random_device rd;
// mt19937_64 gen(rd());
// AutoSeededRandomPool rng;

MasterSecretKey::MasterSecretKey(vector<Integer> keyCandidate) 
{
    this->privateKey = keyCandidate;
    this->l = keyCandidate.size();
}

MasterSecretKey::MasterSecretKey()
{
    this->privateKey = vector<Integer>();
    this->l = -1;
}


const vector<Integer>& MasterSecretKey::getPrivateKey() const
{
    return this->privateKey;
}

void MasterSecretKey::printPrivateKey() const
{
    for(int i = 0; i < this->privateKey.size(); i++)
    {
        //cout << "i = " << i << "; ";
        cout<<this->privateKey[i] << endl;
    }
}
