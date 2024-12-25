#include "../include/LoxClass.hpp"

std::any LoxClass::call(Interpreter& interpreter, const std::vector<std::any>& arguments) {
    return nullptr;
}

std::string LoxClass::toString() const {
    return name;
}