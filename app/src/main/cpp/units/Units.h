#ifndef CPP_UNITS_UNITS_H
#define CPP_UNITS_UNITS_H

#include <string>
#include <memory>

#include "base/Animation.h"

class WhileFrameTask;
class Move;
class UnitData;

class Units : public Animation {
public:
    bool visible = false;
    std::string event;
    Move* move = nullptr;

    // ★ 改成 unique_ptr
    std::unique_ptr<UnitData> data;

    Units();
    ~Units();

    // 禁用拷贝
    Units(const Units&) = delete;
    Units& operator=(const Units&) = delete;

    // 允许移动（可选）
    Units(Units&& other) noexcept;
    Units& operator=(Units&& other) noexcept;

    // ★ init 参数改成 unique_ptr
    void init(int x, int y, bool isActive, bool visible, std::unique_ptr<UnitData> data);
    void init(int x, int y, int w, int h, bool isActive, bool visible, std::unique_ptr<UnitData> data);

    void update();
    void remove();
    void destroy();

private:
    WhileFrameTask* loadTask = nullptr;

    void reset();
};

#endif // CPP_UNITS_UNITS_H