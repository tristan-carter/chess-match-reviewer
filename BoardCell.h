// BoardCell.h
#pragma once
#include "Piece.h"

class BoardCell
{
public:
    Coord coord;
    BoardCell(const BoardCell&) = delete;
    Piece* piece;
};
