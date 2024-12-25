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
        if (!isInitializer) {
            return returnValue.value;
        }
    }

    if (isInitializer) {
        return closure->getAt(0, "this");
    }

    return nullptr;
}

std::shared_ptr<LoxFunction> LoxFunction::bind(std::shared_ptr<LoxInstance> instance) {
    auto environment = std::make_shared<Environment>(closure);
    environment->define("this", instance);
    return std::make_shared<LoxFunction>(declaration, environment, isInitializer);
}