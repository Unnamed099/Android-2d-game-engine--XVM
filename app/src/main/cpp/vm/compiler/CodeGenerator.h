#pragma once

#include "AST.h"

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// ==================== 代码生成器 ====================
class CodeGenerator {
private:
    // 结构体字段/定义
    struct StructField {
        std::string name;
        int offset;       // 字节偏移（按 16 位槽步进）
        TypeSpec type;
    };
    struct StructDef {
        std::vector<StructField> fields;
        int sizeBytes;
        StructDef() : sizeBytes(0) {}
    };

    // 函数局部变量（含参数）在栈帧中的位置
    struct LocalVar {
        std::string name;
        int bpOffset;     // 距 bp 的偏移：局部为负（[bp-2]），参数为正（[bp+4+2i]）
        TypeSpec type;
    };
    struct FuncInfo {
        std::string name;
        ASTNode* node;
        std::vector<LocalVar> locals;
        std::unordered_map<std::string, int> localIndex;
        int nextLocalTop;   // 已分配的局部字节数
        int frameSize;
        int paramCount;
        TypeSpec returnType;  // 函数返回类型（指针表达式推断用）
        std::string endLabel;
        FuncInfo() : node(NULL), nextLocalTop(0), frameSize(0), paramCount(0) {}
    };

    std::vector<std::string> assembly;
    std::unordered_map<std::string, int> globalVars;           // 全局标量（标签发射用）
    std::unordered_map<std::string, TypeSpec> globalVarTypes;  // 全局变量类型
    std::unordered_map<std::string, int> stringLiterals;
    std::unordered_set<std::string> availableBuiltins;  // #include 导入的扩展函数（可用集）
    std::unordered_set<std::string> usedBuiltins;       // 实际调用过的（发射函数名标签）
    // 循环上下文（break/continue 目标；多层循环取最内层）
    struct LoopInfo {
        std::string loopLabel;      // 条件判断标签
        std::string endLabel;       // break 目标
        std::string continueLabel;  // continue 目标
    };
    std::vector<LoopInfo> loopStack;
    int nextReg;
    int labelCounter;
    int nextStringAddr;

    // 结构体
    std::unordered_map<std::string, StructDef> structs;
    std::vector<std::string> globalStructFieldLabels;
    std::vector<std::pair<std::string, std::string> > globalLongEntries;
    bool usesLongPrint;
    std::vector<std::string> gotoModuleNames;  // 跨模块 goto 的模块名字符串标签（step-11）
    std::vector<ASTNode*> globalInits;         // 全局变量初始化（在 main 入口执行一次，step-17）

    // 函数（全部函数生成栈帧；参数/局部都在帧内）
    std::vector<FuncInfo> funcs;
    std::unordered_map<std::string, int> funcIndex;
    FuncInfo* curFunc;

    int allocateReg();
    std::string newLabel(const std::string& prefix);

    // ==================== 位置/类型辅助 ====================
    static std::string bpOperand(int off);
    const TypeSpec* typeOfVar(const std::string& name) const;
    const LocalVar* findLocal(const std::string& name) const;
    std::string varOperand(const std::string& name);
    std::string longWordOperand(const std::string& name, const char* part);
    std::string memberOperand(const ASTNode* member, TypeSpec& fieldTypeOut);
    bool memberTypeOf(const ASTNode* member, TypeSpec& fieldTypeOut) const;
    bool isPointerExpr(const ASTNode* node, TypeSpec& out) const;
    int pointeeSize(const TypeSpec& ptrType) const;
    bool isLongExpr(ASTNode* node) const;
    bool containsCall(ASTNode* node) const;
    int generateLongExpression(ASTNode* node, int& hiRegOut);
    void generateBinaryOperands(ASTNode* left, ASTNode* right, int& leftReg, int& rightReg);

    // ==================== 收集 ====================
    void collectStructDefs(ASTNode* node);
    void collectIncludes(ASTNode* node);
    void collectStrings(ASTNode* node);
    void collectFunctions(ASTNode* ast);
    void collectLocals(FuncInfo* fi, ASTNode* node);
    void addLocal(FuncInfo* fi, const std::string& name, const TypeSpec& ts);
    void declareGlobal(const std::string& name, const TypeSpec& ts);

    // ==================== 生成 ====================
    std::string buildAssembly();
    void generateFunction(FuncInfo* fi);
    void generateBlock(ASTNode* node);
    void generateStatement(ASTNode* node);
    void generateBreak(ASTNode* node);
    void generateContinue(ASTNode* node);
    void generateVarDecl(ASTNode* node);
    void generateIf(ASTNode* node);
    void generateWhile(ASTNode* node);
    void generateDoWhile(ASTNode* node);
    void generateGoto(ASTNode* node);
    void generateFor(ASTNode* node);
    void generateReturn(ASTNode* node);
    void generateCondition(ASTNode* node, const std::string& falseLabel);
    int generateExpression(ASTNode* node);

public:
    CodeGenerator();
    std::string generate(ASTNode* ast);
};
