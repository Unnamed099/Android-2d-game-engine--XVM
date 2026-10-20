#ifndef CPP_JNI_PAINT_H_
#define CPP_JNI_PAINT_H_

#include <string>
#include "JNIBridge.h"
#include "JNIBridgeIds.h"

class LinearGradient {
private:
    explicit LinearGradient(int v) : value(v) {}
public:
    int value = 0;

    static LinearGradient* create(int v) { return new LinearGradient(v); }
    static LinearGradient* create() { return new LinearGradient(JNIBridgeIds::alloc("LinearGradient")); }

    ~LinearGradient() = default;   // 无 GL 资源

    static LinearGradient* create(float x0, float y0, float x1, float y1,
                                  int color0, int color1, int tileMode) {
        int id = JNIBridge::LinearGradient_create(x0, y0, x1, y1,
                                                  color0, color1, tileMode);
        if (id < 0) return nullptr;
        return new LinearGradient(id);
    }
};

class Paint {
private:
    explicit Paint(int v) : value(v) {}
public:
    int value = 0;

    static Paint* create(int v) { return new Paint(v); }
    static Paint* create() { return new Paint(JNIBridgeIds::alloc("Paint")); }

    ~Paint() = default;   // 无 GL 资源

    enum Style { FILL = 0, STROKE = 1, FILL_AND_STROKE = 2 };
    enum Align { LEFT = 0, CENTER = 1, RIGHT = 2 };
    enum Cap   { BUTT = 0, ROUND = 1, SQUARE = 2 };

    void setTextSize(float size)  { JNIBridge::Paint_setTextSize(value, size); }
    void setColor(int color)      { JNIBridge::Paint_setColor(value, color); }
    void setTextAlign(int align)  { JNIBridge::Paint_setTextAlign(value, align); }
    void setAntiAlias(bool aa)    { JNIBridge::Paint_setAntiAlias(value, aa); }
    void setStrokeWidth(float w)  { JNIBridge::Paint_setStrokeWidth(value, w); }
    void setStyle(int style)      { JNIBridge::Paint_setStyle(value, style); }
    void setStrokeCap(int cap)    { JNIBridge::Paint_setStrokeCap(value, cap); }
    void setFilterBitmap(bool f)  { JNIBridge::Paint_setFilterBitmap(value, f); }
    void setDither(bool d)        { JNIBridge::Paint_setDither(value, d); }
    void setAlpha(int alpha)      { JNIBridge::Paint_setAlpha(value, alpha); }
    void setLightingColorFilter(int mul, int add) { JNIBridge::Paint_setLightingColorFilter(value, mul, add); }
    void setShader(int shaderId)  { JNIBridge::Paint_setShader(value, shaderId); }
    void setShadowLayer(float radius, float dx, float dy, int color) {
        JNIBridge::Paint_setShadowLayer(value, radius, dx, dy, color);
    }
    void setFakeBoldText(bool bold) { JNIBridge::Paint_setFakeBoldText(value, bold); }

    float measureText(const std::string& text) { return JNIBridge::Paint_measureText(value, text); }
    float ascent()           { return JNIBridge::Paint_ascent(value); }
    float descent()          { return JNIBridge::Paint_descent(value); }
    float getFontSpacing()   { return JNIBridge::Paint_getFontSpacing(value); }

    int breakText(const std::string& text, int start, int textLength,
                  bool measureForwards, int maxWidth, float* outMeasuredWidth) {
        return JNIBridge::Paint_breakText(value, text, start, textLength,
                                          measureForwards, maxWidth, outMeasuredWidth);
    }

    void setShader(LinearGradient* shader) {
        JNIBridge::Paint_setShader(value, shader ? shader->value : -1);
    }
};

#endif