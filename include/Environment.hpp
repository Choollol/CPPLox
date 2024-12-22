#ifndef CPPLOX_INCLUDE_ENVIRONMENT_HPP
#define CPPLOX_INCLUDE_ENVIRONMENT_HPP

#include <any>
#include <map>
#include <memory>
#include <string>
#include <string_view>

#include "Token.hpp"

class Environment {
   public:
    Environment() : enclosing(nullptr) {}
    Environment(std::shared_ptr<Environment> env) : enclosing(env) {}

    /// @brief Defines a variable, which may be a redefinition of an ealier variable with the same name.
    void define(const std::string&, const std::any&);
    /// @brief Assigns a new value to an existing variable.
    void assign(const Token&, const std::any&);

    /// @brief Returns the value associated with the given token.
    std::any get(const Token&);

   private:
    std::map<std::string, std::any> values;
    std::shared_ptr<Environment> enclosing;
};

#endif