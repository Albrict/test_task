#include <regex>
#include "parse.hpp"
using OP::Object;

std::vector<Object> OP::ParseFile(const file_buffer &buffer)
{
    const std::regex    pattern(R"(^([^\s]+)\s(-?\d+(\.\d+)?)\s(-?\d+(\.\d+)?)\s([^\s]+)\s(\d+(\.\d+)?)$)");
    std::vector<Object> objects = {};
    for (const auto &string : buffer) {
        Object object;
        std::smatch matches;
        if (std::regex_match(string, matches, pattern)) {
            std::time_t time      = static_cast<std::time_t>(std::stof(matches[7]));
            std::tm    *time_info = std::localtime(&time);

            object.name           = matches[1];
            object.position.x     = std::stof(matches[2]);
            object.position.y     = std::stof(matches[4]);
            object.type           = matches[6];
            object.date.tm        = *time_info;
            object.date.unix_time = std::stof(matches[7]);
        }
        objects.push_back(object);
    }
    return objects;
}
