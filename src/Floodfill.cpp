#include "Floodfill.h"
#include <iostream>
#include <math.h>
#include <string>
#include "stdio.h"

#include "globals.h"
#include "sensors.h"

Floodfill::Floodfill() : heading(NORTH), mouseX(0), mouseY(0) {
    int xdistance = 0;
    int ydistance = 0;
    int center = CENTER;
    
    BIG_VAL = 10000000;
    MAX_16 = 38528;
    NORTH_WALL = 0b1 << 8;
    SOUTH_WALL = 0b1 << 9;
    EAST_WALL = 0b1 << 10;
    WEST_WALL = 0b1 << 11;
    VISITED = 0b1 << 12;
    DISTANCE = 255;
    INFOBITS = ~DISTANCE;

    // initialize distances and walls for maze map and map back
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            uint16_t *current = &maze_map[x][y];
            uint16_t *currentBack = &maze_map_back[x][y];

            xdistance = abs(center-x);
            ydistance = abs(center-y);

            if (x < center) {
                xdistance--;
            }
            if (y < center) {
                ydistance--;
            }

            *current = xdistance + ydistance;
            *currentBack = x + y;

            if (x == 0) {
                *current |= WEST_WALL;
                *currentBack |= WEST_WALL;
            }
            if (x == 15) {
                *current |= EAST_WALL;
                *currentBack |= EAST_WALL;
            }
            if (y == 0) {
                *current |= SOUTH_WALL;
                *currentBack |= SOUTH_WALL;
            }
            if (y == 15) {
                *current |= NORTH_WALL;
                *currentBack |= NORTH_WALL;
            }
         }
    }
    // add initial right wall
    addWall(0, 0, EAST);
}

void Floodfill::resetFloodfill() {
    mouseX = 0;
    mouseY = 0;
    heading = NORTH;
}

char Floodfill::nextMove(bool going_back) {
    MouseMovement next;
    if (!going_back) {
        next = nextMovement(mouseX, mouseY);
    }
    else {
        next = nextMovementBack(mouseX, mouseY);    
    }
    
    char ret;
    
    // Note: mouseX, mouseY, heading all modified in moveForward, etc.
    switch(next) {
            case MoveForward:
                moveForward();
                ret = 'F';
                break;
            case TurnClockwise: // right
                turnRight();
                ret = 'R';
                break;
            case TurnCounterClockwise: // left
                turnLeft();
                ret = 'L';
                break;
            case TurnAround:
                turnAround();
                ret = 'T';
                break;
            case Finish:
                ret = 'D';
                break;
            case BackToStart:
                ret = 'S';
                break;
            case Wait:
                ret = 'W';
                break;
            default:
                ret = '?';
    }
    
    return ret;
}


