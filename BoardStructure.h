// BoardStructure.h
#pragma once
#include <vector>

#include "Piece.h"
#include "BoardCell.h"
#include "PastMove.h"
#include "Move.h"

const short int BOARD_SIZE = 8;

enum GameState {
    WHITE_WIN,
    BLACK_WIN,
    DRAW,
    IN_PLAY
};

class BoardStructure
{
public:
    BoardStructure();
    ~BoardStructure();

    struct IsMoveLegalResult {
        bool is_legal;
        bool causes_check;
    };

    unsigned short int move_number{ 0 };
    GameState game_state{ IN_PLAY };
    BoardCell board[8][8]{};
    std::vector<Move> possible_moves;
    std::vector<PastMove> past_moves;

    void print_board();
    bool push_move(Move& player_move, bool check_for_pin = true);
    void pop_move(bool pop_past_possible_moves = true);
    void add_possible_move(Coord move_to, Coord piece_position,
                           Coord piece_taken_position, bool check_for_pin = false,
                           bool is_pawn = false, bool is_castle = false); // has been added after end of sprint 1 testing
    IsMoveLegalResult is_move_legal(Move& possible_move);
    bool in_check();

private:
    std::vector<Move> pin_moves;
    std::vector<std::vector<Move>> past_possible_moves{};

    void find_moves(bool check_for_pin = true);
};
