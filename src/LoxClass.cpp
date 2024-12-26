#include "../include/LoxClass.hpp"
#include "../include/LoxInstance.hpp"

size_t LoxClass::arity() {
    auto initializer = findMethod("init");
    if (initializer) {
        return initializer->arity();
    }

    return 0;
}
std::any LoxClass::call(Interpreter& interpreter, const std::vector<std::any>& arguments) {
    auto loxInstance = std::make_shared<LoxInstance>(shared_from_this());
    auto initializer = findMethod("init");
    if (initializer != nullptr) {
        initializer->bind(loxInstance)->call(interpreter, arguments);
    }
    return loxInstance;
}

std::string LoxClass::toString() const {
    return name;
}

std::shared_ptr<LoxFunction> LoxClass::findMethod(const std::string& name) const {
    if (methods.contains(name)) {
        return methods.at(name);
    }
    if (superclass != nullptr) {
        return superclass->findMethod(name);
    }

    return nullptr;
}