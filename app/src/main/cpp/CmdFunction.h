#ifndef CPP_CMD_CMDFUNCTION_H
#define CPP_CMD_CMDFUNCTION_H

#include <string>
#include <map>
#include <vector>

#include "jni/SimpleJSON.h"

class CmdFunction {
public:
    static void create();

    // 可选：显式从指定资源路径加载（默认 "function/function.json"）
    static void loadFromResource(const std::string& resourcePath);

private:
    static const std::string DEFAULT_PATH;

    // 与 GameGlobal::function 对应：
    // std::map<std::string, std::vector<uint8_t>> / SimpleJSON
    // 这里使用 SimpleJSON 表示 JSONArray
    static void parseAndStore(const std::string& jsonStr,
                              std::map<std::string, SimpleJSON>& out);
};

#endif // CPP_CMD_CMDFUNCTION_H