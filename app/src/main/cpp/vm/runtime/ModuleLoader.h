#pragma once

#include "vm/SegMemory.h"

#include <map>
#include <string>
#include <vector>

// ==================== 模块 ====================
struct Module {
    std::string name;
    SegMemory codeSeg;   // 独立代码段（入口 = 代码偏移 0，即 main）
    SegMemory dataSeg;   // 独立数据段
};

// ==================== 模块加载器（模块表：名字 → 代码段/数据段） ====================
class ModuleLoader {
public:
    // 从二进制文件加载模块（codeBin 必须存在；dataBin 可为空文件）
    bool loadFromFiles(const std::string& name,
                       const std::string& codeBin,
                       const std::string& dataBin);

    // 从内存缓冲加载模块（字符串编译路径，无文件 IO）
    bool loadFromBuffers(const std::string& name,
                         const std::vector<uint8_t>& code,
                         const std::vector<uint8_t>& data);

    bool hasModule(const std::string& name) const;

    // 返回模块指针；不存在返回 NULL
    Module* getModule(const std::string& name);
    const Module* getModule(const std::string& name) const;

private:
    std::map<std::string, Module> modules;
};
