#ifndef CPPLOX_INCLUDE_RESOLVER_HPP
#define CPPLOX_INCLUDE_RESOLVER_HPP

#include <stack>

#include "Expr.hpp"
#include "Interpreter.hpp"
#include "Stmt.hpp"

class Resolver : public ExprVisitor, public StmtVisitor {
    enum class FunctionType {
        NONE,
        FUNCTION,
        INITIALIZER,
        METHOD
    };
    enum class ClassType {
        NONE,
        CLASS
    };

   public:
    Resolver(Interpreter& interp) : interpreter(interp) {}

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

    /// @brief Resolves a list of statements.
    void resolve(const std::vector<std::shared_ptr<Stmt>>&);

   private:
    Interpreter& interpreter;
    std::stack<std::map<std::string, bool>> scopes;

    FunctionType currentFunction = FunctionType::NONE;
    ClassType currentClass = ClassType::NONE;

    /// @brief Begins a new scope. i.e. pushes a new scope onto the stack.
    void beginScope();
    /// @brief Discards a scope. i.e. pops a scope off the stack.
    void endScope();

    /// @brief Resolves a statement by applying the Visitor pattern.
    void resolve(std::shared_ptr<Stmt>);
    /// @brief Resolves an expression by applying the Visitor pattern.
    void resolve(std::shared_ptr<Expr>);
    /// @brief Resolves a local variable expression and records the number of scopes between the variable usage and its original scope.
    void resolveLocal(std::shared_ptr<Expr>, const Token&);
    /// @brief Resolves a function definition, including its parameters and body.
    void resolveFunction(std::shared_ptr<Function>, FunctionType);

    /// @brief Resolves a variable declaration.
    void declare(const Token&);
    /// @brief Resolves a variable definition.
    void define(const Token&);
};

#endif