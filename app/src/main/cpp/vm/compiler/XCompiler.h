#pragma once

#include <string>

// ==================== 编译器主类 ====================
class XCompiler {
public:
    // verbose=false 时静默编译（运行时按需编译模块用）
    static bool compile(const std::string& inputFile, const std::string& outputFile,
                        bool verbose = true);

    // 从内存源码编译为汇编文本（无文件 IO）；errMsg 非空时错误写入其中（不打印 stderr）
    static bool compileSource(const std::string& source, const std::string& fileName,
                              std::string& asmOut, std::string* errMsg = NULL,
                              bool verbose = false);
};
