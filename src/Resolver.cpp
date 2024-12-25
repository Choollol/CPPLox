#include "../include/Resolver.hpp"

#include "../include/Error.hpp"

std::any Resolver::visitAssignExpr(std::shared_ptr<Assign> expr) {
    resolve(expr->value);
    resolveLocal(expr, expr->name);
    return nullptr;
}
std::any Resolver::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    resolve(expr->left);
    resolve(expr->right);
    return nullptr;
}
std::any Resolver::visitCallExpr(std::shared_ptr<Call> expr) {
    resolve(expr->callee);
    for (const auto& arg : expr->arguments) {
        resolve(arg);
    }
    return nullptr;
}
std::any Resolver::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
    resolve(expr->expression);
    return nullptr;
}
std::any Resolver::visitGetExpr(std::shared_ptr<Get> expr) {
    resolve(expr->object);
    return nullptr;
}
std::any Resolver::visitLiteralExpr(std::shared_ptr<Literal> expr) {
    return nullptr;
}
std::any Resolver::visitLogicalExpr(std::shared_ptr<Logical> expr) {
    resolve(expr->left);
    resolve(expr->right);
    return nullptr;
}
std::any Resolver::visitSetExpr(std::shared_ptr<Set> expr) {
    resolve(expr->object);
    resolve(expr->value);
    return nullptr;
}
std::any Resolver::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    resolve(expr->right);
    return nullptr;
}
std::any Resolver::visitVariableExpr(std::shared_ptr<Variable> expr) {
    if (!scopes.empty() && scopes.top().contains(expr->name.lexeme) && scopes.top()[expr->name.lexeme] == false) {
        error(expr->name, "Can't read local variable name in its own initializer.");
    }

    resolveLocal(expr, expr->name);
    return nullptr;
}

std::any Resolver::visitBlockStmt(std::shared_ptr<Block> stmt) {
    beginScope();
    resolve(stmt->statements);
    endScope();
    return nullptr;
}
std::any Resolver::visitClassStmt(std::shared_ptr<Class> stmt) {
    declare(stmt->name);
    define(stmt->name);

    for (auto method : stmt->methods) {
        FunctionType declaration = FunctionType::METHOD;
        resolveFunction(method, declaration);
    }

    return nullptr;
}
std::any Resolver::visitExpressionStmt(std::shared_ptr<Expression> stmt) {
    resolve(stmt->expression);
    return nullptr;
}
std::any Resolver::visitFunctionStmt(std::shared_ptr<Function> stmt) {
    declare(stmt->name);
    define(stmt->name);

    resolveFunction(stmt, FunctionType::FUNCTION);
    return nullptr;
}
std::any Resolver::visitIfStmt(std::shared_ptr<If> stmt) {
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if (stmt->elseBranch != nullptr) {
        resolve(stmt->elseBranch);
    }
    return nullptr;
}
std::any Resolver::visitPrintStmt(std::shared_ptr<Print> stmt) {
    resolve(stmt->expression);
    return nullptr;
}
std::any Resolver::visitReturnStmt(std::shared_ptr<Return> stmt) {
    if (currentFunction == FunctionType::NONE) {
        error(stmt->keyword, "Can't return from top-level code.");
    }
    if (stmt->value != nullptr) {
        resolve(stmt->value);
    }
    return nullptr;
}
std::any Resolver::visitVarStmt(std::shared_ptr<Var> stmt) {
    declare(stmt->name);
    if (stmt->initializer != nullptr) {
        resolve(stmt->initializer);
    }
    define(stmt->name);
    return nullptr;
}
std::any Resolver::visitWhileStmt(std::shared_ptr<While> stmt) {
    resolve(stmt->condition);
    resolve(stmt->body);
    return nullptr;
}

void Resolver::beginScope() {
    scopes.push(std::map<std::string, bool>());
}
void Resolver::endScope() {
    scopes.pop();
}
void Resolver::resolve(const std::vector<std::shared_ptr<Stmt>>& stmts) {
    for (const auto& stmt : stmts) {
        resolve(stmt);
    }
}
void Resolver::resolve(std::shared_ptr<Stmt> stmt) {
    stmt->accept(*this);
}
void Resolver::resolve(std::shared_ptr<Expr> expr) {
    expr->accept(*this);
}
void Resolver::resolveLocal(std::shared_ptr<Expr> expr, const Token& name) {
    std::stack<std::map<std::string, bool>> tempScopes = scopes;
    for (int i = scopes.size() - 1; i >= 0; tempScopes.pop(), --i) {
        if (tempScopes.top().contains(name.lexeme)) {
            interpreter.resolve(expr, scopes.size() - i - 1);
            return;
        }
    }
}
void Resolver::resolveFunction(std::shared_ptr<Function> function, FunctionType type) {
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();
    for (const auto& param : function->params) {
        declare(param);
        define(param);
    }
    resolve(function->body);
    endScope();

    currentFunction = enclosingFunction;
}

void Resolver::declare(const Token& name) {
    // Check for global scope
    if (scopes.empty()) {
        return;
    }
    if (scopes.top().contains(name.lexeme)) {
        error(name, "Already a variable with this name in this scope.");
    }

    scopes.top()[name.lexeme] = false;
}
void Resolver::define(const Token& name) {
    // Check for global scope
    if (scopes.empty()) {
        return;
    }

    scopes.top()[name.lexeme] = true;
}