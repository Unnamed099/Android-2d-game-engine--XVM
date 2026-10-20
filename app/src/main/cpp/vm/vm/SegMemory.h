#pragma once

#include "Memory.h"

#include <cstddef>
#include <cstdint>
#include <string>

// ==================== 分段内存（数据段/代码段视图） ====================
class SegMemory {
private:
    Memory datamemory;
    size_t loadedBytes;  // 静态占用（加载器记录；空闲区/堆起点依据，step-18）

public:
    SegMemory() : loadedBytes(0) {}

    // 静态占用字节数（加载时记录；堆起点 = 其后的对齐地址）
    size_t loadedSize() const { return loadedBytes; }
    void setLoadedSize(size_t n) {
        if (n > loadedBytes) loadedBytes = n;
    }

    template<typename T>
    T read(size_t offset) {
        T data = 0;
        for (size_t i = 0; i < sizeof(T); ++i) {
            data |= static_cast<T>(datamemory.read(offset + i)) << (i * 8);
        }
        return data;
    }

    std::string readString(size_t offset, size_t maxLength = 256);

    size_t writeString(size_t offset, const std::string& str);

    size_t stringLength(size_t offset, size_t maxLength = 256);

    // 比较两个字符串
    bool compareString(size_t offset1, size_t offset2, size_t maxLength = 256);

    // 复制字符串
    size_t copyString(size_t destOffset, size_t srcOffset, size_t maxLength = 256);

    template<typename T>
    void write(size_t offset, T value) {
        const uint8_t* data = reinterpret_cast<const uint8_t*>(&value);
        for (size_t i = 0; i < sizeof(T); ++i) {
            datamemory.write(offset + i, data[i]);
        }
    }

    size_t size() const;
};
