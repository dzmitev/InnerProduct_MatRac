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


//##include "cryptoapp/hrtimer.h"

//#include "cryptopp/modes.h"
//#include "cryptopp/filters.h"

using namespace std;
using namespace CryptoPP;


random_device rd;
mt19937_64 gen(rd());
AutoSeededRandomPool rng;

unsigned long long int MAX_INT_VALUE = numeric_limits<int>::max() - 1;
Integer MAX_INT_INTEGER = Integer(MAX_INT_VALUE);

Integer intToInteger(int a)
{
    Integer x(a);
    return x;
}

Integer minn(Integer a, Integer b)
{
    if(a > b)
    {
        return b;
    }

    return a;
}

Integer maxx(Integer a, Integer b)
{
    if(a > b)
    {
        return a;
    }  
    return b;
}


Integer generateCryptoSecureRandomInteger(int bits) 
{
    /* bits - number of bits of the generated number */


    // Make sure that n > 0
    if (bits <= 0) 
    {
        //cerr << "Number of bits must be positive!" <<endl;
        //return Integer::Zero();
        throw invalid_argument("Number of bits must be positive");
    }

    // Generate random bytes
    //AutoSeededRandomPool rng;
    SecByteBlock bytes(bits / 8 + 1);
    rng.GenerateBlock(bytes, bytes.size());

    // From the generated bytes get an integer
    Integer result;
    result.Decode(bytes.BytePtr(), bytes.size());

    // Truncate if necessary
    result >>= (result.BitCount() - bits); 

    return result;
}


Integer mersenneTwister(unsigned long long int upperBound)
{
    /*the function returns a random value from [3,..., upperBound-1].
     *This will be used for generating a generator of the group of quadratic residues
     */
    uniform_int_distribution<unsigned long long> \
        dis(3, upperBound-1);

    unsigned long long random_number = dis(gen);
    
    return Integer(random_number);
}

bool isGeneratorOfQuadraticResidues(Integer g, Integer p)
{
    /*g - candidate for generator
      p - safe prime */

    Integer q = (p - 1) / 2;
    //Integer interm = a_exp_b_mod_c(g, 2, p);

    if(a_exp_b_mod_c(g,2,p) != 1 && a_exp_b_mod_c(g,q,p) != 1)
    {
        return true;
    }
    return false;
}

Integer generatorOfQuadraticResidues(Integer p)
{
    /*p -- prime of the form 2q + 1*/
    //Integer bnd = minn(p, MAX_ULL_INTEGER)
    unsigned long long int bnd = -1;
    if(p > MAX_INT_VALUE)
    {
        bnd = MAX_INT_VALUE;
    }
    else
    {
        bnd = static_cast<unsigned long long int>(p.ConvertToLong());
    }

    while(true)
    {
        //g = mersenneTwister(p);
        //cout << "p = " << p <<endl;
        //Integer g = mersenneTwister(minn(p, MAX_LL_INTEGER));
        Integer g = mersenneTwister(bnd);
        //cout << "g = " << g <<endl;
        if(isGeneratorOfQuadraticResidues(g, p))
        {
            return g;
        }
    }

}

vector<Integer> setup(int bits, unsigned int B, unsigned int l)
{
    /* bits -- number of bits of the safe prime
     * B -- upper bound on the elements of the vector
     * l -- dimension of the vector
    */

    // Make sure that bits > 0, B > 0, l > 0
    if (bits <= 0) 
    {
        //cerr << "Number of bits must be positive!" <<endl;
        //return Integer::Zero();
        throw invalid_argument("Number of bits must be positive");
    }
    
    if (B <= 0) 
    {
        //cerr << "The bound B must be positive!" <<endl;
        //return Integer::Zero();
        throw invalid_argument("The bound B must be positive");
    }

    if (l <= 0) 
    {
        //cerr << "The dimension l must be positive!" <<endl;
        //return Integer::Zero();
        throw invalid_argument("The dimension l must be positive");
    }


    //AutoSeededRandomPool rng;
    PrimeAndGenerator pg(1, rng, bits);
    Integer p = pg.Prime();
    Integer q = pg.SubPrime();

    Integer sum_size = Integer(l*B*B);
    if(sum_size >= q)
    {
        throw invalid_argument("l*B*B must be smaller than q!");
    }
    
    Integer g = generatorOfQuadraticResidues(p);

    //generate the secret key <s> and public key <h>
    vector<Integer> s;
    vector<Integer> h;
    for (int i = 0; i < l; i++)
    {
        s.push_back(generateCryptoSecureRandomInteger(bits).Modulo(q));
        h.push_back(a_exp_b_mod_c(g, s[i], p));
    }
    //generate the public key

    //return (mpk, msk) = ((g,q,h), s)
    return s;
}

int main()
{
    //take care that the minn function for random numbers is doing its job here
    //Integer l = generatorOfQuadraticResidues(Integer(11));
    PrimeAndGenerator pg1(1, rng, 16);
    PrimeAndGenerator pg2(1, rng, 16);
    PrimeAndGenerator pg3(1, rng, 16);
    //cout << pg1.Prime()<<endl;
    //cout<< pg2.SubPrime()<<endl;
    //cout<< pg3.SubPrime()<<endl;

    Integer p4 = pg1.Prime();
    //cout<<p4<<endl;
    vector<Integer> s = setup(2048,10000, 10);
    for(int i = 0; i < s.size(); i++)
    {
        cout<<1<<endl;
    }
    //cout << minn(2,3) << endl;
}