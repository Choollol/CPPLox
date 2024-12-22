#include "../include/Environment.hpp"

#include "../include/Error.hpp"

void Environment::define(const std::string& name, const std::any& val) {
    values[name] = val;
}

void Environment::assign(const Token& name, const std::any& val) {
    if (values.find(name.lexeme) != values.end()) {
        values[name.lexeme] = val;
        return;
    }

    if (enclosing != nullptr) {
        enclosing->assign(name, val);
        return;
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

std::any Environment::get(const Token& name) {
    if (values.find(name.lexeme) != values.end()) {
        return values[name.lexeme];
    }

    if (enclosing != nullptr) {
        return enclosing->get(name);
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}