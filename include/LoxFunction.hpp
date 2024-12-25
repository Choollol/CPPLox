#ifndef CPPLOX_INCLUDE_LOXFUNCTION_HPP
#define CPPLOX_INCLUDE_LOXFUNCTION_HPP

#include "LoxCallable.hpp"
#include "LoxInstance.hpp"

class LoxFunction : public LoxCallable {
   public:
    LoxFunction(std::shared_ptr<Function> decl, std::shared_ptr<Environment> clos) : declaration(decl), closure(clos) {}

    size_t arity() override { return declaration->params.size(); }
    std::any call(Interpreter&, const std::vector<std::any>&) override;
    std::string toString() const override { return "<fn " + declaration->name.lexeme + ">"; }

    std::any bind(std::shared_ptr<LoxInstance>);

   private:
    const std::shared_ptr<Function> declaration;
    const std::shared_ptr<Environment> closure;
};

#endif