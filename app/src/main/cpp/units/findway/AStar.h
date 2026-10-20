#ifndef CPP_UNITS_FINDWAY_ASTAR_H
#define CPP_UNITS_FINDWAY_ASTAR_H

#include <vector>
#include <string>
#include <memory>
class ChunkManager;
class Node {
public:
    int x;
    int y;
    float g;
    float h;
    float f;
    Node* parent;

    Node(int x, int y) : x(x), y(y), g(0), h(0), f(0), parent(nullptr) {}
};

class AStar {
public:
    std::vector<int> walk_able_tile_id;
    ChunkManager* chunkManager;

    AStar(ChunkManager* chunkManager);

    float heuristic(const Node& node, const Node& endNode, const std::string& type);
    std::vector<std::shared_ptr<Node>> getNeighbors(const Node& node);
    std::vector<std::vector<int>> findPath(int startX, int startY, int endX, int endY);
    std::vector<std::vector<int>> optimizePath(const std::vector<std::vector<int>>& path);
    std::vector<std::vector<int>> getPath(const Node& node);
    std::vector<std::vector<int>> getLinePoints(int x1, int y1, int x2, int y2);

private:
    bool _isStraightLineWalkable(const std::vector<int>& start, const std::vector<int>& end);
};

#endif // CPP_UNITS_FINDWAY_ASTAR_H