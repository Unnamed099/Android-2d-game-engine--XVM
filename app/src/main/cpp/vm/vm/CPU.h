#pragma once

#include "SegMemory.h"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>

// ==================== 标志位定义 ====================
static constexpr uint16_t FLAG_ZF = 0x0001;
static constexpr uint16_t FLAG_CF = 0x0002;
static constexpr uint16_t FLAG_OF = 0x0004;
static constexpr uint16_t FLAG_SF = 0x0008;
static constexpr uint16_t FLAG_PF = 0x0010;
static constexpr uint16_t FLAG_AF = 0x0020;
static constexpr uint16_t FLAG_DF = 0x0040;
static constexpr uint16_t FLAG_IF = 0x0080;

class CPU;

using OpHandler = void(*)(size_t, CPU&);
// 扩展函数签名（step-16 参数结构体约定）：
// 唯一参数 = 参数结构体起始地址；第 k 个实参位于 [addr + 2k]（16 位小端）
using BuiltinFunction = std::function<void(CPU&, uint16_t)>;

// ==================== CPU ====================
class CPU {
private:
    OpHandler oprun[256];
    std::unordered_map<std::string, BuiltinFunction> builtinFuncs;
    void init_oprun();

public:
    SegMemory codesegmemory;
    SegMemory datasegmemory;
    uint16_t regs[256];
    size_t pc;
    uint16_t flags;
    uint16_t sp;
    uint16_t bp;
    uint16_t si;
    uint16_t di;
    uint16_t ds;  // 数据段寄存器
    uint16_t es;  // 附加段寄存器
    bool running;
    // ==== 跨模块跳转（step-10） ====
    bool gotoPending;          // 运行时出现了 goto 模块指令（需挂起/切换）
    uint8_t gotoArgc;          // goto 参数个数（参数在 R0..）
    std::string gotoModule;    // 目标模块名（来自数据段字符串）

    CPU(SegMemory& codesegmemory, SegMemory& datasegmemory);

    void run();
    void run(size_t pc);

    void stop();

    void update_common_flags(uint16_t result);
    void update_flags_add(uint16_t src1, uint16_t src2, uint16_t result);
    void update_flags_sub(uint16_t src1, uint16_t src2, uint16_t result);
    void update_flags_logic(uint16_t result);
    void update_flags_inc(uint16_t result);
    void update_flags_dec(uint16_t result);
    void update_flags_shift_left(uint16_t result, uint16_t original, uint8_t count);
    void update_flags_shift_right(uint16_t result, uint16_t original, uint8_t count);
    void update_flags_cmp(uint16_t src1, uint16_t src2);

    inline void set_flag(uint16_t mask, bool value) {
        if (value) flags |= mask;
        else flags &= ~mask;
    }

    inline bool get_flag(uint16_t mask) const {
        return (flags & mask) != 0;
    }

    void registerBuiltin(const std::string& name, BuiltinFunction func);

    std::unordered_map<std::string, BuiltinFunction>& getBuiltinFuncs();

    // 替换代码段/数据段（模块切换用，step-10）
    void setSegments(const SegMemory& codeSeg, const SegMemory& dataSeg);

    // 空闲内存起始地址（数据段静态占用之后的偶数对齐地址；堆/bump 分配起点，step-18）
    uint16_t freeMemoryStart() const;
};
