#ifndef CPP_BASE_CACHE_H
#define CPP_BASE_CACHE_H

#include <list>
#include "jni/Bitmap.h"

class Cache {
public:
    class B {
    public:
        Bitmap* orginBmp;
        Bitmap* bmp;
        int special;
        B(Bitmap* orginBmp, int special, Bitmap* bmp);
    };

    Bitmap* getBitmapCache(Bitmap* orginBmp, int special);
    void putBitmapCache(Bitmap* orginBmp, int special, Bitmap* bmp);
    void clear();

private:
    int MAXNUM = 50;
    std::list<B> queue_bmp;
};

#endif