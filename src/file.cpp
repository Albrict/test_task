#include <fstream>
#include <sstream>
#include <regex>
#include "file.hpp"
#include "log.hpp"

[[nodiscard]] static bool ValidateString(const std::string &line);

std::optional<OP::file_buffer> OP::ReadFileAndValidate(const std::filesystem::path &path)
{
    std::ifstream infile(path.string());
    if (infile.is_open() == false) {
        LogError("Can't open a file!");
        return {};
    }

    std::string  line        = {};
    file_buffer  buffer      = {};
    unsigned int line_number = 1;

    while (std::getline(infile, line)) {
        if (ValidateString(line) == true) {
            buffer.push_back(line);
            ++line_number;
        } else {
            std::stringstream error;
            error << "File is invalid at line " << line_number;
            LogError(error.str());
            return {};
        }
    }
    infile.close();
    return buffer;
}

[[nodiscard]] static bool ValidateString(const std::string& line)
{
    const std::regex pattern(R"(^([^\s]+)\s(-?\d+(\.\d+)?)\s(-?\d+(\.\d+)?)\s([^\s]+)\s(\d+(\.\d+)?)$)");
    return std::regex_match(line, pattern);
}
