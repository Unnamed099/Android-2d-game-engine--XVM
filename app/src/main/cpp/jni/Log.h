#ifndef CPP_JNI_LOG_H
#define CPP_JNI_LOG_H


#include <android/log.h>
#include <string>
#include <sstream>
#include <cstdarg>
#include <cstdio>

// ═══════════════════════════════════════════════════════════════
//  日志开关
//  LOG_DEBUG_ENABLED: Debug 日志总开关（Log::d）
//  LOG_INFO_ENABLED : Info 日志总开关（Log::i）
//  LOG_WARN_ENABLED : Warn 日志总开关（Log::w）
//  LOG_ERROR_ENABLED: Error 日志总开关（Log::e）
//  LOG_VERBOSE_ENABLED: Verbose 日志总开关（Log::v）
//
//  把对应宏设为 0，编译时该级别日志会被完全优化掉，零开销
// ═══════════════════════════════════════════════════════════════
#ifndef LOG_DEBUG_ENABLED
#define LOG_DEBUG_ENABLED 1
#endif

#ifndef LOG_INFO_ENABLED
#define LOG_INFO_ENABLED 1
#endif

#ifndef LOG_WARN_ENABLED
#define LOG_WARN_ENABLED 1
#endif

#ifndef LOG_ERROR_ENABLED
#define LOG_ERROR_ENABLED 1
#endif

#ifndef LOG_VERBOSE_ENABLED
#define LOG_VERBOSE_ENABLED 1
#endif

class Log {
public:
    // ═══════════════════════════════════════════════════════
    //  Debug 级别
    // ═══════════════════════════════════════════════════════
#if LOG_DEBUG_ENABLED
    static void d(const std::string& tag, const std::string& msg) {
        __android_log_print(ANDROID_LOG_DEBUG, tag.c_str(), "%s", msg.c_str());
    }
    static void d(const std::string& tag, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        __android_log_vprint(ANDROID_LOG_DEBUG, tag.c_str(), fmt, args);
        va_end(args);
    }
    template<typename T>
    static void d(const std::string& tag, const T& value) {
        std::ostringstream oss;
        oss << value;
        d(tag, oss.str());
    }
#else
    static void d(const std::string&, const std::string&) {}
    static void d(const std::string&, const char*, ...) {}
    template<typename T>
    static void d(const std::string&, const T&) {}
#endif

    // ═══════════════════════════════════════════════════════
    //  Info 级别
    // ═══════════════════════════════════════════════════════
#if LOG_INFO_ENABLED
    static void i(const std::string& tag, const std::string& msg) {
        __android_log_print(ANDROID_LOG_INFO, tag.c_str(), "%s", msg.c_str());
    }
    static void i(const std::string& tag, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        __android_log_vprint(ANDROID_LOG_INFO, tag.c_str(), fmt, args);
        va_end(args);
    }
    template<typename T>
    static void i(const std::string& tag, const T& value) {
        std::ostringstream oss;
        oss << value;
        i(tag, oss.str());
    }
#else
    static void i(const std::string&, const std::string&) {}
    static void i(const std::string&, const char*, ...) {}
    template<typename T>
    static void i(const std::string&, const T&) {}
#endif

    // ═══════════════════════════════════════════════════════
    //  Warn 级别
    // ═══════════════════════════════════════════════════════
#if LOG_WARN_ENABLED
    static void w(const std::string& tag, const std::string& msg) {
        __android_log_print(ANDROID_LOG_WARN, tag.c_str(), "%s", msg.c_str());
    }
    static void w(const std::string& tag, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        __android_log_vprint(ANDROID_LOG_WARN, tag.c_str(), fmt, args);
        va_end(args);
    }
    template<typename T>
    static void w(const std::string& tag, const T& value) {
        std::ostringstream oss;
        oss << value;
        w(tag, oss.str());
    }
#else
    static void w(const std::string&, const std::string&) {}
    static void w(const std::string&, const char*, ...) {}
    template<typename T>
    static void w(const std::string&, const T&) {}
#endif

    // ═══════════════════════════════════════════════════════
    //  Error 级别
    // ═══════════════════════════════════════════════════════
#if LOG_ERROR_ENABLED
    static void e(const std::string& tag, const std::string& msg) {
        __android_log_print(ANDROID_LOG_ERROR, tag.c_str(), "%s", msg.c_str());
    }
    static void e(const std::string& tag, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        __android_log_vprint(ANDROID_LOG_ERROR, tag.c_str(), fmt, args);
        va_end(args);
    }
    template<typename T>
    static void e(const std::string& tag, const T& value) {
        std::ostringstream oss;
        oss << value;
        e(tag, oss.str());
    }
#else
    static void e(const std::string&, const std::string&) {}
    static void e(const std::string&, const char*, ...) {}
    template<typename T>
    static void e(const std::string&, const T&) {}
#endif

    // ═══════════════════════════════════════════════════════
    //  Verbose 级别
    // ═══════════════════════════════════════════════════════
#if LOG_VERBOSE_ENABLED
    static void v(const std::string& tag, const std::string& msg) {
        __android_log_print(ANDROID_LOG_VERBOSE, tag.c_str(), "%s", msg.c_str());
    }
    static void v(const std::string& tag, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        __android_log_vprint(ANDROID_LOG_VERBOSE, tag.c_str(), fmt, args);
        va_end(args);
    }
    template<typename T>
    static void v(const std::string& tag, const T& value) {
        std::ostringstream oss;
        oss << value;
        v(tag, oss.str());
    }
#else
    static void v(const std::string&, const std::string&) {}
    static void v(const std::string&, const char*, ...) {}
    template<typename T>
    static void v(const std::string&, const T&) {}
#endif
};

#endif // CPP_JNI_LOG_H