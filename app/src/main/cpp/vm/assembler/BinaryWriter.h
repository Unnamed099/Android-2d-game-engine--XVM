#pragma once

#include <cstdint>
#include <string>
#include <vector>

// ==================== 二进制文件写入 ====================
class BinaryWriter {
public:
    // 将字节缓冲写入文件（二进制模式）
    static bool writeFile(const std::string& filename, const std::vector<uint8_t>& data);
};
