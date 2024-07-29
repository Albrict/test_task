#pragma once
#include <iostream>

namespace OP {
    inline void LogError(const std::string &error)
    { std::cerr << "Error: " << error << '\n'; }
}
