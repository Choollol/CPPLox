#ifndef CPPLOX_INCLUDE_LOXINSTANCE_HPP
#define CPPLOX_INCLUDE_LOXINSTANCE_HPP

#include "LoxClass.hpp"

class LoxInstance {
   public:
    LoxInstance(std::shared_ptr<LoxClass> loxCl) : loxClass(loxCl) {}

    std::string toString();

   private:
    std::shared_ptr<LoxClass> loxClass;
};

#endif