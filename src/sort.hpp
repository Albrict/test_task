#pragma once
#include <unordered_map>
#include "parse.hpp"

namespace OP {
    using group         = std::vector<Object>;
    using sorted_groups = std::unordered_map<std::string, group>;

    [[nodiscard]] sorted_groups SortByDistance(group objects);     
    [[nodiscard]] sorted_groups SortByName(group objects);
    [[nodiscard]] sorted_groups SortByDate(group objects);
    [[nodiscard]] sorted_groups SortByType(group objects);
}
