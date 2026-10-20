#pragma once

#include <vector>
#include <memory>
#include <string>
#include <deque>
#include <unordered_map>
#include <cstdint>
#include "BiomeConfigLoader.h"

class PerlinNoise;

class PerlinNoise {
public:
    static const int PERMUTATION_SIZE = 256;

    // ---- 波叠加 ----
    struct WaveComponent {
        std::string type;      // "sin" / "cos" / "random" / "perlin"
        float amplitude;
        float frequency;
        float phase;
        PerlinNoise* perlin;
        WaveComponent(const std::string& t, float a, float f, float p, PerlinNoise* pn);
    };

    // ---- 几何工具 ----
    struct RectData {
        int leftUpX, leftUpY, sizeX, sizeY;
        RectData();
        void init(int leftUpX, int leftUpY, int sizeX, int sizeY);
        int getCenterX() const;
        int getCenterY() const;
        bool isInside(int x, int y) const;
        static RectData* getIn(int X1, int X2, int Y1, int Y2,
                               int sizeX1, int sizeX2, int sizeY1, int sizeY2);
        static std::vector<RectData*> optainNoLoaded(RectData* needTest, RectData* save);
    };

    struct RandomFuncCreate {
        int interval, trust_interval, fluctuation, scope;
        int gosNonZero[4][2];
        static const int GOS_ZERO[9][2];
        RandomFuncCreate();
        void init(int scope, int interval);
        float opt(int n, float a);
        RectData* getResult(int xn, int yn);
        RectData* getRecenlyPoint(int x, int y);
    };

    // ---- RD：每区域一块缓存 + 一份 X 脚本 ----
    class RD {
    public:
        RD(int leftUpX, int leftUpY, int sizeX, int sizeY,
           PerlinNoise* perlin, const std::string& scriptFile);
        ~RD();

        void putData(int x, int y, float f);
        float* getData(int x, int y);
        float* getCacheArray();

        void copyBlock(int x, int y, int w, int h, float* src, int srcOffset);
        void copyBlockGlobal(int globalX, int globalY, int w, int h, float* src, int srcOffset);

        int leftUpX, leftUpY, sizeX, sizeY;
        int getCenterX() const { return leftUpX + (sizeX - 1) / 2; }
        int getCenterY() const { return leftUpY + (sizeY - 1) / 2; }

        PerlinNoise* perlin;
        int step;

    private:
        float* cacheArray;
        std::string scriptFile;
    };

    PerlinNoise(bool isNeedCompute);
    PerlinNoise(bool isNeedCompute, const std::vector<BiomeConfigLoader::CorrectionNoise>& correct);
    PerlinNoise(long long seed, float scale, int octaves, float persistence, float lacunarity,
                const std::vector<BiomeConfigLoader::CorrectionNoise>& correct);
    ~PerlinNoise();

    void setUseSuperposition(bool use);
    void setIncludeBaseNoise(bool include);
    void clearWaves();
    void addSinWave(float amplitude, float frequency, float phase);
    void addCosWave(float amplitude, float frequency, float phase);
    void addRandomWave(float amplitude, float frequency);
    void addPerlinWave(PerlinNoise* otherNoise, float amplitude, float frequency);

    float noise(float x, float y);
    float fractalNoise(int x0, int y0);
    float fractalNoise2(int x0, int y0);
    float perlinNoiseRaw(float x, float y);

    float mutation(int x, int y);

private:
    std::shared_ptr<std::vector<int>> permutation;
    int octaves;
    float persistence;
    float lacunarity;
    float scale;
    bool useSuperposition;
    bool includeBaseNoise;
    long long seed;
    bool isNeedCompute;
    std::vector<WaveComponent> waves;

    std::vector<BiomeConfigLoader::CorrectionNoise> correct;

    float evaluateWave(const WaveComponent& w, float x, float y);
    float randomNoise(float x, float y);
    float fade(float t);
    float lerp(float t, float a, float b);
    float grad(int hash, float x, float y);

    std::unordered_map<long long, std::shared_ptr<RD>> map;
    std::deque<long long> map_data;
    long long createKey(int x, int y);
    std::shared_ptr<RD> getData(long long key);
    void putData(long long key, std::shared_ptr<RD> rd);
};