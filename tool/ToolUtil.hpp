#ifndef CPPLOX_TOOL_TOOLUTIL_HPP
#define CPPLOX_TOOL_TOOLUTIL_HPP

#include <string>
#include <string_view>
#include <algorithm>
#include <vector>

/// @brief Converts all letters in the given string to uppercase and returns a copy.
inline std::string toUpper(std::string_view s) {
    std::string result;
    for (char c : s) {
        if (islower(c)) {
            c = toupper(c);
        }
        result += c;
    }
    return result;
}

/// @brief Converts all letters in the given string to lowercase and returns a copy.
inline std::string toLower(std::string_view s) {
    std::string result;
    for (char c : s) {
        if (isupper(c)) {
            c = tolower(c);
        }
        result += c;
    }
    return result;
}

/// @brief Splits a string into a std::vector of std::string_view's based on the given delimiter.
std::vector<std::string_view> split(std::string_view str, std::string_view delim) {
    std::vector<std::string_view> out;

    std::string_view::size_type start = 0;
    std::string_view::size_type end = str.find(delim);

    while (end != std::string_view::npos) {
        out.push_back(str.substr(start, end - start));

        start = end + delim.length();
        end = str.find(delim, start);
    }

    out.push_back(str.substr(start, end - start));

    return out;
}

/// @brief Trim whitespace from the beginning and end of the string and return a copy.
std::string_view trim(std::string_view str) {
    auto start = std::find_if_not(str.cbegin(), str.cend(), isspace);
    auto end = std::find_if_not(str.crbegin(), str.crend(), isspace).base();

    return std::string_view(start, end - start);
}

#endif