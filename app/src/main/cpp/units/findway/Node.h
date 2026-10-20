#ifndef CPP_UNITS_FINDWAY_NODE_H
#define CPP_UNITS_FINDWAY_NODE_H

class Node {
public:
    int x;
    int y;
    float g;
    float h;
    float f;
    Node* parent;

    Node(int x, int y);
};

#endif // CPP_UNITS_FINDWAY_NODE_H