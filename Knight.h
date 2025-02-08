// Knight.h
#pragma once
#include "Piece.h"
#include "BoardStructure.h"

class Knight : public Piece {
public:
    Knight(const PieceType piece_type_param,
           const PlayerSide player_side_param, Coord coord_param)
        : Piece(piece_type_param, player_side_param, coord_param) {};

    virtual ~Knight() {};

    int knight_x_possibilities[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    int knight_y_possibilities[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

    virtual void add_possible_moves(BoardStructure& board,
                                    short int player_move_multiplier, bool check_for_pin, BoardCell& board_cell);
};
