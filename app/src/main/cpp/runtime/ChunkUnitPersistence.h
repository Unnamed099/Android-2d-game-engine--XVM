#ifndef CHUNKUNITPERSISTENCE_H
#define CHUNKUNITPERSISTENCE_H

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <iomanip>

class Context;
class Units;
class ObjectValue;
class ChunkUnitPersistence {
public:
    // 内部类前向声明
    class UnitDataSave;
    class MoveSave;
    class UnitsSave;
    class ChunkUnitsData;
    class Converter;
    class UUIDGenerator;
    // 常量
    static const std::string TAG;
    static const std::string BASE_DIR;
    static const int REGION_SIZE = 10;
    static const int CHUNKS_PER_REGION = REGION_SIZE * REGION_SIZE;
    static const long FLUSH_INTERVAL_MS;
    static const std::string CHUNK_TABLE_FILE;
    static const long INDEX_SAVE_INTERVAL_MS;

    // 类型标记
    static const uint8_t TYPE_NULL    = 0;
    static const uint8_t TYPE_INT     = 1;
    static const uint8_t TYPE_FLOAT   = 2;
    static const uint8_t TYPE_DOUBLE  = 3;
    static const uint8_t TYPE_STRING  = 4;
    static const uint8_t TYPE_BOOLEAN = 5;
    static const uint8_t TYPE_LIST    = 6;
    static const uint8_t TYPE_MAP     = 7;

    // 静态初始化（由静态对象调用）
    static void initialize();

    // 公共接口
    static int saveChunkUnits(int chunkX, int chunkY, int mapId, int chunkSize, Context* context);
    static int loadChunkUnits(int chunkX, int chunkY, int mapId, Context* context, UUIDGenerator* uuidGenerator);
    static bool hasChunkSave(int chunkX, int chunkY, int mapId, Context* context);
    static void removeChunkIndex(int chunkX, int chunkY, int mapId);
    static void clearChunkIndex();
    static int getChunkIndexSize();
    static void flushChunkIndex();
    static void startAutoFlush();
    static void stopAutoFlush();
    static void flushDirtyRegions();
    static void flushAll();

    // 通用二进制读写工具（设为 public 以便嵌套类使用）
    static void writeString(std::vector<uint8_t>& out, const std::string& s);
    static std::string readString(const std::vector<uint8_t>& in, size_t& offset);
    static void writeIntList(std::vector<uint8_t>& out, const std::vector<int>& list);
    static std::vector<int> readIntList(const std::vector<uint8_t>& in, size_t& offset);
    static void writeStringList(std::vector<uint8_t>& out, const std::vector<std::string>& list);
    static std::vector<std::string> readStringList(const std::vector<uint8_t>& in, size_t& offset);
    static void writeObject(std::vector<uint8_t>& out, const ObjectValue& obj);
    static ObjectValue readObject(const std::vector<uint8_t>& in, size_t& offset);
    static void writeMap(std::vector<uint8_t>& out, const std::map<std::string, ObjectValue>& map);
    static std::map<std::string, ObjectValue> readMap(const std::vector<uint8_t>& in, size_t& offset);

    // 区域计算
    static int floorDiv(int a, int b);
    static int floorMod(int a, int b);
    static int getRegionX(int chunkX);
    static int getRegionY(int chunkY);
    static int getLocalIndex(int chunkX, int chunkY);
    static std::string getRegionFileName(int regionX, int regionY, int mapId);

    // 磁盘读写（GZIP）
    static std::map<int, std::vector<uint8_t>> readRegionFile(const std::string& regionFilePath);
    static void writeRegionFile(const std::string& regionFilePath, const std::map<int, std::vector<uint8_t>>& blocks);
    static bool chunkExistsInFile(const std::string& regionFilePath, int index);
    static std::vector<uint8_t> serializeChunkData(const ChunkUnitsData& data);
    static ChunkUnitsData deserializeChunkData(const std::vector<uint8_t>& bytes);

    // 内存映射访问
    static std::map<int, std::vector<uint8_t>>& getOrCreateRegionMap(const std::string& regionPath);

    // 辅助
    static bool isUnitInChunk(Units* unit, int chunkX, int chunkY, int chunkSize, int cellWidth, int cellHeight);

    // 定时器线程函数
    static void flushThreadFunc();
    static void indexSaveThreadFunc();

    // 数据成员
    static std::unordered_map<std::string, std::map<int, std::vector<uint8_t>>> memoryRegions;
    static std::unordered_set<std::string> dirtyRegions;
    static std::unordered_map<std::string, bool> chunkIndex;
    static std::mutex dataMutex;
    static std::thread flushScheduler;
    static std::thread indexSaveScheduler;
    static std::atomic<bool> flushRunning;
    static std::atomic<bool> indexSaveRunning;

private:
    static void loadChunkIndex();
    static void saveChunkIndex();
    static void startIndexAutoSave();
    static std::string chunkKey(int chunkX, int chunkY, int mapId);
};

// 内部类定义
class ChunkUnitPersistence::UnitDataSave {
public:
    int id;
    std::string name;
    float hp;
    float maxHp;
    float radius;
    float speed;
    int team;
    float angle;
    float attackR;
    float attack_angle;
    bool isBuilding;
    int w, h;
    int hitBoxLeftTopX;
    int hitBoxLeftTopY;
    int hitBoxRightBootomX;
    int hitBoxRightBootomY;

    // ★ 新增：持久化字段
    bool load_chunk = false;
    std::string vm_code;

    std::vector<int> walkAbleTileId;
    std::vector<std::string> events;
    std::vector<std::map<std::string, std::string>> actionJson;

    void write(std::vector<uint8_t>& out) const;
    static UnitDataSave read(const std::vector<uint8_t>& in, size_t& offset);
};

class ChunkUnitPersistence::MoveSave {
public:
    float targetX;
    float targetY;

    void write(std::vector<uint8_t>& out) const;
    static MoveSave read(const std::vector<uint8_t>& in, size_t& offset);
};

class ChunkUnitPersistence::UnitsSave {
public:
    int x, y, w, h;
    bool visible;
    std::string event;
    std::string animationName;
    std::string skeletonName;
    std::unique_ptr<UnitDataSave> unitData;
    std::unique_ptr<MoveSave> moveData;

    void write(std::vector<uint8_t>& out) const;
    static UnitsSave read(const std::vector<uint8_t>& in, size_t& offset);
};

class ChunkUnitPersistence::ChunkUnitsData {
public:
    int chunkX, chunkY, mapId;
    std::vector<UnitsSave> units;

    void write(std::vector<uint8_t>& out) const;
    static ChunkUnitsData read(const std::vector<uint8_t>& in, size_t& offset);
};

class ChunkUnitPersistence::Converter {
public:
    static UnitsSave fromUnits(Units* unit);
    static void toUnits(const UnitsSave& save, Units* unit);
};

class ChunkUnitPersistence::UUIDGenerator {
public:
    virtual ~UUIDGenerator() {}
    virtual int generate() = 0;

    // ★ 新增：读档时同步已使用的最大 id（默认空实现，兼容旧实现）
    virtual void observe(int /*id*/) {}
};

#endif // CHUNKUNITPERSISTENCE_H