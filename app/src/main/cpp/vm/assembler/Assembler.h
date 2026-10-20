#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>
#include <unordered_map>
#include <vector>

// ==================== 汇编器（.asm -> _code.bin / _data.bin） ====================
class Assembler {
public:
    // 编译文本文件，生成代码段和数据段二进制文件
    static bool compile(const std::string& txtFile,
                        const std::string& codeBinFile,
                        const std::string& dataBinFile);

    // 从字符串汇编（内存路径，无临时文件）；错误写入 errMsg，不打印 stderr
    static bool assembleText(const std::string& text,
                             std::vector<uint8_t>& codeOut,
                             std::vector<uint8_t>& dataOut,
                             std::string& errMsg);

private:
    // 核心汇编（两遍扫描）；errMsg 非空时错误写入其中，否则打印 stderr
    static bool assembleStream(std::istream& in,
                               std::vector<uint8_t>& codeOut,
                               std::vector<uint8_t>& dataOut,
                               std::string* errMsg);

    // 提取标签：如果行中有冒号，冒号前为标签，冒号后为剩余内容
    // 返回是否有标签，设置 label 和 rest
    static bool extractLabel(const std::string& line, std::string& label, std::string& rest);

    // ==================== 数值解析 ====================
    static bool parseValue(const std::string& token, uint16_t& value);
    static bool parseReg(const std::string& token, uint8_t& reg);

    // 解析内存引用，返回方括号内的字符串
    static bool parseMemRef(const std::string& token, std::string& inner);

    // 解析 bp 相对引用: bp+off / bp-off / bp → 16 位偏移（负数用补码表示）
    static bool parseBpRef(const std::string& inner, uint16_t& offset);

    // 解析地址（立即数或标签）
    static bool resolveAddress(const std::string& str,
                               const std::unordered_map<std::string, uint16_t>& labels,
                               uint16_t& addr);

    // ==================== 辅助写入函数 ====================
    static void appendPadding(std::vector<uint8_t>& buffer, size_t n);
    static void append16(std::vector<uint8_t>& buffer, uint16_t val);
    static void emit4(std::vector<uint8_t>& buffer, uint8_t b0, uint8_t b1 = 0,
                      uint8_t b2 = 0, uint8_t b3 = 0);

    // ==================== 数据段解析 ====================
    static bool parseDataLine(const std::string& line, std::vector<uint8_t>& buffer);

    // ==================== 代码段解析 ====================
    static bool parseCodeLine(const std::string& line,
                              std::vector<uint8_t>& buffer,
                              const std::unordered_map<std::string, uint16_t>& codeLabels,
                              const std::unordered_map<std::string, uint16_t>& dataLabels);

    // 辅助函数：发射单寄存器指令
    static bool emitR(std::vector<uint8_t>& buffer, uint8_t opcode, const std::string& r);
    // 辅助函数：发射双寄存器指令
    static bool emitRR(std::vector<uint8_t>& buffer, uint8_t opcode,
                       const std::string& r1, const std::string& r2);
    // 辅助函数：发射寄存器+立即数指令
    static bool emitRI(std::vector<uint8_t>& buffer, uint8_t opcode,
                       const std::string& r, const std::string& imm);
    // 无操作数指令发射
    static bool emitNone(std::vector<uint8_t>& buffer, uint8_t opcode);
};
