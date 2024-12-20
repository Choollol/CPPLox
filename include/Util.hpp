#ifndef CPPLOX_INCLUDE_UTIL_HPP
#define CPPLOX_INCLUDE_UTIL_HPP

#include <any>
#include <string>

/**
 * @brief Converts a given bool to its string form.
 */
inline std::string boolToString(bool b) {
    return b ? "true" : "false";
}

/**
 * @brief Casts the given std::any to a bool and converts it to its string form.
 */
inline std::string boolToString(const std::any& obj) {
    return boolToString(std::any_cast<bool>(obj));
}

#endif