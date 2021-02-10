#include "benchmark.h"

Benchmark::Benchmark() {
}

void Benchmark::start() {
    m_start = std::chrono::system_clock::now();
}

float Benchmark::end() {
    m_end = std::chrono::system_clock::now();
    m_elapsed = m_end - m_start;
    return(m_elapsed.count());
}
