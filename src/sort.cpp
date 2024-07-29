#include "sort.hpp"
#include "parse.hpp"
#include <algorithm>
#include <chrono>

static inline std::string ToLowerUTF8(const std::string &str);
static inline std::string ToUpperUTF8(const std::string &str);
static inline std::string ExtractFirstUTF8Symbol(const std::string &str);

OP::sorted_groups OP::SortByDistance(std::vector<Object> objects)
{
    auto sort_lambda = [](const OP::Object &a, const OP::Object &b) -> bool
    { return a.position < b.position ; };
    std::sort(objects.begin(), objects.end(), sort_lambda); 

    sorted_groups groups = {};
    group up_to_100      = {};
    group up_to_1000     = {};
    group up_to_10000    = {};
    group too_far        = {};
    
    for (const auto &object : objects) {
        if (object.position < Object::Position{100, 100})
            up_to_100.push_back(object);
        else if (object.position < Object::Position{1'000, 1'000})
            up_to_1000.push_back(object);
        else if (object.position < Object::Position{10'000, 10'000})
            up_to_10000.push_back(object);
        else
            too_far.push_back(object);
    }
    
    groups.emplace("Before 100", up_to_100);
    groups.emplace("Before 1000", up_to_1000);
    groups.emplace("Before 10000", up_to_10000);
    groups.emplace("Too far", too_far);

    for (auto& [key, object] : groups)
        std::sort(object.begin(), object.end(), sort_lambda);
    return groups;
}

OP::sorted_groups OP::SortByName(std::vector<Object> objects)
{
    auto sort_lambda = [](const OP::Object &a, const OP::Object &b) -> bool
    { return ToLowerUTF8(a.name) < ToLowerUTF8(b.name); };
    std::sort(objects.begin(), objects.end(), sort_lambda);

    sorted_groups groups = {};
    groups.emplace("#", group{});

    std::string   key    = {};
    for (const auto &object : objects) {
        key                  = ToUpperUTF8(object.name);
        std::string utf8_key = ExtractFirstUTF8Symbol(key);
        if (std::isdigit(key[0]) || 
            std::ispunct(key[0]) || 
            std::isalpha(key[0])) {
            groups["#"].push_back(object);
        } else if (groups.find(utf8_key) != groups.end()) {
            groups[utf8_key].push_back(object);
        } else {
            groups.emplace(utf8_key, group{object});
        }
    }
    return groups;
}

OP::sorted_groups OP::SortByDate(std::vector<Object> objects)
{
    using namespace std::chrono;

    auto sort_lambda = [](const OP::Object &a, const OP::Object &b) -> bool
    { return a.date.unix_time < b.date.unix_time; };
    
    std::sort(objects.begin(), objects.end(), sort_lambda);
    sorted_groups groups     = {};

    group created_today      = {};
    group created_yesterday  = {};
    group created_this_week  = {};
    group created_this_month = {};
    group created_this_year  = {};
    group created_before     = {}; 
    
    std::time_t current_time = system_clock::to_time_t(system_clock::now());
    std::time_t yesterday    = system_clock::to_time_t(system_clock::from_time_t(current_time) - hours(24));
    std::tm     current_tm   = *std::localtime(&current_time);
    std::tm     yesterday_tm = *std::localtime(&yesterday);
    std::time_t week         = system_clock::to_time_t(system_clock::now() - days(current_tm.tm_wday));
    std::tm     week_tm      = *std::localtime(&week);


    for (const auto &object : objects) {
        if (object.date.tm.tm_year == current_tm.tm_year && 
            object.date.tm.tm_mon == current_tm.tm_mon && 
            object.date.tm.tm_mday == current_tm.tm_mday) {
            created_today.push_back(object);
        } else if (object.date.tm.tm_year == yesterday_tm.tm_year && 
                   object.date.tm.tm_mon == yesterday_tm.tm_mon &&
                   object.date.tm.tm_mday == yesterday_tm.tm_mday) {
            created_yesterday.push_back(object);
        } else if (object.date.tm.tm_year == week_tm.tm_year &&
                   week_tm.tm_yday <= object.date.tm.tm_yday &&
                   object.date.tm.tm_yday <= current_tm.tm_yday) {
            created_this_week.push_back(object);
        } else if (object.date.tm.tm_mon == current_tm.tm_mon) {
            created_this_month.push_back(object);
        } else if (object.date.tm.tm_year == current_tm.tm_year) {
            created_this_year.push_back(object);
        } else {
            created_before.push_back(object);
        }
    }
    groups.emplace("Today", created_today);
    groups.emplace("Yesterday", created_yesterday);
    groups.emplace("This week", created_this_week);
    groups.emplace("This month", created_this_month);
    groups.emplace("This year", created_this_year);
    groups.emplace("Created before", created_before);
    return groups;
}

OP::sorted_groups OP::SortByType(std::vector<Object> objects)
{
    sorted_groups groups     = {};
    group         misc_group = {};

    for (const auto &object : objects) {
        if (groups.find(object.type) != groups.end()) {
            groups[object.type].push_back(object);
        } else {
            groups.emplace(object.type, group{object});
        }
    }

    for (auto it = groups.begin(); it != groups.end(); ) {
        if (it->second.size() <= 1) {
            misc_group.insert(misc_group.end(), it->second.begin(), it->second.end());
            it = groups.erase(it);
        } else {
            ++it;
        }
    }

    auto sort_lambda = [](const OP::Object &a, const OP::Object &b) -> bool
    { return a.position < b.position ; };
    
    if (misc_group.size() > 0)
        groups.emplace("Miscellaneous", misc_group);
    for (auto &[key, group] : groups)
        std::sort(group.begin(), group.end(), sort_lambda);
    return groups;
}

static std::string ToLowerUTF8(const std::string &str)
{
    std::string result;
    for (auto ch : str) {
        result += std::tolower(static_cast<unsigned char>(ch));
    }
    return result;
}

static inline std::string ToUpperUTF8(const std::string &str)
{
    std::string result;
    for (auto ch : str) {
        result += std::toupper(static_cast<unsigned char>(ch));
    }
    return result;
}

static inline std::string ExtractFirstUTF8Symbol(const std::string &str)
{
    if (str.empty())
        return "";

    unsigned char firstByte = str[0];
    size_t charLength = 0;

    if (firstByte < 0x80) {
        charLength = 1;
    } else if ((firstByte & 0xE0) == 0xC0) {
        charLength = 2;
    } else if ((firstByte & 0xF0) == 0xE0) {
        charLength = 3;
    } else if ((firstByte & 0xF8) == 0xF0) {
        charLength = 4;
    } else {
        return "";
    }

    std::string result = str.substr(0, charLength);
    return result;
}
