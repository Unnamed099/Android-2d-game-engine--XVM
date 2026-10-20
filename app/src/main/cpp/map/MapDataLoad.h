#ifndef CPP_MAP_MAPDATALOAD_HPP
#define CPP_MAP_MAPDATALOAD_HPP

#include <string>
#include <vector>
#include <map>
#include <functional>

class Context;
class Figure;

class MapDataLoad {
public:
    // ============ 地图项基类 ============
    class MapItem {
    public:
        virtual ~MapItem() {}
        virtual std::string getType() = 0;
    };

    // ============ 图像数据项 ============
    class ImageItem : public MapItem {
    private:
        int id = 0;
        std::string path;
        int specialValue = 0;

    public:
        ImageItem(int id, const std::string& path, int specialValue);

        std::string getType() override;
        int getId();
        std::string getPath();
        int getSpecial();
    };

    // ============ 地图瓦片 ============
    class Tile {
    private:
        int id = 0;
        int specialValue = 0;

    public:
        Tile(int id, int specialValue);
        int getId();
        int getSpecialValue();
    };

    // ============ 地图中的物品 ============
    class Item {
    private:
        int x = 0;
        int y = 0;
        int id = 0;

    public:
        Item(int x, int y, int id);
        bool match_position(int x, int y);
        int getId();
        int getX();
        int getY();
    };

    // ============ 物品组 ============
    class Items {
    private:
        int index = 0;
        std::vector<Item*> items;

    public:
        Items(int index);
        ~Items();

        std::vector<Item*>& getItems();
        int getIndex();
        void addItem(Item* item);
        int getItemListSize();
        Item* getItem(int index);
    };

    // ============ 地图信息项 ============
    class MapInfoItem : public MapItem {
    private:
        int width = 0;
        int height = 0;
        int backgroundId = 0;
        int horizontal_plane_index = 0;
        std::vector<Tile*> tiles;
        std::vector<Items*> items;
        std::vector<Figure*> figures;

    public:
        MapInfoItem();
        ~MapInfoItem();

        std::string getType() override;

        int getWidth();
        void setWidth(int width);

        int getHeight();
        void setHeight(int height);

        int getHorizontal_plane_index();
        void setHorizontal_plane_index(int index);

        int getBackgroundId();
        void setBackgroundId(int backgroundId);

        std::vector<Tile*>& getTiles();
        Items* getItems(int i);
        int getItemsSize();
        void addTile(Tile* tile);
        void addItems(Items* items);

        void addFigure(Figure* figure);
        Figure* getFigure(int i);
        int getFigureSize();
        void setFigure(int index, Figure* figure);
        std::vector<Figure*>& getFigures();
    };

    // ============ 回调接口 ============
    using OnSuccessCallback = std::function<void()>;
    using OnErrorCallback = std::function<void(const std::string&)>;

    class DataLoadCallback {
    public:
        OnSuccessCallback onSuccess;
        OnErrorCallback onError;
    };

private:
    std::vector<MapItem*> mapData;
    Context* context = nullptr;
    DataLoadCallback callback;

public:
    MapDataLoad(Context* context, const std::string& filePath,
                const DataLoadCallback& callback);
    ~MapDataLoad();

    // ============ 公共访问方法 ============
    std::string getPathById(int id);
    std::vector<MapItem*>& getPaths();
    int getWidth();
    int getHeigth();   // ★ 保留原拼写错误
    Tile* getTileAtPosition(int x, int y);
    int getBackgroundId();
    MapInfoItem* getMapInfo();
    std::vector<ImageItem*> getImageItems();
    bool isDataLoaded();
    void clear();
    std::vector<std::string> getImagePaths();
    void printSummary();
    MapInfoItem* getMapInfoitem();

private:
    void loadMapFile(const std::string& filePath);
    std::string readJsonFromAssets(const std::string& filePath);
    void parseJSONData(const std::string& jsonString);
};

#endif