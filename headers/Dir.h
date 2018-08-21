#ifndef Dir_h
#define Dir_h

enum Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST,
    NVALID   
};

inline Direction opposite(Direction d) {
    switch (d) {
        case NORTH:
            return SOUTH;
        case SOUTH:
            return NORTH;
        case EAST:
            return WEST;
        case WEST:
            return EAST;
        case NVALID:
        default:
            return NVALID;
    }
}

inline Direction clockwise(Direction d) {
    switch (d) {
        case NORTH:
            return EAST;
        case SOUTH:
            return WEST;
        case EAST:
            return SOUTH;
        case WEST:
            return NORTH;
        case NVALID:
        default:
            return NVALID;
    }
}

inline Direction counterClockwise(Direction d) {
    switch (d) {
        case NORTH:
            return WEST;
        case SOUTH:
            return EAST;
        case EAST:
            return NORTH;
        case WEST:
            return SOUTH;
        case NVALID:
        default:
            return NVALID;
    }
}

#endif
