// BoardStructure.cpp
#include <iostream>
using namespace std;

#include "BoardStructure.h"

#include "Pawn.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"


void BoardStructure::print_board() {
    cout << "========================" << endl << endl;
    for (short int y = BOARD_SIZE-1; y > -1; --y) {
        for (short int x = 0; x < BOARD_SIZE; x++) {
            const BoardCell& board_cell = this->board[x][y];
            const Piece* board_piece = board_cell.piece;
            if (board_piece == nullptr) {
                cout << " - ";
            }
            else {
                switch (board_piece->piece_type)
                {
                case PAWN:
                    if (board_piece->player_side == WHITE) { cout << " P "; }
                    else { cout << " p "; };
                    break;
                case ROOK:
                    if (board_piece->player_side == WHITE) { cout << " R "; }
                    else { cout << " r "; };
                    break;
                case KNIGHT:
                    if (board_piece->player_side == WHITE) { cout << " N "; }
                    else { cout << " n "; };
                    break;
                case BISHOP:
                    if (board_piece->player_side == WHITE) { cout << " B "; }
                    else { cout << " b "; };
                    break;
                case QUEEN:
                    if (board_piece->player_side == WHITE) { cout << " Q "; }
                    else { cout << " q "; };
                    break;
                case KING:
                    if (board_piece->player_side == WHITE) { cout << " K "; }
                    else { cout << " k "; };
                    break;
                }

            }
        }
        cout << endl;
    }
}

BoardStructure::BoardStructure()
{
    // Step 1 - assigns coordinates to each board cell
    for (short int y = 0; y < BOARD_SIZE; ++y) {
        for (short int x = 0; x < BOARD_SIZE; x++) {
            board[x][y].coord = Coord{ x, y };
        }
    }

    // Step 2 - creates the pieces on the board
    for (short int i = 0; i < 2; i++) {
        PlayerSide player_side{};
        if (i == 0) {
            player_side = WHITE;
        }
        else {
            player_side = BLACK;
        }

        short int y_coord = i * 5 + 1 ;
        for (short int x = 0; x < BOARD_SIZE; x++) {
            board[x][y_coord].piece = new Pawn{ PAWN, player_side, Coord{ x, y_coord } };
        }

        y_coord = i * 7;
        board[0][y_coord].piece = new Rook{ ROOK, player_side, Coord{ 0, y_coord } };

        board[1][y_coord].piece = new Knight{ KNIGHT, player_side, Coord{ 1, y_coord } };

        board[2][y_coord].piece = new Bishop{BISHOP, player_side, Coord{ 2, y_coord } };

        board[3][y_coord].piece = new Queen{QUEEN, player_side, Coord{ 3, y_coord } };

        board[4][y_coord].piece = new King{ KING, player_side, Coord{ 4, y_coord } };

        board[5][y_coord].piece = new Bishop{ BISHOP, player_side, Coord{ 5, y_coord } };

        board[6][y_coord].piece = new Knight{ KNIGHT, player_side, Coord{ 6, y_coord } };

        board[7][y_coord].piece = new Rook{ ROOK, player_side, Coord{ 7, y_coord } };
    }

    // Step 3 - finds legal moves available in the starting board position
    this->find_moves();
    std::cout << "Finished Constructing" << std::endl;
}

BoardStructure::~BoardStructure()
{
    /*std::cout << "BoardStructure deconstructing" << std::endl;
    // Step 1 - deletes each piece on the board from memory
    for (short int y = 7; y > -1; --y) {
        for (short int x = 0; x < BOARD_SIZE; x++) {
            const Piece* board_piece = this->board[x][y].piece;
            delete board_piece;
            board_piece = nullptr;
        }
    }

    // Step 2 delete previously taken pieces from past moves
    for (PastMove& past_move : this->past_moves) {
        if (past_move.piece_taken != nullptr) {
            const Piece* board_piece = past_move.piece_taken;
            delete board_piece;
            board_piece = nullptr;
        }
    }*/

    //std::cout << "BoardStructure deconstructed" << std::endl;
}

