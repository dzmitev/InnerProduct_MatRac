// MasterSecretKey.cpp

#include <iostream>
#include <vector>
#include <crypto++/integer.h>
#include "MasterSecretKey.h"

using namespace std;
using namespace CryptoPP;


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
