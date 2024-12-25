#ifndef CPPLOX_INCLUDE_LOXINSTANCE_HPP
#define CPPLOX_INCLUDE_LOXINSTANCE_HPP

#include <memory>
#include <string>
#include <any>
#include <map>

#include "Token.hpp"

class LoxClass;

class LoxInstance : public std::enable_shared_from_this<LoxInstance> {
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