#include "../include/LoxFunction.hpp"

std::any LoxFunction::call(Interpreter& interpreter, const std::vector<std::any>& arguments) {
    std::shared_ptr<Environment> environment = interpreter.globals; 

    for (size_t i = 0, len = declaration->params.size(); i < len; ++i) {
        environment->define(declaration->params[i].lexeme, arguments[i]);
    }

    interpreter.executeBlock(declaration->body, environment);
    return nullptr;
}