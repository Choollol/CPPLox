#include "../include/LoxInstance.hpp"

#include "../include/Error.hpp"

std::any LoxInstance::get(const Token& name) {
    if (fields.contains(name.lexeme)) {
        return fields[name.lexeme];
    }
    else if (auto method = loxClass->findMethod(name.lexeme)) {
        return method;
    }

    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}

void LoxInstance::set(const Token& name, const std::any& value) {
    fields[name.lexeme] = value;
}

std::string LoxInstance::toString() {
    return loxClass->name + " instance";
}