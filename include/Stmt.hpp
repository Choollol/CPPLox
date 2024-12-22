#ifndef CPPLOX_STMT_HPP
#define CPPLOX_STMT_HPP

#include <any>
#include <memory>
#include <vector>
#include "../include/Token.hpp"
#include "Expr.hpp"

class Block;
class Expression;
class If;
class Print;
class Var;

struct StmtVisitor {
	virtual std::any visitBlockStmt(std::shared_ptr<Block> stmt) = 0;
	virtual std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
	virtual std::any visitIfStmt(std::shared_ptr<If> stmt) = 0;
	virtual std::any visitPrintStmt(std::shared_ptr<Print> stmt) = 0;
	virtual std::any visitVarStmt(std::shared_ptr<Var> stmt) = 0;
	virtual ~StmtVisitor() = default;
};

class Stmt {
	public:
	 virtual std::any accept(StmtVisitor& visitor) = 0;
};

class Block : public Stmt, public std::enable_shared_from_this<Block> {
	public:
	Block(const std::vector<std::shared_ptr<Stmt>>& statements) : statements(statements){}

	std::any accept(StmtVisitor& visitor) override {
		return visitor.visitBlockStmt(shared_from_this());
	}

	const std::vector<std::shared_ptr<Stmt>> statements;
};

class Expression : public Stmt, public std::enable_shared_from_this<Expression> {
	public:
	Expression(std::shared_ptr<Expr> expression) : expression(expression){}

	std::any accept(StmtVisitor& visitor) override {
		return visitor.visitExpressionStmt(shared_from_this());
	}

	const std::shared_ptr<Expr> expression;
};

class If : public Stmt, public std::enable_shared_from_this<If> {
	public:
	If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch) : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch){}

	std::any accept(StmtVisitor& visitor) override {
		return visitor.visitIfStmt(shared_from_this());
	}

	const std::shared_ptr<Expr> condition;
	const std::shared_ptr<Stmt> thenBranch;
	const std::shared_ptr<Stmt> elseBranch;
};

class Print : public Stmt, public std::enable_shared_from_this<Print> {
	public:
	Print(std::shared_ptr<Expr> expression) : expression(expression){}

	std::any accept(StmtVisitor& visitor) override {
		return visitor.visitPrintStmt(shared_from_this());
	}

	const std::shared_ptr<Expr> expression;
};

class Var : public Stmt, public std::enable_shared_from_this<Var> {
	public:
	Var(const Token& name, std::shared_ptr<Expr> initializer) : name(name), initializer(initializer){}

	std::any accept(StmtVisitor& visitor) override {
		return visitor.visitVarStmt(shared_from_this());
	}

	const Token name;
	const std::shared_ptr<Expr> initializer;
};

#endif
