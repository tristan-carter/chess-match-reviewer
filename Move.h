// Move.h
#pragma once
#include "Coord.h"
#include "Piece.h"

class Move
{
public:
    Coord from;
    Coord to;
    Coord piece_taken{-1, 0}; // by default no piece is being taken so starts equal to (-1, 0)
    PieceType promotion_to;
    bool is_castle;
    bool causes_check;

    bool operator==(const Move& rhs) const {
        return from == rhs.from && to == rhs.to
            && promotion_to == rhs.promotion_to;
    }
};
