#pragma once

#include <stdint.h>

template<typename T>
struct Vec2
{
    Vec2(){}
    Vec2(const T a, const T b):x(a), y(b){}
    
    inline bool operator==(const T o)
    {
        return x == o.x && y == o.y;
    }
    
    T x, y;
};


template<typename T>
struct Vec3
{
    Vec3(){}
    Vec3(const T a, const T b, const T c):x(a), y(b), z(c) {}
    
    inline bool operator==(const T o)
    {
        return x == o.x && y == o.y;
    }
    
    T x, y, z;
};

typedef Vec2<int16_t> Vec2S;
typedef Vec2<int32_t> Vec2I;
typedef Vec2<float>   Vec2F;
typedef Vec3<int16_t> Vec3S;
typedef Vec3<int32_t> Vec3I;
typedef Vec3<float>   Vec3F;


