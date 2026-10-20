//
// JNIBridgeIds.h —— 纯 C++ 的 GPU 资源 id 池
//
// 设计目标：
//   1. 与 Java 完全脱钩
//   2. 每种资源类型（Bitmap/Canvas/Paint/...）独立 id 空间
//   3. id 单调递增，不复用（避免裸指针 + id 复用导致的误删）
//   4. 线程安全由调用方保证（当前都在 GL 线程操作，无需锁）
//
#ifndef 仙垣_JNIBRIDGE_IDS_H
#define 仙垣_JNIBRIDGE_IDS_H

namespace JNIBridgeIds {

// 分配一个新 id
//   typeName: 资源类型名，如 "Bitmap" / "Canvas" / "Paint" / "Matrix" / "Path"
//   返回：非负整数，首次调用返回 0
    int alloc(const char* typeName);

// 归还一个 id
//   当前实现为空操作（id 不复用，保证安全）
//   保留此接口是为了将来若改成 id 复用 + 代标记（generation）时不用改调用方
    void release(const char* typeName, int id);

// 查询某类型已分配的 id 总数（调试 / 诊断用）
//   注意：不是"当前存活数"，因为 id 不复用，这个值只增不减
    int count(const char* typeName);

// 重置所有池（仅用于测试 / 进程重启场景，正常不该调）
    void resetAll();

}

#endif //仙垣_JNIBRIDGE_IDS_H