#ifndef CPPLOX_INCLUDE_ERROR_HPP
#define CPPLOX_INCLUDE_ERROR_HPP

#include <iostream>
#include <string>
#include <string_view>

#include "Token.hpp"

extern bool hadError;
extern bool hadRuntimeError;

class ParseError : public std::runtime_error {
   public:
    using std::runtime_error::runtime_error;
};

class RuntimeError : public std::runtime_error {
   public:
    RuntimeError(const Token& tok, const std::string& msg) : std::runtime_error(msg), token(tok) {}

    Token token;
};

/**
 * @brief Helper function to report errors.
 *
 * @param line: Line number on which the error occurred.
 * @param where: Error descriptor.
 * @param message: Error message to display.
 */
inline void report(size_t line, std::string_view where, std::string_view message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    hadError = true;
}

/**
 * @brief Reports an error with a given line number and message.
 *
 * @param line: Line number on which the error occurred.
 * @param message: Error message to display.
 */
inline void error(size_t line, const std::string& message) { report(line, "", message); }

/**
 * @brief Reports an error at a given token.
 */
inline void error(const Token& token, std::string_view message) {
    if (token.type == TokenType::LOX_EOF) {
        report(token.line, " at end", message);
    }
    else {
        report(token.line, " at '" + token.lexeme + "'", message);
    }
}

/**
 * @brief Reports a runtime error.
 */
inline void runtimeError(RuntimeError error) {
    std::cerr << error.what() << "\n[line " << error.token.line << "]" << std::endl;
    hadRuntimeError = true;
}

#endif