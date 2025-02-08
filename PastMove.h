// PastMove.h
#pragma once
#include "Coord.h"
#include "Piece.h"

class Piece;

class PastMove
{
public:
    Coord from;
    Coord to;
    PlayerSide player_side;
    Piece* piece_taken;

    PieceType promoted_from;
    bool was_castle;
};
