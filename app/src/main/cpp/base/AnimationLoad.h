#ifndef CPP_BASE_ANIMATIONLOAD_H
#define CPP_BASE_ANIMATIONLOAD_H

#include <istream>
#include <map>
#include <string>

#include "Animation.h"

class AnimationLoad {
public:
    static std::map<std::string, Animation*> res;
    static const std::string INDEX_PATH;

    static void init();
    static void parse(const std::string& json);
    static void singleParse(const std::string& path);
    static std::string readAllText(std::istream* inputStream);
    static void* load_skeletion_animation(const std::string& json_path);
};

#endif