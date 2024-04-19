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

// ##include "cryptoapp/hrtimer.h"

// #include "cryptopp/modes.h"
// #include "cryptopp/filters.h"
#include "my_utilities.h"
#include "KeyPair.h"

using namespace std;
using namespace CryptoPP;

// random_device rd;
// mt19937_64 gen(rd());
// AutoSeededRandomPool rng;


KeyPair setup(int l, int bits, unsigned long long int B)
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
    cout << "bit check passed" << endl;

    if (B <= 0)
    {
        // cerr << "The bound B must be positive!" <<endl;
        // return Integer::Zero();
        throw invalid_argument("The bound B must be positive");
    }
    cout << "B check passed" << endl;

    if (l <= 0)
    {
        // cerr << "The dimension l must be positive!" <<endl;
        // return Integer::Zero();
        throw invalid_argument("The dimension l must be positive");
    }
    cout << "l check passed" << endl;

    KeyPair key(l, bits);
    cout << "key generated for l = " << l << " and bits = " << bits << endl;
    Integer q = key.getSubPrime(); 
    cout << "in if " << endl;
    if(l * B * B >= q)
    {
        throw invalid_argument\
            ("l * B * B >= q! You need a larger prime or a smaller bound!");
    }
    cout << "before return "<<endl;
    return key;
}

int main()
{
    
    KeyPair key = setup(100, 1024, 10000); //length, bits, bound
    cout << "bits for prime: " << (key.getSafePrime()).BitCount() << endl;
}
