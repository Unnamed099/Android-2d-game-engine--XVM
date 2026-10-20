#pragma once

#include "vm/CPU.h"

#include <string>
#include <unordered_map>
#include <vector>

// ==================== 扩展函数注册表（哈希表<vector>：组名 → 函数向量） ====================
struct BuiltinEntry {
    std::string name;
    BuiltinFunction fn;

    BuiltinEntry(const std::string& n, const BuiltinFunction& f) : name(n), fn(f) {}
};

// 参数结构体约定（step-16）：读取第 index 个 16 位实参（[argsAddr + 2*index]）
inline uint16_t builtinArg(CPU& cpu, uint16_t argsAddr, int index) {
    return cpu.datasegmemory.read<uint16_t>(static_cast<uint16_t>(argsAddr + index * 2));
}

class BuiltinRegistry {
public:
    // 注册一组扩展函数（各 builtin_*.cpp 模块在静态初始化时调用）
    static void addGroup(const std::string& group, const std::vector<BuiltinEntry>& entries);

    // 组表（哈希表<vector>）
    static const std::unordered_map<std::string, std::vector<BuiltinEntry> >& groups();

    // 按函数名跨组查找实现；未找到返回 NULL
    static const BuiltinEntry* find(const std::string& name);
};
