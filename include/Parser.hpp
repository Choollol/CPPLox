#ifndef CPPLOX_INCLUDE_PARSER_HPP
#define CPPLOX_INCLUDE_PARSER_HPP

#include <memory>
#include <string_view>
#include <vector>

#include "Error.hpp"
#include "Expr.hpp"
#include "Token.hpp"

class Parser {
   public:
    Parser(const std::vector<Token>& tok) : tokens(tok) {}

    /**
     * @brief Begins the parsing process.
     */
    std::shared_ptr<Expr> parse();

   private:
    std::vector<Token> tokens;
    size_t current = 0;

    /**
     * @brief Lowest precedence, handles general expressions.
     */
    std::shared_ptr<Expr> expression() {
        return equality();
    }

    /**
     * @brief Second lowest precedence, handles equality expressions.
     */
    std::shared_ptr<Expr> equality();

    /**
     * @brief Third lowest precedence, handles comparison/relative expressions.
     */
    std::shared_ptr<Expr> comparison();

    /**
     * @brief Fourth lowest precedence, handles addition and subtraction.
     */
    std::shared_ptr<Expr> term();

    /**
     * @brief Fifth lowest precedence, handles multiplication and division.
     */
    std::shared_ptr<Expr> factor();

    /**
     * @brief Sixth lowest and second highest precedence, handles unary expressions.
     */
    std::shared_ptr<Expr> unary();

    /**
     * @brief Highest and seventh lowest precedence, handles literals and parenthesized expressions.
     */
    std::shared_ptr<Expr> primary();

    /**
     * @brief Helper function to reduce code duplication when parsing binary expressions.
     */
    template <typename... Args>
    std::shared_ptr<Expr> binaryExpression(std::shared_ptr<Expr> (Parser::*)(), const Args...);

    /// @brief Returns true if any of the given TokenTypes are matched with the current token.
    template <typename... Args>
    bool match(const Args&...);

    /**
     * @brief Checks the current token against the given TokenType.
     */
    bool check(TokenType);

    /// @brief Returns the current token without advancing.
    Token peek();
    /// @brief Returns the previous token. 
    Token previous();

    /// @brief Check whether the end of the list of tokens has been reached. 
    bool isAtEnd();
    /// @brief Advance to the next token. 
    Token advance();

    /// @brief If the current token matches the given TokenType, advance. Otherwise, throw a ParseError.
    Token consume(TokenType, std::string_view);

    /// @brief Report an error and return a ParseError.
    ParseError error(const Token&, std::string_view);

    /// @brief Try to recover from a ParseError.
    void synchronize();
};

#endif