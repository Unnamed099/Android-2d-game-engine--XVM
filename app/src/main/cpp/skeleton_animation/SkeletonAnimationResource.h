#ifndef SKELETONANIMATIONRESOURCE_H
#define SKELETONANIMATIONRESOURCE_H

#include <string>
#include <unordered_map>
#include <istream>

#include "ComponentAnimation.h"

class SkeletonLoader;

class SkeletonAnimationResource {
public:
    struct SkeletonData {
        ComponentAnimation::MainComponent* mainComponent = nullptr;
        std::string name;

        SkeletonData() {}
    };

    static const std::string INDEX_PATH;

    static void init();
    static void singleParse(const std::string& path);
    static ComponentAnimation::MainComponent* getSkeleton(const std::string& name);
    static ComponentAnimation::MainComponent* getSkeletonRaw(const std::string& name);
    static bool hasSkeleton(const std::string& name);
    static void clearCache();
    static void destroy();

private:
    static std::unordered_map<std::string, SkeletonData>* skeletonCache;

    static void parseIndex(const std::string& json);
    static std::string readAllText(std::istream* inputStream);
    static void ensureCache();
};

#endif