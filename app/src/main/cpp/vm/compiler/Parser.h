#pragma once

#include "common/Token.h"
#include "AST.h"

#include <string>
#include <vector>

// ==================== 语法分析器 ====================
class Parser {
private:
    std::vector<Token> tokens;
    size_t pos;
    bool verbose;
    Token& current();
    Token& peek(int offset = 1);
    void advance();
    bool match(TokenType type);
    void expect(TokenType type, const std::string& error);
    std::string tokenTypeToString(TokenType type);

    // 是否为类型起始 token
    bool isTypeStart(TokenType t) const;

    // 类型名（用于错误信息与声明列表标签）
    std::string typeNameOf(const TypeSpec& ts) const;

    // 解析类型（支持全部标量/struct/指针）；失败不消耗 token
    bool parseType(TypeSpec& out);

    // 函数指针声明符: (*name)(参数类型列表)（在 parseType 之后调用）
    bool tryParseFuncPtr(TypeSpec& out, std::string& name);

public:
    Parser(const std::vector<Token>& toks, bool verbose = true);

    ASTNode* parse();

    // ==================== 程序结构 ====================
    ASTNode* parseProgram();
    ASTNode* parseStructDef();
    ASTNode* parseInclude();
    ASTNode* parseFunction();
    ASTNode* parseBlock();

    // ==================== 语句 ====================
    ASTNode* parseStatement();
    ASTNode* parseVarDecl();
    ASTNode* parseIf();
    ASTNode* parseWhile();
    ASTNode* parseDoWhile();
    ASTNode* parseFor();
    ASTNode* parseGoto();

    // ==================== 表达式解析（支持指针） ====================
    ASTNode* parseExpression();
    ASTNode* parseAssignment();
    ASTNode* parseLogicalOr();
    ASTNode* parseLogicalAnd();
    ASTNode* parseComparison();
    ASTNode* parseAdditive();
    ASTNode* parseMultiplicative();
    ASTNode* parseUnary();
    ASTNode* parsePrimary();
};
