// Pawn.cpp
#include "Pawn.h"

void Pawn::add_possible_moves(BoardStructure& board,
                              short int player_move_multiplier, bool check_for_pin, BoardCell& board_cell) {
    Coord piece_position = this->coord;
    Coord possible_move_to{};

    // move forward one place
    possible_move_to.x = piece_position.x;
    possible_move_to.y = piece_position.y + player_move_multiplier;
    if (this->is_coord_in_board(possible_move_to))
    {
        if (board.board[piece_position.x][piece_position.y + player_move_multiplier].piece == nullptr)
        {
            board.add_possible_move(possible_move_to,
                                    piece_position, Coord{ -1, 0 }, check_for_pin, true);
        }
    }

    // move forawrd two places if on starting square
    possible_move_to.x = piece_position.x;
    possible_move_to.y = piece_position.y + (2 * player_move_multiplier);
    if (this->is_coord_in_board(possible_move_to))
    {
        if (board.board[piece_position.x][piece_position.y + (player_move_multiplier * 2)].piece == nullptr
            && piece_position.y == board_cell.piece->player_side * 5 + 1
            && board.board[piece_position.x][piece_position.y + (player_move_multiplier)].piece == nullptr)
        {
            board.add_possible_move(possible_move_to,
                                    piece_position, Coord{ -1, 0 }, check_for_pin, true);
        }
    }

    // take piece to the right
    possible_move_to.x = piece_position.x + 1;
    possible_move_to.y = piece_position.y + player_move_multiplier;
    if (this->is_coord_in_board(possible_move_to))
    {
        BoardCell& piece_to_capture_cell = board.board[possible_move_to.x][possible_move_to.y];
        if (piece_to_capture_cell.piece != nullptr
            && piece_to_capture_cell.piece->player_side != board_cell.piece->player_side)
        {
            board.add_possible_move(possible_move_to,
                                    piece_position, possible_move_to, check_for_pin, true);
        }
    }

    // take piece to the left
    possible_move_to.x = piece_position.x - 1;
    possible_move_to.y = piece_position.y + player_move_multiplier;
    if (this->is_coord_in_board(possible_move_to))
    {
        BoardCell& piece_to_capture_cell = board.board[possible_move_to.x][possible_move_to.y];
        if (piece_to_capture_cell.piece != nullptr
            && piece_to_capture_cell.piece->player_side != board_cell.piece->player_side)
        {
            board.add_possible_move(possible_move_to,
                                    piece_position, possible_move_to, check_for_pin, true);
        }
    }

    // en passant
    if (board.move_number > 0)
    {
        PastMove last_move = board.past_moves.back();

        // en passant to the left
        possible_move_to.x = piece_position.x - 1;
        possible_move_to.y = piece_position.y + player_move_multiplier;
        if (this->is_coord_in_board(possible_move_to))
        {
            BoardCell& piece_to_capture_cell = board.board[piece_position.x - 1][piece_position.y];
            if (piece_to_capture_cell.piece != nullptr
                && piece_to_capture_cell.piece->player_side != board_cell.piece->player_side
                && piece_to_capture_cell.piece->piece_type == PAWN
                && last_move.to == piece_to_capture_cell.coord
                && last_move.from.y - (player_move_multiplier * 2) == piece_to_capture_cell.coord.y)
            {
                board.add_possible_move(possible_move_to, piece_position, piece_to_capture_cell.coord,
                    check_for_pin, true, false);
            }
        }

        // en passant to the right
        possible_move_to.x = piece_position.x + 1;
        possible_move_to.y = piece_position.y + player_move_multiplier;
        if (this->is_coord_in_board(possible_move_to))
        {
            BoardCell& piece_to_capture_cell = board.board[piece_position.x + 1][piece_position.y];
            if (piece_to_capture_cell.piece != nullptr
                && piece_to_capture_cell.piece->player_side != board_cell.piece->player_side
                && piece_to_capture_cell.piece->piece_type == PAWN
                && last_move.to == piece_to_capture_cell.coord
                && last_move.from.y - (player_move_multiplier * 2) == piece_to_capture_cell.coord.y)
            {
                board.add_possible_move(possible_move_to, piece_position, piece_to_capture_cell.coord,
                    check_for_pin, true, false);
            }
        }
    }
}
