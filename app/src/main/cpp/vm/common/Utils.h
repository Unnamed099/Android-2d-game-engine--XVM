#pragma once

#include <string>
#include <vector>

// ==================== 通用字符串工具（C++11） ====================
namespace xutils {

// 去除首尾空白
std::string trim(const std::string& s);

// 按分隔符拆分
std::vector<std::string> split(const std::string& s, char delim);

// 转小写
std::string toLower(std::string s);

} // namespace xutils
