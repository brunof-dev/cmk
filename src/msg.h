#pragma once

// General modules
#include <cstdio>
#include <cstdarg>

// Local modules
#include "behav.h"

namespace msg {
    void debug(const char* const format, ...);
    void info(const char* const format, ...);
}
