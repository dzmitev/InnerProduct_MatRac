#include <iostream>
#include <vector>
#include <crypto++/nbtheory.h>
#include <crypto++/osrng.h>
#include <crypto++/integer.h>
#include <chrono>
#include <numeric>
#include <algorithm>

using namespace std;
using namespace CryptoPP;

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

void generateSafePrime(int bits)
{
    AutoSeededRandomPool rng;
    PrimeAndGenerator pg(1, rng, bits);
}

int main()
{
    vector<int> bitsVals = {1024, 2048, 4096};
    
    int iters = 50;

    cout << "Bit size, avg_time (s), best_time (s), worst_time (s)" << endl;
    for (int bits : bitsVals)
    {
        auto [avgTime, bestTime, worstTime] = measureTime(generateSafePrime, iters, bits);
        cout << bits << ", " << avgTime << ", " << bestTime << ", " << worstTime << "\n";
    }

    return 0;
}
