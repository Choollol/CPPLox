#include "../include/LoxFunction.hpp"

#include "../include/LoxReturn.hpp"

std::any LoxFunction::call(Interpreter& interpreter, const std::vector<std::any>& arguments) {
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);

    for (size_t i = 0, len = declaration->params.size(); i < len; ++i) {
        environment->define(declaration->params[i].lexeme, arguments[i]);
    }

    try {
        interpreter.executeBlock(declaration->body, environment);
    }
    catch (LoxReturn& returnValue) {
        return returnValue.value;
    }

    return nullptr;
}