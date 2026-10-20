#ifndef SKELETON_LOADER_H
#define SKELETON_LOADER_H

#include <string>
#include "ComponentAnimation.h"

class SkeletonLoader {
public:
    static ComponentAnimation::MainComponent* loadFromJSONPath(const std::string& path);
};

#endif