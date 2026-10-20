//
// Created by Wu_Ju on 2026/10/5.
//

#ifndef 仙垣_XVM_H
#define 仙垣_XVM_H
#include <iosfwd>
#include <map>
#include <string>
#include "ComponentAnimation.h"
#include "PerlinNoise.h"

class UnitData;
class Units;

struct CompileResult;
struct CPUDATA {
    static Units* unit;
    static ComponentAnimation::SonComponent* anim;
    static std::map<std::string, int> persist;
    static PerlinNoise::RD* noise;
};

class XVM {
public:
    static const std::string INDEX_PATH;
    static std::map<std::string, CompileResult> compiledMap;
    static void parse(const std::string& json);
    static std::string readAllText(std::istream* inputStream);
    static void loadAndCompile(const std::string& path);
    static void init();
    static void run(const std::string& name);
    static void destroy();
    ~XVM();
};


#endif //仙垣_XVM_H
