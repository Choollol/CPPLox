#ifndef CPPLOX_INCLUDE_ASTPRINTER_HPP
#define CPPLOX_INCLUDE_ASTPRINTER_HPP

#include <iostream>
#include <string_view>
#include <string>

#include "Expr.hpp"

class AstPrinter : public ExprVisitor {
   public:
    std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
    std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
    std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
    std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;

    std::string print(std::shared_ptr<Expr> expr) {
        return std::any_cast<std::string>(expr->accept(*this));
    }

   private:
    template <typename... Args>
    std::string parenthesize(std::string_view, Args...);
};

#endif