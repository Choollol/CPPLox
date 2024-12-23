#ifndef CPPLOX_INCLUDE_LOXCALLABLE_HPP
#define CPPLOX_INCLUDE_LOXCALLABLE_HPP

#include <any>
#include <memory>
#include <vector>

#include "Interpreter.hpp"

class LoxCallable {
   public:
    /// @brief Returns the number of arguments that this callable takes. 
    virtual size_t arity() = 0;
    /// @brief Calls this callable with the given interpreter and argument list.
    virtual std::any call(Interpreter&, const std::vector<std::any>&) = 0;
    /// @brief Returns a string representation of this callable.
    virtual std::string toString() const = 0;
};

#endif