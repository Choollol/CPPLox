#ifndef CPPLOX_INCLUDE_LOXINSTANCE_HPP
#define CPPLOX_INCLUDE_LOXINSTANCE_HPP

#include "LoxClass.hpp"

class LoxInstance {
   public:
    LoxInstance(std::shared_ptr<LoxClass> loxCl) : loxClass(loxCl) {}

    std::string toString();

    std::any get(const Token&);
    void set(const Token&, const std::any&);

   private:
    const std::shared_ptr<LoxClass> loxClass;
    std::map<std::string, std::any> fields;
};

#endif