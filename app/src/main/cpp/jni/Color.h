#ifndef CPP_JNI_COLOR_H_
#define CPP_JNI_COLOR_H_

/**
 * 颜色常量类
 * 对应 android.graphics.Color
 * 不需要 JNI，纯 C++ 常量
 */
class Color {
public:
    // ============ 常用颜色常量 ============
    static const int BLACK       = 0xFF000000;
    static const int DKGRAY      = 0xFF444444;
    static const int GRAY        = 0xFF888888;
    static const int LTGRAY      = 0xFFCCCCCC;
    static const int WHITE       = 0xFFFFFFFF;
    static const int RED         = 0xFFFF0000;
    static const int GREEN       = 0xFF00FF00;
    static const int BLUE        = 0xFF0000FF;
    static const int YELLOW      = 0xFFFFFF00;
    static const int CYAN        = 0xFF00FFFF;
    static const int MAGENTA     = 0xFFFF00FF;
    static const int TRANSPARENT = 0x00000000;

    // ============ 构造 RGB 颜色 ============
    static int rgb(int red, int green, int blue) {
        return 0xFF000000 | ((red & 0xFF) << 16) | ((green & 0xFF) << 8) | (blue & 0xFF);
    }

    // ============ 构造 ARGB 颜色 ============
    static int argb(int alpha, int red, int green, int blue) {
        return ((alpha & 0xFF) << 24) | ((red & 0xFF) << 16) | ((green & 0xFF) << 8) | (blue & 0xFF);
    }

    // ============ 提取分量 ============
    static int alpha(int color) {
        return (color >> 24) & 0xFF;
    }

    static int red(int color) {
        return (color >> 16) & 0xFF;
    }

    static int green(int color) {
        return (color >> 8) & 0xFF;
    }

    static int blue(int color) {
        return color & 0xFF;
    }

private:
    Color() {}   // 禁止实例化
};

#endif