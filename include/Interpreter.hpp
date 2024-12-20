#ifndef CPPLOX_INCLUDE_INTERPRETER_HPP
#define CPPLOX_INCLUDE_INTERPRETER_HPP

#include <any>
#include <memory>

#include "Expr.hpp"

class Interpreter : public ExprVisitor {
   public:
    std::any visitBinaryExpr(std::shared_ptr<Binary>) override;
    std::any visitGroupingExpr(std::shared_ptr<Grouping>) override;
    std::any visitLiteralExpr(std::shared_ptr<Literal>) override;
    std::any visitUnaryExpr(std::shared_ptr<Unary>) override;

    /// @brief Interprets a given expression. i.e. run the interpreter.
    void interpret(std::shared_ptr<Expr>);

   private:
    /// @brief Helper method that uses the visitor pattern to return an expression's std::any. 
    std::any evaluate(std::shared_ptr<Expr>);
    /// @brief Returns a truth value based on the given std::any.
    bool isTruthy(const std::any&);
    /// @brief Checks if two std::any's are equal.
    bool isEqual(const std::any&, const std::any&);
    /// @brief Checks if the given std::any holds a number. If it doesn't, throw an error with the given token. 
    void checkNumberOperand(const Token&, const std::any&);
    /// @brief Checks if the given std::any's hold numbers. If either doesn't, throw an error with the given token.
    void checkNumberOperands(const Token&, const std::any&, const std::any&);
    /// @brief Returns a std::string representation of the given std::any
    std::string stringify(const std::any&);
};

#endif