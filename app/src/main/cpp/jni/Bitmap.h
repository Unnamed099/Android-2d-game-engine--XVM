#ifndef CPP_JNI_BITMAP_H_
#define CPP_JNI_BITMAP_H_

#include <string>
#include "JNIBridge.h"
#include "JNIBridgeIds.h"

class Bitmap {
private:
    explicit Bitmap(int v) : value(v) {}
public:
    int value = 0;

    static Bitmap* create(int v) { return new Bitmap(v); }
    static Bitmap* create() { return new Bitmap(JNIBridgeIds::alloc("Bitmap")); }

    ~Bitmap() {
        JNIBridge::Bitmap_recycle(value);
    }

    enum Config {
        ARGB_8888 = 0,
        RGB_565   = 1,
        ALPHA_8   = 2
    };

    bool isRecycled() { return JNIBridge::Bitmap_isRecycled(value); }
    void recycle()    { JNIBridge::Bitmap_recycle(value); }
    bool isValid()    { return value >= 0 && !isRecycled(); }

    int getWidth()  { return JNIBridge::Bitmap_getWidth(value); }
    int getHeight() { return JNIBridge::Bitmap_getHeight(value); }

    static Bitmap* createFromPath(const std::string& path) {
        int id = JNIBridge::Bitmap_create(path);
        if (id < 0) return nullptr;
        return new Bitmap(id);
    }

    static Bitmap* createBitmap(int width, int height, int config) {
        int id = JNIBridge::Bitmap_createBitmap(width, height, config);
        if (id < 0) return nullptr;
        return new Bitmap(id);
    }

    int  getPixel(int x, int y)            { return JNIBridge::Bitmap_getPixel(value, x, y); }
    void setPixel(int x, int y, int color) { JNIBridge::Bitmap_setPixel(value, x, y, color); }

    static Bitmap* createScaledBitmap(Bitmap* src, int dw, int dh, bool filter) {
        if (!src) return nullptr;
        int id = JNIBridge::Bitmap_createScaledBitmap(src->value, dw, dh, filter);
        if (id < 0) return nullptr;
        return new Bitmap(id);
    }

    static Bitmap* createBitmap(Bitmap* source, int x, int y, int w, int h) {
        if (!source) return nullptr;
        int id = JNIBridge::Bitmap_createSubBitmap(source->value, x, y, w, h);
        if (id < 0) return nullptr;
        return new Bitmap(id);
    }
};

#endif