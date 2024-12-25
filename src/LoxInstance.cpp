#include "../include/LoxInstance.hpp"

std::string LoxInstance::toString() {
    return loxClass->name + " instance";
}