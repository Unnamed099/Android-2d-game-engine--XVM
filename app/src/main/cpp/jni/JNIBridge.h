#pragma once
#include <jni.h>
#include <string>
#include <vector>
#include <cstdint>
#include <android/asset_manager.h>
#include <functional>

class Bitmap;

class JNIBridge {
public:
    static JavaVM* vm_;
    static jobject assetManager_;

    // ═══════════════════════════════════════════════════════════
    //  生命周期
    // ═══════════════════════════════════════════════════════════
    static void   init(JavaVM* vm);
    static JNIEnv* getEnv();
    static jclass getJNIDataClass(JNIEnv* env);

    // GL 初始化（GLSurfaceView 的 onSurfaceCreated / onSurfaceChanged 调）
    static bool glInit(AAssetManager* am);
    static void glResize(int w, int h);
    static void glBeginFrame();
    static void glEndFrame();
    static void glShutdown();

    // ═══════════════════════════════════════════════════════════
    //  Canvas
    // ═══════════════════════════════════════════════════════════
    static void Canvas_drawBitmap(int r1, int r2, float left, float top, int r3);
    static void Canvas_drawText(int r1, const std::string& text, float x, float y, int r3);
    static void Canvas_drawRect(int r1, float left, float top, float right, float bottom, int r3);
    static void Canvas_drawCircle(int r1, float cx, float cy, float radius, int r3);
    static void Canvas_drawBitmapWithMatrix(int r1, int r2, int rMatrix, int r3);
    static void Canvas_drawBitmapRects(int r1, int r2,
                                       int sl, int st, int sr, int sb,
                                       int dl, int dt, int dr, int db, int r3);
    static void Canvas_save(int r);
    static void Canvas_restore(int r);
    static int  Canvas_createFromBitmap(int bitmapId);
    static void Canvas_drawColor(int r1, int color);
    static void Canvas_drawPath(int r1, int rPath, int r3);
    static void Canvas_drawLine(int r1, float startX, float startY,
                                float stopX, float stopY, int r3);
    static void Canvas_drawRoundRect(int r1,
                                     float left, float top, float right, float bottom,
                                     float rx, float ry, int r3);
    // ★ 新增：Canvas 析构
    static void Canvas_destroy(int r);
    // ★ 新增：裁剪（可选，给 Canvas_save/restore 用）
    static void Canvas_clipRect(int r, float l, float t, float rr, float b);

    // ═══════════════════════════════════════════════════════════
    //  Bitmap
    // ═══════════════════════════════════════════════════════════
    static bool Bitmap_isRecycled(int r);
    static void Bitmap_recycle(int r);
    static int  Bitmap_getWidth(int r);
    static int  Bitmap_getHeight(int r);
    static int  Bitmap_create(const std::string& path);
    static int  Bitmap_createBitmap(int width, int height, int config);
    static int  Bitmap_createScaledBitmap(int r, int dstWidth, int dstHeight, bool filter);
    static int  Bitmap_createSubBitmap(int sourceId, int x, int y, int width, int height);
    static int  Bitmap_getPixel(int r, int x, int y);
    static void Bitmap_setPixel(int r, int x, int y, int color);
    // ★ 新增：setPixel 后如果立刻要画，显式上传
    static void Bitmap_uploadDirty(int r);
    // ★ 新增：拿 GL 纹理 id（内部 FBO / 高级用途）
    static unsigned int Bitmap_getGlTexture(int r);

    // ═══════════════════════════════════════════════════════════
    //  Paint
    // ═══════════════════════════════════════════════════════════
    static void  Paint_setTextSize(int r, float size);
    static void  Paint_setColor(int r, int color);
    static void  Paint_setTextAlign(int r, int align);
    static void  Paint_setAntiAlias(int r, bool aa);
    static float Paint_measureText(int r, const std::string& text);
    static void  Paint_setStrokeWidth(int r, float width);
    static void  Paint_setStyle(int r, int style);
    static void  Paint_setStrokeCap(int r, int cap);
    static void  Paint_setFilterBitmap(int r, bool filter);
    static void  Paint_setDither(int r, bool dither);
    static void  Paint_setAlpha(int r, int alpha);
    static void  Paint_setLightingColorFilter(int r, int mul, int add);
    static void  Paint_setShader(int r, int shaderId);
    static void  Paint_setShadowLayer(int r, float radius, float dx, float dy, int color);
    static float Paint_ascent(int r);
    static float Paint_descent(int r);
    static float Paint_getFontSpacing(int r);
    static int   Paint_breakText(int r, const std::string& text,
                                 int start, int textLength,
                                 bool measureForwards, int maxWidth,
                                 float* outMeasuredWidth);
    static void  Paint_setFakeBoldText(int r, bool bold);

