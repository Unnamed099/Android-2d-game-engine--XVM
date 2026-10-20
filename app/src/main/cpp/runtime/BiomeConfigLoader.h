#ifndef CPP_RUNTIME_BIOMECONFIGLOADER_H
#define CPP_RUNTIME_BIOMECONFIGLOADER_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <cstdint>

// 本地 JSON 类（在 .cpp 里定义）
class LocalJSONObject;

class BiomeConfigLoader {
public:
    class TileConfig {
    public:
        int id = 0;
        std::string path;
        int special = 0;
        int dominance = 0;
    };

    class ItemSpawnRule {
    public:
        int id = 0;
        float spawnChance = 0;
        float minHeight = 0;
        float maxHeight = 1;
        std::vector<int> avoidTiles;
        std::vector<int> requireTiles;
        int layer = 0;
    };

    class UnitSpawnRule {
    public:
        int id = 0;
        std::string name;
        float spawnChance = 0;
        int team = 0;
        std::vector<int> allowedTiles;
        std::vector<int> avoidedTiles;
    };

    class SuperpositionConfig;
    class WaveConfig;
    class TileChance;
    class LayerConfig;
    class PatternConfig;
    class ExcessiveWave;
    class ExcessiveCliff;

    class NoiseConfig {
    public:
        int64_t seed = 0;
        float scale = 0.01f;
        int octaves = 4;
        float persistence = 0.5f;
        float lacunarity = 2.0f;
        std::shared_ptr<SuperpositionConfig> superposition;
    };

    class SuperpositionConfig {
    public:
        bool enabled = false;
        bool includeBaseNoise = true;
        std::vector<WaveConfig> waves;
    };

    class WaveConfig {
    public:
        std::string type;
        float amplitude = 0;
        float frequency = 0;
        float phase = 0;
        std::shared_ptr<NoiseConfig> perlin;
    };

    class TileChance {
    public:
        int id = 0;
        float chance = 1.0f;
    };

    class LayerConfig {
    public:
        float minHeight = 0;
        float maxHeight = 1;
        int tileId = 0;
        std::vector<TileChance> tileChances;
        bool isRandomMode() const { return !tileChances.empty(); }
    };

    class PatternConfig {
    public:
        std::string name;
        int rows = 0;
        int cols = 0;
        std::vector<std::set<int>> maskPatterns;
        std::vector<std::set<int>> maskExcepts;
        int centerRow = 0;
        int centerCol = 0;
        int replaceId = 0;
        std::set<int> positiveIds;
    };

    class CorrectionNoise {
    public:
        int scope = 0;
        int interval = 0;
        float minHeight = 0;
        float maxHeight = 1;
        std::string vm_code;
    };

    class ExcessiveWave {
    public:
        std::set<int> center_ids;
        std::set<int> check_ids;
    };

    class ExcessiveCliff {
    public:
        std::set<int> high;
        std::set<int> low;
        int cliff_face_id = 0;
    };

    class SubNoiseConfig {
    public:
        int id = 0;
        std::string name;
        float minThreshold = 0;
        float maxThreshold = 1;
        int priority = 0;
        std::shared_ptr<NoiseConfig> tileNoise;
        std::shared_ptr<NoiseConfig> itemNoise;
        std::vector<LayerConfig> tileLayers;
        std::vector<ItemSpawnRule> items;
        std::vector<CorrectionNoise> correction;
        std::vector<PatternConfig> patterns;
        std::vector<UnitSpawnRule> units;
    };

    class BiomeConfig {
    public:
        std::string name = "default";
        std::shared_ptr<NoiseConfig> masterNoise;
        std::vector<SubNoiseConfig> subNoises;
        std::vector<PatternConfig> globalPatterns;
        std::vector<ExcessiveWave> waves;
        std::vector<ExcessiveCliff> cliff;
    };

    class LoadCallback {
    public:
        virtual ~LoadCallback() {}
        virtual void onSuccess() = 0;
        virtual void onError(const std::string& error) = 0;
    };

private:
    static const std::string TAG;

    std::shared_ptr<BiomeConfig> biomeConfig;
    std::map<int, TileConfig> tileConfigs;
    int horizontalPlaneIndex = 0;

public:
    std::string path;

    BiomeConfigLoader();
    ~BiomeConfigLoader();

    void loadFromJson(const std::string& jsonPath, LoadCallback* callback);

    std::shared_ptr<BiomeConfig> getBiomeConfig() { return biomeConfig; }
    const std::map<int, TileConfig>& getTileConfigs() const { return tileConfigs; }
    int getHorizontalPlaneIndex() const { return horizontalPlaneIndex; }

private:
    std::string readJsonFromAssets(const std::string& filePath);
    void parseJsonData(const std::string& jsonString);

    // ★ 全部改成 LocalJSONObject
    NoiseConfig parseNoiseConfig(const LocalJSONObject& obj);
    SuperpositionConfig parseSuperposition(const LocalJSONObject& obj);
    SubNoiseConfig parseSubNoise(const LocalJSONObject& obj);
    UnitSpawnRule parseUnitSpawnRule(const LocalJSONObject& obj);
    PatternConfig parsePattern(const LocalJSONObject& obj);
};

#endif