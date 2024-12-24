#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "include/AstPrinter.hpp"
#include "include/Error.hpp"
#include "include/Interpreter.hpp"
#include "include/Parser.hpp"
#include "include/Resolver.hpp"
#include "include/Scanner.hpp"
#include "include/Token.hpp"

#define DEBUG_PRINT 0

extern bool hadError;
extern bool hadRuntimeError;

namespace {
Interpreter interpreter;
}

/**
 * @brief Reads source code from a file and executes it.
 *
 * @param path: std::string containing the file's path.
 */
void runFile(const std::string& path);

/**
 * @brief Reads source code line-by-line from the command prompt.
 */
void runPrompt();

int main(int argc, char* argv[]) {
    // Incorrect usage
    if (argc > 2) {
        std::cerr << "Usage: cpplox [script]" << std::endl;
        exit(64);
    }
    // Read source code from file
    else if (argc == 2) {
        runFile(argv[1]);
    }
    // Interact with user through command prompt
    else {
        runPrompt();
    }
    return 0;
}

/**
 * @brief Reads and executes source code in the form of a std::string.
 *
 * @param source: std::string containing the code to be run.
 */
void run(const std::string& source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();
#if DEBUG_PRINT != 0
    std::cout << "Scanning completed" << std::endl;
#endif

    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> stmts = parser.parse();
    // Check for syntax error
    if (hadError) {
        return;
    }
#if DEBUG_PRINT != 0
    std::cout << "Parsing completed" << std::endl;
#endif

    Resolver resolver(interpreter);
    resolver.resolve(stmts);
    // Check for resolution error
    if (hadError) {
        return;
    }
#if DEBUG_PRINT != 0
    std::cout << "Resolution completed" << std::endl;
#endif

    interpreter.interpret(stmts);
#if DEBUG_PRINT != 0
    std::cout << "Interpreting completed" << std::endl;
#endif
}

void runFile(const std::string& path) {
    // Get source code from path
    std::ifstream inFile(path);
    auto begin = std::istreambuf_iterator<char>(inFile);
    auto end = std::istreambuf_iterator<char>();
    std::string fileContents(begin, end);

    // Execute source code
    run(fileContents);

    // Terminate program if error was found
    if (hadError) {
        exit(65);
    }
    else if (hadRuntimeError) {
        exit(70);
    }
}

void runPrompt() {
    std::string line;

    // Get code from user
    std::cout << "> ";
    while (std::getline(std::cin, line)) {
        std::cout << "> ";
        run(line);

        // Errors shouldn't stop line-by-line prompt code input
        hadError = false;
    }
}
