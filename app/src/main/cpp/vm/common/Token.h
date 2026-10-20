#pragma once

#include <string>

// ==================== 词法分析器 ====================
enum TokenType {
    TOK_INT, TOK_VOID, TOK_IF, TOK_ELSE, TOK_WHILE, TOK_FOR,
    TOK_RETURN, TOK_BREAK, TOK_CONTINUE, TOK_STRING, TOK_IMPORT,
    TOK_IDENTIFIER, TOK_NUMBER, TOK_STRING_LITERAL,
    TOK_PLUS, TOK_MINUS, TOK_MULTIPLY, TOK_DIVIDE, TOK_MODULO,
    TOK_ASSIGN, TOK_EQUAL, TOK_NOT_EQUAL, TOK_LESS, TOK_GREATER,
    TOK_LESS_EQUAL, TOK_GREATER_EQUAL,
    TOK_AND, TOK_OR, TOK_NOT,
    TOK_LPAREN, TOK_RPAREN, TOK_LBRACE, TOK_RBRACE,
    TOK_SEMICOLON, TOK_COMMA,
    TOK_AMPERSAND, TOK_POINTER,  // & 和 * 作为独立 token
    TOK_EOF, TOK_UNKNOWN,
    // ==== 类型系统扩展（step-5） ====
    TOK_BYTE, TOK_CHAR, TOK_SHORT, TOK_LONG, TOK_STRUCT,
    TOK_DOT,          // .
    TOK_ARROW,        // ->
    TOK_CHAR_LITERAL, // 'A'（value 为字符编码的十进制字符串）
    // ==== 语句扩展（step-7） ====
    TOK_DO,           // do...while
    // ==== 指针扩展（step-8） ====
    TOK_PHYS,         // phys 物理地址限定符
    // ==== 跨模块跳转（step-11） ====
    TOK_GOTO,         // goto b(...) 跨模块跳转
    // ==== 指针运算（step-14） ====
    TOK_LBRACKET, TOK_RBRACKET,  // [ ]
    // ==== 扩展函数模块（step-15） ====
    TOK_INCLUDE,               // #include <组名>（value = 组名）
    // ==== bool（step-20） ====
    TOK_BOOL                   // bool（0/1，存储同 byte）
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    Token(TokenType t, const std::string& v, int l) : type(t), value(v), line(l) {}
};
