#ifndef CPP_JNI_CANVAS_H_
#define CPP_JNI_CANVAS_H_

#include <string>
#include "Bitmap.h"
#include "Paint.h"
#include "Matrix.h"
#include "Path.h"
#include "RectF.h"
#include "JNIBridge.h"
#include "JNIBridgeIds.h"

class Canvas {
private:
    explicit Canvas(int v) : value(v) {}
public:
    int value = 0;

    static Canvas* create(int v) { return new Canvas(v); }
    static Canvas* create() { return new Canvas(JNIBridgeIds::alloc("Canvas")); }

    ~Canvas() {
        JNIBridge::Canvas_destroy(value);
    }

    static Canvas* createFromBitmap(Bitmap* bitmap) {
        if (!bitmap) return nullptr;
        int id = JNIBridge::Canvas_createFromBitmap(bitmap->value);
        if (id < 0) return nullptr;
        return new Canvas(id);
    }

    void drawBitmap(Bitmap* bitmap, float left, float top, Paint* paint) {
        JNIBridge::Canvas_drawBitmap(value, bitmap->value, left, top, paint->value);
    }

    void drawText(const std::string& text, float x, float y, Paint* paint) {
        JNIBridge::Canvas_drawText(value, text, x, y, paint->value);
    }

    void drawRect(RectF& rect, Paint& paint) {
        JNIBridge::Canvas_drawRect(value, rect.left, rect.top, rect.right, rect.bottom, paint.value);
    }

    void drawBitmap(Bitmap* bitmap, Matrix* matrix, Paint* paint) {
        if (!bitmap || !matrix || !paint) return;
        JNIBridge::Canvas_drawBitmapWithMatrix(value, bitmap->value, matrix->value, paint->value);
    }

    void save()    { JNIBridge::Canvas_save(value); }
    void restore() { JNIBridge::Canvas_restore(value); }

    void drawCircle(float cx, float cy, float radius, Paint* paint) {
        if (!paint) return;
        JNIBridge::Canvas_drawCircle(value, cx, cy, radius, paint->value);
    }

    void drawPath(Path* path, Paint* paint) {
        if (!path || !paint) return;
        JNIBridge::Canvas_drawPath(value, path->value, paint->value);
    }

    void drawRect(float left, float top, float right, float bottom, Paint* paint) {
        if (!paint) return;
        JNIBridge::Canvas_drawRect(value, left, top, right, bottom, paint->value);
    }

    void drawColor(int color) { JNIBridge::Canvas_drawColor(value, color); }

    void drawBitmap(Bitmap* bitmap, RectF* srcRect, RectF* dstRect, Paint* paint) {
        if (!bitmap || !srcRect || !dstRect) return;
        JNIBridge::Canvas_drawBitmapRects(
                value, bitmap->value,
                srcRect->left, srcRect->top, srcRect->right, srcRect->bottom,
                dstRect->left, dstRect->top, dstRect->right, dstRect->bottom,
                paint ? paint->value : -1);
    }

    void drawLine(float sx, float sy, float ex, float ey, Paint* paint) {
        if (!paint) return;
        JNIBridge::Canvas_drawLine(value, sx, sy, ex, ey, paint->value);
    }

    void drawRoundRect(float left, float top, float right, float bottom,
                       float rx, float ry, Paint* paint) {
        if (!paint) return;
        JNIBridge::Canvas_drawRoundRect(value, left, top, right, bottom, rx, ry, paint->value);
    }

    void drawRoundRect(RectF& rect, float rx, float ry, Paint& paint) {
        JNIBridge::Canvas_drawRoundRect(value, rect.left, rect.top, rect.right, rect.bottom,
                                        rx, ry, paint.value);
    }

    void drawBitmap(Bitmap* bitmap, RectF* dstRect, Paint* paint) {
        if (!bitmap || !dstRect) return;
        JNIBridge::Canvas_drawBitmapRects(
                value, bitmap->value,
                0, 0, bitmap->getWidth(), bitmap->getHeight(),
                (int)dstRect->left, (int)dstRect->top,
                (int)dstRect->right, (int)dstRect->bottom,
                paint ? paint->value : -1);
    }
};

#endif