// JsonToMap.h
#ifndef CPP_UNITS_JSONTOMAP_H
#define CPP_UNITS_JSONTOMAP_H

#include <string>
#include <vector>
#include <map>
#include "jni/SimpleJSON.h"

class JsonToMap {
public:
    class Value {
    public:
        enum Type { Null, String, Number, Boolean, Object, Array };
        Type type = Null;
        std::string stringValue;
        double numberValue = 0;
        bool boolValue = false;
        std::map<std::string, Value> objectValue;
        std::vector<Value> arrayValue;
    };

    static std::map<std::string, Value> jsonToMap(const std::string& jsonStr);
    static std::vector<Value> jsonArrayToList(const SimpleJSON& arr);

private:
    static std::map<std::string, Value> jsonToMapObj(const SimpleJSON& obj);
    static Value toValue(const SimpleJSON& v);
};

#endif