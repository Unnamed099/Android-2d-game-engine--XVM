#pragma once

#include "common/Error.h"

#include <cstdint>
#include <string>
#include <vector>

// ==================== 载入来源（任务书 §16.1） ====================
enum class SourceKind {
    String,
    XFile,
    AsmFile,
    Binary
};

struct CompileOptions {
    bool debug;              // 打印编译进度
    bool keepAsm;            // 保留生成的 .asm（.x 载入时）
    std::string moduleName;  // 入口模块名

    CompileOptions() : debug(false), keepAsm(false), moduleName("main") {}
};

struct CompileResult {
    bool success;
    ToolchainError error;
    std::string asmText;
    std::vector<uint8_t> codeBin;
    std::vector<uint8_t> dataBin;

    CompileResult() : success(false) {}
};

// ==================== XToolchain 统一接口（任务书 §16） ====================
class XToolchain {
public:
    // ---- 载入/编译（不运行） ----
    static CompileResult compileFromString(const std::string& source,
                                           const CompileOptions& opt = CompileOptions());
    static CompileResult compileFromXFile(const std::string& path,
                                          const CompileOptions& opt = CompileOptions());
    static CompileResult compileFromAsmFile(const std::string& path,
                                            const CompileOptions& opt = CompileOptions());
    static CompileResult loadFromBinary(const std::string& codeBinPath,
                                        const std::string& dataBinPath,
                                        const CompileOptions& opt = CompileOptions());

    // 自动识别输入类型并编译（.x / .asm / .bin / 字符串），searchDir 返回解析链搜索目录
    // allowString=false 时非文件输入直接报“找不到文件”
    static CompileResult compileAuto(const std::string& input, const CompileOptions& opt,
                                     std::string& searchDir, bool allowString = true);

    // ---- 直接运行（返回 0 成功；错误按统一结构打印） ----
    static int runFromString(const std::string& source,
                             const CompileOptions& opt = CompileOptions());
    static int runFromXFile(const std::string& path,
                            const CompileOptions& opt = CompileOptions());
    static int runFromAsmFile(const std::string& path,
                              const CompileOptions& opt = CompileOptions());
    static int runFromBinary(const std::string& codeBinPath,
                             const std::string& dataBinPath,
                             const CompileOptions& opt = CompileOptions());

    // 自动识别输入类型并直接运行（单工具统一入口）
    static int runAuto(const std::string& input,
                       const CompileOptions& opt = CompileOptions(),
                       bool allowString = true);

    // 统一错误打印：错误: [stage] file:line: message
    static void printError(const ToolchainError& err);
};
