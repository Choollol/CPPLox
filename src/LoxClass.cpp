#include "../include/LoxClass.hpp"
#include "../include/LoxInstance.hpp"

std::any LoxClass::call(Interpreter& interpreter, const std::vector<std::any>& arguments) {
    auto loxInstance = std::make_shared<LoxInstance>(shared_from_this());
    return loxInstance;
}

std::string LoxClass::toString() const {
    return name;
}