//Rook.h
#pragma once
#include "Piece.h"
#include "BoardStructure.h"

class Rook : public Piece {
public:
    Rook(const PieceType piece_type_param,
         const PlayerSide player_side_param, Coord coord_param)
        : Piece(piece_type_param, player_side_param, coord_param) {};
    virtual ~Rook() {};

    virtual void add_possible_moves(BoardStructure& board,
                                    short int player_move_multiplier, bool check_for_pin, BoardCell& board_cell);
};
