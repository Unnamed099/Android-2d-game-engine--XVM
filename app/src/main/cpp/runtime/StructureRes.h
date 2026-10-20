#ifndef CPP_RUNTIME_STRUCTURERES_H
#define CPP_RUNTIME_STRUCTURERES_H

#include <string>
#include <map>
#include <vector>

class InputStream;

class StructureRes {
public:
    class Data {
    public:
        std::string name;
        int w;
        int h;
        std::vector<float> data;
    };

    static const std::string INDEX_PATH;
    static std::map<std::string, Data> res;

    static void init();
    static void parse(const std::string& json);
    static void singleParse(const std::string& path);
    static std::string readAllText(std::istream* inputStream);
};

#endif