Floodfill::MouseMovement Floodfill::nextMovement(unsigned x, unsigned y) {
    // get info for current cell and mark as visited
    uint16_t *current = &(maze_map[x][y]);

    *current |= VISITED;

    //pc.printf("getting next movement at (%d, %d)\n", x, y);

    if (isAtCenter(x, y)) {
        return Finish;
    }

    if (!(shortWall()) && shouldGoForward) {
        shouldGoForward = false;
        return MoveForward;
    }

    int neighbors[4];

    if (y < 15) {
        if (*current & NORTH_WALL) {
            neighbors[NORTH] = BIG_VAL;
        }
        else {
            neighbors[NORTH] = maze_map[x][y+1] & DISTANCE;
        }
    } else {
        neighbors[NORTH] = BIG_VAL;
    }

    if (y > 0) {
        if (*current & SOUTH_WALL) {
            neighbors[SOUTH] = BIG_VAL;
        }
        else {
            neighbors[SOUTH] = maze_map[x][y-1] & DISTANCE;
        }
    } else {
        neighbors[SOUTH] = BIG_VAL;
    }

    if (x < 15) {
        if (*current & EAST_WALL) {
            neighbors[EAST] = BIG_VAL;
        }
        else {
            neighbors[EAST] = maze_map[x+1][y] & DISTANCE;
        }
    } else {
        neighbors[EAST] = BIG_VAL;
    }

    if (x > 0) {
        if (*current & WEST_WALL) {
            neighbors[WEST] = BIG_VAL;
        }
        else {
            neighbors[WEST] = maze_map[x-1][y] & DISTANCE;
        }
    } else {
        neighbors[WEST] = BIG_VAL;
    }


    int nextCell = BIG_VAL;
    int min = *current & DISTANCE;
    
    int cell_index = 0;
    for (int i = 0; i < 4; i++) {
        // Start checking the heading you are currently at
        cell_index = (heading + i) % 4;
        if (neighbors[cell_index] < min) {
            min = neighbors[cell_index];
            nextCell = cell_index;
        }
    }

    // move to neighbor cell with smallest distance
    if (nextCell != BIG_VAL) {
        return nextCellMovement(nextCell);
    }
    else if (nextCell == BIG_VAL) 
    {
        // run floodfill to reassign distance values
        std::stack<uint16_t*> cell_stack;
        cell_stack.push(current);

        int min_distance;
        while (!cell_stack.empty()) {
            uint16_t *cur = cell_stack.top();
            cell_stack.pop();

            uint16_t this_x = (cur - &maze_map[0][0]) / 16;
            uint16_t this_y = (cur - &maze_map[0][0]) % 16;

            if ((*cur & DISTANCE) == 0) {
                continue;
            }
            min_distance = BIG_VAL;

            // check all neighbors
            for (int i = 0; i < 4; i++) {
                uint16_t tmp = MAX_16;
                switch (i) {
                    case NORTH:
                        if (this_y < 15) {
                            tmp = maze_map[this_x][this_y+1] & DISTANCE;
                        }
                        if (*cur & NORTH_WALL) {
                            continue;
                        }
                        break;
                    case SOUTH:
                        if (this_y > 0) {
                            tmp = maze_map[this_x][this_y-1] & DISTANCE;
                        }
                        if (*cur & SOUTH_WALL) {
                            continue;
                        }
                        break;
                    case EAST:
                        if (this_x < 15) {
                            tmp = maze_map[this_x+1][this_y] & DISTANCE;
                        }
                        if (*cur & EAST_WALL) {
                            continue;
                        }
                        break;
                    case WEST:
                        if (this_x > 0) {
                            tmp = maze_map[this_x-1][this_y] & DISTANCE;
                        }
                        if (*cur & WEST_WALL) {
                            continue;
                        }
                        break;                            
                }

                if (tmp == MAX_16) {
                    continue;
                }
                if (tmp < min_distance) {
                    min_distance = tmp;
                }
            }

            if (min_distance == BIG_VAL) {
                continue;
            }
            if ((*cur & DISTANCE) > min_distance) {
                continue;
            }
            
            // update min current cell's distance
            uint16_t temp_info = *cur & INFOBITS;
            *cur = min_distance + 1;
            *cur |= temp_info;

            // push all neighbors onto stack
            if (this_y < 15) {
                cell_stack.push(&maze_map[this_x][this_y+1]);
            }
            if (this_y > 0) {
                cell_stack.push(&maze_map[this_x][this_y-1]);
            }
            if (this_x < 15) {
                cell_stack.push(&maze_map[this_x+1][this_y]);
            }
            if (this_x > 0) {
                cell_stack.push(&maze_map[this_x-1][this_y]);
            }
        }

        return Wait;
    }

    return Finish;
}

