#ifndef LOG_CAT_H
#define LOG_CAT_H

#include <string>
#include <vector>
#include "jni/Canvas.h"
#include "jni/Paint.h"

class LogCat {
public:
    static bool log;
    static std::string m;
    static void addMessage(const std::string* message);
    static void setMessage(const std::string* message);
    static void setMessage(const std::string message);
    static void render(Canvas* ctx);
    static void addMessage(const std::string message);
private:
    static long long screen_dwell_time;
    static long long time;
    static bool screen_dwell;

    static int textSizePx;
    static int lineHeightPx;

    static const int MARGIN_LEFT = 100;
    static const int MARGIN_RIGHT = 100;
    static const int MARGIN_TOP = 50;
    static const int MARGIN_BOTTOM = 20;

    static std::vector<std::string> breakText(const std::string* text, Paint* paint, float maxWidth);
};

#endif