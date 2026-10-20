#ifndef CHUNKRENDERER_H
#define CHUNKRENDERER_H

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include <limits>

#include "BiomeConfigLoader.h"

class Bitmap;
class Canvas;
class Paint;
class RectF;
class ChunkManager;
class CliffManager;
class TileArcManager;

class ChunkRenderer{
public:
    class O {
    public:
        int value;
        BiomeConfigLoader::ExcessiveWave* wave;
    };

    class CachedArcInfo {
    public:
        const int neighborId;
        const int arcType;
        CachedArcInfo(int neighborId, int arcType);
    };

    class CachedCliffInfo {
    public:
        const int cliffType;
        const int cliffFaceId;
        CachedCliffInfo(int cliffType, int cliffFaceId);
    };

    ChunkRenderer(ChunkManager* chunkManager);
    virtual ~ChunkRenderer();

    void init0();
    O* check(int b, int a);

    void render(Canvas* canvas, Paint* p);
    void renderSurface(Canvas* canvas, Paint* p);

    static long long concatToLong(int high, int low);
    ChunkManager* getChunkManager();
    void destroy();

private:
    ChunkManager* chunkManager;
    Paint* paint;
    RectF* srcRect;
    RectF* dstRect;

    int cellWidth;
    int cellHeight;

    std::map<int, Bitmap*> tileBitmaps;

    float alignedStartX;
    float alignedStartY;
    static constexpr float ALIGN_STEP = 1.0f;

    CliffManager* cachedCliffManager;
    TileArcManager* cachedArcManager;
    BiomeConfigLoader::BiomeConfig* cachedBiomeConfig;
    bool managersCached;

    class DecorCommand {
    public:
        Bitmap* bitmap;
        int worldX, worldY;
    };
    std::vector<DecorCommand> decorCommands;

    class DrawCommand {
    public:
        Bitmap* bitmap;
        int srcLeft, srcTop, srcRight, srcBottom;
        int dstLeft, dstTop, dstRight, dstBottom;
        DrawCommand(Bitmap* b, int sl, int st, int sr, int sb, int dl, int dt, int dr, int db);
    };
    std::vector<DrawCommand> drawCommandList;

    int lastChunkX;
    int lastChunkY;

    static const int MAX_CACHE_SIZE = 2048;

    template<typename K, typename V>
    class LruCache {
    private:
        size_t maxSize_;
        std::unordered_map<K, V> data_;
        std::list<K> order_;
    public:
        explicit LruCache(size_t maxSize) : maxSize_(maxSize) {}

        bool containsKey(const K& key) const {
            return data_.find(key) != data_.end();
        }

        V* get(const K& key) {
            auto it = data_.find(key);
            if (it != data_.end()) {
                return &it->second;
            }
            return nullptr;
        }

        void put(const K& key, const V& value) {
            if (data_.find(key) != data_.end()) {
                return;
            }
            data_.emplace(key, value);
            order_.push_back(key);
            if (data_.size() > maxSize_) {
                K oldest = order_.front();
                order_.pop_front();
                data_.erase(oldest);
            }
        }

        void clear() {
            data_.clear();
            order_.clear();
        }

        size_t size() const {
            return data_.size();
        }
    };

public:
    LruCache<long long, CachedArcInfo> op_map_cache;
private:
    std::unordered_set<long long> checkedEmpty;
public:
    LruCache<long long, std::vector<CachedCliffInfo>> cliff_cache;
private:
    std::unordered_set<long long> cliffCheckedEmpty;

public:
    std::unordered_map<int, std::vector<O>> op_map;

    int getTileIdAtWorld(int worldX, int worldY);
    void drawBaseChunks(Canvas* canvas);
    void ensureManagersCached(int worldX, int worldY);
    void collectDecorations(int wx, int wy, int myTileId);
    void checkAndCacheArc(int wx, int wy, int myTileId, long long xy);
    void drawArcBitmap(int wx, int wy, int neighborId, int arcType);
    void checkAndCacheCliffs(int wx, int wy, int myTileId, long long xy);
    void drawCliffBitmap(int wx, int wy, int myTileId, int cliffFaceId, int cliffType);
    std::vector<std::vector<int>> buildTileArray(int centerWorldX, int centerWorldY,
                                                 int arrayWidth, int arrayHeight,
                                                 int centerCol, int centerRow);
    void drawSurfaceChunks(Canvas* canvas);
};

#endif