Floodfill::MouseMovement Floodfill::nextMovementBack(unsigned x, unsigned y) {
    // copy maze map and initialize values
    // keep wall information but set new distance values
    uint16_t *current = &(maze_map_back[x][y]);
    
    *current |= VISITED;
    
    if (x == 0 && y == 0) {
        return BackToStart;
    }
    
    if (!(shortWall()) && shouldGoForward) {
        shouldGoForward = false;
        return MoveForward;
    }
    
    int neighbors[4];
    
    if (y < 15) {
        if (*current & NORTH_WALL) {
            neighbors[NORTH] = BIG_VAL;
        }
        else {
            neighbors[NORTH] = maze_map_back[x][y+1] & DISTANCE;
        }
    } else {
        neighbors[NORTH] = BIG_VAL;
    }
    
    if (y > 0) {
        if (*current & SOUTH_WALL) {
            neighbors[SOUTH] = BIG_VAL;
        }
        else {
            neighbors[SOUTH] = maze_map_back[x][y-1] & DISTANCE;
        }
    } else {
        neighbors[SOUTH] = BIG_VAL;
    }
    
    if (x < 15) {
        if (*current & EAST_WALL) {
            neighbors[EAST] = BIG_VAL;
        }
        else {
            neighbors[EAST] = maze_map_back[x+1][y] & DISTANCE;
        }
    } else {
        neighbors[EAST] = BIG_VAL;
    }
    
    if (x > 0) {
        if (*current & WEST_WALL) {
            neighbors[WEST] = BIG_VAL;
        }
        else {
            neighbors[WEST] = maze_map_back[x-1][y] & DISTANCE;
        }
    } else {
        neighbors[WEST] = BIG_VAL;
    }
    
    int nextCell = BIG_VAL;
    int min = *current & DISTANCE;
    
    int cell_index = 0;
    for (int i = 0; i < 4; i++) {
        // Start checking the heading you are currently at
        cell_index = (heading + i) % 4;
        if (neighbors[cell_index] < min) {
            min = neighbors[cell_index];
            nextCell = cell_index;
        }
    }
    
    // move to neighbor cell with smallest distance
    if (nextCell != BIG_VAL) {
        return nextCellMovement(nextCell);
    }
    else if (nextCell == BIG_VAL)
    {
        // run floodfill to reassign distance values
        std::stack<uint16_t*> cell_stack;
        cell_stack.push(current);
        
        int min_distance;
        while (!cell_stack.empty()) {
            uint16_t *cur = cell_stack.top();
            cell_stack.pop();
            
            uint16_t this_x = (cur - &maze_map_back[0][0]) / 16;
            uint16_t this_y = (cur - &maze_map_back[0][0]) % 16;
            
            if ((*cur & DISTANCE) == 0) {
                continue;
            }
            min_distance = BIG_VAL;
            
            // check all neighbors
            for (int i = 0; i < 4; i++) {
                uint16_t tmp = MAX_16;
                switch (i) {
                    case NORTH:
                        if (this_y < 15) {
                            tmp = maze_map_back[this_x][this_y+1] & DISTANCE;
                        }
                        if (*cur & NORTH_WALL) {
                            continue;
                        }
                        break;
                    case SOUTH:
                        if (this_y > 0) {
                            tmp = maze_map_back[this_x][this_y-1] & DISTANCE;
                        }
                        if (*cur & SOUTH_WALL) {
                            continue;
                        }
                        break;
                    case EAST:
                        if (this_x < 15) {
                            tmp = maze_map_back[this_x+1][this_y] & DISTANCE;
                        }
                        if (*cur & EAST_WALL) {
                            continue;
                        }
                        break;
                    case WEST:
                        if (this_x > 0) {
                            tmp = maze_map_back[this_x-1][this_y] & DISTANCE;
                        }
                        if (*cur & WEST_WALL) {
                            continue;
                        }
                        break;
                }
                
                if (tmp == MAX_16) {
                    continue;
                }
                if (tmp < min_distance) {
                    min_distance = tmp;
                }
            }
            
            if (min_distance == BIG_VAL) {
                continue;
            }
            if ((*cur & DISTANCE) > min_distance) {
                continue;
            }
            
            // update min current cell's distance
            uint16_t temp_info = *cur & INFOBITS;
            *cur = min_distance + 1;
            *cur |= temp_info;
            
            // push all neighbors onto stack
            if (this_y < 15) {
                cell_stack.push(&maze_map_back[this_x][this_y+1]);
            }
            if (this_y > 0) {
                cell_stack.push(&maze_map_back[this_x][this_y-1]);
            }
            if (this_x < 15) {
                cell_stack.push(&maze_map_back[this_x+1][this_y]);
            }
            if (this_x > 0) {
                cell_stack.push(&maze_map_back[this_x-1][this_y]);
            }
        }
        
        return Wait;
    }
   
    return BackToStart;
}

void Floodfill::addWallLeftDiag() {
    switch (heading) {
        case NORTH:
            addWall(mouseX, mouseY+1, WEST);
            break;
        case SOUTH:
            addWall(mouseX, mouseY-1, EAST);
            break;
        case EAST:
            addWall(mouseX+1, mouseY, NORTH);
            break;
        case WEST:
            addWall(mouseX-1, mouseY, SOUTH);
            break;
        default:
            break;
    }
}

void Floodfill::addWallRightDiag() {
    switch (heading) {
        case NORTH:
            addWall(mouseX, mouseY+1, EAST);
            break;
        case SOUTH:
            addWall(mouseX, mouseY-1, WEST);
            break;
        case EAST:
            addWall(mouseX+1, mouseY, SOUTH);
            break;
        case WEST:
            addWall(mouseX-1, mouseY, NORTH);
            break;
        default:
            break;
    }
}


