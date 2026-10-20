#ifndef CPP_BASE_ANIMATIONRESOURCE_H
#define CPP_BASE_ANIMATIONRESOURCE_H

#include <string>
#include <vector>
#include <map>
#include "skeleton_animation/ComponentAnimation.h"
#include "Bitmap.h"
#include "DynamicShadow.h"


class AnimationResource {
public:
    class A {
    public:
        std::vector<Bitmap*> l;
        std::string name;
        int single_frame_tick;
        DynamicShadow* dynamicShadow;

        A();
        ~A();
    };

    static std::map<std::string, A*> res;
    static const std::string INDEX_PATH;

    static void init();
    static void parse(const std::string& json);
    static void singleParse(const std::string& path);
    static std::string readAllText(std::istream* inputStream);
    static ComponentAnimation::MainComponent* load_skeletion_animation(const std::string& json_path);
    static void destroy();
};

#endif