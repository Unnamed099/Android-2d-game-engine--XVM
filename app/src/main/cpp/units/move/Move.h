#ifndef MOVE_H
#define MOVE_H

class AStar;
class Units;

class Move {
public:
    Units* units;
    float x;
    float y;
    float tox;
    float toy;
    float dx;
    float dy;
    bool isMoving;
    Move(Units* units): units(units),x(0),y(0),tox(0),toy(0),
              dx(0),
              dy(0),
              isMoving(false),
              count(0),
              find_way(nullptr) {

    }

    void move();
    void setTarget(float x, float y);
    void setMoving(bool b);

private:
    int count;
    AStar* find_way;

    void initgo(Units* units);
    float calculateAngle(float x, float y);
};

#endif