    // ═══════════════════════════════════════════════════════════
    //  Matrix
    // ═══════════════════════════════════════════════════════════
// ═══════════════════════════════════════════════════════════════
//  Matrix —— 与 android.graphics.Matrix 语义一致
// ═══════════════════════════════════════════════════════════════
    static void Matrix_reset(int r);
    static bool Matrix_isIdentity(int r);

// set*
    static void Matrix_setTranslate(int r, float dx, float dy);
    static void Matrix_setScale(int r, float sx, float sy);
    static void Matrix_setScale(int r, float sx, float sy, float px, float py);
    static void Matrix_setRotate(int r, float deg);
    static void Matrix_setRotate(int r, float deg, float px, float py);
    static void Matrix_setSinCos(int r, float sinv, float cosv);
    static void Matrix_setSkew(int r, float kx, float ky);
    static void Matrix_setSkew(int r, float kx, float ky, float px, float py);

// post*
    static void Matrix_postTranslate(int r, float dx, float dy);
    static void Matrix_postScale(int r, float sx, float sy);
    static void Matrix_postScale(int r, float sx, float sy, float px, float py);
    static void Matrix_postRotate(int r, float deg);
    static void Matrix_postRotate(int r, float deg, float px, float py);
    static void Matrix_postSkew(int r, float kx, float ky);
    static void Matrix_postSkew(int r, float kx, float ky, float px, float py);
    static void Matrix_postConcat(int r, int other);

// pre*
    static void Matrix_preTranslate(int r, float dx, float dy);
    static void Matrix_preScale(int r, float sx, float sy);
    static void Matrix_preScale(int r, float sx, float sy, float px, float py);
    static void Matrix_preRotate(int r, float deg);
    static void Matrix_preSkew(int r, float kx, float ky);
    static void Matrix_preConcat(int r, int other);

// 查询/计算
    static void Matrix_mapPoints(int r, float* pts, int count);
    static void Matrix_getValues(int r, float* values);   // 9 个 float
    static bool Matrix_invert(int r);
    static bool Matrix_rectStaysRect(int r);

    // ═══════════════════════════════════════════════════════════
    //  Path
    // ═══════════════════════════════════════════════════════════
    static void Path_moveTo(int r, float x, float y);
    static void Path_lineTo(int r, float x, float y);
    static void Path_close(int r);
    static void Path_reset(int r);
    static void Path_cubicTo(int r, float x1, float y1, float x2, float y2, float x3, float y3);
    // ★ 新增：Path 析构
    static void Path_destroy(int r);

    // ═══════════════════════════════════════════════════════════
    //  Screen
    // ═══════════════════════════════════════════════════════════
    static int Render_SCREEN_WIDTH();
    static int Render_SCREEN_HEIGHT();

    // ═══════════════════════════════════════════════════════════
    //  LinearGradient
    // ═══════════════════════════════════════════════════════════
    static int LinearGradient_create(float x0, float y0, float x1, float y1,
                                     int color0, int color1, int tileMode);

    // ═══════════════════════════════════════════════════════════
    //  Resource（走 AAssetManager，不再 JNI 反射）
    // ═══════════════════════════════════════════════════════════
    static std::vector<uint8_t> Resource_readBytes(const std::string& path);
    static std::string          Resource_readText(const std::string& path);
    static bool                 Resource_exists(const std::string& path);

    // ═══════════════════════════════════════════════════════════
    //  Controller（触摸事件，保留 JNI 入口）
    // ═══════════════════════════════════════════════════════════
    static void Controller_onTouchEvent(JNIEnv* env,
                                        int action, int actionIndex, int pointerCount,
                                        jintArray ids, jfloatArray xs, jfloatArray ys,
                                        int screenWidth);

    static void setAssetManager(AAssetManager* am);

    static void onGLContextLost();
    static void glRestore();

    // ★ 内容缓存（FBO 绘制型 Bitmap 专用）
    static Bitmap* getCachedBitmap(int key, std::function<Bitmap*()> makeFn);
    static void invalidateCachePrefix(int prefix);
    static void clearAllCachedBitmaps();

};