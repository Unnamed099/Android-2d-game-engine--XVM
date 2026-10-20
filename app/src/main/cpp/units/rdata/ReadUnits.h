#ifndef CPP_UNITS_RDATA_READUNITS_H
#define CPP_UNITS_RDATA_READUNITS_H

#include <string>
#include <vector>
#include <mutex>
#include <thread>

class LoadUnits;

class ReadUnits {
public:
    ReadUnits(LoadUnits* load);
    ~ReadUnits();

    // 对应 Java getDatas(): 返回原始 JSON 字符串列表
    std::vector<std::string> getDatas();

    // 对应 Java getData(int): 返回原始 JSON 字符串
    std::string getData(int index);

    void destroy();

private:
    struct JsonFileResult {
        std::string path;
        std::string data;
        bool hasError = false;
        std::string errorMessage;
    };

    LoadUnits* load_ = nullptr;
    std::string indexfile_ = "index.json";

    std::vector<std::string> jsonData_;

    mutable std::mutex jsonDataMutex_;

    std::thread initThread_;

    void init();
    std::string readFile(const std::string& filePath);
    JsonFileResult loadJsonFile(const std::string& filePath);
    void parseIndexFile(const std::string& content, std::vector<std::string>& fileList);
};

#endif