void Floodfill::addWallFront() {
    //pc.printf("adding wall front x: %d y: %d\n", mouseX, mouseY);
    switch (heading) {
        case NORTH:
            addWall(mouseX, mouseY, NORTH);
            break;
        case SOUTH:
            addWall(mouseX, mouseY, SOUTH);
            break;
        case EAST:
            addWall(mouseX, mouseY, EAST);
            break;
        case WEST:
            addWall(mouseX, mouseY, WEST);
            break;
        default:
            break;
    }
}

void Floodfill::addWallRight() {
    //pc.printf("adding wall right x: %d y: %d\n", mouseX, mouseY);
    switch (heading) {
        case NORTH:
            addWall(mouseX, mouseY, EAST);
            break;
        case SOUTH:
            addWall(mouseX, mouseY, WEST);
            break;
        case EAST:
            addWall(mouseX, mouseY, SOUTH);
            break;
        case WEST:
            addWall(mouseX, mouseY, NORTH);
            break;
        default:
            break;
    }
}

void Floodfill::addWallLeft() {
    //pc.printf("adding wall left x: %d y: %d\n", mouseX, mouseY);
    switch (heading) {
        case NORTH:
            addWall(mouseX, mouseY, WEST);
            break;
        case SOUTH:
            addWall(mouseX, mouseY, EAST);
            break;
        case EAST:
            addWall(mouseX, mouseY, NORTH);
            break;
        case WEST:
            addWall(mouseX, mouseY, SOUTH);
            break;
        default:
            break;
    }
}

void Floodfill::removeWallFront() {
    //pc.printf("removing wall front (%d, %d) ", mouseX, mouseY);
    switch (heading) {
        case NORTH:
            //pc.printf("north\n");
            removeWall(mouseX, mouseY, NORTH);
            break;
        case SOUTH:
            removeWall(mouseX, mouseY, SOUTH);
            break;
        case EAST:
            removeWall(mouseX, mouseY, EAST);
            break;
        case WEST:
            removeWall(mouseX, mouseY, WEST);
            break;
        default:
            break;
    }
}


void Floodfill::removeWallRight() {
    switch (heading) {
        case NORTH:
            removeWall(mouseX, mouseY, EAST);
            break;
        case SOUTH:
            removeWall(mouseX, mouseY, WEST);
            break;
        case EAST:
            removeWall(mouseX, mouseY, SOUTH);
            break;
        case WEST:
            removeWall(mouseX, mouseY, NORTH);
            break;
        default:
            break;
    }
}

void Floodfill::removeWallLeft() {
    switch (heading) {
        case NORTH:
            removeWall(mouseX, mouseY, WEST);
            break;
        case SOUTH:
            removeWall(mouseX, mouseY, EAST);
            break;
        case EAST:
            removeWall(mouseX, mouseY, NORTH);
            break;
        case WEST:
            removeWall(mouseX, mouseY, SOUTH);
            break;
        default:
            break;
    }
}


void Floodfill::addWall(int x, int y, int side) {
    uint16_t *current = &maze_map[x][y];
    uint16_t *currentBack = &maze_map_back[x][y];
    uint16_t *neighbor;
    uint16_t *neighborBack;
    
    bool print_vals = false;
    
    if (print_vals) pc.printf("adding wall (%d, %d) ", x, y);

    switch (side) {
        case NORTH:
            if (print_vals) pc.printf("north\n");
            *current |= NORTH_WALL;
            *currentBack |= NORTH_WALL;
            if (y < 15) {
                neighbor = &maze_map[x][y+1];
                neighborBack = &maze_map_back[x][y+1];
                *neighbor |= SOUTH_WALL;
                *neighborBack |= SOUTH_WALL;
            }
            break;
        case SOUTH:
            if (print_vals) pc.printf("south\n");
            *current |= SOUTH_WALL;
            *currentBack |= SOUTH_WALL;
            if (y > 0) {
                neighbor = &maze_map[x][y-1];
                neighborBack = &maze_map_back[x][y-1];
                *neighbor |= NORTH_WALL;
                *neighborBack |= NORTH_WALL;
            }
            break;
        case EAST:
            if (print_vals) pc.printf("east\n");
            *current |= EAST_WALL;
            *currentBack |= EAST_WALL;
            if (x < 15) {
                neighbor = &maze_map[x+1][y];
                neighborBack = &maze_map_back[x+1][y];
                *neighbor |= WEST_WALL;
                *neighborBack |= WEST_WALL;
            }
            break;
        case WEST:
            if (print_vals) pc.printf("west\n");
            *current |= WEST_WALL;
            *currentBack |= WEST_WALL;
            if (x > 0) {
                neighbor = &maze_map[x-1][y];
                neighborBack = &maze_map_back[x-1][y];
                *neighbor |= EAST_WALL;
                *neighborBack |= EAST_WALL;
            }
            break;
        case NVALID:
        default:
            return;
    }
    return;
}

