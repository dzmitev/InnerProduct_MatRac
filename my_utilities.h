#pragma once

#ifndef MYUTILITIES_H
#define MYUTILITIES_H

#include <crypto++/integer.h>
#include <crypto++/integer.h>
#include <crypto++/osrng.h>
#include <vector>

using namespace std;
using namespace CryptoPP;

extern unsigned long long int MAX_INT_VALUE;
extern Integer MAX_INT_INTEGER;

// Function to convert int to Integer
Integer intToInteger(int a);

// Function to find minimum of two Integers
Integer minn(const Integer& a, const Integer& b);

// Function to find maximum of two Integers
Integer maxx(const Integer& a, const Integer& b);

// Function to generate a cryptographically secure random Integer
Integer generateCryptoSecureRandomInteger(int bits);

// Function to generate a random number for quadratic residue generation
Integer mersenneTwister(unsigned long long int upperBound);

// Function to find a generator of quadratic residues modulo safe prime
Integer generatorOfQuadraticResidues(const Integer& p);

// Function to solve discrete logarithm problem using baby-step giant-step algorithm
Integer babyStepGiantStep(const Integer& alpha, const Integer& beta, const Integer& p, const Integer& bound);

// Function that checks if an integer is probable prime using the Miller Rabin test
bool isProbablePrime(const Integer& n, int k); 

// Function that checks if all the elements of a vector are smaller or equal than a given bound
bool isVectorInBound(vector<Integer> vec, unsigned long long int bound);

// Computes the scalar product of two vectors of the same size
Integer scalarProduct(vector<Integer> v1, vector<Integer> v2);

// generate a random vector of Integers of size <size> with elements in [3, ..., upperBound]
vector<Integer> randomIntegerVector(long long int size, long long int upperBound);

#endif
