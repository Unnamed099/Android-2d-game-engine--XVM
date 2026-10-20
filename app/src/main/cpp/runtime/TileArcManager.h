#ifndef CPP_RUNTIME_TILEARCMANAGER_H
#define CPP_RUNTIME_TILEARCMANAGER_H

#include <map>
#include <set>
#include <vector>

#include "jni/Bitmap.h"
#include "runtime/BiomeConfigLoader.h"
#include "runtime/ProceduralChunkGenerator.h"

class TileArcManager {
public:
    static const int ORL = 0;
    static const int ORH = 1;
    static const int OLL = 2;
    static const int OLH = 3;
    static const int TRL = 4;
    static const int TRH = 5;
    static const int TLL = 6;
    static const int TLH = 7;
    static const int NU = 8;
    static const int ND = 9;
    static const int NL = 10;
    static const int NR = 11;

    TileArcManager(int cellWidth, int cellHeight,
                   const std::map<int, int>& tileDominance,
                   const std::map<int, Bitmap*>& tileBitmaps,
                   ProceduralChunkGenerator* generator);
    ~TileArcManager();

    Bitmap* getArcBitmap(int tileId, int arcType);

    // ★ 改成 std::set<int>
    int getArcTypeForEdge(const std::vector<std::vector<int>>& tileIds, int cx, int cy,
                          const std::set<int>& A, const std::set<int>& B,
                          int startWorldX, int startWorldY, int size);

    void clearCache();

    static int getDirection(int dx, int dy);
    static int oppositeDirection(int dir);

    void setArcCurvature(float curvature);
    float getArcCurvature() const;

private:
    BiomeConfigLoader::PatternConfig* getPresetPattern(int arcType,
                                                       const std::set<int>& A,
                                                       const std::set<int>& B);
    std::set<int> setOf(int id);
    std::set<int> setOfs(const std::set<int>& id);

    Bitmap* generateArcMask(int arcType);
    Bitmap* fillMaskWithTexture(Bitmap* mask, int tileId, int arcType);
    Bitmap* createDirectionalShadow(Bitmap* mask, int arcType);

    int makeKey(int tileId, int arcType);

private:
    const int cellWidth_;
    const int cellHeight_;
    std::map<int, int> tileDominance_;
    std::map<int, Bitmap*> tileBitmaps_;
    ProceduralChunkGenerator* generator_;
    std::map<int, Bitmap*> arcCache_;
    float arcCurvature_;
};

#endif