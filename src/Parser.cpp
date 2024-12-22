#include "../include/Parser.hpp"

#include <iostream>

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }

    return statements;
}

std::shared_ptr<Stmt> Parser::declaration() {
    try {
        if (match(TokenType::VAR)) {
            return varDeclaration();
        }
        return statement();
    }
    catch (ParseError& error) {
        synchronize();
        return nullptr;
    }
}
std::shared_ptr<Stmt> Parser::statement() {
    if (match(TokenType::IF)) {
        return ifStatement();
    }
    else if (match(TokenType::PRINT)) {
        return printStatement();
    }
    else if (match(TokenType::WHILE)) {
        return whileStatement();
    }
    else if (match(TokenType::LEFT_BRACE)) {
        return std::make_shared<Block>(block());
    }

    return expressionStatement();
}

std::shared_ptr<Print> Parser::printStatement() {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_shared<Print>(expr);
}

std::shared_ptr<Expression> Parser::expressionStatement() {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_shared<Expression>(expr);
}

std::shared_ptr<If> Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

    std::shared_ptr<Stmt> thenBranch = statement();
    std::shared_ptr<Stmt> elseBranch = nullptr;
    if (match(TokenType::ELSE)) {
        elseBranch = statement();
    }

    return std::make_shared<If>(condition, thenBranch, elseBranch);
}

std::shared_ptr<While> Parser::whileStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after while condition.");

    std::shared_ptr<Stmt> body = statement();

    return std::make_shared<While>(condition, body);
}

std::vector<std::shared_ptr<Stmt>> Parser::block() {
    std::vector<std::shared_ptr<Stmt>> statements;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expect } after block.");
    return statements;
}

std::shared_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    std::shared_ptr<Expr> initializer;
    if (match(TokenType::EQUAL)) {
        initializer = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<Var>(name, initializer);
}

std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}
std::shared_ptr<Expr> Parser::assignment() {
    auto expr = logicalOr();

    if (match(TokenType::EQUAL)) {
        Token equals = previous();
        std::shared_ptr<Expr> value = assignment();

        std::shared_ptr<Variable> var = std::dynamic_pointer_cast<Variable>(expr);
        if (var) {
            return std::make_shared<Assign>(var->name, value);
        }

        error(equals, "Invalid assignment target.");
    }

    return expr;
}
std::shared_ptr<Expr> Parser::logicalOr() {
    return binaryExpression<Logical>(&Parser::logicalAnd, TokenType::OR);
}
std::shared_ptr<Expr> Parser::logicalAnd() {
    return binaryExpression<Logical>(&Parser::equality, TokenType::AND);
}

std::shared_ptr<Expr> Parser::equality() {
    return binaryExpression(&Parser::comparison, TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL);
}
std::shared_ptr<Expr> Parser::comparison() {
    return binaryExpression(&Parser::term, TokenType::LESS, TokenType::LESS_EQUAL, TokenType::GREATER, TokenType::GREATER_EQUAL);
}
std::shared_ptr<Expr> Parser::term() {
    return binaryExpression(&Parser::factor, TokenType::PLUS, TokenType::MINUS);
    return binaryExpression(&Parser::factor, TokenType::PLUS, TokenType::MINUS);
}
std::shared_ptr<Expr> Parser::factor() {
    return binaryExpression(&Parser::unary, TokenType::SLASH, TokenType::STAR);
}
std::shared_ptr<Expr> Parser::unary() {
    if (match(TokenType::BANG, TokenType::MINUS)) {
        Token op = previous();
        auto rhs = unary();
        return std::make_shared<Unary>(op, rhs);
    }

    return primary();
}
std::shared_ptr<Expr> Parser::primary() {
    // Booleans and null
    if (match(TokenType::FALSE)) {
        return std::make_shared<Literal>(false);
    }
    if (match(TokenType::TRUE)) {
        return std::make_shared<Literal>(true);
    }
    if (match(TokenType::NIL)) {
        return std::make_shared<Literal>(nullptr);
    }

    // Number or string literal
    if (match(TokenType::NUMBER, TokenType::STRING)) {
        return std::make_shared<Literal>(previous().literal);
    }

    if (match(TokenType::IDENTIFIER)) {
        return std::make_shared<Variable>(previous());
    }

    if (match(TokenType::IDENTIFIER)) {
        return std::make_shared<Variable>(previous());
    }

    // Parenthesized expression
    if (match(TokenType::LEFT_PAREN)) {
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }

    throw error(peek(), "Expect expression.");
}

template <typename exprClass, typename... Args>
std::shared_ptr<Expr> Parser::binaryExpression(std::shared_ptr<Expr> (Parser::*func)(), const Args... args) {
    std::shared_ptr<Expr> expr = (this->*func)();

    while (match(args...)) {
        Token op = previous();
        auto rhs = (this->*func)();
        expr = std::make_shared<exprClass>(expr, op, rhs);
    }

    return expr;
}

Token Parser::consume(TokenType type, std::string_view msg) {
    if (check(type)) {
        return advance();
    }

    throw error(peek(), msg);
}

ParseError Parser::error(const Token& token, std::string_view msg) {
    ::error(token, msg);
    return ParseError("");
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) {
            return;
        }

        switch (peek().type) {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
        }

        advance();
    }
}

template <typename... Args>
bool Parser::match(const Args&... args) {
    if ((check(args) || ...)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) {
        return false;
    }
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) {
        ++current;
    }
    return previous();
}

inline Token Parser::peek() {
    return tokens[current];
}

inline Token Parser::previous() {
    return tokens[current - 1];
}

inline bool Parser::isAtEnd() {
    return peek().type == TokenType::LOX_EOF;
}