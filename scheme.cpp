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

Integer intToInteger(int a)
{
    Integer x(a);
    return x;
}

Integer minn(Integer a, Integer b)
{
    if(a > b)
    {
        return Integer(a);
    }

    return Integer(b);
}

Integer mersenneTwister(Integer p)
{
    // return a random number from [3, UNSIGNED_LONG_MAX]
    random_device rd;
    mt19937_64 gen(rd());
    //Integer p_copy = Integer(p);
    unsigned long long upperBound = -1;

    Integer bnd = minn(p, numeric_limits<unsigned long long>::max() - 1);

    upperBound = static_cast<unsigned long long>(bnd.ConvertToLong());
    
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

Integer generatorOfQuadraticReisidues(Integer p)
{
    /*p -- prime of the form 2q + 1*/
    Integer g;
    while(true)
    {
        g = mersenneTwister(p);
        cout << g <<endl;
        if(isGeneratorOfQuadraticResidues(g, p))
        {
            return g;
        }
    }

}

Integer setup(int bits)
{
    AutoSeededRandomPool rng;
    PrimeAndGenerator pg(1, rng, bits);
    Integer p = pg.Prime();
    Integer q = pg.SubPrime();
    
    Integer g = generatorOfQuadraticReisidues(p);


    //return (mpk, msk) = ((g,q,h), s)
    return p;
}

int main()
{
    Integer l = generatorOfQuadraticReisidues(Integer(11));
    
    cout << l << endl;
}