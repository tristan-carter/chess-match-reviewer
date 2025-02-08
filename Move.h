// Move.h
#pragma once
#include "Coord.h"
#include "Piece.h"

class Move
{
public:
    Coord from;
    Coord to;
    Coord piece_taken;
    PieceType promotion_to;
    bool is_castle;
    bool causes_check;

    bool operator==(const Move& rhs) const {
        return from == rhs.from && to == rhs.to;
    }
};
