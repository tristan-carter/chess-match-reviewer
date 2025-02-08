// Coord.h
#pragma once
#include <ostream>

class Coord
{
public:
    short int x;
    short int y;

    bool operator==(const Coord& otherCoord) const {
        return x == otherCoord.x && y == otherCoord.y;
    }

    friend std::ostream& operator<<(std::ostream& os, const Coord& coord) {
        os << "(" << coord.x << ", " << coord.y << ")";
        return os;
    }
};
