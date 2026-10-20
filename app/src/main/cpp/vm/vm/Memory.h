#pragma once

#include <cstddef>
#include <cstdint>

// ==================== 内存 ====================
class Memory {
private:
    static const size_t SIZE = 65536;
    uint8_t mem[SIZE];
public:
    Memory();
    uint8_t read(size_t offset);
    void write(size_t offset, uint8_t value);
    size_t size() const;
};
