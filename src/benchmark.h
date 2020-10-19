#pragma once

#include <chrono>
#include <iostream>

class Benchmark {

    private:
        std::chrono::time_point<std::chrono::system_clock> m_start;
        std::chrono::time_point<std::chrono::system_clock> m_end;
        std::chrono::duration<float, std::milli> m_elapsed;

    public:
        Benchmark();
        void start();
        float end(const char* const msg = "elapsed");
};
