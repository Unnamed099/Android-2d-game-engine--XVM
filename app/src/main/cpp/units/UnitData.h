#ifndef UNITDATA_H
#define UNITDATA_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cstdint>

class Units;
class UnitDataCommand;

class UnitData {
public:
    UnitData();
    ~UnitData();

    // 禁止拷贝，允许移动
    UnitData(const UnitData&) = delete;
    UnitData& operator=(const UnitData&) = delete;
    UnitData(UnitData&& other) noexcept;
    UnitData& operator=(UnitData&& other) noexcept;

    void init(Units* units);
    void update();
    void destroy();

public:
    std::string name;
    float hp = 0.f;
    float maxHp = 0.f;
    float speed = 0.f;
    float attackR = 0.f;
    int   radius = 0;
    float angle = 0.f;
    float attack_angle = 0.f;
    int   w = 0;
    int   h = 0;
    bool  isBuilding = false;
    bool  load_chunk = false;
    int   id = 0;
    int   team = 0;
    int   hitBoxLeftTopX = 0;
    int   hitBoxLeftTopY = 0;
    int   hitBoxRightBootomX = 0;
    int   hitBoxRightBootomY = 0;
    std::string animation_name;
    std::string skeleton_name;
    std::vector<int> walkAbleTileId;
    std::vector<std::string> events;
    std::vector<std::string> add_events;
    std::map<std::string, class CommandNode*> data_array;
    std::map<std::string, class CommandNode*> final_data;

    Units* units = nullptr;
    std::string vm_code;
};

#endif // UNITDATA_H