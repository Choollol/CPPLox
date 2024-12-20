#ifndef CPPLOX_UTIL_HPP
#define CPPLOX_UTIL_HPP

#include <algorithm>
#include <string>
#include <vector>

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

std::string_view trim(std::string_view str) {
    auto start = std::find_if_not(str.begin(), str.end(), isspace);
    auto end = std::find_if_not(str.rbegin(), str.rend(), isspace).base();

    return std::string_view(start, end - start);
}

#endif