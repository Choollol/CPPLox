#ifndef CPPLOX_INCLUDE_INTERPRETER_HPP
#define CPPLOX_INCLUDE_INTERPRETER_HPP

#include <any>
#include <memory>

#include "Environment.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"

class Interpreter : public ExprVisitor, public StmtVisitor {
    friend class LoxFunction;

   public:
    Interpreter();

    std::any visitAssignExpr(std::shared_ptr<Assign>) override;
    std::any visitBinaryExpr(std::shared_ptr<Binary>) override;
    std::any visitCallExpr(std::shared_ptr<Call>) override;
    std::any visitGetExpr(std::shared_ptr<Get>) override;
    std::any visitGroupingExpr(std::shared_ptr<Grouping>) override;
    std::any visitLiteralExpr(std::shared_ptr<Literal>) override;
    std::any visitLogicalExpr(std::shared_ptr<Logical>) override;
    std::any visitSetExpr(std::shared_ptr<Set>) override;
    std::any visitThisExpr(std::shared_ptr<This>) override;
    std::any visitUnaryExpr(std::shared_ptr<Unary>) override;
    std::any visitVariableExpr(std::shared_ptr<Variable>) override;

    std::any visitBlockStmt(std::shared_ptr<Block>) override;
    std::any visitClassStmt(std::shared_ptr<Class>) override;
    std::any visitExpressionStmt(std::shared_ptr<Expression>) override;
    std::any visitFunctionStmt(std::shared_ptr<Function>) override;
    std::any visitIfStmt(std::shared_ptr<If>) override;
    std::any visitPrintStmt(std::shared_ptr<Print>) override;
    std::any visitReturnStmt(std::shared_ptr<Return>) override;
    std::any visitVarStmt(std::shared_ptr<Var>) override;
    std::any visitWhileStmt(std::shared_ptr<While>) override;

    /// @brief Interprets a given expression. i.e. run the interpreter.
    void interpret(std::vector<std::shared_ptr<Stmt>>);

    /// @brief Records the number of scopes between the variable usage and its original scope.
    void resolve(std::shared_ptr<Expr>, size_t);

   private:
    std::shared_ptr<Environment> globals{new Environment};
    std::shared_ptr<Environment> environment = globals;
    std::map<std::shared_ptr<Expr>, size_t> locals;

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
    std::string stringify(std::any);

    /// @brief Executes a statement.
    void execute(std::shared_ptr<Stmt>);
    /// @brief Executes a block statement.
    void executeBlock(const std::vector<std::shared_ptr<Stmt>>&, std::shared_ptr<Environment>);

    /// @brief Get a variable's value by searching the enclosing environments.
    std::any lookUpVariable(const Token&, std::shared_ptr<Expr>);
};

#endif