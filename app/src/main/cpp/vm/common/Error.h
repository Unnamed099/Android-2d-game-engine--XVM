#pragma once

#include <string>

// ==================== 统一错误结构（任务书 §16.8） ====================
// 所有工具链接口通过该结构返回错误，不直接 exit()。
struct ToolchainError {
    bool ok;
    std::string stage;    // lexer/parser/codegen/assembler/vm（当前按编译/汇编/运行粗分）
    std::string file;     // 出错文件；字符串载入为 <string>，二进制载入为文件名
    int line;             // 行号；无法确定时为 -1
    int column;           // 列号；当前实现不支持（恒为 -1）
    std::string message;  // 错误原因

    ToolchainError() : ok(true), line(-1), column(-1) {}
};

// 从“...（第 N 行）”形式的编译器错误信息中提取行号；找不到返回 -1
int extractLineFromMessage(const std::string& msg);
