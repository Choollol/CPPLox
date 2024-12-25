#ifndef CPPLOX_INCLUDE_LOXCLASS_HPP
#define CPPLOX_INCLUDE_LOXCLASS_HPP

#include <string>

#include "LoxCallable.hpp"

class LoxClass : public LoxCallable {
    public:
    LoxClass(const std::string& s) : name(s) {}

    size_t arity() override { return 0; }
    std::any call(Interpreter&, const std::vector<std::any>&) override;
    std::string toString() const override;

    std::string name;
};

#endif