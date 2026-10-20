#ifndef CPP_JNI_PATH_H_
#define CPP_JNI_PATH_H_

#include "JNIBridge.h"
#include "JNIBridgeIds.h"

class Path {
private:
    explicit Path(int v) : value(v) {}
public:
    int value = 0;

    static Path* create(int v) { return new Path(v); }
    static Path* create() { return new Path(JNIBridgeIds::alloc("Path")); }

    ~Path() { JNIBridge::Path_destroy(value); }

    void moveTo(float x, float y) { JNIBridge::Path_moveTo(value, x, y); }
    void lineTo(float x, float y) { JNIBridge::Path_lineTo(value, x, y); }
    void cubicTo(float x1, float y1, float x2, float y2, float x3, float y3) {
        JNIBridge::Path_cubicTo(value, x1, y1, x2, y2, x3, y3);
    }
    void close() { JNIBridge::Path_close(value); }
    void reset() { JNIBridge::Path_reset(value); }
};

#endif