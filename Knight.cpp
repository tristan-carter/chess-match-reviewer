// Knight.cpp
#include "Knight.h"

void Knight::add_possible_moves(BoardStructure& board,
                                short int player_move_multiplier, bool check_for_pin, BoardCell& board_cell)
{
    Coord piece_position = this->coord;
    Coord possible_move_to{};

    // iterates through possible L shape moves around the knight
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        possible_move_to.x = piece_position.x + this->knight_x_possibilities[i];
        possible_move_to.y = piece_position.y + this->knight_y_possibilities[i];

        if (this->is_coord_in_board(possible_move_to))
        {
            BoardCell& piece_to_capture_cell = board.board[possible_move_to.x][possible_move_to.y];
            if (piece_to_capture_cell.piece == nullptr
                || (piece_to_capture_cell.piece != nullptr
                    && piece_to_capture_cell.piece->player_side != board_cell.piece->player_side))
            {
                if (piece_to_capture_cell.piece != nullptr)
                {
                    board.add_possible_move(possible_move_to,
                                            piece_position, piece_to_capture_cell.coord, check_for_pin);
                }
                else
                {
                    board.add_possible_move(possible_move_to,
                                            piece_position, Coord{ -1, 0 }, check_for_pin);
                }
            }
        }
    }
}
