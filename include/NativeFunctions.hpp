#ifndef CPPLOX_INCLUDE_NATIVEFUNCTIONS_HPP
#define CPPLOX_INCLUDE_NATIVEFUNCTIONS_HPP

#include "LoxCallable.hpp"

class NativeClock : public LoxCallable {
   public:
    size_t arity() override { return 0; }
    std::any call(Interpreter&, const std::vector<std::any>&) override;
    std::string toString() const override { return "<native fn: clock>"; }
};

#endif