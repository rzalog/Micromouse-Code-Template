#ifndef Floodfill_h
#define Floodfill_h

#include <string>
#include <stack>
#include <cstdint>
#include <stdint.h>
#include "Dir.h"

const int CENTER = 8;

class Floodfill {
public:
    Floodfill();
    
    char nextMove(bool going_back=false);
    void resetFloodfill();

    void moveForward();

    inline void turnRight() {
        heading = clockwise(heading);
    }
    inline void turnLeft() {
        heading = counterClockwise(heading);
    }
    inline void turnAround() {
        heading = opposite(heading);
    }
    
    void addWallLeftDiag();
    void addWallRightDiag();
    void addWallFront();
    void addWallRight();
    void addWallLeft();
    
    void removeWallFront();
    void removeWallRight();
    void removeWallLeft();

//    std::string draw(int xx, int yy) const;
//    std::string drawBack(int xx, int yy) const;

protected:
    uint16_t maze_map[16][16];
    uint16_t maze_map_back[16][16];
    Direction heading;
    bool shouldGoForward;
    unsigned int mouseX;
    unsigned int mouseY;

    enum MouseMovement {
        MoveForward,            // Move in the direction mouse is facing
        TurnClockwise,          // Self explanatory
        TurnCounterClockwise,   // Self explanatory
        TurnAround,             // Face the opposite direction currently facing
        Wait,                   // Do nothing this time, do some computation, then try again later
        Finish,                 // Mouse has achieved goals and is ending the simulation
        BackToStart             // Mouse has gone back to the start
    };

    bool isAtCenter(unsigned x, unsigned y) const;
    MouseMovement nextMovement(unsigned x, unsigned y);
    MouseMovement nextMovementBack(unsigned x, unsigned y);

    MouseMovement nextCellMovement(int nextCell);
    
    void addWall(int x, int y, int side);
    void removeWall(int x, int y, int side);
    
    int BIG_VAL;
    int MAX_16;
    uint16_t NORTH_WALL;
    uint16_t SOUTH_WALL;
    uint16_t EAST_WALL;
    uint16_t WEST_WALL;
    uint16_t VISITED;
    uint16_t DISTANCE;
    uint16_t INFOBITS;
};

#endif
