#ifndef CPPLOX_INCLUDE_LOXCLASS_HPP
#define CPPLOX_INCLUDE_LOXCLASS_HPP

#include <string>

#include "LoxCallable.hpp"
#include "LoxFunction.hpp"

class LoxClass : public LoxCallable, public std::enable_shared_from_this<LoxClass> {
   public:
    LoxClass(const std::string& s, std::shared_ptr<LoxClass> super, std::map<std::string, std::shared_ptr<LoxFunction>>&& methds)
        : name(s), superclass(super), methods(std::move(methds)) {}

    size_t arity() override;
    std::any call(Interpreter&, const std::vector<std::any>&) override;
    std::string toString() const override;

    /// @brief Searches for and returns the method with the given name. Returns nullptr if not found.
    std::shared_ptr<LoxFunction> findMethod(const std::string&) const;

    const std::string name;
    const std::shared_ptr<LoxClass> superclass;
    const std::map<std::string, std::shared_ptr<LoxFunction>> methods;
};

#endif