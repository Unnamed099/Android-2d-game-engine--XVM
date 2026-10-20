#ifndef CPP_JNI_MATRIX_H_
#define CPP_JNI_MATRIX_H_

#include "JNIBridge.h"
#include "JNIBridgeIds.h"

class Matrix {
private:
    explicit Matrix(int v) : value(v) {}
public:
    int value = 0;

    static Matrix* create(int v) { return new Matrix(v); }
    static Matrix* create() { return new Matrix(JNIBridgeIds::alloc("Matrix")); }

    ~Matrix() = default;

    // ---- set* ----
    void reset()                                 { JNIBridge::Matrix_reset(value); }
    bool isIdentity()                            { return JNIBridge::Matrix_isIdentity(value); }

    void setTranslate(float dx, float dy)        { JNIBridge::Matrix_setTranslate(value, dx, dy); }
    void setScale(float sx, float sy)            { JNIBridge::Matrix_setScale(value, sx, sy); }
    void setScale(float sx, float sy, float px, float py) { JNIBridge::Matrix_setScale(value, sx, sy, px, py); }
    void setRotate(float d)                      { JNIBridge::Matrix_setRotate(value, d); }
    void setRotate(float d, float px, float py)  { JNIBridge::Matrix_setRotate(value, d, px, py); }
    void setSinCos(float sinv, float cosv)       { JNIBridge::Matrix_setSinCos(value, sinv, cosv); }
    void setSkew(float kx, float ky)             { JNIBridge::Matrix_setSkew(value, kx, ky); }
    void setSkew(float kx, float ky, float px, float py) { JNIBridge::Matrix_setSkew(value, kx, ky, px, py); }

    // ---- post* ----
    void postTranslate(float dx, float dy)       { JNIBridge::Matrix_postTranslate(value, dx, dy); }
    void postScale(float sx, float sy)           { JNIBridge::Matrix_postScale(value, sx, sy); }
    void postScale(float sx, float sy, float px, float py) { JNIBridge::Matrix_postScale(value, sx, sy, px, py); }
    void postRotate(float d)                     { JNIBridge::Matrix_postRotate(value, d); }
    void postRotate(float d, float px, float py) { JNIBridge::Matrix_postRotate(value, d, px, py); }
    void postSkew(float kx, float ky)            { JNIBridge::Matrix_postSkew(value, kx, ky); }
    void postSkew(float kx, float ky, float px, float py) { JNIBridge::Matrix_postSkew(value, kx, ky, px, py); }
    void postConcat(Matrix* other)               { JNIBridge::Matrix_postConcat(value, other->value); }

    // ---- pre* ----
    void preTranslate(float dx, float dy)        { JNIBridge::Matrix_preTranslate(value, dx, dy); }
    void preScale(float sx, float sy)            { JNIBridge::Matrix_preScale(value, sx, sy); }
    void preScale(float sx, float sy, float px, float py) { JNIBridge::Matrix_preScale(value, sx, sy, px, py); }
    void preRotate(float d)                      { JNIBridge::Matrix_preRotate(value, d); }
    void preSkew(float kx, float ky)             { JNIBridge::Matrix_preSkew(value, kx, ky); }
    void preConcat(Matrix* other)                { JNIBridge::Matrix_preConcat(value, other->value); }

    // ---- 查询 / 计算 ----
    void mapPoints(float* pts, int count)        { JNIBridge::Matrix_mapPoints(value, pts, count); }
    void getValues(float* values)                { JNIBridge::Matrix_getValues(value, values); }   // 9 个 float
    bool invert()                                { return JNIBridge::Matrix_invert(value); }
    bool rectStaysRect()                         { return JNIBridge::Matrix_rectStaysRect(value); }
};

#endif