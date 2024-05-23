#include <iostream>
#include <vector>
#include <crypto++/integer.h>
#include <crypto++/osrng.h>
#include <optional>
#include "KeyPair.h"
#include "MasterSecretKey.h"
#include "MasterPublicKey.h"
#include "my_utilities.h"
using namespace std;
using namespace CryptoPP;

// random_device rd;
// mt19937_64 gen(rd());
// AutoSeededRandomPool rng;

KeyPair setup(int l, int bits, long long int B, const optional<Integer>& p = nullopt)
{
    // bits -- number of bits of the safe prime
    // B -- upper bound on the elements of the vector
    // l -- dimension of the vector

    // Make sure that bits > 0, B > 0, l > 0
    if (bits <= 0)
    {
        throw std::invalid_argument("Number of bits must be positive");
    }

    if (B <= 0)
    {
        throw std::invalid_argument("The bound B must be positive");
    }

    if (l <= 0)
    {
        throw std::invalid_argument("The dimension l must be positive");
    }


    // Check if p is provided
    if(p.has_value() == true)
    {
        // Call KeyPair constructor with p
        KeyPair key(l, bits, p.value());
        Integer q = key.getMasterPublicKey().getSubPrime();

        if (l * B * B >= q)
        {
            throw invalid_argument("l * B * B >= q! You need a larger prime or a smaller bound!");
        }
        return key;
    }
    else
    {
        // No prime p provided, call KeyPair constructor without p
        KeyPair key(l, bits);
        Integer q = key.getMasterPublicKey().getSubPrime();

        if (l * B * B >= q)
        {
            throw invalid_argument("l * B * B >= q! You need a larger prime or a smaller bound!");
        }
        return key;
    }
}


vector<Integer> encrypt(MasterPublicKey mpk, vector<Integer> plaintext, long long int B)
{
    Integer g = mpk.getGenerator();
    Integer p = mpk.getSafePrime();
    Integer q = mpk.getSubPrime();
    vector<Integer> pk = mpk.getPublicKey();

    if(isVectorInBound(plaintext, B) == false)
    {
        throw invalid_argument("The vector is not in the bound!");
    }

    if(mpk.getLength() != plaintext.size())
    {
        throw invalid_argument("The key and the plaintext vectors are of different size!");
    }

    //generate a cryptographically secure random integer from {2,3,...,q-1}
    AutoSeededRandomPool rng;
    Integer r = Integer(rng, 2, q - 2);

    //construct the ciphertext
    vector<Integer> ciphertext;
    Integer ct_0 = a_exp_b_mod_c(g, r, p);
    ciphertext.push_back(ct_0);

    for(int i = 0; i < mpk.getLength(); i++)
    {
        Integer mult1 = a_exp_b_mod_c(pk[i], r, p);
        Integer mult2 = a_exp_b_mod_c(g, plaintext[i], p);
        ciphertext.push_back(a_times_b_mod_c(mult1, mult2, p));
    }

    return ciphertext;
}

Integer keygen(KeyPair key, vector<Integer> y, long long int B)
{
    MasterPublicKey mpk = key.getMasterPublicKey();
    MasterSecretKey msk = key.getMasterSecretKey();

    //Integer g = mpk.getGenerator();
    //Integer p = mpk.getSafePrime();
    Integer q = mpk.getSubPrime();
    //vector<Integer> pk = mpk.getPublicKey();

    vector<Integer> sk = msk.getPrivateKey();

    if(isVectorInBound(y, B) == false)
    {
        throw invalid_argument("The keygen vector is not in the bound!");
    }
    if(mpk.getLength() != y.size())
    {
        throw invalid_argument("The key and the keygen vector are of different size!");
    }

    Integer sk_y("0");
    for(int i = 0; i < y.size(); i++)
    {
        sk_y = (sk_y + a_times_b_mod_c(sk[i], y[i], q)).Modulo(q);
    }
    return sk_y;
}

Integer decrypt(MasterPublicKey mpk, vector<Integer> ciphertext, vector<Integer> y, Integer sk_y, long long int B)
{
    Integer g = mpk.getGenerator();
    Integer p = mpk.getSafePrime();
    Integer q = mpk.getSubPrime();

    // caluclate the product prod = (ct_1 ^ y_1 * ... * ct_l ^ y_l) * ct_0^(-sk_y)

    //denomInvese is the inverse of the denominator, i.e. denomInverse = ct_0^(-sk_y)
    Integer denomInverse = a_exp_b_mod_c(ciphertext[0], sk_y, p);
    denomInverse = denomInverse.InverseMod(p);

    Integer product("1");
    for(int i = 1; i < ciphertext.size(); i++)
    {
        Integer mult2 = a_exp_b_mod_c(ciphertext[i], y[i-1], p);

        product = a_times_b_mod_c(product, mult2, p);
    }

    product = a_times_b_mod_c(product, denomInverse, p);
    Integer upperBound = y.size() * B * B;

    Integer disc_log = babyStepGiantStep(g, product, p, upperBound);

    return disc_log;
}


int main()
{
    vector<Integer> grades = {8, 7,  9, 6, 10, 6, 7, 9, 6, 8, \
          7, 10, 8, 9, 8, 8, 9, 9, 7, 9, \
          7, 9 , 8, 10, 8, 7, 7, 9, 9, 10};

    vector<Integer> ects = {7, 7, 6, 6, 10, 5, 7, 7, 5, 5, 7, 10, \
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, \
        3,3,3,3};


    //int l = grades.size();
    int B = 10000; // we assume that every subject is worth at most 10 ECTS
    int bits = 1024;
    int l = 100;
    vector<Integer> plaintextVec = randomIntegerVector(l, B);
    vector<Integer> keyVec = randomIntegerVector(l, B);

    Integer p1("136607070847329078202190522504350756341844973016949670397906556513586960612188818280604150245997995440972736560804417244040369560156029916789431171583879198790487519180250259136321970832683028539422904064797240421435541885698081957638584796566250762491993938886264096354704395440135477295514059359739396691483");
    KeyPair key = setup(l, bits, B);
    MasterPublicKey pk = key.getMasterPublicKey();
    MasterSecretKey sk = key.getMasterSecretKey();

    vector<Integer> Ct = encrypt(pk, plaintextVec, B);
    Integer sk_keyVec = keygen(key, keyVec, B);
    Integer decValue = decrypt(pk, Ct, keyVec, sk_keyVec, B);

    Integer innerProd = scalarProduct(plaintextVec, keyVec);
    cout<< "innerProd = " << innerProd <<endl;
    cout << "decValue = " << decValue << endl;



}
