#include "../include/Interpreter.hpp"

#include "../include/Error.hpp"
#include "../include/Util.hpp"

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> stmts) {
    try {
        for (auto stmt : stmts) {
            execute(stmt);
        }
    }
    catch (RuntimeError& error) {
        runtimeError(error);
    }
}

std::any Interpreter::visitLiteralExpr(std::shared_ptr<Literal> expr) {
    return expr->value;
}
std::any Interpreter::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
    return evaluate(expr->expression);
}
std::any Interpreter::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    std::any right = evaluate(expr->right);

    switch (expr->oper.type) {
        case TokenType::MINUS:
            checkNumberOperand(expr->oper, right);
            return -std::any_cast<double>(right);
        case TokenType::BANG:
            return !isTruthy(right);
    }

    // Unreachable
    return nullptr;
}
std::any Interpreter::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    std::any left = evaluate(expr->left);
    std::any right = evaluate(expr->right);

    switch (expr->oper.type) {
        // Comparison operators
        case TokenType::GREATER:
            checkNumberOperand(expr->oper, left);
            return std::any_cast<double>(left) > std::any_cast<double>(right);
        case TokenType::GREATER_EQUAL:
            return std::any_cast<double>(left) >= std::any_cast<double>(right);
        case TokenType::LESS:
            return std::any_cast<double>(left) < std::any_cast<double>(right);
        case TokenType::LESS_EQUAL:
            return std::any_cast<double>(left) <= std::any_cast<double>(right);

        // Equality operators
        case TokenType::EQUAL_EQUAL:
            return isEqual(left, right);
        case TokenType::BANG_EQUAL:
            return !isEqual(left, right);

        // Arithmetic operators
        case TokenType::PLUS:
            if (left.type() == typeid(double) && right.type() == typeid(double)) {
                return std::any_cast<double>(left) + std::any_cast<double>(right);
            }
            else if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
                return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
            }
            else {
                throw RuntimeError(expr->oper, "Operands must be two numbers or strings.");
            }
        case TokenType::MINUS:
            checkNumberOperands(expr->oper, left, right);
            return std::any_cast<double>(left) - std::any_cast<double>(right);
        case TokenType::STAR:
            checkNumberOperands(expr->oper, left, right);
            return std::any_cast<double>(left) * std::any_cast<double>(right);
        case TokenType::SLASH:
            checkNumberOperands(expr->oper, left, right);
            // Check for division by zero
            if (std::any_cast<double>(right) == 0) {
                throw RuntimeError(expr->oper, "Cannot divide by zero.");
            }
            else {
                return std::any_cast<double>(left) / std::any_cast<double>(right);
            }
    }

    // Unreachable
    return nullptr;
}
std::any Interpreter::visitVariableExpr(std::shared_ptr<Variable> expr) {
    return environment->get(expr->name);
}
std::any Interpreter::visitAssignExpr(std::shared_ptr<Assign> expr) {
    std::any value = evaluate(expr->value);
    environment->assign(expr->name, value);
    return value;
}
std::any Interpreter::visitLogicalExpr(std::shared_ptr<Logical> expr) {
    std::any left = evaluate(expr->left);

    if ((expr->oper.type == TokenType::OR && isTruthy(left)) ||    // Logical OR short-circuit
        (expr->oper.type == TokenType::AND && !isTruthy(left))) {  // Logical AND short-circuit
        return left;
    }

    return evaluate(expr->right);
}

std::any Interpreter::visitBlockStmt(std::shared_ptr<Block> stmt) {
    executeBlock(stmt->statements, std::make_shared<Environment>(environment));
    return nullptr;
}
std::any Interpreter::visitExpressionStmt(std::shared_ptr<Expression> stmt) {
    evaluate(stmt->expression);
    return nullptr;
}
std::any Interpreter::visitIfStmt(std::shared_ptr<If> stmt) {
    if (isTruthy(evaluate(stmt->condition))) {
        execute(stmt->thenBranch);
    }
    else if (stmt->elseBranch) {
        execute(stmt->elseBranch);
    }

    return nullptr;
}
std::any Interpreter::visitPrintStmt(std::shared_ptr<Print> stmt) {
    std::any obj = evaluate(stmt->expression);
    std::cout << stringify(obj) << std::endl;
    return nullptr;
}

std::any Interpreter::visitVarStmt(std::shared_ptr<Var> stmt) {
    std::any value = nullptr;
    if (stmt->initializer != nullptr) {
        value = evaluate(stmt->initializer);
    }

    environment->define(stmt->name.lexeme, value);
    return nullptr;
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> expr) {
    return expr->accept(*this);
}

bool Interpreter::isTruthy(const std::any& right) {
    if (right.type() == typeid(bool)) {
        return std::any_cast<bool>(right);
    }
    else if (right.type() == typeid(nullptr)) {
        return false;
    }
    return true;
}

bool Interpreter::isEqual(const std::any& left, const std::any& right) {
    if (left.type() != right.type()) {
        return false;
    }
    if (left.type() == typeid(double)) {
        return std::any_cast<double>(left) == std::any_cast<double>(right);
    }
    else if (left.type() == typeid(std::string)) {
        return std::any_cast<std::string>(left) == std::any_cast<std::string>(right);
    }
    else if (left.type() == typeid(bool)) {
        return std::any_cast<bool>(left) == std::any_cast<bool>(right);
    }
    else if (left.type() == typeid(nullptr)) {
        return true;
    }

    // Unreachable
    return false;
}

void Interpreter::checkNumberOperand(const Token& op, const std::any& operand) {
    if (operand.type() != typeid(double)) {
        throw RuntimeError(op, "Operand must be a number.");
    }
}

void Interpreter::checkNumberOperands(const Token& op, const std::any& left, const std::any& right) {
    checkNumberOperand(op, left);
    checkNumberOperand(op, right);
}

std::string Interpreter::stringify(const std::any& obj) {
    if (obj.type() == typeid(nullptr)) {
        return "nil";
    }
    else if (obj.type() == typeid(double)) {
        std::string text = std::to_string(std::any_cast<double>(obj));
        if (text.size() >= 8 && text.substr(text.size() - 7, 7) == ".000000") {
            text = text.substr(0, text.size() - 7);
        }
        return text;
    }
    else if (obj.type() == typeid(bool)) {
        return boolToString(obj);
    }
    else if (obj.type() == typeid(std::string)) {
        return std::any_cast<std::string>(obj);
    }

    return "Unrecognized type in Interpreter::stringify()";
}

void Interpreter::execute(std::shared_ptr<Stmt> stmt) {
    stmt->accept(*this);
}
void Interpreter::executeBlock(const std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> env) {
    auto previous = environment;
    try {
        environment = env;

        for (const auto& statement : statements) {
            execute(statement);
        }
    }
    catch (...) {
        environment = previous;
        throw;
    }

    environment = previous;
}