#include <iostream>

#include "../include/AstPrinter.hpp"
#include "../include/Expr.hpp"

int main() {
    std::shared_ptr<Expr> expression = std::make_shared<Binary>(
        std::make_shared<Unary>(
            Token(TokenType::MINUS, "-", nullptr, 1),
            std::make_shared<Literal>(123.)),
        Token(TokenType::STAR, "*", nullptr, 1),
        std::make_shared<Grouping>(
            std::make_shared<Literal>(45.67)));

    std::cout << AstPrinter().print(expression) << "\n";
}