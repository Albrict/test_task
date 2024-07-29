#pragma once
#include <filesystem>
#include <optional>
#include "def.hpp"

namespace OP {
    using file_buffer = std::vector<std::string>;
    std::optional<file_buffer> ReadFileAndValidate(const std::filesystem::path &path);
}
