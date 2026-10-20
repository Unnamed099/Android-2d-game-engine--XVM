#ifndef CPP_GAMEGLOBAL_H
#define CPP_GAMEGLOBAL_H

#include <map>
#include <vector>
#include <string>
#include <cstdint>
#include <functional>
#include <iosfwd>
#include "JsonUIParser.h"

// std::map<std::string, JSONArray> 作为静态成员需要 JSONArray 的完整定义

class DataBus;
class Paint;
class Context;
class SimpleJSON;
/*
class Render {
public:
    using OnDrawCallback = std::function<void()>;
};

 */
class Main;
class GameGlobal {
public:
    static DataBus* databus;
    static Paint* paint;
    static Context* context;
    static std::map<std::string, std::vector<uint8_t>> fileInputStream;
    static std::map<std::string, SimpleJSON> function;
private:
public:
    static std::vector<std::string> global_events;
    static std::vector<std::string> add_global_events;
    static JsonUIParser* ui;
    static Main* game_main;

public:
    static void addGlobalEvent(const std::string& s);
    static std::istream* getResource(const std::string& name);
    static std::string getResourceString(const std::string& name);
};

#endif