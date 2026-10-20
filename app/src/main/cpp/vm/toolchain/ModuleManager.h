#pragma once

#include "runtime/ModuleLoader.h"

#include <string>
#include <vector>

// ==================== 多模块统一管理（解析链 .x → .asm → 二进制） ====================
class ModuleManager {
public:
    // 从 X 源文件加载模块（内部：编译为 .asm → 汇编为二进制 → 加载）
    bool addModuleFromXFile(const std::string& name, const std::string& path);

    // 从 .asm 汇编文件加载模块
    bool addModuleFromAsmFile(const std::string& name, const std::string& path);

    // 从字符串加载模块（内存编译，无临时文件）；errMsg 非空时写入错误
    bool addModuleFromString(const std::string& name, const std::string& source,
                             std::string* errMsg = NULL);

    // 从二进制加载模块
    bool addModuleFromBinary(const std::string& name,
                             const std::string& codeBin,
                             const std::string& dataBin);

    // 按给定搜索目录解析链自动加载：dir/<name>.x → .asm → _code.bin/_data.bin
    bool resolveAndLoad(const std::string& name, const std::string& searchDir);

    // 设置搜索目录（一参 resolveAndLoad 与 runModule 使用）
    void setSearchDir(const std::string& dir);

    // 按已设置的搜索目录解析并加载
    bool resolveAndLoad(const std::string& name);

    // 运行模块（新建 CPU；返回值 0 表示成功）
    int runModule(const std::string& name,
                  const std::vector<uint16_t>& args = std::vector<uint16_t>());

    bool hasModule(const std::string& name) const;

    ModuleLoader& loader();
    const ModuleLoader& loader() const;

private:
    ModuleLoader moduleLoader;
    std::string searchDir;
};
