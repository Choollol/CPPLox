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

/// @brief If the given std::any holds a std::shared_ptr to the specified type, return a casted std::shared_ptr. Otherwise, return nullptr.
template <typename T>
std::shared_ptr<T> ptrAnyCast(const std::any& obj) {
    if (obj.type() == typeid(std::shared_ptr<T>)) {
        return std::any_cast<std::shared_ptr<T>>(obj);
    }
    else {
        return nullptr;
    }
}

#endif