#include "../include/Parser.hpp"
#include "../include/Expr.hpp"

#include <iostream>

std::shared_ptr<Expr> Parser::parse() {
    try {
        return expression();
    }
    catch (ParseError error) {
        return nullptr;
    }
}

std::shared_ptr<Expr> Parser::equality() {
    return binaryExpression(&Parser::comparison, TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL);
}
std::shared_ptr<Expr> Parser::comparison() {
    return binaryExpression(&Parser::term, TokenType::LESS, TokenType::LESS_EQUAL, TokenType::GREATER, TokenType::GREATER_EQUAL);
}
std::shared_ptr<Expr> Parser::term() {
    return binaryExpression(&Parser::factor, TokenType::LESS, TokenType::LESS_EQUAL, TokenType::GREATER, TokenType::GREATER_EQUAL);
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

    // Parenthesized expression
    if (match(TokenType::LEFT_PAREN)) {
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }

    std::cout << current << "  " << peek() << std::endl;

    throw error(peek(), "Expect expression.");
}

template <typename... Args>
std::shared_ptr<Expr> Parser::binaryExpression(std::shared_ptr<Expr> (Parser::*func)(), const Args... args) {
    std::shared_ptr<Expr> expr = (this->*func)();

    while (match(args...)) {
        Token op = previous();
        auto rhs = factor();
        expr = std::make_shared<Binary>(expr, op, rhs);
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