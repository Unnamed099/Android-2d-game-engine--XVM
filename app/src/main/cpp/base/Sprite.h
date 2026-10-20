#ifndef SPRITE_H
#define SPRITE_H

#include "Emitter.h"
#include <string>
#include <cmath>
class Bitmap;
class Matrix;
class Paint;
class Canvas;

/**
 * 游戏基础的精灵类
 * C++版本
 */
class Sprite : public Emitter {
public:
    Bitmap* img;      // 精灵图片
    int width;        // 精灵宽度
    int height;       // 精灵高度
    int x;            // x坐标
    int y;            // y坐标

protected:
    bool visible = true;                       // 是否可见
    bool _isActive = true;                      // 是否可碰撞
    bool _isFollowMap = true;                   // 是否跟随地图
    float rotation = (float)(45 * M_PI / 180); // 旋转角度（弧度）

    // 用于旋转绘制的矩阵
    Matrix* rotationMatrix;
    Paint* paint;

public:
    // ★ 虚析构（有虚函数必须加）
    virtual ~Sprite();

    /**
     * 默认构造函数
     */
    Sprite();

    /**
     * 构造函数
     */
    Sprite(Bitmap* img, int width, int height);

    /**
     * 构造函数
     */
    Sprite(Bitmap* img, int width, int height, int x, int y);

    /**
     * 构造函数
     */
    Sprite(const std::string& path, int width, int height, int x, int y);

    // ============ 虚函数（子类可覆盖） ============

    /**
     * 将精灵图绘制在canvas上，支持旋转
     */
    virtual void render(Canvas* ctx);

    /**
     * 使用指定的Paint绘制精灵
     */
    virtual void render(Canvas* ctx, Paint* paint);

    /**
     * 设置旋转角度的方法（接收角度值）
     */
    virtual void setRotation(float angle);

    /**
     * 设置旋转角度的方法（接收弧度值）
     */
    virtual void setRotationRadians(float radians);

    /**
     * 获取当前旋转角度的方法（返回弧度值）
     */
    virtual float getRotation();

    /**
     * 获取当前旋转角度的方法（返回角度值）
     */
    virtual float getRotationDegrees();

    /**
     * 获取精灵图片
     */
    virtual Bitmap* getImage();

    /**
     * 设置精灵图片
     */
    virtual void setImage(Bitmap* img);

    /**
     * 获取宽度
     */
    virtual int getWidth();

    /**
     * 设置宽度
     */
    void setWidth(int width);

    /**
     * 获取高度
     */
    virtual int getHeight();

    /**
     * 设置高度
     */
    void setHeight(int height);

    /**
     * 获取X坐标
     */
    virtual int getX();

    /**
     * 设置X坐标
     */
    virtual void setX(int x);

    /**
     * 获取Y坐标
     */
    virtual int getY();

    /**
     * 设置Y坐标
     */
    virtual void setY(int y);

    /**
     * 是否可见
     */
    virtual bool isVisible();

    /**
     * 设置可见性
     */
    virtual void setVisible(bool visible);

    /**
     * 是否可碰撞
     */
    virtual bool isActive();

    /**
     * 设置是否可碰撞
     */
    virtual void setActive(bool active);

    /**
     * 是否跟随地图
     */
    virtual bool isFollowMap();

    /**
     * 设置是否跟随地图
     */
    virtual void setFollowMap(bool followMap);

    /**
     * 清理资源
     */
    virtual void destroy();

    /**
     * 重置精灵状态，便于对象池重用
     */
    virtual void reset();

    // ============ 非虚函数 ============

    /**
     * 简单的碰撞检测定义：
     * 另一个精灵的中心点处于本精灵所在的矩形内即可
     */
    bool isCollideWith(Sprite* sp);
};

#endif // SPRITE_H