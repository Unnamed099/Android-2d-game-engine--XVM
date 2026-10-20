#ifndef CPP_UNITS_FINDWAY_DENY_H
#define CPP_UNITS_FINDWAY_DENY_H

#include <vector>

class ChunkManager;

class Deny {
public:
    static bool getDeny(ChunkManager* chunkManager, const std::vector<int>& walk_able_tile_id, int x, int y);
};

#endif // CPP_UNITS_FINDWAY_DENY_H