#ifndef CPP_RUNTIME_PROCEDURALCHUNKGENERATOR_H
#define CPP_RUNTIME_PROCEDURALCHUNKGENERATOR_H

#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "jni/Matrix.h"
#include "runtime/ChunkManager.h"
#include "BiomeConfigLoader.h"

class Bitmap;
class PerlinNoise;
class TileArcManager;
class CliffManager;
class WhileFrameTask;
class ChunkUnitPersistence;
class GameGlobal;

class ProceduralChunkGenerator {
public:
    class SubNoiseInstance {
    public:
        BiomeConfigLoader::SubNoiseConfig config;
        PerlinNoise* tileNoise;
        PerlinNoise* itemNoise;
        int priority;
        SubNoiseInstance() : tileNoise(nullptr), itemNoise(nullptr), priority(0) {}
    };

    class BoundaryTileCache {
    private:
        std::map<int64_t, int> cache;
        ProceduralChunkGenerator* generator;
        int hits;
        int misses;

    public:
        BoundaryTileCache(ProceduralChunkGenerator* generator);
        static int64_t makeKey(int worldX, int worldY);
        int getOrCompute(int worldX, int worldY);
        void preloadRegion(int minX, int minY, int maxX, int maxY);
        void trimToSize(int maxSize);
        void clear();
        float getHitRate();
    };

private:
    static const std::string TAG;
    static const bool DEBUG_LOG = false;

    const int chunkSize;
    const int cellWidth;
    const int cellHeight;
    BiomeConfigLoader::BiomeConfig biomeConfig;
    const std::map<int, Bitmap*> tileBitmaps;
    const std::map<int, int> tileSpecials;
    const int horizontalPlaneIndex;
public:
    int mapId;
private:
    PerlinNoise* masterNoise;
    std::vector<SubNoiseInstance> subNoises;
public:
    const Matrix* matrix;
private:
    std::vector<WhileFrameTask*> activeTasks;
    std::vector<void*> pendingRequests;
    std::set<int64_t> pendingChunkIds;
    static const int MAX_ACTIVE_TASKS = 3;

    int64_t lastFrameCost;
    int lastTaskCount;
    int lastStepsPerTask;

    TileArcManager* arcManager;
    CliffManager* cliffManager;
    BoundaryTileCache* boundaryCache;

    friend class BoundaryTileCache;

public:
    ProceduralChunkGenerator(int chunkSize, BiomeConfigLoader* loader,
                             std::map<int, Bitmap*> tileBitmaps, int id);

private:
    int getTileIdWithBoundaryCache(int worldX, int worldY);
    int computeTileIdAt(int worldX, int worldY);

public:
    bool matchPattern(const std::vector<std::vector<int>>& tileIds, int cx, int cy,
                      const BiomeConfigLoader::PatternConfig* pattern,
                      int startWorldX, int startWorldY, int size);
    void generateChunkAsync(const int worldChunkX, const int worldChunkY,
                            const int relativeX, const int relativeY,
                            std::function<void(ChunkManager::Chunk*)> callback);

private:
    void startTask(const int worldChunkX, const int worldChunkY,
                   const int relativeX, const int relativeY,
                   std::function<void(ChunkManager::Chunk*)> callback);
    void createAndSubmitPhaseTask(const std::map<std::string, void*>& taskData);
    int getTileIdFromLayer(const BiomeConfigLoader::SubNoiseConfig& selected, float heightValue, int worldX, int worldY);
    int selectTileByChance(const std::vector<BiomeConfigLoader::TileChance>& chances, int worldX, int worldY);
    void generateUnitsForChunk(const int worldChunkX, const int worldChunkY,
                               const std::vector<std::vector<int>>& tileIds,
                               const ChunkManager::Chunk* chunk,
                               const std::function<void(ChunkManager::Chunk*)> callback);
    void applyPatterns(int step, std::map<std::string, void*>& data);
    void generateBaseTerrain(int step, std::map<std::string, void*>& data);
    void generateItems(int step, std::map<std::string, void*>& data);
    void finishChunk(std::map<std::string, void*>& data);

public:
    void update();
    int getActiveTaskCount();

private:
    static int64_t makeChunkId(int x, int y);
    void removePendingChunkId(int worldChunkX, int worldChunkY);
};

#endif