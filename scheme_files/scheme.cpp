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

    Integer q = mpk.getSubPrime();
    

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

// -----------------------------------PERFORMING TESTS------------------------------------
// ----REMOVE THE COMMENTS BELOW AND COMMENT THE EXAMPLE USAGE IN ORDER TO PERFORM THE TESTS YOURSELF----

/*
#include <chrono>
#include <numeric>
#include <algorithm>
template <typename Func, typename... Args>

tuple<double, double, double> measureTime(Func func, int iters, Args&&... args) 
{
    vector<double> times;
    for (int i = 0; i < iters; i++) 
    {
        auto start = chrono::high_resolution_clock::now();
        func(forward<Args>(args)...);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double> elapsed = end - start;
        times.push_back(elapsed.count());
    }

    double avgTime = accumulate(times.begin(), times.end(), 0.0) / times.size();
    double bestTime = *min_element(times.begin(), times.end());
    double worstTime = *max_element(times.begin(), times.end());
    return make_tuple(avgTime, bestTime, worstTime);
}

int main()
{
    
    vector<int> lVals = {1000, 10000, 100000}; 
    vector<int> BVals = {10000, 100000, 1000000};  
    vector<int> bitsVals = {1024, 2048, 4096};      

    // generate primes beforehand so that -setup- will execute faster
    // we will perform the tests with a 1024, 2048 and 4096-bit safe primes
    map<int, Integer> bitsToPrime; 
    bitsToPrime[1024] = Integer("139259988675681141496060356544260911883145824406069142262293697297539663563694801869649008068759183465677926805752822997993578606296302510255175531986529352634604705939809332890759760387712305597648735028531508550704932967512133746128880478183424463236454253391405843653061863925772676774557945991026476898087");
    bitsToPrime[2048] = Integer("25748580094613158775606410992511563041496816732077453058276402511823221330132040107596415532544147480032149419472985439752287914803320936447690947564760661305013154719581761993374770224732960944973348216975342218478387814399659722103932318107194214012125639363332262135084855713148759499624886904659201219405107748196395325431553905556192811571907607327667524513674094932909614996446273822162756094521919958900339863368135387066701212524992030385824615618680454528874410708873578012514976806828449116437582704960610408769748152731083545368007406049308403905343443043227164849865920367475749567550486711961820335677539");
    bitsToPrime[4096] = Integer("881028188502072853270516327809681150187012328448930356849815361183130043301041153865540847021474049893260510009359426622904627930171238796655132147893403023851444171499620437459770364449469265276734932260194852812233199097556926081302503550086490829507349488257198667726147363620673307172607153206415574411330941218488124658535542462214699954252478716028868407795921972257713972818640581480095481018040632146352881162244747964851879912994504568228305524457764072600478493679023030872005734144942079568129115292786460298352757371325916996231982969056408527724141986977569189963060147264182975735348296285776213505296828957784679077280849966396451664650697823717638779006947514073461751245020084866074596620480130894057918071917437651834240263645828436057914881377580198607185077736775448695566192183533893416798810069412041675654863833882232144857575841264778143363293522985257127357481999129067673857343795486702421401584980857405149376038458663054794072066510648219325944828312230112874954874187610731183985018337991469246893223687581436594449909276283926170190444393799895145926614855026562774584379000351750250386726639752699931491802519622294623353942825902977011034547066495192556286227973960087409264863517249323272434815282059");


    int iters = 50;

    for (int l : lVals)
    {
        for (int B : BVals)
        {
            for (int bits : bitsVals)
            {
                cout << "Testing with l = " << l << ", B = " << B << ", bits = " << bits << endl;

                // Measure setup
                auto [setupAvg, setupBest, setupWorst] = measureTime(setup, iters, l, bits, B, bitsToPrime[bits]);
                cout << "Setup:      Avg: " << setupAvg << "s, Best: " << setupBest << "s, Worst: " << setupWorst << "s" << endl;

                // Generate random vectors for testing
                vector<Integer> plaintextVec = randomIntegerVector(l, B);
                vector<Integer> keyVec = randomIntegerVector(l, B);

                KeyPair key = setup(l, bits, B, bitsToPrime[bits]);
                MasterPublicKey pk = key.getMasterPublicKey();
                MasterSecretKey sk = key.getMasterSecretKey();

                // Measure encrypt
                auto encrypt_func = [&]() { return encrypt(pk, plaintextVec, B); };
                auto [encAvg, encBest, encWorst] = measureTime(encrypt_func, iters);
                cout << "Encrypt:    Avg: " << encAvg << "s, Best: " << encBest << "s, Worst: " << encWorst << "s" << endl;
                

                // Measure keygen
                auto keygen_func = [&]() { return keygen(key, keyVec, B); };
                auto [keygenAvg, keygenBest, keygenWorst] = measureTime(keygen_func, iters);
                cout << "Keygen      Avg: " << keygenAvg << "s, Best: " << keygenBest << "s, Worst: " << keygenWorst << "s" << endl;

                // Measure decrypt
                Integer sk_keyVec = keygen(key, keyVec, B);
                auto decrypt_func = [&]() { return decrypt(pk, encrypt(pk, plaintextVec, B), keyVec, sk_keyVec, B); };
                auto [avg_decrypt, best_decrypt, worst_decrypt] = measureTime(decrypt_func, iters);
                cout << "Decrypt:    Avg: " << avg_decrypt << "s, Best: " << best_decrypt << "s, Worst: " << worst_decrypt << "s" << endl;
                cout << endl;
                
            }
        }
    }
    return 0;
}
*/

// -------------------------------- EXAMPLE USAGE ----------------------------------
int main()
{
    int B = 10000; // we assume that every subject is worth at most 10 ECTS
    int bits = 1024;
    int l = 100;
    vector<Integer> plaintextVec = randomIntegerVector(l, B);
    vector<Integer> keyVec = randomIntegerVector(l, B);

    // 1024-bit safe prime
    //Integer p1("136607070847329078202190522504350756341844973016949670397906556513586960612188818280604150245997995440972736560804417244040369560156029916789431171583879198790487519180250259136321970832683028539422904064797240421435541885698081957638584796566250762491993938886264096354704395440135477295514059359739396691483");
    //KeyPair key = setup(l, bits, B, p1)
    
    // let setup generate a safe prime
    KeyPair key = setup(l, bits, B);
    MasterPublicKey pk = key.getMasterPublicKey();
    MasterSecretKey sk = key.getMasterSecretKey();

    vector<Integer> Ct = encrypt(pk, plaintextVec, B);
    Integer sk_keyVec = keygen(key, keyVec, B);
    Integer decValue = decrypt(pk, Ct, keyVec, sk_keyVec, B);

    Integer innerProd = scalarProduct(plaintextVec, keyVec);
    cout << "innerProd = " << innerProd << endl;
    cout << "decValue = " << decValue << endl;

    return 0;
} 