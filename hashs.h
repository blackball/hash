#pragma once

#include <functional>
#include "types.h"

struct Hash_Vec3S_Rehash
{
    std::size_t  operator()(const Vec3S &pt) const 
    {
        return std::hash<size_t>()(73856093 * pt.x + 19349669 * pt.y + 83492791 * pt.z);
    }
};


struct Hash_Vec3S
{
    std::size_t operator()(const Vec3S &pt) const 
    {
        return (73856093 * pt.x + 19349669 * pt.y + 83492791 * pt.z);
    }
};

struct Hash_Vec3F
{
    std::size_t operator()(const Vec3F &pt) const 
    {
        const size_t h1 = std::hash<double>()(pt.x);
        const size_t h2 = std::hash<double>()(pt.y);
        const size_t h3 = std::hash<double>()(pt.z);

        return 73856093 * h1 ^ 19349669 * h2 ^ 83492791 * h3;
    }    
};

// TODO: sth wrong with this hash function 
struct Hash_Vec3F_Cast
{
    std::size_t operator()(const Vec3F &pt) const 
    {
        const std::size_t h1 = static_cast<std::size_t>(pt.x);
        const std::size_t h2 = static_cast<std::size_t>(pt.y);
        const std::size_t h3 = static_cast<std::size_t>(pt.z);
        return 73856093 * h1 ^ 19349669 * h2 ^ 83492791 * h3;
    }
};


