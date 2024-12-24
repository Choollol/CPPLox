#include "../include/Environment.hpp"

#include "../include/Error.hpp"

void Environment::define(const std::string& name, const std::any& val) {
    values[name] = val;
}

void Environment::assign(const Token& name, const std::any& value) {
    if (values.find(name.lexeme) != values.end()) {
        values[name.lexeme] = value;
        return;
    }

    if (enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}
void Environment::assignAt(size_t distance, const Token& name, const std::any& value) {
    //ancestor(distance)->assign(name, value);
    ancestor(distance)->values[name.lexeme] = value;
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
std::any Environment::getAt(size_t depth, const std::string& name) {
    return ancestor(depth)->values[name];
}

std::shared_ptr<Environment> Environment::ancestor(size_t depth) {
    std::shared_ptr<Environment> env = shared_from_this();
    while (depth--) {
        env = env->enclosing;
    }
    return env;
}