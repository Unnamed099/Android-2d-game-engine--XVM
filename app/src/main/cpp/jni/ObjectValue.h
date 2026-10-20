#ifndef OBJECTVALUE_H
#define OBJECTVALUE_H

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <cctype>
#include <stdexcept>

class ObjectValue {
public:
    enum Type { NIL, INT, FLOAT, DOUBLE, STRING, BOOL, LIST, MAP } type;

    int intVal = 0;
    float floatVal = 0.0f;
    double doubleVal = 0.0;
    bool boolVal = false;
    std::string stringVal;
    std::vector<ObjectValue> listVal;
    std::map<std::string, ObjectValue> mapVal;

    ObjectValue() : type(NIL) {}

    // ==================== 工厂方法 ====================
    static ObjectValue makeInt(int v) {
        ObjectValue o; o.type = INT; o.intVal = v; return o;
    }
    static ObjectValue makeFloat(float v) {
        ObjectValue o; o.type = FLOAT; o.floatVal = v; return o;
    }
    static ObjectValue makeDouble(double v) {
        ObjectValue o; o.type = DOUBLE; o.doubleVal = v; return o;
    }
    static ObjectValue makeString(const std::string& v) {
        ObjectValue o; o.type = STRING; o.stringVal = v; return o;
    }
    static ObjectValue makeBool(bool v) {
        ObjectValue o; o.type = BOOL; o.boolVal = v; return o;
    }
    static ObjectValue makeList(const std::vector<ObjectValue>& v) {
        ObjectValue o; o.type = LIST; o.listVal = v; return o;
    }
    static ObjectValue makeMap(const std::map<std::string, ObjectValue>& v) {
        ObjectValue o; o.type = MAP; o.mapVal = v; return o;
    }

    // ==================== 序列化：JSON ====================
    std::string toJsonString() const {
        std::ostringstream oss;
        writeJson(oss);
        return oss.str();
    }

    // ==================== 反序列化：JSON ====================
    // 从 JSON 字符串解析为 map<string, ObjectValue>（要求根是对象）
    static std::map<std::string, ObjectValue> fromJsonString(const std::string& json) {
        size_t pos = 0;
        skipWhitespace(json, pos);
        ObjectValue root = parseValue(json, pos);
        skipWhitespace(json, pos);
        if (pos != json.size()) {
            throw std::runtime_error("JSON parse error: trailing characters at position "
                                     + std::to_string(pos));
        }
        if (root.type != MAP) {
            throw std::runtime_error("JSON root is not an object");
        }
        return root.mapVal;
    }

    // ==================== 与 map<string,string> 互转 ====================

    // map<string,string> → map<string,ObjectValue>（每个 value 包成 STRING）
    static std::map<std::string, ObjectValue>
    fromStringMap(const std::map<std::string, std::string>& in) {
        std::map<std::string, ObjectValue> out;
        for (const auto& kv : in) {
            out.emplace(kv.first, makeString(kv.second));
        }
        return out;
    }

    // vector<map<string,string>> → ObjectValue（LIST of MAP of STRING）
    static ObjectValue
    fromStringMapVector(const std::vector<std::map<std::string, std::string>>& vec) {
        std::vector<ObjectValue> list;
        list.reserve(vec.size());
        for (const auto& m : vec) {
            list.push_back(makeMap(fromStringMap(m)));
        }
        return makeList(list);
    }

    // map<string,ObjectValue> → map<string,string>（非 STRING 通过 toString() 转）
    static std::map<std::string, std::string>
    toStringMap(const std::map<std::string, ObjectValue>& in) {
        std::map<std::string, std::string> out;
        for (const auto& kv : in) {
            out.emplace(kv.first, kv.second.toString());
        }
        return out;
    }

    // ObjectValue(LIST of MAP) → vector<map<string,string>>
    static std::vector<std::map<std::string, std::string>>
    toStringMapVector(const ObjectValue& v) {
        std::vector<std::map<std::string, std::string>> out;
        if (v.type != LIST) return out;
        out.reserve(v.listVal.size());
        for (const auto& item : v.listVal) {
            if (item.type != MAP) continue;
            out.push_back(toStringMap(item.mapVal));
        }
        return out;
    }

