#include <iostream>
//#include <iomanip>
//#include <bitset>
#include <vector>
//#include <random>
//#include <crypto++/nbtheory.h>
#include <crypto++/integer.h>
#include <crypto++/osrng.h>
//#include <sodium.h>
//#include <sstream>
//#include <numeric>
//
//// ##include "cryptoapp/hrtimer.h"
//
//// #include "cryptopp/modes.h"
//// #include "cryptopp/filters.h"

#include "KeyPair.h"
#include "MasterSecretKey.h"
#include "MasterPublicKey.h"
#include "my_utilities.h"
using namespace std;
using namespace CryptoPP;

// random_device rd;
// mt19937_64 gen(rd());
// AutoSeededRandomPool rng;


KeyPair setup(int l, int bits, long long int B)
{
    // bits -- number of bits of the safe prime
    // B -- upper bound on the elements of the vector
    // l -- dimension of the vector
     

    // Make sure that bits > 0, B > 0, l > 0
    if (bits <= 0)
    {
        // cerr << "Number of bits must be positive!" <<endl;
        // return Integer::Zero();
        throw invalid_argument("Number of bits must be positive");
    }
    //cout << "bit check passed" << endl;

    if (B <= 0)
    {
        // cerr << "The bound B must be positive!" <<endl;
        // return Integer::Zero();
        throw invalid_argument("The bound B must be positive");
    }
    //cout << "B check passed" << endl;

    if (l <= 0)
    {
        // cerr << "The dimension l must be positive!" <<endl;
        // return Integer::Zero();
        throw invalid_argument("The dimension l must be positive");
    }
    //cout << "l check passed" << endl;

    KeyPair key(l, bits);
    //cout << "key generated for l = " << l << " and bits = " << bits << endl;
    //Integer q = (key.getMasterPublicKey()).getSubPrime();
    //MasterPublicKey mpk1 = key.getMasterPublicKey();
    //MasterSecretKey msk1 = key.getMasterSecretKey();

    Integer q = key.getMasterPublicKey().getSubPrime();
    //cout << "in if " << endl;
    if(l * B * B >= q)
    {
        throw invalid_argument\
            ("l * B * B >= q! You need a larger prime or a smaller bound!");
    }
    //cout << "before return "<<endl;
    return key;
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

    // caluclate the product
    // prod = (ct_1 ^ y_1 * ... * ct_l ^ y_l) / ct_0^(sk_y)

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

//
int main()
{

//    KeyPair key = setup(3, 32, 10000); //length, bits, bound
//    MasterPublicKey pk = key.getMasterPublicKey();
//    MasterSecretKey sk = key.getMasterSecretKey();
//    vector<Integer> pt = {10, 10, 10};
//    int B = 10000;
//    vector<Integer> ct = encrypt(pk, pt, B);
//    vector<Integer> y = {1, 1, 1};
//    Integer sk_y = keygen(key, y, B);
//
//    for(int i = 0; i < ct.size(); i++)
//    {
//        cout <<"ct["<<i<<"] = "<< ct[i] <<" ";
//    }
//    cout<<endl;
//
//    for(int i = 0; i < y.size(); i++)
//    {
//        cout <<"y["<<i<<"] = "<< y[i] <<" ";
//    }
//    cout<<endl;
//
//    cout << sk_y << endl;
//    //cout << "bits for prime: " << (key.getSafePrime()).BitCount() << endl;
//    //cout << "safePrime = " << key.getMasterPublicKey().getSafePrime() << endl;
//    //cout << "subPrime  = " << key.getMasterPublicKey().getSubPrime() << endl;
//    //key.getMasterSecretKey().printPrivateKey();
//    //key.getMasterPublicKey().printPublicKey();

    vector<Integer> grades = {8, 7,  9, 6, 10, 6, 7, 9, 6, 8, \
          7, 10, 8, 9, 8, 8, 9, 9, 7, 9, \
          7, 9 , 8, 10, 8, 7, 7, 9, 9, 10};

    vector<Integer> ects = {7, 7, 6, 6, 10, 5, 7, 7, 5, 5, 7, 10, \
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, \
        3,3,3,3};
    int l = grades.size();
    int B = 11; // we assume that every subject is worth at most 10 ECTS
    int bits = 128;

    KeyPair key = setup(l, bits, B);
    MasterPublicKey pk = key.getMasterPublicKey();
    MasterSecretKey sk = key.getMasterSecretKey();

    vector<Integer> Ct = encrypt(pk, grades, B);
    Integer sk_ects = keygen(key, ects, B);
    Integer decValue = decrypt(pk, Ct, ects, sk_ects, B);

    Integer innerProd = scalarProduct(grades, ects);
    //cout << "innerProd = " << innerProd << endl;
    //cout<<babyStepGiantStep(5, 45, 131, 130);
    cout << "decValue = " << decValue << endl;



}