bool BoardStructure::in_check()
{
    // Step 1 - finds the opposition to the current player's king on the board by
    // iterating through each cell on the board, checking whether any pieces found
    // are a king and are on the opponent's side (this is done by taking the modulus
    // of one more than the current move number, this is because the modulus of the
    // current move number tells whether it is white or black as white's turns will
    // have a move_number modulus of 0 and black will have 1, since we want to check
    // if the piece belongs to the opponent we add one to the move number when
    // comparing to the player_side). If no move which can take the opposition's
    // king is found, then the method will return false at the end of the method
    for (int y = 0; y < BOARD_SIZE; ++y)
    {
        for (int x = 0; x < BOARD_SIZE; ++x)
        {
            BoardCell& board_cell{ board[x][y] };
            if (board_cell.piece != nullptr && board_cell.piece->piece_type == KING
                && board_cell.piece->player_side == (move_number + 1) % 2)
            {
                Coord king_position = board_cell.coord;

                // Step 2 - checks if any of the enemy pieces can take the king on
                // the following move and returns true if any can
                for (Move check_move_pin : pin_moves)
                {
                    if (check_move_pin.to == king_position)
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool BoardStructure::push_move(Move& player_move, bool check_for_pin)
{
    // Step 1 - validates that the match has not finished yet
    // and returns that the move was not made if the match has finished
    if (game_state != IN_PLAY)
    {
        return false;
    }

    // Step 2 - checks if either the move to be made is an available legal move
    // in the current board position or the move is being pushed to check for a pin
    bool is_legal{ false };
    for (Move& legal_move : possible_moves)
    {
        if (legal_move == player_move)
        {
            is_legal = true;
            legal_move.promotion_to = player_move.promotion_to;
            player_move = legal_move;

        }
    }
    if (is_legal == false && check_for_pin)
    {
        return false;
    }

    // Step 3 - creates the past move object of the move so that the move
    // can be stored in the past moves vector
    Piece* piece_from = board[player_move.from.x][player_move.from.y].piece;
    PastMove past_move{ player_move.from, player_move.to,
                       piece_from->player_side, nullptr, piece_from->piece_type, player_move.is_castle };

    // Step 4 - removes the piece taken from the board (purpose of this step is for the
    // en passant move as the position of the piece taken and position the piece moves
    // to are different)

    if (player_move.piece_taken.x != -1) // -1 as the x coordinate of a piece is obviously
    // not allowed so this is how I signal whether a piece is taken in a move
    {
        past_move.piece_taken = board[player_move.piece_taken.x][player_move.piece_taken.y].piece;
        board[player_move.piece_taken.x][player_move.piece_taken.y].piece = nullptr;
    }

    // Step 5 - removes the piece moving from its old board cell to the move's destination board
    // cell if the piece is not to be promotoed, if it does need to be promoted as well then a
    // new piece with the promotion is constructed to go in the destination board cell, with
    // the old piece being deleted
    switch (player_move.promotion_to)
    {
    case PAWN:
        board[player_move.to.x][player_move.to.y].piece = new Pawn{ player_move.promotion_to,
                                                                   piece_from->player_side, player_move.to };
        break;
    case BISHOP:
        board[player_move.to.x][player_move.to.y].piece = new Bishop{ player_move.promotion_to,
                                                                     piece_from->player_side, player_move.to };
        break;
    case ROOK:
        board[player_move.to.x][player_move.to.y].piece = new Rook{ player_move.promotion_to,
                                                                   piece_from->player_side, player_move.to };
        break;
    case KNIGHT:
        board[player_move.to.x][player_move.to.y].piece = new Knight{ player_move.promotion_to,
                                                                     piece_from->player_side, player_move.to };
        break;
    case KING:
        board[player_move.to.x][player_move.to.y].piece = new King{ player_move.promotion_to,
                                                                   piece_from->player_side, player_move.to };
        break;
    case QUEEN:
        board[player_move.to.x][player_move.to.y].piece = new Queen{ player_move.promotion_to,
                                                                    piece_from->player_side, player_move.to };
        break;
    default:
        board[player_move.to.x][player_move.to.y].piece = board[player_move.from.x][player_move.from.y].piece;
        board[player_move.to.x][player_move.to.y].piece->coord = player_move.to;
        break;
    }
    board[player_move.from.x][player_move.from.y].piece = nullptr;

    // Step 6 - adds the move to the past moves vector so it is possible for the move to be undone
    past_moves.push_back(past_move);

    // Step 7 - handles moving both the rook if the move is a castle
    if (player_move.is_castle)
    {
        if (player_move.to.x == 6)
        {
            // short castle
            board[5][player_move.to.y].piece = board[7][player_move.to.y].piece;
            board[7][player_move.to.y].piece = nullptr;
            board[5][player_move.to.y].piece->coord = Coord{ 5, player_move.to.y };
        }
        else if (player_move.to.x == 2)
        {
            // long castle
            board[3][player_move.to.y].piece = board[0][player_move.to.y].piece;
            board[0][player_move.to.y].piece = nullptr;
            board[3][player_move.to.y].piece->coord = Coord{ 3, player_move.to.y };
        }
    }

    // Step 8 - increases the move number by one followed by calling find_moves to
    // find the new possible moves in the new board position. Then pushes this onto
    // past_possible_moves (except if the move is being pushed to check for a pin
    // in which case the possible moves shouldn't be added to past_possible_moves as
    // the found possible moves will not contain the whether each move causes a check
    // and the pieces moving won't have been checked for being pinned)
    ++move_number;
    this->find_moves(check_for_pin);
    if (check_for_pin) {
        past_possible_moves.push_back(possible_moves);
    }

    // Step 9 - returns true so the code which called this method knows
    // the move passed was legal and was pushed on the board successfully
    return true;
}

void BoardStructure::pop_move(bool pop_past_possible_moves)
{
    // Step 1 - changes the game state to in play in case the move being undone was
    // a game ending move
    game_state = IN_PLAY;

    // Step 2 - creates variables for better code readability and development time as I
    // won't have to keep retyping long lines of code to access what these variables store
    PastMove last_move = past_moves.back();
    BoardCell* board_cell_to = &board[last_move.to.x][last_move.to.y];

    // Step 3 - reverses any promotions done to the piece in the last move by creating
    // a new piece of its old piece type if it was promoted
    switch (last_move.promoted_from)
    {
    case PAWN:
        board[last_move.to.x][last_move.to.y].piece = new Pawn{ last_move.promoted_from,
                                                               last_move.player_side, board_cell_to->coord };
        break;
    case BISHOP:
        board[last_move.to.x][last_move.to.y].piece = new Bishop{ last_move.promoted_from,
                                                                 last_move.player_side, board_cell_to->coord };
        break;
    case ROOK:
        board[last_move.to.x][last_move.to.y].piece = new Rook{ last_move.promoted_from,
                                                               last_move.player_side, board_cell_to->coord };
        break;
    case KNIGHT:
        board[last_move.to.x][last_move.to.y].piece = new Knight{ last_move.promoted_from,
                                                                 last_move.player_side, board_cell_to->coord };
        break;
    case KING:
        board[last_move.to.x][last_move.to.y].piece = new King{ last_move.promoted_from,
                                                               last_move.player_side, board_cell_to->coord };
        break;
    case QUEEN:
        board[last_move.to.x][last_move.to.y].piece = new Queen{ last_move.promoted_from,
                                                                last_move.player_side, board_cell_to->coord };
        break;
    default:
        break;
    }

    board_cell_to->piece->coord = last_move.from;
    board[last_move.from.x][last_move.from.y].piece = board_cell_to->piece;
    board_cell_to->piece = nullptr;

    // Step 4 - places back any taken pieces back on the board, if there are none then the
    // piece is taken off the board cell it moved to during the last move
    if (last_move.piece_taken != nullptr)
    {
        board_cell_to->piece = last_move.piece_taken;
    }
    else
    {
        board_cell_to->piece = nullptr;
    }

    // Step 5 - moves the rook back to its original position if the last move was a castle
    if (last_move.was_castle)
    {
        if (last_move.to.x == 6)
        {
            // undo short castle
            board[7][last_move.from.y].piece = board[5][last_move.from.y].piece;
            board[7][last_move.from.y].piece->coord = Coord{ 7, last_move.to.y };
            board[5][last_move.from.y].piece = nullptr;
        }
        else if (last_move.to.x == 2)
        {
            // undo long castle
            board[0][last_move.from.y].piece = board[3][last_move.from.y].piece;
            board[0][last_move.from.y].piece->coord = Coord{ 0, last_move.to.y };
            board[3][last_move.from.y].piece = nullptr;
        }
    }

    // Step 6 - decreases the move_number by one and deletes the most recent move
    // from past_moves
    --move_number;
    past_moves.erase(past_moves.end() - 1);

    // Step 7 - if pop_past_possible_moves is true (would be false if the move being
    // popped was pushed to check for a pin so didn't have the new possible moves added
    // to past_possible_moves) and the length of past_possible_moves is greater than 0
    // then removes the final item from past_possible_moves and makes the current possible
    // moves equal to the new final item in past_possible_moves which are the possible
    // move before the move being undone was made
    if (pop_past_possible_moves && past_possible_moves.size() > 0)
    {
        past_possible_moves.erase(past_possible_moves.end() - 1);
        possible_moves = past_possible_moves.back();
    }
}

BoardStructure::IsMoveLegalResult BoardStructure::is_move_legal(Move& possible_move)
{
    // Step 1 - creates an instance of the IsMoveLegalResult called
    // legal_and_check to keep track of information found by the method
    // before being returned. Then pushes the possible move on the board
    // with check_for_pin as false as whether this piece is pinned itself
    // does not affect whether this piece can cause a pin, then adds to
    // legal_and_check whether this move is legal by investigating whether
    // the original player is now in check after making the move
    IsMoveLegalResult legal_and_check{};
    this->push_move(possible_move, false);
    legal_and_check.is_legal = !this->in_check();

    // Step 2 -increases the move number by one  before adding to
    // legal_and_check whether this move causes a check, increasing
    // the move number by one means the in_check method finds out whether
    // the opposition is in check rather than the current player
    ++move_number;
    legal_and_check.causes_check = this->in_check();

    // Step 3 - decreases move number by one back to where it was before
    // prior to step 2 and undoes the move made in step 1, passing false
    // for the pop_past_possible_moves parameter of the push_move method
    // since in step 1 check_for_pin was passed as false so the new possible
    // moves wouldn't have been added to past_possible_moves so doesn't need
    // to be popped off now. Then returns the legal_and_check struct containing
    // information about the move's legality and whether it causes a check
    --move_number;
    this->pop_move(false);
    return legal_and_check;
}

void BoardStructure::add_possible_move(Coord move_to, Coord piece_position,
                                       Coord piece_taken_position, bool check_for_pin,
                                       bool is_pawn, bool is_castle)
{
    // Step 1 - constructs a possible move using the variables passed to the method but
    // with the promotion_to attribute initially defined to NOPIECE as this will be
    // properly defined in Step 2
    Move possible_move{piece_position, move_to, piece_taken_position, NOPIECE, is_castle};

    // Step 2 - if this possible move is for finding out whether another piece is pinned
    // then the method pushes the possible move straight onto pin_moves, if it is not
    // then a possible_promotions vector is declared and if the is_pawn parameter was
    // passed to the method as true then the method checks if the piece has reached the
    // end of the board and if it has the method adds each type of piece this piece could
    // promote to the possible_promotions variable, with the pawn not being able to stay
    // a pawn so only promotional moves are added in the scenario the piece moving is a
    // pawn. If is_pawn has been passed as false, so the piece is not a pawn NOPIECE is
    // made the only item of the possible_promotions variable. Then the method iterates
    // through possible_promotions, adding a possible move to the possible_moves
    // attribute of BoardStructure for each type of promotion that is available (NOPIECE
    // if there is none available)
    if (!check_for_pin) {
        pin_moves.push_back(possible_move);
    }
    else {
        vector<PieceType> possible_promotions{};
        if (is_pawn && move_to.y == (move_number + 1) % 2 * 7) {
            possible_promotions = { BISHOP, KNIGHT, ROOK, QUEEN };
        }
        else {
            possible_promotions = { NOPIECE };
        }

        for (PieceType possible_promotion : possible_promotions) {
            possible_move.promotion_to = possible_promotion;
            IsMoveLegalResult move_info = this->is_move_legal(possible_move);

            if (move_info.is_legal) {
                possible_move.causes_check = move_info.causes_check;
                possible_moves.push_back(possible_move);
            }
        }
    }
}

void BoardStructure::find_moves(bool check_for_pin)
{
    // Step 1 - clears whichever vector is about to have the new possible moves added to
    if (check_for_pin)
    {
        possible_moves.clear();
    }
    else
    {
        pin_moves.clear();
    }

    // Step 2 - initiates the player_move_multiplier variable as either 1 or -1
    // depending on whether the current player is playing white or black. This
    // variable is used to keep track of whether a piece moves up or down on the
    // board when pieces move forward/backwards relative to their player. This
    // variable(1 as white and -1 as black) is especially important for when
    // working out where pawns on the board can go to
    short int player_move_multiplier{ 1 };
    if (move_number % 2 == 1)
    {
        player_move_multiplier = -1;
    }

    // Step 3 - iterates through each board cell on the board then checks if
    // there is a piece on that board cell and if there is, whether that piece
    // is owned by whichever player's turn it is currently
    for (int y = 0; y < BOARD_SIZE; ++y)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            BoardCell& board_cell = board[x][y];
            if (board_cell.piece != nullptr
                && board_cell.piece->player_side == move_number % 2)
            {
                // Step 4 - calls the currently iterated piece's
                // add_possible_moves method which will call the
                // BoardStructure's add_possible_move method on each
                // possible move found for that individual piece
                board_cell.piece->add_possible_moves(*this,
                                                     player_move_multiplier, check_for_pin, board_cell);
            }
        }
    }
    // Step 5 - checks if the game has been won or drawn yet. First checks if
    // the move number is greater than 1 and if check_for_pin is true, this is
    // because we do not need to check if the game is won or drawn with moves
    // checking for pins and checks if there are no possible legal moves as this
    // would mean the match has been drawn or lost for the player whose turn
    // it is currently. The move_number is increased by 1 before calling the
    // in_check method, this is to find out whether the opposition can take
    // the current player's king, if they can take the current player's king
    // then the opposing player has won, if they can't then it is a draw. The
    // move number is then decreased by one at the end to make up for the
    // fact it was increased to check if the game was a loss or draw
    if (move_number > 1 && check_for_pin == true && size(possible_moves) == 0)
    {
        ++move_number;
        bool currently_in_check = this->in_check();
        if (currently_in_check)
        {
            // checkmate
            if (move_number % 2 == 0) {
                game_state = WHITE_WIN;
            }
            else {
                game_state = BLACK_WIN;
            }
        }
        else
        {
            // draw as there are no possible legal moves and the king is not in check
            game_state = DRAW;
        }
        --move_number;
    }
}
