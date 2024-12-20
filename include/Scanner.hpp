#ifndef CPPLOX_INCLUDE_SCANNER_HPP
#define CPPLOX_INCLUDE_SCANNER_HPP

#include <map>
#include <string>
#include <vector>

#include "Token.hpp"

class Scanner {
   public:
    Scanner(const std::string&);

    /**
     * @brief Gets tokens from source.
     */
    std::vector<Token> scanTokens();

   private:
    std::string source;
    std::vector<Token> tokens;

    size_t start = 0;
    size_t current = 0;
    size_t line = 1;

    size_t sourceLen;

    /**
     * @brief Checks whether end of source has been reached.
     */
    bool isAtEnd();

    /**
     * @brief Gets the next token in source.
     */
    void scanToken();

    /**
     * @brief Helper method to advance to the next character in source.
     */
    char advance();

    /**
     * @brief Adds a token that has no literal.
     */
    void addToken(TokenType);

    /**
     * @brief Adds a token to the container of tokens.
     */
    void addToken(TokenType, std::any);

    /**
     * @brief Checks whether the current character matches the given char. If so, increments current
     * position.
     */
    bool match(char);

    /**
     * @brief Returns the character at the current position without advancing.
     */
    char peek();

    /**
     * @brief Returns the character at the position after the current position.
     */
    char peekNext();

    /**
     * @brief Handle string literals.
     */
    void string();

    /**
     * @brief Handle number literals.
     */
    void number();

    /**
     * @brief Handle identifiers.
     */
    void identifier();

    /**
     * @brief Handle single-line comments.
     */
    void singleLineComment();

    /**
     * @brief Handle block comments.
     */
    void blockComment();

    /**
     * @brief Check whether the given character is a letter or an underscore.
     */
    bool isAlphaOrUscore(char c) { return isalpha(c) || c == '_'; }

    /**
     * @brief Check whether the given character is a valid identifier character (alphabetic, digit,
     * or '_').
     */
    bool isValidIdentifierChar(char c) { return isAlphaOrUscore(c) || isdigit(c); }

    static const std::map<std::string, TokenType> keywords;
};

#endif