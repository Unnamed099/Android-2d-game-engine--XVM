#pragma once

#include <string>
#include <vector>

// ==================== 类型描述 ====================
enum TypeBase {
    TB_VOID,
    TB_BYTE,     // 1 字节无符号（0..255），占 1 个 16 位槽
    TB_CHAR,     // 1 字节字符（0..255），占 1 个 16 位槽
    TB_SHORT,    // 2 字节（与 VM 字长一致）
    TB_INT,      // 2 字节（与 VM 字长一致）
    TB_LONG,     // 4 字节（2 个 16 位槽：低字/高字）
    TB_STRING,   // 字符串（数据段指针）
    TB_STRUCT    // 结构体（structName 有效）
};

struct TypeSpec {
    TypeBase base;
    int ptrLevel;                       // 指针层数（* 个数）
    std::string structName;             // base == TB_STRUCT 时有效
    bool isFuncPtr;                     // 函数指针类型 int (*fp)(int, int)
    bool isPhys;                        // phys 限定符（物理地址；本 VM 地址空间平坦，语义等同）
    std::vector<TypeSpec> paramTypes;   // 函数指针形参类型

    TypeSpec() : base(TB_VOID), ptrLevel(0), isFuncPtr(false), isPhys(false) {}

    bool isPointer() const { return ptrLevel > 0; }
    bool isPlainStruct() const { return base == TB_STRUCT && ptrLevel == 0; }
    bool isPlainLong() const { return base == TB_LONG && ptrLevel == 0; }

    // 占用的 16 位槽数（结构体为字段之和，由代码生成器按结构体表处理）
    int slotCount() const { return isPlainLong() ? 2 : 1; }
};

// ==================== AST 节点 ====================
struct ASTNode {
    enum Type {
        AST_PROGRAM, AST_FUNCTION, AST_BLOCK, AST_VARDECL,
        AST_IF, AST_WHILE, AST_FOR, AST_RETURN,
        AST_EXPRSTMT, AST_BINOP, AST_UNARYOP, AST_ASSIGN,
        AST_IDENTIFIER, AST_NUMBER, AST_STRING_LITERAL,
        AST_FUNCCALL, AST_INCLUDE,
        AST_BREAK, AST_CONTINUE,
        AST_DECL_LIST,
        AST_POINTER_DEREF,    // 解引用 *ptr
        AST_ADDRESS_OF,       // 取地址 &var
        AST_POINTER_TYPE,     // 指针类型声明
        AST_STRUCTDEF,        // 结构体定义 struct Point { ... };
        AST_MEMBER,           // 成员访问 p.x（isArrow=false）/ p->x（isArrow=true）
        AST_DO_WHILE,         // do...while（children: [body, cond]）
        AST_CAST,             // 类型转换 (type)expr（typeSpec 为转换目标类型）
        AST_GOTO              // goto b(参数)（value = 模块名；children = 参数表达式）
    };

    Type type;
    std::string value;
    std::vector<ASTNode*> children;
    TypeSpec typeSpec;   // 节点的类型描述（替代旧的 isString/ptrLevel 两个字段）
    bool isArrow;        // AST_MEMBER: 是否为 -> 访问
    std::vector<std::string> paramNames;   // AST_FUNCTION: 形参名（供栈帧调用约定）
    std::vector<TypeSpec> paramTypes;      // AST_FUNCTION: 形参类型

    ASTNode(Type t, const std::string& v = "", const TypeSpec& ts = TypeSpec());
    ~ASTNode();
};
