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
    // Step 1 - iterates through each possible direction the piece could
    // move (up, down, left, right)
    for (int direction = 1; direction < 5; direction++) {
        // Step 2 - iterates from 1 to the furthest the piece could move
        // in one straight direction (one less than the board size), if
        // an obstacle is reached then this for loop will be broken out
        // of so you cannot jump over other pieces or go off the end off
        // the end of the board
        for (int move_by = 1; move_by < BOARD_SIZE; move_by++)
        {
            // Step 3 - assigns the move_to coordinate to be investigated
            // a value for each iteration depending on the direction and
            // the value of move_by
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

            // Step 4 - validates that the coordinate to move to is a valid coordinate on the board
            // and that if the piece moving is a king that the piece is only moving by one square
            if (this->is_coord_in_board(move_to)
                && (!is_king || move_by == 1) /*(King's can only move one square)*/) {

                // Step 5 - initiates a variable to keep track of the cell which the piece
                // is moving to so that code is repeated less. Then checks that if there
                // is a piece on the cell to move to that it belongs to the opposition
                BoardCell& move_to_cell{ board.board[move_to.x][move_to.y] };

                if (move_to_cell.piece == nullptr || (move_to_cell.piece != nullptr
                                                      && move_to_cell.piece->player_side !=
                                                             board.board[piece_position.x][piece_position.y].piece->player_side)) {

                    // Step 6 - checks if the board cell to move to has a piece on it, we already
                    // know this piece would have to be on the opposition's side due to the checks
                    // completed in step 5 so don't need to check for which side the piece is on.
                    // If there is a piece on the cell to move to then a possible move is added
                    // with the coordinate the piece is moving to being passed as the  piece_taken
                    // coordinate parameter to the add_possible_move method. If there is no piece
                    // on the board cell to move to (so no piece will be taken) then the
                    // add_possible_move method is called with the same parameters except a
                    // coordinate of (-1, 0) is passed as the piece_taken coordinate parameter,
                    // this coordinate is not on the board and represents that no piece is to be taken
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

void Piece::find_diagonal_moves(BoardStructure& board, Coord piece_position,
                                short int player_move_multiplier, bool is_king, bool check_for_pin) {
    // Step 1 - iterates through each possible direction the piece could
    // move (top-right, top-left, bottom-right, bottom-left)
    for (int direction = 1; direction < 5; direction++) {
        // Step 2 - iterates from 1 to the furthest the piece could move
        // in one straight direction (one less than the board size), if
        // an obstacle is reached then this for loop will be broken out
        // of so you cannot jump over other pieces or go off the end off
        // the end of the board
        for (int move_by = 1; move_by < BOARD_SIZE; move_by++)
        {
            // Step 3 - assigns the move_to coordinate to be investigated
            // a value for each iteration depending on the direction and
            // the value of move_by
            Coord move_to{};
            if (direction == 1) {
                // top right
                move_to.x = piece_position.x + move_by;
                move_to.y = piece_position.y + move_by;
            }
            else if (direction == 2) {
                // top left
                move_to.x = piece_position.x - move_by;
                move_to.y = piece_position.y + move_by;
            }
            else if (direction == 3) {
                // bottom right
                move_to.x = piece_position.x + move_by;
                move_to.y = piece_position.y - move_by;
            }
            else if (direction == 4) {
                // bottom left
                move_to.x = piece_position.x - move_by;
                move_to.y = piece_position.y - move_by;
            }

            // Step 4 - validates that the coordinate to move to is a valid coordinate on the board
            // and that if the piece moving is a king that the piece is only moving by one square
            if (this->is_coord_in_board(move_to)
                && (!is_king || move_by == 1) /*(King's can only move one square)*/) {

                // Step 5 - initiates a variable to keep track of the cell which the piece
                // is moving to so that code is repeated less. Then checks that if there
                // is a piece on the cell to move to that it belongs to the opposition
                BoardCell& move_to_cell{ board.board[move_to.x][move_to.y] };

                if (move_to_cell.piece == nullptr || (move_to_cell.piece != nullptr
                                                      && move_to_cell.piece->player_side !=
                                                             board.board[piece_position.x][piece_position.y].piece->player_side)) {

                    // Step 6 - checks if the board cell to move to has a piece on it, we already
                    // know this piece would have to be on the opposition's side due to the checks
                    // completed in step 5 so don't need to check for which side the piece is on.
                    // If there is a piece on the cell to move to then a possible move is added
                    // with the coordinate the piece is moving to being passed as the  piece_taken
                    // coordinate parameter to the add_possible_move method. If there is no piece
                    // on the board cell to move to (so no piece will be taken) then the
                    // add_possible_move method is called with the same parameters except a
                    // coordinate of (-1, 0) is passed as the piece_taken coordinate parameter,
                    // this coordinate is not on the board and represents that no piece is to be taken
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
    // Validates that a coordinate is within the board
    return (coord.x > -1
            && coord.x < BOARD_SIZE
            && coord.y > -1
            && coord.y < BOARD_SIZE);
}
