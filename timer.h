#pragma once

#include <stdio.h>
#include <chrono>

class Timer {
    std::chrono::high_resolution_clock::time_point _start, _stop;    
public:
    inline void Start() { _start = std::chrono::high_resolution_clock::now(); }
    inline void Stop() { _stop = std::chrono::high_resolution_clock::now(); }
    inline double GetDurationMS() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(_stop - _start).count();
    }
    
    inline double GetDurationNS() const {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(_stop - _start).count();
    }
};
