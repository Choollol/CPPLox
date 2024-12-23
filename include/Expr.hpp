#ifndef CPPLOX_EXPR_HPP
#define CPPLOX_EXPR_HPP

#include <any>
#include <memory>
#include <vector>
#include "../include/Token.hpp"

class Assign;
class Binary;
class Call;
class Grouping;
class Literal;
class Logical;
class Unary;
class Variable;

struct ExprVisitor {
	virtual std::any visitAssignExpr(std::shared_ptr<Assign> expr) = 0;
	virtual std::any visitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
	virtual std::any visitCallExpr(std::shared_ptr<Call> expr) = 0;
	virtual std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
	virtual std::any visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
	virtual std::any visitLogicalExpr(std::shared_ptr<Logical> expr) = 0;
	virtual std::any visitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
	virtual std::any visitVariableExpr(std::shared_ptr<Variable> expr) = 0;
	virtual ~ExprVisitor() = default;
};

class Expr {
	public:
	 virtual std::any accept(ExprVisitor& visitor) = 0;
};

class Assign : public Expr, public std::enable_shared_from_this<Assign> {
	public:
	Assign(const Token& name, std::shared_ptr<Expr> value) : name(name), value(value){}

	std::any accept(ExprVisitor& visitor) override {
		return visitor.visitAssignExpr(shared_from_this());
	}

	const Token name;
	const std::shared_ptr<Expr> value;
};

class Binary : public Expr, public std::enable_shared_from_this<Binary> {
	public:
	Binary(std::shared_ptr<Expr> left, const Token& oper, std::shared_ptr<Expr> right) : left(left), oper(oper), right(right){}

	std::any accept(ExprVisitor& visitor) override {
		return visitor.visitBinaryExpr(shared_from_this());
	}

	const std::shared_ptr<Expr> left;
	const Token oper;
	const std::shared_ptr<Expr> right;
};

class Call : public Expr, public std::enable_shared_from_this<Call> {
	public:
	Call(std::shared_ptr<Expr> callee, const Token& paren, const std::vector<std::shared_ptr<Expr>>& arguments) : callee(callee), paren(paren), arguments(arguments){}

	std::any accept(ExprVisitor& visitor) override {
		return visitor.visitCallExpr(shared_from_this());
	}

	const std::shared_ptr<Expr> callee;
	const Token paren;
	const std::vector<std::shared_ptr<Expr>> arguments;
};

class Grouping : public Expr, public std::enable_shared_from_this<Grouping> {
	public:
	Grouping(std::shared_ptr<Expr> expression) : expression(expression){}

	std::any accept(ExprVisitor& visitor) override {
		return visitor.visitGroupingExpr(shared_from_this());
	}

	const std::shared_ptr<Expr> expression;
};

class Literal : public Expr, public std::enable_shared_from_this<Literal> {
	public:
	Literal(std::any value) : value(value){}

	std::any accept(ExprVisitor& visitor) override {
		return visitor.visitLiteralExpr(shared_from_this());
	}

	const std::any value;
};

class Logical : public Expr, public std::enable_shared_from_this<Logical> {
	public:
	Logical(std::shared_ptr<Expr> left, const Token& oper, std::shared_ptr<Expr> right) : left(left), oper(oper), right(right){}

	std::any accept(ExprVisitor& visitor) override {
		return visitor.visitLogicalExpr(shared_from_this());
	}

	const std::shared_ptr<Expr> left;
	const Token oper;
	const std::shared_ptr<Expr> right;
};

class Unary : public Expr, public std::enable_shared_from_this<Unary> {
	public:
	Unary(const Token& oper, std::shared_ptr<Expr> right) : oper(oper), right(right){}

	std::any accept(ExprVisitor& visitor) override {
		return visitor.visitUnaryExpr(shared_from_this());
	}

	const Token oper;
	const std::shared_ptr<Expr> right;
};

class Variable : public Expr, public std::enable_shared_from_this<Variable> {
	public:
	Variable(const Token& name) : name(name){}

	std::any accept(ExprVisitor& visitor) override {
		return visitor.visitVariableExpr(shared_from_this());
	}

	const Token name;
};

#endif
