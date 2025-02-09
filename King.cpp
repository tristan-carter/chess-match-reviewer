// King.cpp
#include "King.h"

// has been updated, new screenshots needed
void King::add_possible_moves(BoardStructure& board,
                              short int player_move_multiplier, bool check_for_pin, BoardCell& board_cell)
{
    Coord piece_position = this->coord;
    Coord possible_move_to{};

    this->find_straight_moves(board, board_cell.coord, player_move_multiplier, true, check_for_pin);
    this->find_diagonal_moves(board, board_cell.coord, player_move_multiplier, true, check_for_pin);

    ++board.move_number;
    if (check_for_pin && !board.in_check())
    {
        --board.move_number;
        // for castling
        bool has_king_moved{ false };
        for (PastMove& past_move : board.past_moves)
        {
            if (past_move.from == board.board[4][7 * board_cell.piece->player_side].coord)
            {
                has_king_moved = true;
            }
        }

        if (!has_king_moved)
        {
            possible_move_to.y = piece_position.y;
            // checks whether the rooks have moved
            bool right_rook_moved{ false };
            bool left_rook_moved{ false };
            for (PastMove& past_move : board.past_moves)
            {
                if (past_move.from == board.board[7][7 * board_cell.piece->player_side].coord)
                {
                    right_rook_moved = true;
                }
                if (past_move.from == board.board[0][7 * board_cell.piece->player_side].coord)
                {
                    left_rook_moved = true;
                }
            }

            // short castle (castle to right from white player's perspective)
            if (!right_rook_moved)
            {
                bool can_castle = true;

                BoardCell* check_cell = &board.board[piece_position.x + 1][piece_position.y];
                if (check_cell->piece == nullptr)
                {
                    possible_move_to.x = piece_position.x + 1;
                    Move possible_move{ piece_position, possible_move_to };
                    if (!board.is_move_legal(possible_move).is_legal)
                    {
                        can_castle = false;
                    }
                }
                else { can_castle = false; }

                check_cell = &board.board[piece_position.x + 2][piece_position.y];
                if (check_cell->piece == nullptr && can_castle)
                {
                    possible_move_to.x = piece_position.x + 2;
                    Move possible_move{ piece_position, possible_move_to };
                    if (!board.is_move_legal(possible_move).is_legal)
                    {
                        can_castle = false;
                    }
                }
                else { can_castle = false; }

                if (can_castle)
                {
                    possible_move_to.x = piece_position.x + 2;
                    possible_move_to.y = piece_position.y;
                    board.add_possible_move(possible_move_to,
                                            piece_position, Coord{ -1, 0 }, check_for_pin, false, true);
                }
            }

            // long castle (castle to left from white player's perspective)
            if (!left_rook_moved)
            {
                bool can_castle = true;

                BoardCell* check_cell = &board.board[piece_position.x - 1][piece_position.y];
                if (check_cell->piece == nullptr)
                {
                    possible_move_to.x = piece_position.x - 1;
                    Move possible_move{ piece_position, possible_move_to };
                    if (!board.is_move_legal(possible_move).is_legal)
                    {
                        can_castle = false;
                    }
                }
                else { can_castle = false; }

                check_cell = &board.board[piece_position.x - 2][piece_position.y];
                if (check_cell->piece == nullptr && can_castle)
                {
                    possible_move_to.x = piece_position.x - 2;
                    Move possible_move{ piece_position, possible_move_to };
                    if (!board.is_move_legal(possible_move).is_legal)
                    {
                        can_castle = false;
                    }
                }
                else { can_castle = false; }

                check_cell = &board.board[1][piece_position.y];
                if (check_cell->piece != nullptr && can_castle) { can_castle = false; }

                if (can_castle)
                {
                    possible_move_to.x = piece_position.x - 2;
                    possible_move_to.y = piece_position.y;
                    board.add_possible_move(possible_move_to,
                        piece_position, Coord{ -1, 0 }, check_for_pin, false, true);
                }
            }

        }
    }
    else { --board.move_number; }
}
