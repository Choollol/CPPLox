#include "../include/Token.hpp"

#include <map>

Token::Token(TokenType t, const std::string& lex, std::any lit, size_t ln)
    : type(t), lexeme(lex), literal(lit), line(ln) {}

/**
 * @brief Get a std::string from a TokenType
 */
std::string tokenTypeString(TokenType type) {
    static const std::map<TokenType, std::string> tokenTypeNames{{TokenType::LEFT_PAREN, "LEFT_PAREN"},
                                                                 {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
                                                                 {TokenType::LEFT_BRACE, "LEFT_BRACE"},
                                                                 {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
                                                                 {TokenType::COMMA, "COMMA"},
                                                                 {TokenType::COLON, "COLON"},
                                                                 {TokenType::DOT, "DOT"},
                                                                 {TokenType::QUESTION, "QUESTION"},
                                                                 {TokenType::SEMICOLON, "SEMICOLON"},
                                                                 {TokenType::SLASH, "SLASH"},
                                                                 {TokenType::STAR, "STAR"},
                                                                 {TokenType::BANG, "BANG"},
                                                                 {TokenType::BANG_EQUAL, "BANG_EQUAL"},
                                                                 {TokenType::EQUAL, "EQUAL"},
                                                                 {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
                                                                 {TokenType::GREATER, "GREATER"},
                                                                 {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
                                                                 {TokenType::LESS, "LESS"},
                                                                 {TokenType::LESS_EQUAL, "LESS_EQUAL"},
                                                                 {TokenType::MINUS, "MINUS"},
                                                                 {TokenType::MINUS_MINUS, "MINUS_MINUS"},
                                                                 {TokenType::PLUS, "PLUS"},
                                                                 {TokenType::PLUS_PLUS, "PLUS_PLUS"},
                                                                 {TokenType::IDENTIFIER, "IDENTIFIER"},
                                                                 {TokenType::STRING, "STRING"},
                                                                 {TokenType::NUMBER, "NUMBER"},
                                                                 {TokenType::AND, "AND"},
                                                                 {TokenType::CLASS, "CLASS"},
                                                                 {TokenType::ELSE, "ELSE"},
                                                                 {TokenType::FALSE, "FALSE"},
                                                                 {TokenType::FUN, "FUN"},
                                                                 {TokenType::FOR, "FOR"},
                                                                 {TokenType::IF, "IF"},
                                                                 {TokenType::NIL, "NIL"},
                                                                 {TokenType::OR, "OR"},
                                                                 {TokenType::PRINT, "PRINT"},
                                                                 {TokenType::RETURN, "RETURN"},
                                                                 {TokenType::SUPER, "SUPER"},
                                                                 {TokenType::THIS, "THIS"},
                                                                 {TokenType::TRUE, "TRUE"},
                                                                 {TokenType::VAR, "VAR"},
                                                                 {TokenType::WHILE, "WHILE"},
                                                                 {TokenType::LOX_EOF, "EOF"}};

    return tokenTypeNames.at(type);
}

std::string Token::toString() const {
    std::string literalText;

    switch (type) {
        case TokenType::IDENTIFIER:
            literalText = lexeme;
            break;
        case TokenType::STRING:
            literalText = std::any_cast<std::string>(literal);
            break;
        case TokenType::NUMBER:
            literalText = std::to_string(std::any_cast<double>(literal));
            break;
        case TokenType::TRUE:
            literalText = "true";
            break;
        case TokenType::FALSE:
            literalText = "false";
            break;
        default:
            literalText = "nil";
            break;
    }

    return tokenTypeString(type) + " " + lexeme + " " + literalText;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.toString();
    return os;
}