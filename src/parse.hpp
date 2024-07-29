#pragma once
#include "def.hpp"
#include <ctime>

namespace OP {
    struct Object {
        struct Position {
            float x = {};
            float y = {};
            
            friend bool operator<(const Position &a, const Position &b)
            { return a.x < b.x || a.y < b.y; }

            friend bool operator>(const Position &a, const Position &b)
            { return a.x > b.x || a.y > b.y; }
        };
        
        struct Date {
            std::tm tm        = {};
            float   unix_time = {};
        };

        std::string name       = {};
        std::string type       = {}; 
        Position    position   = {};
        Date        date       = {};
    };
    std::vector<Object> ParseFile(const file_buffer &buffer);
}
