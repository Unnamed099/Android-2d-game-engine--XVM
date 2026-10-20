#ifndef CPP_RUNTIME_CHUNKMANAGER_H
#define CPP_RUNTIME_CHUNKMANAGER_H

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <deque>
#include <memory>
#include <mutex>
#include <atomic>
#include "BiomeConfigLoader.h"

class CliffManager;
class TileArcManager;
class ProceduralChunkGenerator;

class ChunkManager {
public:
    static const int PLAYER_RELATIVE_X;
    static const int PLAYER_RELATIVE_Y;
    static const int MAX_CACHE_SIZE;
    static const int PRELOAD_RADIUS;
    static const std::string TAG;

    class Chunk {
    public:
        Chunk(int worldChunkX, int worldChunkY);
        ~Chunk();

        void setDecorManagers(CliffManager* cliffManager, TileArcManager* arcManager,
                              BiomeConfigLoader::BiomeConfig* biomeConfig);

        void setTileIds(const std::vector<std::vector<int>>& ids);
        std::vector<std::vector<int>>& getTileIds();

        void setItems(const std::unordered_map<int, std::vector<std::vector<int>>>& items);
        std::unordered_map<int, std::vector<std::vector<int>>>& getItems();

        void setSpecialValues(const std::vector<std::vector<int>>& values);
        int getSpecialValue(int localX, int localY) const;
        std::vector<std::vector<int>>& getSpecialValues();

        void setRelativePosition(int relX, int relY);

        bool isActive() const;
        void markActive();
        int64_t getLastActiveTime() const;

        int64_t getChunkId() const;
        int getWorldChunkX() const;
        int getWorldChunkY() const;
        int getRelativeX() const;
        int getRelativeY() const;

        void destroy();

        CliffManager* getCliffManager() const;
        TileArcManager* getArcManager() const;
        BiomeConfigLoader::BiomeConfig* getBiomeConfig() const;

    private:
        static int64_t makeChunkId(int x, int y);

        const int64_t chunkId;
        const int worldChunkX;
        const int worldChunkY;
        int relativeX;
        int relativeY;
        std::vector<std::vector<int>> tileIds;
        std::vector<std::vector<int>> specialValues;
        std::unordered_map<int, std::vector<std::vector<int>>> items;
        std::atomic<int64_t> lastActiveTime;
        CliffManager* cliffManager;
        TileArcManager* arcManager;
        BiomeConfigLoader::BiomeConfig* biomeConfig;
    };

    class ChunkUpdateListener {
    public:
        virtual ~ChunkUpdateListener() {}
        virtual void onChunkLoaded(int relativeX, int relativeY, Chunk* chunk) = 0;
        virtual void onAllChunksReady() = 0;
    };

    ChunkManager(ProceduralChunkGenerator* chunkGenerator);
    ~ChunkManager();

    void setUpdateListener(ChunkUpdateListener* listener);
    void updata(int x, int y);
    void setPreloadEnabled(bool enabled);
    void moveToOrigin(int newOriginChunkX, int newOriginChunkY);
    Chunk* getChunkByRelative(int relX, int relY);
    Chunk* getPlayerChunk();
    std::string getCacheStats();
    void cleanup();
    Chunk* getChunkByWorld(int worldChunkX, int worldChunkY);
    int getSpecialValue(int x, int y);
    int getChunkPixelWidth() const;
    int getChunkPixelHeight() const;
    int getOriginChunkX() const;
    int getOriginChunkY() const;
    bool isAllChunksReady() const;

private:
    struct ChunkRecord {
        int64_t chunkId;
        int64_t lastActiveTime;
        ChunkRecord(int64_t chunkId, int64_t lastActiveTime)
                : chunkId(chunkId), lastActiveTime(lastActiveTime) {}
    };

    struct PreloadTask {
        int worldX;
        int worldY;
        int dist;
        PreloadTask() = default;
        PreloadTask(int worldX, int worldY, int dist)
                : worldX(worldX), worldY(worldY), dist(dist) {}
    };

    void checkPreload();
    void doPreload();
    void putToCache(Chunk* chunk);

    // === 重构后新增/替换 ===
    // 只在持有 nonActiveMutex 时调用
    int64_t findOldestChunkIdLocked();
    void eraseFromNonActiveListLocked(int64_t chunkId);
    // 锁外做淘汰慢操作（IO / 遍历 units / delete）
    void evictChunkSlow(int64_t chunkId);

    void removeFromNonActiveList(int64_t chunkId);
    void addToNonActiveList(int64_t chunkId, int64_t lastActiveTime);
    void checkAllChunksReady();
    static int64_t makeChunkId(int x, int y);

    const int chunkPixelWidth;
    const int chunkPixelHeight;
    std::vector<std::vector<Chunk*>> activeChunks;
    std::unordered_map<int64_t, Chunk*> cacheChunks;
    std::mutex cacheMutex;
    std::vector<ChunkRecord> nonActiveCacheList;
    std::mutex nonActiveMutex;
    bool isFirst;
    ProceduralChunkGenerator* chunkGenerator;
    std::atomic<int> activeLoadedCount;
    std::atomic<bool> allActiveChunksReady;
    ChunkUpdateListener* updateListener;
    bool preloadEnabled;
    std::deque<PreloadTask> preloadQueue;
    std::mutex preloadMutex;

    // 已删除: std::mutex activeMutex;
};

#endif