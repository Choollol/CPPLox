#ifndef CPPLOX_INCLUDE_TOKEN_HPP
#define CPPLOX_INCLUDE_TOKEN_HPP

#include <optional>
#include <string>
#include <any>
#include <string_view>
#include <iostream>

enum class TokenType {
    // Single-character tokens
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    COLON,
    DOT,
    QUESTION,
    MINUS,
    MINUS_MINUS,
    PLUS,
    PLUS_PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // 1-or-2 character tokens
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    LOX_EOF
};

class Token {
   public:
    Token(TokenType, const std::string&, std::any, size_t);

    /**
     * @brief Returns a std::string representation of this Token.
     */
    std::string toString() const;

    TokenType type;
    std::string lexeme;
    std::any literal;
    size_t line;
};

std::ostream& operator<<(std::ostream&, const Token&);

#endif