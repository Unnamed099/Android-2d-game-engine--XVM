#ifndef CPP_SHADER_FIGURE_H
#define CPP_SHADER_FIGURE_H

#include <cstdint>
#include <string>
#include <vector>

#include "jni/Canvas.h"
#include "jni/Paint.h"
class SimpleJSON;
class Figure {
private:
    int64_t color;
    int x;
    int y;
    int r;
    int size;
    std::string type;
    int point_num;

public:
    Figure();
    ~Figure() = default;

    void setC(const SimpleJSON* json, const std::string& type);
    void render(Canvas* ctx);
    Path* createSmoothIrregularShape(float centerX, float centerY,float baseRadius, int numPoints);
};

#endif // CPP_SHADER_FIGURE_H