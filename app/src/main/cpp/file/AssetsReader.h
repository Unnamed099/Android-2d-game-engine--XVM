#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>

class AssetsReader {
public:
    // ── 读取接口 ──
    static std::vector<uint8_t> readFile(const std::string& name);
    static std::string          readText(const std::string& name);
    static bool                 exists(const std::string& name);

    // ── 缓存控制 ──
    static void   enableCache(bool enable);
    static void   setMaxCacheCount(int count);
    static void   setMaxCacheSize(int bytes);
    static void   clearCache();
    static int    getCacheSize();
    static int    getHitCount();
    static int    getMissCount();
    static double getHitRate();

private:
    static std::vector<uint8_t> readFromAssets(const std::string& name);

    // ── 静态成员 ──
    static bool  cacheEnabled_;
    static int   maxCacheCount_;
    static int   maxCacheSize_;
    static std::map<std::string, std::vector<uint8_t>> cache_;
    static int   hitCount_;
    static int   missCount_;
};