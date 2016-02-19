// -*- C++ -*-
#pragma once

#include <chrono>

using std::chrono::steady_clock;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::duration_cast;


class Timer
{
public:

    Timer()
    {
        reset();
    }

    void reset()
    {
        start = now();
    }

    microseconds elapsedTimeUs()
    {
        return duration_cast<microseconds>(now() - start);
    }

    milliseconds elapsedTimeMs()
    {
        return duration_cast<milliseconds>(now() - start);
    }

private:

    steady_clock::time_point now()
    {
        return steady_clock::now();
    }

private:

    steady_clock::time_point start;
};
