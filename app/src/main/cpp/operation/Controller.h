#ifndef CPP_CONTROLLER_H
#define CPP_CONTROLLER_H

#include <vector>
#include <string>
#include "jni/Paint.h"
#include "jni/Canvas.h"

class ChunkManager;
class Units;

class Controller {
public:
    // ===== 静态状态（跟原来一样）=====
    static float select_x;
    static float select_y;
    static std::vector<Units*> select_item;

    static int pressX;
    static int pressY;

    static float joystickDirX;
    static float joystickDirY;
    static float attackDirX;
    static float attackDirY;

    static int joystickCenterX;
    static int joystickCenterY;
    static const int JOYSTICK_OUTER_RADIUS = 120;
    static const int JOYSTICK_INNER_RADIUS = 50;
    static float joystickInnerOffsetX;
    static float joystickInnerOffsetY;
    static bool joystickActive;
    static int joystickPointerId;

    static int attackCenterX;
    static int attackCenterY;
    static const int ATTACK_OUTER_RADIUS = 100;
    static const int ATTACK_INNER_RADIUS = 45;
    static float attackInnerOffsetX;
    static float attackInnerOffsetY;
    static bool attackActive;
    static int attackPointerId;

    static Paint* selectPaint;
    static Paint* joystickOuterGlowPaint;
    static Paint* joystickOuterBorderPaint;
    static Paint* joystickInnerGradientPaint;
    static Paint* joystickInnerHighlightPaint;
    static Paint* joystickDirectionArrowPaint;
    static Paint* joystickDirectionDotPaint;

    static Paint* attackOuterGlowPaint;
    static Paint* attackOuterBorderPaint;
    static Paint* attackInnerGradientPaint;
    static Paint* attackInnerHighlightPaint;
    static Paint* attackDirectionArrowPaint;
    static Paint* attackDirectionDotPaint;

    // ★ 去掉 View*，只保留 Canvas 和 Main
    static Canvas* ctx;

    // ===== 初始化 / 销毁 =====
    static void initTouchControl(Canvas* canvas);
    static void initPaints();
    static void destroy();

    // ===== ★ 新增：给 JNI 层调用的触摸接口 =====
    // screenWidth / screenHeight 由 Java 层传入（因为不再依赖 View）
    static void onTouchDown(int pointerId, float x, float y, int screenWidth);
    static void onTouchMove(int pointerId, float x, float y);
    static void onTouchUp(int pointerId);
    static void onTouchCancel();

    // ===== 摇杆逻辑 =====
    static void updateJoystick(float x, float y);
    static void updateAttackJoystick(float x, float y);
    static void resetJoystick();
    static void resetAttack();

    static float getJoystickDirectionX();
    static float getJoystickDirectionY();
    static bool isJoystickActive();
    static float getAttackDirectionX();
    static float getAttackDirectionY();
    static bool isAttackActive();

    static void updateJoystickMove(ChunkManager* chunkManager);

    // ===== 渲染 =====
    static void render(Canvas* canvas);
    static void renderJoystick(Canvas* canvas);
    static void renderAttackJoystick(Canvas* canvas);
    static void drawDirectionIndicator(Canvas* canvas, float cx, float cy,
                                       float dirX, float dirY, bool isAttack);

private:
    // ★ 去掉 setupTouchListeners(View*)
    // 换成从 JNI 调用的统一入口
    static bool handleTouch(int action, int pointerId, float x, float y, int screenWidth);
};

#endif