#ifndef CPP_RUNTIME_CHUNKTILELOADER_H
#define CPP_RUNTIME_CHUNKTILELOADER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "base/Emitter.h"

class Bitmap;
class ChunkTileLoader : public Emitter {
public:
    class TileConfig {
    public:
        int id;
        std::string imagePath;
        int specialValue;

        TileConfig() : id(0), specialValue(0) {}
        TileConfig(int id, const std::string& imagePath, int specialValue)
            : id(id), imagePath(imagePath), specialValue(specialValue) {}
    };

    class LoadCallback {
    public:
        virtual ~LoadCallback() {}
        virtual void onSuccess() = 0;
        virtual void onError(const std::string& errorMessage) = 0;
        virtual void onProgress(int current, int total) = 0;
    };

    ChunkTileLoader();

    void loadFromJson(const std::string& jsonPath, LoadCallback* callback);
    void loadAllTileImages();
    void loadAllTileImagesSync(LoadCallback* callback);

    const std::vector<Bitmap*>& getTileBitmaps() const;
    Bitmap* getTileBitmapByIndex(int index);
    TileConfig* getTileConfig(int id);
    TileConfig* getTileConfigByIndex(int index);
    int getSpecialValue(int id);
    Bitmap* getTileBitmap(int id);
    std::vector<int> getAllTileIds();
    std::vector<TileConfig> getTileConfigList();
    int getTileCount();
    bool isLoaded() const;
    bool isImagesLoaded() const;

    void waitForImagesLoaded();
    void clearImageCache();
    void cleanup();

private:
    std::string readJsonFromAssets(const std::string& filePath);
    void parseJsonData(const std::string& jsonString);

    // 简化事件系统，隐藏基类同名方法
    void on(const std::string& event, std::function<void()> listener);
    void off(const std::string& event);
    void emit(const std::string& event);

    std::unordered_map<int, TileConfig> tileConfigs_;
    std::vector<TileConfig> tileConfigList_;
    std::vector<Bitmap*> tileBitmaps_;
    std::unordered_map<int, Bitmap*> tileBitmapCache_;

    std::atomic<bool> isLoaded_;
    std::atomic<bool> isImagesLoaded_;
    std::string currentJsonPath_;

    LoadCallback* callback_;

    std::mutex mutex_;
    std::unordered_map<std::string, std::vector<std::function<void()>>> listeners_;
};

#endif // CPP_RUNTIME_CHUNKTILELOADER_H