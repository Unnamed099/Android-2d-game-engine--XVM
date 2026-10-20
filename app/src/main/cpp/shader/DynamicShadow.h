#ifndef CPP_SHADER_DYNAMICSHADOW_H
#define CPP_SHADER_DYNAMICSHADOW_H

#include "jni/Matrix.h"
#include "jni/Paint.h"
#include "jni/Canvas.h"

class DynamicShadow {
public:
    DynamicShadow();
    ~DynamicShadow();
    void draw(Bitmap* bmp, Canvas* ctx, float x, float y, float w, float h,
              float offsetScaH, bool horizontal_mirror, bool vertical_mirror);
    void destroy();

private:
    Matrix* shadowMatrix = Matrix::create();
    float scaleY = 0.7f;
    float skewX = 0.6f;
    Paint* shadowPaint = Paint::create();
};

#endif