#ifndef CPPLOX_INCLUDE_LOXFUNCTION_HPP
#define CPPLOX_INCLUDE_LOXFUNCTION_HPP

#include "LoxCallable.hpp"

class LoxFunction : public LoxCallable {
   public:
    LoxFunction(std::shared_ptr<Function> decl) : declaration(decl) {}

    size_t arity() override { return declaration->params.size(); }
    std::any call(Interpreter&, const std::vector<std::any>&) override;
    std::string toString() const override { return "<fn " + declaration->name.lexeme + ">"; }

   private:
    const std::shared_ptr<Function> declaration;
};

#endif