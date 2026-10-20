#ifndef CPP_DATABUS_H
#define CPP_DATABUS_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "units/UnitData.h"
#include "Pool.h"

class Units;
class Bitmap;
class AStar;
class Animation;
class CommandNode;
class UnitDataCommand;

class DataBus {
public:
    // ─────────── 常量 ───────────
    static const int CHUNK_SIZE = 12;
    static const int GRID_COLS = 3;
    static const int GRID_ROWS = 3;

    std::map<std::string, double> Global_data;

    // ─────────── 静态成员 ───────────
    static std::vector<Units*> units;
    static int frame;
    static float offsetX;
    static float offsetY;
    static float startX;
    static float startY;
    static std::vector<std::vector<int>> plot_index;
    static int CELL_WIDTH;
    static int CELL_HEIGHT;
    static AStar* find_way;
    static std::map<std::string, std::string> singleUnitsData;
    static int originChunkX;
    static int originChunkY;
    static std::map<int, Bitmap*> tileBitmaps;
    static std::map<std::string, CommandNode*> final_data;
    static std::map<std::string, std::map<std::string, CommandNode*>> map_data;
    static std::map<std::string, std::vector<CommandNode*>> list_data;
    static Pool pool;
    static std::vector<Animation*> animations;
    static Units* load_units;
    static int UUID;
    static std::vector<std::string> log;
    static int time;

    // ─────────── 构造 / 析构 ───────────
    DataBus();
    ~DataBus();

    // ─────────── 单位管理 ───────────
    static void addUnit(Units* unit);
    static void removeUnit(Units* unit);
    static void removeUnitNow(Units* unit);
    static std::vector<Units*>& getUnits();

    // ─────────── 查找 ───────────
    static Units* findUnitAt(float x, float y);
    static Units* findUnitAtWithSize(float x, float y, float w, float h, float rm);
    static Units* findUnitsXY(float x, float y, float r);
    static Units* findUnitsXY2(float x, float y, float w, float h, float rm);
    static void findUnitByRect(std::vector<int>& v,int x, int y, int d);
    static void findUnitByCirCle(std::vector<int>& v, int x, int y, int r);
    static void findUnitBySector(std::vector<int>& v, int x, int y, int r, int angle, int offsetAngle);

    // ─────────── 工具 ───────────
    static bool isIntersect(int X1, int X2, int Y1, int Y2,
                            int sizeX1, int sizeX2, int sizeY1, int sizeY2);
    static bool testCollision(int cx, int cy, int r, int ul_x, int ut_y, int ur_x, int ub_y);
    static float calculateDistance(float x1, float y1, float x2, float y2);
    static std::vector<std::vector<int>> transposeMatrix(const std::vector<std::vector<int>>& matrix);
    static std::vector<std::vector<float>> transposeMatrix(const std::vector<std::vector<float>>& matrix);

    // ─────────── 单位属性 ───────────
    static float getUnitX(Units* unit);
    static float getUnitY(Units* unit);
    static float getUnitWidth(Units* unit);
    static float getUnitHeight(Units* unit);
    static float getUnitRadius(Units* unit);

    // ─────────── 动画 ───────────
    static void addAnimation(int x, int y, const std::string& name);
    static void removeAnimation(Animation* animation);

    // ─────────── 数据 ───────────
    static std::unique_ptr<UnitData> getSingleUnitsData(const std::string& name);

    // ─────────── 召唤 ───────────
    static int summon(const std::string& name, int x, int y, int team);

    // ─────────── 帧 ───────────
    static int getFrame();
    static void setFrame(int frame);
    static void incrementFrame();
    static void updataTime();

    // ─────────── 偏移 ───────────
    static float getOffsetX();
    static void setOffsetX(float offsetX);
    static float getOffsetY();
    static void setOffsetY(float offsetY);
    static float getStartX();
    static void setStartX(float startX);
    static float getStartY();
    static void setStartY(float startY);

    // ─────────── 地图 ───────────
    static std::vector<std::vector<int>>& getPlotIndex();
    static void setPlotIndex(const std::vector<std::vector<int>>& newPlotIndex);
    static int getCellWidth();
    static void setCellWidth(int cellWidth);
    static int getCellHeight();
    static void setCellHeight(int cellHeight);
    static AStar* getFindWay();
    static void setFindWay(AStar* findWay);

    // ─────────── 对象池 ───────────
    static Pool& getPool();

    // ─────────── 清理 ───────────
    static void reset();
    static void destroy();
};

#endif // CPP_DATABUS_H