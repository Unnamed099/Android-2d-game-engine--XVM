#ifndef CPP_BASE_ANIMATION_H
#define CPP_BASE_ANIMATION_H

#include "Sprite.h"
#include <string>
#include "AnimationResource.h"
#include "ComponentAnimation.h"

class Bitmap;
class Canvas;
class Matrix;

class Animation : public Sprite {
public:
    int id = 0;
    int index = -1;
    std::string animation_name;
    std::string skeleton_name;
    int h = 0;
    AnimationResource::A* a = nullptr;
    ComponentAnimation::MainComponent* mainComponent = nullptr;
    bool loop = true;

    Animation();
    void init(Bitmap* imgSrc, int x, int y, int width, int height);
    Animation(Bitmap* imgSrc, int width, int height);
    Animation(Bitmap* img, int width, int height, int x, int y);
    Animation(int width, int height);

    void render(Canvas* ctx) override;
    void setRotation(float angle) override;
    float getRotation() override;
    void setRotationRadians(float radians) override;
    float getRotationDegrees() override;
    Bitmap* getImage() override;
    void setImage(Bitmap* img) override;
    int getWidth() override;
    void destroy() override;

    // ============ Animation 特有 ============
    void setHorizontalMirror(bool b);
    void setVerticalMirror(bool b);

private:
    float rotation = 0.0f;
    bool horizontal_mirror = false;
    bool vertical_mirror = false;
    Matrix* rotationMatrix = nullptr;
};

#endif