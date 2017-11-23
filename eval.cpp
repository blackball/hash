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
Mean(const std::vector<T> &ts)
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

    printf("Speed(ns): %lf\n", Mean(ts));
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


int
main(int argc, char *argv[]) {    
    Eval<Vec3S, Hash_Vec3S_Rehash, 2048>("Hash_Vec3S_Rehash");    
    Eval<Vec3S, Hash_Vec3S, 2048>("Hash_Vec3S");
    
    //Eval<Vec3F, Hash_Vec3F, 2048>("Hash_Vec3F");
    //Eval<Vec3F, Hash_Vec3F_Cast, 2048>("Hash_Vec3F_Cast");

    return 0;
}
