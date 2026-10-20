#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// ==================== 扩展函数组表（编译器可见，纯名字） ====================
// 编译器用它展开 `#include <组名>` 并校验调用（函数未包含时提示所需模块）。
// 实现位于 src/vm/builtins/builtin_*.cpp（按组合并的独立模块），
// 启动时 initBuiltinFunctions 会做一致性检查。
namespace builtin_groups {

typedef std::unordered_map<std::string, std::vector<std::string> > GroupTable;

// 组表：组名 → 函数名列表（哈希表<vector>）
const GroupTable& table();

// 函数名 → 所在组名；未找到返回空串
std::string groupOf(const std::string& funcName);

// 所有可用组名（", " 分隔，错误提示用）
std::string availableGroups();

// 返回 32 位 long（低字在 R0、高字在 R1）的扩展函数（step-19）；
// 编译器据此生成双字接收：`long x = mul(a,b);`
bool isLongReturnBuiltin(const std::string& funcName);

}  // namespace builtin_groups
