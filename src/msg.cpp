#include "msg.h"

void msg::debug(const char* const format, ...) {
    if (behav::DEBUG) {
        char buffer[256];
        std::va_list args;
        va_start(args, format);
        std::vsnprintf(buffer, sizeof(buffer), format, args);
        std::printf("[DEBUG]: %s", buffer);
        va_end(args);
    }
}

void msg::info(const char* const format, ...) {
    char buffer[256];
    std::va_list args;
    va_start(args, format);
    std::vsnprintf(buffer, sizeof(buffer), format, args);
    std::printf("[INFO]: %s", buffer);
    va_end(args);
}
