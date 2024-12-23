#include "../include/NativeFunctions.hpp"

#include <chrono>

// NativeClock
std::any NativeClock::call(Interpreter& interpreter, const std::vector<std::any>& arguments) {
    return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
}
