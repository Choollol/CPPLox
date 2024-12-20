#include "../include/AstPrinter.hpp"

#include <sstream>

std::any AstPrinter::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    return parenthesize(expr->oper.lexeme, expr->left, expr->right);
}
std::any AstPrinter::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
    return parenthesize("group", expr->expression);
}
std::any AstPrinter::visitLiteralExpr(std::shared_ptr<Literal> expr) {
    const auto& value_type = expr->value.type();

    if (value_type == typeid(nullptr)) {
        return "nil";
    }
    else if (value_type == typeid(std::string)) {
        return std::any_cast<std::string>(expr->value);
    }
    else if (value_type == typeid(double)) {
        return std::to_string(std::any_cast<double>(expr->value));
    }
    else if (value_type == typeid(bool)) {
        return std::any_cast<bool>(expr->value) ? "true" : "false";
    }

    return "Unrecognized literal";
}
std::any AstPrinter::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    return parenthesize(expr->oper.lexeme, expr->right);
}

template <typename... Args>
std::string AstPrinter::parenthesize(std::string_view name, Args... args) {
    std::ostringstream os;
    os << "(" << name;
    ((os << " " << print(args)), ...);
    os << ")";

    return os.str();
}