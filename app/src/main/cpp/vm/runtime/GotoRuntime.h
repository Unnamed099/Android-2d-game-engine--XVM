#pragma once

#include "vm/CPU.h"
#include "runtime/ModuleLoader.h"

#include <cstddef>
#include <functional>
#include <string>
#include <vector>

// ==================== 跨模块跳转运行时（step-10） ====================
// 语义（用户确认，覆盖任务书 §6.2）：
//  - 模块解析顺序：b.x → b.asm → 二进制（b_code.bin/b_data.bin）；均不存在 → 跳过并继续运行；
//  - goto 运行新模块时，旧模块挂起（CPU 状态 + 段副本保存）；新模块 main 结束后恢复旧模块，
//    从 goto 之后继续执行；
//  - 参数从 R0 起依次放（由 gotomodule 指令发生前已就位）。
struct Suspended {
    std::string name;
    size_t pc;
    uint16_t regs[256];
    uint16_t flags;
    uint16_t sp;
    uint16_t bp;
    uint16_t si;
    uint16_t di;
    uint16_t ds;
    uint16_t es;
    SegMemory codeSeg;
    SegMemory dataSeg;
    Suspended() : pc(0), flags(0), sp(0), bp(0), si(0), di(0), ds(0), es(0) {}
};
class GotoRuntime {
public:
    // 模块解析回调：负责按解析链加载 name 模块；成功返回 true
    typedef std::function<bool(const std::string& name)> Resolver;

    GotoRuntime();

    // 运行入口模块直至整个程序结束（处理所有嵌套 goto 挂起/恢复）
    static void run(CPU& cpu, ModuleLoader& loader, const std::string& entry,
             const Resolver& resolver);

    static void capture(CPU& cpu, const std::string& name, Suspended& out);
    static void restore(CPU& cpu, const Suspended& in);
};