void Floodfill::removeWall(int x, int y, int side) {
    uint16_t *current = &maze_map[x][y];
    uint16_t *currentBack = &maze_map_back[x][y];
    uint16_t *neighbor;
    uint16_t *neighborBack;
    
    switch (side) {
        case NORTH:
            *current &= ~NORTH_WALL;
            *currentBack &= ~NORTH_WALL;
            if (y < 15) {
                neighbor = &maze_map[x][y+1];
                neighborBack = &maze_map_back[x][y+1];
                *neighbor &= ~SOUTH_WALL;
                *neighborBack &= ~SOUTH_WALL;
            }
            break;
        case SOUTH:
            *current &= ~SOUTH_WALL;
            *currentBack &= ~SOUTH_WALL;
            if (y > 0) {
                neighbor = &maze_map[x][y-1];
                neighborBack = &maze_map_back[x][y-1];
                *neighbor &= ~NORTH_WALL;
                *neighborBack &= ~NORTH_WALL;
                
            }
            break;
        case EAST:
            *current &= ~EAST_WALL;
            *currentBack &= ~EAST_WALL;
            if (x < 15) {
                neighbor = &maze_map[x+1][y];
                neighborBack = &maze_map_back[x+1][y];
                *neighbor &= ~WEST_WALL;
                *neighborBack &= ~WEST_WALL;
            }
            break;
        case WEST:
            *current &= ~WEST_WALL;
            *currentBack &= ~WEST_WALL;
            if (x > 0) {
                neighbor = &maze_map[x-1][y];
                neighborBack = &maze_map_back[x-1][y];
                *neighbor &= ~EAST_WALL;
                *neighborBack &= ~EAST_WALL;
            }
            break;
        case NVALID:
        default:
            return;
    }
    return;
}


bool Floodfill::isAtCenter(unsigned x, unsigned y) const {
    unsigned midpoint = CENTER;

    return  (x == midpoint     && y == midpoint    ) ||
    (x == midpoint - 1 && y == midpoint    ) ||
    (x == midpoint     && y == midpoint - 1) ||
    (x == midpoint - 1 && y == midpoint - 1);
}


void Floodfill::moveForward() {
    switch(heading) {
        case NORTH:
            if (mouseY < 15)
                mouseY++;
            break;
        case SOUTH:
            if (mouseY > 0)
                mouseY--;
            break;
        case EAST:
            if (mouseX < 15)
                mouseX++;
            break;
        case WEST:
            if (mouseX > 0)
                mouseX--;
            break;
        case NVALID:
        default:
            break;
    }
}


Floodfill::MouseMovement Floodfill::nextCellMovement(int nextCell) {
    shouldGoForward = true;
    switch (heading) {
        case NORTH:
            switch (nextCell) {
                case NORTH:
                    shouldGoForward = false;
                    return MoveForward;
                case SOUTH:
                    return TurnAround;
                case EAST:
                    return TurnClockwise;
                case WEST:
                    return TurnCounterClockwise;
            }
        case SOUTH:
            switch (nextCell) {
                case NORTH:
                    return TurnAround;
                case SOUTH:
                    shouldGoForward = false;
                    return MoveForward;
                case EAST:
                    return TurnCounterClockwise;
                case WEST:
                    return TurnClockwise;
            }
        case EAST:
            switch (nextCell) {
                case NORTH:
                    return TurnCounterClockwise;
                case SOUTH:
                    return TurnClockwise;
                case EAST:
                    shouldGoForward = false;
                    return MoveForward;
                case WEST:
                    return TurnAround;
            }
        case WEST:
            switch (nextCell) {
                case NORTH:
                    return TurnClockwise;
                case SOUTH:
                    return TurnCounterClockwise;
                case EAST:
                    return TurnAround;
                case WEST:
                    shouldGoForward = false;
                    return MoveForward;
            }
        case NVALID:
        default:
            return Wait;
    }
}
