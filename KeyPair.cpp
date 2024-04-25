//
// KeyPair.cpp
//
//#include <crypto++/integer.h>
//#include <vector>
#include "KeyPair.h"
#include "my_utilities.h"
#include "MasterPublicKey.h"
#include "MasterSecretKey.h"

KeyPair::KeyPair(int length, int bits, Integer p)
{
    vector<Integer> publicKey;
    vector<Integer> privateKey;
    Integer g = generatorOfQuadraticResidues(p);
    Integer q = (p - 1) / 2;
    for (int i = 0; i < length; i++)
    {
        //cout << "i = " << i << endl;
        //secretKey.push_back(generateCryptoSecureRandomInteger(bits).Modulo(q));
        //Integer sk_i = generateCryptoSecureRandomInteger(bits).Modulo(q);
        privateKey.push_back(generateCryptoSecureRandomInteger(bits).Modulo(q));
        publicKey.push_back(a_exp_b_mod_c(g, privateKey[i], p));
    }

    //MasterPublicKey mpk1(p, g, publicKey);
    //MasterSecretKey msk1(secretKey);

    this->length = length;
    //this->mpk = mpk1;
    //this->msk = msk1;
    this->mpk = MasterPublicKey(p, g, publicKey);
    this->msk = MasterSecretKey(privateKey);
}

KeyPair::KeyPair(int length, int bits)
{
    PrimeAndGenerator pg(1, rng, bits);
    Integer p = pg.Prime(); // Retrieve the prime from PrimeAndGenerator
    // Now, call the other constructor with the retrieved prime
    *this = KeyPair(length, bits, p);
}

const MasterPublicKey& KeyPair::getMasterPublicKey() const
{
    return this->mpk;
}

const MasterSecretKey& KeyPair::getMasterSecretKey() const
{
    return this->msk;
}

int KeyPair::getLength() const
{
    return this->length;
}
