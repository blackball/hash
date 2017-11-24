#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "timer.h"
#include "types.h"
#include "hashs.h"

static void
GenerateDB(std::vector<Vec3S> &db) {
    srand(47853); // fix seed 
    //db.resize(65536 * 65536);
    db.resize(1000000);
    for (int i = 0; i < db.size(); ++i) {
        db[i] = Vec3S(rand()/INT16_MAX, rand()/INT16_MAX, rand()/INT16_MAX);
    }
}


static void
GenerateDB(std::vector<Vec3F> &db) {
    srand(47853); // fix seed 
    //db.resize(65536 * 65536);
    db.resize(1000000);
    for (int i = 0; i < db.size(); ++i) {
        db[i] = Vec3F(rand(), rand(), rand());
    }
}


template<typename T>
static double
CalcMean(const std::vector<T> &ts)
{
    double sum = 0;
    for (int i = 0; i < ts.size(); ++i)
        sum += ts[i];
    if (sum == 0) return -1.0;
    return sum / ts.size();
}


template<typename T, class H>
static void 
SpeedTest(const std::vector<T> &db, int trials = 20) {
    if (db.size() < trials) return ;
    
    std::vector<double> ts(trials, 0);
    Timer timer;
    H hash; 
    for (int i = 0; i < trials; ++i) {
        timer.Start();        
        hash(db[i]); // here may have some kind of cache miss ... 
        timer.Stop();
        ts[i] = timer.GetDurationNS();
    }

    printf("Speed(ns): %lf\n", CalcMean(ts));
}

/**
 *  \brief Calculate the quality of the hash function by using the formula from "Red Dragon Book"
 *
 * if the function is ideal, the formula should give 1.
 * In reality, a good function is somewhere between 0.95 and 1.05.
 * If it's more, there is a high number of collisions (slow!).
 * If it's less, the function gives less collisions than the randomly
 * distributing function, which is not bad.
 *
 * \return double quality 
 */
static double
CalcQuality(const std::vector<std::size_t> &slots) {
    double m = slots.size(), n = 0, quality = 0;
    for (const auto v : slots)
        n += v;
    for (const auto v : slots)
        quality += v * (v + 1);
    
    quality /= 2.0 * (n + 2*m - 1) * n / (2*m);
    return quality;
}

/**
 *  \brief check the collision distribution
 *
 *  Use the formula from the dragon book: http://www.strchr.com/hash_functions (search "Red Dragon Book" on this page)
 * 
 *  NOTE: there're many different tests, for example http://burtleburtle.net/bob/hash/avalanche.html
 *  but I am not sure if it's a good test for the hashing function for 2D or 3D points 
 *
 */
template<typename T, class H, size_t BINNUM>
static void 
CollisionTest(const std::vector<T> &db) {
    if (db.size() <= 0) return ;
    
    std::vector<std::size_t> slots(BINNUM, 0);  // slots used to evalue the collision
    H hash; 
    for (size_t i = 0; i < db.size(); ++i) {
        ++ slots[hash(db[i])%BINNUM];
    }
    
    double quality = CalcQuality(slots);
    printf("Quality:   %lf\n", quality);
}

// evaluate if a hash function is good or not 
template<typename T, class H, size_t BINNUM>
static void
Eval(const char *name) {
    printf("Eval: %s\n", name);    
    std::vector<T> db;
    GenerateDB(db);    
    SpeedTest<T, H>(db);
    CollisionTest<T, H, BINNUM>(db);
}


struct Hash_Old
{
    std::size_t operator()(const Vec3F &pt) const 
    {
        std::hash<double> H; 
        const size_t h1 = H(pt.x);
        const size_t h2 = H(pt.y);
        const size_t h3 = H(pt.z);
        return 73856093 * h1 ^ 19349669 * h2 ^ 83492791 * h3;
    }    
};


struct Hash_New
{
    std::size_t operator()(const Vec3F &pt) const 
    {
        const size_t h1 = std::hash<double>()(pt.x);
        const size_t h2 = std::hash<double>()(pt.y);
        const size_t h3 = std::hash<double>()(pt.z);
        return 73856093 * h1 ^ 19349669 * h2 ^ 83492791 * h3;
    }    
};


template<class H>
static void
Bench(unsigned int seed = 7653) {
    srand((unsigned int)time(NULL));
    const int N = 10000;

    Timer timer; 
    H hash;
    
    std::vector<double> ts(N);
    std::vector<std::size_t> hvs(N);
    
#define RAND() static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10000))
    for (int i = 0; i < N; ++i)
    {
        const Vec3F v(RAND(), RAND(), RAND());

        timer.Start();
        const std::size_t hv = hash(v);
        timer.Stop();

        hvs[i] = hv; 
        ts[i] = timer.GetDurationNS();
    }
    printf("mean: %lf\n", CalcMean(ts));
}

static void
BenchDriver()
{
    Bench<Hash_New>(898918);
    Bench<Hash_Old>(898918);


    Bench<Hash_New>(8933918);
    Bench<Hash_Old>(8933918);

    Bench<Hash_New>(89339118);
    Bench<Hash_Old>(89339118);    
}

static void
Test()
{
    srand(21321);
    Hash_Vec3F_Cast H; 
    for (int i = 0; i < 10; ++i)
    {
        //const float f = rand();
        const std::size_t ii = rand();
        const float f = static_cast <float> (ii) / (static_cast <float> (RAND_MAX/10000));
        Vec3F v(f,f,f);
        printf("ii: %lu, val: %f, hash: %lu\n", ii, f, H(v));
    }
}


int
main(int argc, char *argv[]) {
    
    // Eval<Vec3S, Hash_Vec3S_Rehash, 2048>("Hash_Vec3S_Rehash");    
    // Eval<Vec3S, Hash_Vec3S, 2048>("Hash_Vec3S");
    
    // Eval<Vec3F, Hash_Vec3F, 2048>("Hash_Vec3F");
    // Eval<Vec3F, Hash_Vec3F_ByVal, 2048>("Hash_Vec3F_ByVal");
    // Eval<Vec3F, Hash_Vec3F_Opt, 2048>("Hash_Vec3F_Opt");
    // Eval<Vec3F, Hash_Vec3F_Opt_ByVal, 2048>("Hash_Vec3F_Opt_ByVal");

    // Test();
    //Eval<Vec3F, Hash_Vec3F_Cast, 2048>("Hash_Vec3F_Cast");

    BenchDriver();
    return 0;
}
