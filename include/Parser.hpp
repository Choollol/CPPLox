#ifndef CPPLOX_INCLUDE_PARSER_HPP
#define CPPLOX_INCLUDE_PARSER_HPP

#include <memory>
#include <string_view>
#include <vector>

#include "Error.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"
#include "Token.hpp"

class Parser {
   public:
    Parser(const std::vector<Token>& tok) : tokens(tok) {}

    /**
     * @brief Begin the parsing process.
     */
    std::vector<std::shared_ptr<Stmt>> parse();

   private:
    std::vector<Token> tokens;
    size_t current = 0;

    /// @brief Handles declarations.
    std::shared_ptr<Stmt> declaration();

    /// @brief Handles statements.
    std::shared_ptr<Stmt> statement();

    /**
     * @brief Handles general expressions.
     */
    std::shared_ptr<Expr> expression();

    /// @brief Handles assignment expressions.
    std::shared_ptr<Expr> assignment();

    /// @brief Handles logical or expressions.
    std::shared_ptr<Expr> logicalOr();

    /// @brief Handles logical and expressions.
    std::shared_ptr<Expr> logicalAnd();

    /**
     * @brief Handles equality expressions.
     */
    std::shared_ptr<Expr> equality();

    /**
     * @brief Handles comparison/relative expressions.
     */
    std::shared_ptr<Expr> comparison();

    /**
     * @brief Handles addition and subtraction.
     */
    std::shared_ptr<Expr> term();

    /**
     * @brief Handles multiplication and division.
     */
    std::shared_ptr<Expr> factor();

    /**
     * @brief Handles unary expressions.
     */
    std::shared_ptr<Expr> unary();

    /**
     * @brief Handles literals and parenthesized expressions.
     */
    std::shared_ptr<Expr> primary();

    /// @brief Handles call expressions.
    std::shared_ptr<Expr> call();

    /// @brief Helper function to reduce code duplication when parsing (possibly) binary expressions. Defaults to class Binary.
    template <typename exprClass = Binary, typename... Args>
    std::shared_ptr<Expr> binaryExpression(std::shared_ptr<Expr> (Parser::*)(), const Args...);

    /// @brief Parse a call expression's argument list.
    std::shared_ptr<Expr> finishCall(std::shared_ptr<Expr>);

    /// @brief Returns true and advances if any of the given TokenTypes are matched with the current token.
    template <typename... Args>
    bool match(const Args&...);

    /// @brief Checks the current token against the given TokenType without advancing.
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

    /// @brief Handle a print statement.
    std::shared_ptr<Print> printStatement();

    /// @brief Handle an expression statement.
    std::shared_ptr<Expression> expressionStatement();

    /// @brief Handles an if statement.
    std::shared_ptr<If> ifStatement();

    /// @brief Handles a while loop.
    std::shared_ptr<While> whileStatement();

    /// @brief Handles a for loop.
    std::shared_ptr<Stmt> forStatement();

    /// @brief Handle a block statement. Assumes the opening LEFT_BRACE has been consumed already.
    std::vector<std::shared_ptr<Stmt>> block();

    /// @brief Handle variable declaration.
    std::shared_ptr<Stmt> varDeclaration();

    /// @brief Handle function or method declaration.
    std::shared_ptr<Function> functionDeclaration(const std::string&);
};

#endif