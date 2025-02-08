// Bishop.cpp
#include "Bishop.h"

void Bishop::add_possible_moves(BoardStructure& board,
                                short int player_move_multiplier, bool check_for_pin, BoardCell& board_cell)
{
    this->find_diagonal_moves(board, board_cell.coord, player_move_multiplier, false, check_for_pin);
}
