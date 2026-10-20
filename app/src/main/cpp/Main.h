#ifndef CPP_MAIN_H
#define CPP_MAIN_H

#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>

class Canvas;
class ChunkRenderer;
class ChunkManager;
class ChunkTileLoader;
class LogCat;
class Units;
class Handler;

class Main {
public:
    static const std::string BMP_PATH;

    Handler* handler = nullptr;
    int aniId = 0;

    ChunkRenderer* chunkRenderer = nullptr;
    ChunkManager* chunkManager = nullptr;
    ChunkTileLoader* tileLoader = nullptr;
    Canvas* ctx = nullptr;
    bool startGame = false;
    bool FL = false;

private:
    std::vector<Units*> list_units;
    bool isFirstLoad = true;

public:
    Main() = default;
    ~Main();

    void load_game(const std::string& mapPath, int id);
    void renderToCanvas(Canvas* canvas);
    void update();
    void clear();
    void destroy();
    std::string getCacheStats();
};

#endif