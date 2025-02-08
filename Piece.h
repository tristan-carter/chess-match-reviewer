// Piece.h
#pragma once
#include "Coord.h"

enum PieceType {
    NOPIECE,
    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING
};

enum PlayerSide {
    WHITE,
    BLACK
};

class BoardStructure;
class BoardCell;

class Piece
{
public:
    Piece(PieceType piece_type, PlayerSide player_side, Coord coord);
    virtual ~Piece();

    virtual void add_possible_moves(BoardStructure& board, short int player_move_multiplier,
                                    bool check_for_pin, BoardCell& board_cell) {};

    Coord coord{};
    PlayerSide player_side{};
    PieceType piece_type{};
protected:
    bool is_coord_in_board(Coord coord);

    void find_straight_moves(BoardStructure& board, Coord piece_position,
                             short int player_move_multiplier, bool is_king = false, bool check_for_pin = true);

    void find_diagonal_moves(BoardStructure& board, Coord piece_position,
                             short int player_move_multiplier, bool is_king = false, bool check_for_pin = true);
};
