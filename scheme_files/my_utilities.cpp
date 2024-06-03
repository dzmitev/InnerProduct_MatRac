#include <iostream>
#include <iomanip>
#include <bitset>
#include <vector>
#include <random>
#include <crypto++/nbtheory.h>
#include <crypto++/integer.h>
#include <crypto++/osrng.h>
#include <cryptopp/rng.h>

#include <sodium.h>
#include <sstream>

#include "KeyPair.h"
#include "my_utilities.h"

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

Integer minn(const Integer& a, const Integer& b)
{
    if (a > b)
    {
        return b;
    }

    return a;
}

Integer maxx(const Integer& a, const Integer& b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}

Integer mersenneTwister(unsigned long long int upperBound)
{
    /*the function returns a random value from [3,..., upperBound-1].
     *This will be used for generating a generator of the group of quadratic residues
     */
    uniform_int_distribution<unsigned long long>
        dis(3, upperBound - 1);

    unsigned long long random_number = dis(gen);

    return Integer(random_number);
}

Integer generatorOfQuadraticResidues(const Integer& p)
{
    /*p -- prime of the form 2q + 1 where q is also prime*/
    

    unsigned long long int bnd = -1;
    if (p > MAX_INT_VALUE)
    {
        bnd = MAX_INT_VALUE;
    }
    else
    {
        bnd = static_cast<unsigned long long int>(p.ConvertToLong());
    }

    while(true)
    {
        Integer root = mersenneTwister(bnd);

        //g = root^2 which is a quad. residue
        Integer g = a_times_b_mod_c(root, root, p);

        if(g % p != 1)
        {
            return g;
        }
    }
}

Integer babyStepGiantStep(const Integer& alpha, const Integer& beta, const Integer& p, const Integer& bound)
{
    /*
     * Return x such that alpha^x = beta (mod p)
     * bound - parameter, that bounds the search for the disc. log. from above
     */

    Integer m = Integer(bound.SquareRoot() + 1);
    map<Integer, Integer> T;

    // fill the map
    Integer alpha_j = Integer(1);

    for (long long int j = 0; j < m; j++)
    {
        T[alpha_j] = Integer(j);
        alpha_j = a_times_b_mod_c(alpha_j, alpha, p);
    }

    // Setting interm = alpha^(-m) (mod p)
    Integer interm = Integer(alpha.InverseMod(p));
    interm = a_exp_b_mod_c(interm, m, p);

    // Giant step calculation
    Integer gamma = Integer(beta);
    for (long long int i = 0; i < m; i++)
    {
        // Check if gamma is in the baby steps map
        if (T.count(gamma) > 0)
        {
            // Solution found! return it
            return Integer(T.at(gamma) + (i * m));
        }

        // Move to the next giant step
        gamma = a_times_b_mod_c(gamma, interm, p);
    }

    // No solution in {0..., bound}
    return Integer(-1);
}

bool isProbablePrime(const Integer& n, int k) 
{
    /*
    check if a candidate is probable prime using the Miller-Rabin test
        k -- number of iterations to be performed    
    */

    //check for boundary cases
    if(n <= 1)
    {
        return false;
    }

    if(n <= 3)
    {
        return true;
    }
    
    // before proceeding to the M-R test
    // check if the candidate is divisible by the first few primes
    int fewPrimes[] = {2, 3, 5, 7, 11, 13, 17, 19,\
     23, 29, 31, 37, 41, 43, 47, 53, 59, \
     61, 67, 71, 73, 79, 83, 89, 97};

    for(int i = 0; i < sizeof(fewPrimes) / sizeof(fewPrimes[0]); i++)
    {
        if(n % fewPrimes[i] == 0)
        {
            return false;
        }
    }

    // Find r, d such that n - 1 = (2^r * d) for d odd
    Integer d = n - 1;
    int r = 0;
    
    while(d % 2 == 0) 
    {
        d = d / 2;
        r++;
    }

    // AutoSeededRandomPool rng;
    for (int i = 0; i < k; i++) 
    {
        Integer a = Integer(rng, 2, n - 2); // Random integer in [2, n-2]
        Integer x = a_exp_b_mod_c(a, d, n);

        if(x == 1 || x == n - 1) //x = +-1 mod(n)
            continue;

        bool composite = true;
        for(int j = 0; j < r - 1; j++)
        {
            Integer y = a_times_b_mod_c(x, x, n);
            if(y == 1)
            {
                return false;
            }

            if(y == n - 1) 
            {
                composite = false;
                break;
            }
        }

        if(composite == true)
        {
            return false;
        }
    }

    return true;
}

bool isVectorInBound(vector<Integer> vec, unsigned long long int bound)
{
    for(long long int i = 0; i < vec.size(); i++)
    {
        if(vec[i] > bound)
        {
            cout << "vec[" << i <<"] = " << vec[i] << "; ";
            cout << "bound = " << bound << endl;
            return false;
        }
    }
    return true;
}

Integer scalarProduct(vector<Integer> v1, vector<Integer> v2)
{
    if(v1.size() != v2.size())
    {
        throw invalid_argument("Cannot compute scalar product of vectors of different sizes!");
    }
    Integer s = 0;

    for(int i = 0; i < v1.size(); i++)
    {
        s = s + v1[i] * v2[i];
    }
    return s;

}
vector<Integer> randomIntegerVector(long long int size, long long int upperBound)
{
    vector<Integer> vec;
    for(long long int i = 0; i < size; i++)
    {
        vec.push_back(Integer(rng, 3, upperBound));
    }
    return vec;
}