    // ==================== 单值转字符串 ====================
    // STRING 原样返回；NIL → ""；标量转字面量；LIST/MAP 转 JSON 字符串
    std::string toString() const {
        switch (type) {
            case NIL:    return "";
            case INT:    return std::to_string(intVal);
            case FLOAT:  return std::to_string(floatVal);
            case DOUBLE: return std::to_string(doubleVal);
            case BOOL:   return boolVal ? "true" : "false";
            case STRING: return stringVal;
            case LIST:
            case MAP: {
                std::ostringstream oss;
                writeJson(oss);
                return oss.str();
            }
        }
        return "";
    }

private:
    // ---------- JSON 序列化辅助 ----------
    static void escapeString(std::ostringstream& oss, const std::string& s) {
        oss << '"';
        for (char c : s) {
            switch (c) {
                case '"':  oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\b': oss << "\\b";  break;
                case '\f': oss << "\\f";  break;
                case '\n': oss << "\\n";  break;
                case '\r': oss << "\\r";  break;
                case '\t': oss << "\\t";  break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20) {
                        char buf[8];
                        std::snprintf(buf, sizeof(buf), "\\u%04x",
                                      static_cast<unsigned char>(c));
                        oss << buf;
                    } else {
                        oss << c;
                    }
            }
        }
        oss << '"';
    }

    void writeJson(std::ostringstream& oss) const {
        switch (type) {
            case NIL:    oss << "null"; break;
            case INT:    oss << intVal; break;
            case FLOAT:  oss << std::setprecision(9) << floatVal; break;
            case DOUBLE: oss << std::setprecision(17) << doubleVal; break;
            case BOOL:   oss << (boolVal ? "true" : "false"); break;
            case STRING: escapeString(oss, stringVal); break;
            case LIST: {
                oss << '[';
                for (size_t i = 0; i < listVal.size(); ++i) {
                    if (i > 0) oss << ',';
                    listVal[i].writeJson(oss);
                }
                oss << ']';
                break;
            }
            case MAP: {
                oss << '{';
                bool first = true;
                for (const auto& kv : mapVal) {
                    if (!first) oss << ',';
                    first = false;
                    escapeString(oss, kv.first);
                    oss << ':';
                    kv.second.writeJson(oss);
                }
                oss << '}';
                break;
            }
        }
    }

    // ---------- JSON 反序列化辅助 ----------
    static void skipWhitespace(const std::string& s, size_t& pos) {
        while (pos < s.size()) {
            char c = s[pos];
            if (c == ' ' || c == '\t' || c == '\n' || c == '\r') ++pos;
            else break;
        }
    }

    static char peek(const std::string& s, size_t pos) {
        if (pos >= s.size()) {
            throw std::runtime_error("JSON parse error: unexpected end of input");
        }
        return s[pos];
    }

    static void expect(const std::string& s, size_t& pos, char c) {
        if (pos >= s.size() || s[pos] != c) {
            throw std::runtime_error(std::string("JSON parse error: expected '") + c
                                     + "' at position " + std::to_string(pos));
        }
        ++pos;
    }

    static ObjectValue parseValue(const std::string& s, size_t& pos) {
        skipWhitespace(s, pos);
        char c = peek(s, pos);
        switch (c) {
            case '{': return parseObject(s, pos);
            case '[': return parseArray(s, pos);
            case '"': return makeString(parseString(s, pos));
            case 't': case 'f': return parseBool(s, pos);
            case 'n': return parseNull(s, pos);
            default:  return parseNumber(s, pos);
        }
    }

    static ObjectValue parseObject(const std::string& s, size_t& pos) {
        expect(s, pos, '{');
        std::map<std::string, ObjectValue> m;
        skipWhitespace(s, pos);
        if (peek(s, pos) == '}') { ++pos; return makeMap(m); }
        while (true) {
            skipWhitespace(s, pos);
            std::string key = parseString(s, pos);
            skipWhitespace(s, pos);
            expect(s, pos, ':');
            ObjectValue val = parseValue(s, pos);
            m.emplace(std::move(key), std::move(val));
            skipWhitespace(s, pos);
            char c = peek(s, pos);
            if (c == ',') { ++pos; continue; }
            else if (c == '}') { ++pos; break; }
            else throw std::runtime_error(std::string("JSON parse error: expected ',' or '}' at position ") + std::to_string(pos));
        }
        return makeMap(m);
    }

    static ObjectValue parseArray(const std::string& s, size_t& pos) {
        expect(s, pos, '[');
        std::vector<ObjectValue> v;
        skipWhitespace(s, pos);
        if (peek(s, pos) == ']') { ++pos; return makeList(v); }
        while (true) {
            v.push_back(parseValue(s, pos));
            skipWhitespace(s, pos);
            char c = peek(s, pos);
            if (c == ',') { ++pos; continue; }
            else if (c == ']') { ++pos; break; }
            else throw std::runtime_error(std::string("JSON parse error: expected ',' or ']' at position ") + std::to_string(pos));
        }
        return makeList(v);
    }

    static std::string parseString(const std::string& s, size_t& pos) {
        expect(s, pos, '"');
        std::string result;
        while (pos < s.size()) {
            char c = s[pos++];
            if (c == '"') return result;
            if (c == '\\') {
                if (pos >= s.size())
                    throw std::runtime_error("JSON parse error: incomplete escape sequence");
                char e = s[pos++];
                switch (e) {
                    case '"':  result += '"';  break;
                    case '\\': result += '\\'; break;
                    case '/':  result += '/';  break;
                    case 'b':  result += '\b'; break;
                    case 'f':  result += '\f'; break;
                    case 'n':  result += '\n'; break;
                    case 'r':  result += '\r'; break;
                    case 't':  result += '\t'; break;
                    case 'u': {
                        if (pos + 4 > s.size())
                            throw std::runtime_error("JSON parse error: incomplete \\u escape");
                        unsigned int code = 0;
                        for (int i = 0; i < 4; ++i) {
                            char h = s[pos++];
                            code <<= 4;
                            if (h >= '0' && h <= '9')      code |= (h - '0');
                            else if (h >= 'a' && h <= 'f') code |= (h - 'a' + 10);
                            else if (h >= 'A' && h <= 'F') code |= (h - 'A' + 10);
                            else throw std::runtime_error("JSON parse error: invalid \\u hex digit");
                        }
                        // UTF-8 编码（不处理代理对）
                        if (code < 0x80) {
                            result += static_cast<char>(code);
                        } else if (code < 0x800) {
                            result += static_cast<char>(0xC0 | (code >> 6));
                            result += static_cast<char>(0x80 | (code & 0x3F));
                        } else {
                            result += static_cast<char>(0xE0 | (code >> 12));
                            result += static_cast<char>(0x80 | ((code >> 6) & 0x3F));
                            result += static_cast<char>(0x80 | (code & 0x3F));
                        }
                        break;
                    }
                    default:
                        throw std::runtime_error("JSON parse error: invalid escape character");
                }
            } else {
                result += c;
            }
        }
        throw std::runtime_error("JSON parse error: unterminated string");
    }

    static ObjectValue parseBool(const std::string& s, size_t& pos) {
        if (s.compare(pos, 4, "true") == 0)  { pos += 4; return makeBool(true); }
        if (s.compare(pos, 5, "false") == 0) { pos += 5; return makeBool(false); }
        throw std::runtime_error("JSON parse error: invalid literal at position " + std::to_string(pos));
    }

    static ObjectValue parseNull(const std::string& s, size_t& pos) {
        if (s.compare(pos, 4, "null") == 0) { pos += 4; return ObjectValue(); }
        throw std::runtime_error("JSON parse error: invalid literal at position " + std::to_string(pos));
    }

    static ObjectValue parseNumber(const std::string& s, size_t& pos) {
        size_t start = pos;
        bool isFloat = false;
        if (pos < s.size() && (s[pos] == '-' || s[pos] == '+')) ++pos;
        while (pos < s.size() && std::isdigit(static_cast<unsigned char>(s[pos]))) ++pos;
        if (pos < s.size() && s[pos] == '.') {
            isFloat = true; ++pos;
            while (pos < s.size() && std::isdigit(static_cast<unsigned char>(s[pos]))) ++pos;
        }
        if (pos < s.size() && (s[pos] == 'e' || s[pos] == 'E')) {
            isFloat = true; ++pos;
            if (pos < s.size() && (s[pos] == '-' || s[pos] == '+')) ++pos;
            while (pos < s.size() && std::isdigit(static_cast<unsigned char>(s[pos]))) ++pos;
        }
        if (pos == start)
            throw std::runtime_error("JSON parse error: invalid number at position " + std::to_string(start));
        std::string num = s.substr(start, pos - start);
        if (isFloat) return makeDouble(std::stod(num));
        try { return makeInt(std::stoi(num)); }
        catch (const std::out_of_range&) { return makeDouble(std::stod(num)); }
    }
};

#endif // OBJECTVALUE_H