// Piece.cpp
#include "iostream"
#include "Piece.h"
#include "BoardStructure.h"
#include "BoardCell.h"

Piece::Piece(PieceType piece_type, PlayerSide player_side, Coord coord)
    : piece_type(piece_type), player_side(player_side), coord(coord) {
    //std::cout << "Piece constructed" << std::endl;
}

Piece::~Piece() {
    //std::cout << "Piece deconstructed" << std::endl;
}

void Piece::find_straight_moves(BoardStructure& board, Coord piece_position,
                                short int player_move_multiplier, bool is_king, bool check_for_pin) {
    for (int direction = 1; direction < 5; direction++) {
        for (int move_by = 1; move_by < BOARD_SIZE; move_by++)
        {
            bool is_direction_cutoff = false;
            Coord move_to{};
            if (direction == 1) {
                // up
                move_to.x = piece_position.x;
                move_to.y = piece_position.y + move_by;
            }
            else if (direction == 2) {
                // down
                move_to.x = piece_position.x;
                move_to.y = piece_position.y - move_by;
            }
            else if (direction == 3) {
                // left
                move_to.x = piece_position.x - move_by;
                move_to.y = piece_position.y;
            }
            else if (direction == 4) {
                // right
                move_to.x = piece_position.x + move_by;
                move_to.y = piece_position.y;
            }

            if (this->is_coord_in_board(move_to)
                && (is_king == false || move_by == 1) /*(King's can only move one square)*/
                && is_direction_cutoff == false) {

                BoardCell& move_to_cell{ board.board[move_to.x][move_to.y] };

                if (move_to_cell.piece == nullptr || (move_to_cell.piece != nullptr
                    && move_to_cell.piece->player_side != board.board[piece_position.x][piece_position.y].piece->player_side)) {
                    
                    if (move_to_cell.piece != nullptr) {
                        board.add_possible_move(move_to, piece_position, move_to, check_for_pin);
                        is_direction_cutoff = true;
                        break;
                    }
                    else {
                        board.add_possible_move(move_to, piece_position, Coord{ -1, 0 }, check_for_pin);
                    }
                }
                else { is_direction_cutoff = true; break; }
            }
            else { is_direction_cutoff = true; break; }
        }
    }
}

void Piece::find_diagonal_moves(BoardStructure& board, Coord piece_position,
                                short int player_move_multiplier, bool is_king, bool check_for_pin) {
    for (int direction = 1; direction < 5; direction++) {
        for (int move_by = 1; move_by < BOARD_SIZE; move_by++)
        {
            Coord move_to{};
            if (direction == 1) {
                // top right diagonal
                move_to.x = piece_position.x + move_by;
                move_to.y = piece_position.y + move_by;
            }
            else if (direction == 2) {
                // top left diagonal
                move_to.x = piece_position.x - move_by;
                move_to.y = piece_position.y + move_by;
            }
            else if (direction == 3) {
                // bottom right diagonal
                move_to.x = piece_position.x + move_by;
                move_to.y = piece_position.y - move_by;
            }
            else if (direction == 4) {
                // bottom left diagonal
                move_to.x = piece_position.x - move_by;
                move_to.y = piece_position.y - move_by;
            }

            if (this->is_coord_in_board(move_to)
                && (!is_king || move_by == 1) /*(King's can only move one square)*/) {

                BoardCell& move_to_cell{ board.board[move_to.x][move_to.y] };

                if (move_to_cell.piece == nullptr || (move_to_cell.piece != nullptr
                    && move_to_cell.piece->player_side != board.board[piece_position.x][piece_position.y].piece->player_side)) {

                    if (move_to_cell.piece != nullptr) {
                        board.add_possible_move(move_to, piece_position, move_to, check_for_pin);
                        break;
                    }
                    else {
                        board.add_possible_move(move_to, piece_position, Coord{ -1, 0 }, check_for_pin);
                    }
                }
                else { break; }
            }
            else { break; }
        }
    }
}

bool Piece::is_coord_in_board(Coord coord)
{
    return (coord.x > -1
            && coord.x < BOARD_SIZE
            && coord.y > -1
            && coord.y < BOARD_SIZE);
}
