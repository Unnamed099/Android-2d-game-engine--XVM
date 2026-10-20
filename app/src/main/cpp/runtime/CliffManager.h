#ifndef CliffManager_H
#define CliffManager_H

#include <map>
#include <set>
#include <utility>
#include <vector>

#include "jni/Bitmap.h"
#include "jni/Canvas.h"
#include "jni/Paint.h"
#include "BiomeConfigLoader.h"
#include "../GameGlobal.h"

class ProceduralChunkGenerator;

class CliffManager {
public:
    static const int CLIFF_LG    = 0;
    static const int CLIFF_RG    = 1;
    static const int CLIFF_UP    = 2;
    static const int CLIFF_DOWN  = 3;
    static const int CLIFF_LEFT  = 4;
    static const int CLIFF_RIGHT = 5;
    static const int CLIFF_TLT   = 6;
    static const int CLIFF_TLB   = 7;
    static const int CLIFF_TRT   = 8;
    static const int CLIFF_TRB   = 9;

    CliffManager(int cellWidth, int cellHeight,
                 const std::map<int, Bitmap*>& tileBitmaps,
                 ProceduralChunkGenerator* generator);

    void setGenerator(ProceduralChunkGenerator* generator);

    int getCliffTypeSimple(int myTileId, int neighborTileId, int dir,
                           const std::vector<std::vector<int>>& tileIds, int cx, int cy, int nx, int ny,
                           int startWorldX, int startWorldY, int size,
                           const std::set<int>& lowSet, const std::set<int>& highSet);

    std::pair<int, BiomeConfigLoader::ExcessiveCliff*> getCliffType(
                           int myTileId, int neighborTileId, int dir,
                           const std::vector<std::vector<int>>& tileIds, int cx, int cy, int nx, int ny,
                           int startWorldX, int startWorldY, int size,
                           const std::vector<BiomeConfigLoader::ExcessiveCliff*>& cliff);

    Bitmap* getCliffBitmap(int tileId, int cliffId, int cliffType);

    void clearCache();

private:
    BiomeConfigLoader::ExcessiveCliff* isCliffPair(
        int lowId, int highId,
        const std::vector<BiomeConfigLoader::ExcessiveCliff*>& cliff);

    int matchCliff(const std::vector<std::vector<int>>& tileIds,
                   int cx, int cy,
                   const std::set<int>& lowSet,
                   const std::set<int>& highSet,
                   int startWorldX, int startWorldY, int size);

    BiomeConfigLoader::PatternConfig* buildCliffPattern(
        int cliffType, const std::set<int>& A, const std::set<int>& B);

    Bitmap* generateCliffBitmap(int tileId, int cliffId, int cliffType);

    void mallco_list(std::vector<std::set<int>>& m, int n);

    std::set<int> setOf(int id);

    int makeKey(int tileId, int type) const;

private:
    int cellWidth_;
    int cellHeight_;
    std::map<int, Bitmap*> tileBitmaps_;
    ProceduralChunkGenerator* generator_;
};

#endif