#pragma once

#include <cstddef>
#include <string>

class SegMemory;

// ==================== 二进制加载器 ====================
class BinaryLoader {
public:
    // 从文件加载代码段
    static bool loadCode(const std::string& filename, SegMemory& codeSeg, size_t offset = 0);

    // 从文件加载数据段
    static bool loadData(const std::string& filename, SegMemory& dataSeg, size_t offset = 0);

    // 从文件加载指定内存段（通用实现）
    static bool load(const std::string& filename, SegMemory& seg, size_t offset = 0);

    // 便捷方法：加载代码并运行
    static bool runFromFile(const std::string& codeFile, const std::string& dataFile = "");
};
