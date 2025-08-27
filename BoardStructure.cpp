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
                case NOPIECE:
                    break;
                }
            }
        }
        cout << endl;
    }
}

BoardStructure::BoardStructure()
{
    // assigns coordinates to each board cell
    for (short int y = 0; y < BOARD_SIZE; ++y) {
        for (short int x = 0; x < BOARD_SIZE; x++) {
            board[x][y].coord = Coord{ x, y };
        }
    }

    // creates the pieces on the board
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

    // finds legal moves available in the starting board position
    this->find_moves();
}

BoardStructure::~BoardStructure()
{
    //std::cout << "BoardStructure deconstructed" << std::endl;
}

bool BoardStructure::in_check()
{
    // finds the opposition's king on the board
    for (int y = 0; y < BOARD_SIZE; ++y)
    {
        for (int x = 0; x < BOARD_SIZE; ++x)
        {
            BoardCell& board_cell{ board[x][y] };
            if (board_cell.piece != nullptr && board_cell.piece->piece_type == KING
                && board_cell.piece->player_side == (move_number + 1) % 2)
            {
                Coord king_position = board_cell.coord;

                // checks if any of the enemy pieces can take the king on
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
    if (game_state != IN_PLAY)
    {
        return false;
    }

    // validates that either the move to be made is an available legal
    // move or the move is being pushed to check for a pin
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

    Piece* piece_from = board[player_move.from.x][player_move.from.y].piece;
    PastMove past_move{ player_move.from, player_move.to,
                       piece_from->player_side, nullptr, NOPIECE, player_move.is_castle };

    if (player_move.promotion_to != NOPIECE) {
        past_move.promoted_from = piece_from->piece_type;
    }

    if (player_move.piece_taken.x != -1) // -1 indicates there is no piece taken in the move
    {
        past_move.piece_taken = board[player_move.piece_taken.x][player_move.piece_taken.y].piece;
        board[player_move.piece_taken.x][player_move.piece_taken.y].piece = nullptr;
    }

    // moves piece from its original position to its destination and promotes it (if apart of the move)
    switch (player_move.promotion_to)
    {
    case PAWN:
        board[player_move.to.x][player_move.to.y].piece = new Pawn{ player_move.promotion_to,
                                                                   piece_from->player_side, player_move.to };
        delete piece_from;
        break;
    case BISHOP:
        board[player_move.to.x][player_move.to.y].piece = new Bishop{ player_move.promotion_to,
                                                                     piece_from->player_side, player_move.to };
        delete piece_from;
        break;
    case ROOK:
        board[player_move.to.x][player_move.to.y].piece = new Rook{ player_move.promotion_to,
                                                                   piece_from->player_side, player_move.to };
        delete piece_from;
        break;
    case KNIGHT:
        board[player_move.to.x][player_move.to.y].piece = new Knight{ player_move.promotion_to,
                                                                     piece_from->player_side, player_move.to };
        delete piece_from;
        break;
    case KING:
        board[player_move.to.x][player_move.to.y].piece = new King{ player_move.promotion_to,
                                                                   piece_from->player_side, player_move.to };
        delete piece_from;
        break;
    case QUEEN:
        board[player_move.to.x][player_move.to.y].piece = new Queen{ player_move.promotion_to,
                                                                    piece_from->player_side, player_move.to };
        delete piece_from;
        break;
    default:
        board[player_move.to.x][player_move.to.y].piece = board[player_move.from.x][player_move.from.y].piece;
        board[player_move.to.x][player_move.to.y].piece->coord = player_move.to;
        break;
    }

    board[player_move.from.x][player_move.from.y].piece = nullptr;

    past_moves.push_back(past_move);

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

    ++move_number;
    this->find_moves(check_for_pin);

    return true;
}

void BoardStructure::pop_move(bool pop_past_possible_moves)
{
    // changes the game state to in play in case the move being undone was a match ending move
    game_state = IN_PLAY;

    if (past_moves.size() == 0) {
        std::cout << "ERROR - ATTEMPT TO UNDO MOVE WHEN IT IS THE FIRST MOVE OF THE MATCH" << std::endl;
    }
    PastMove last_move = past_moves.back();
    BoardCell* board_cell_to = &board[last_move.to.x][last_move.to.y];

    // reverses any promotions done to the piece in the last move
    switch (last_move.promoted_from)
    {
    case PAWN:
        delete board_cell_to->piece;
        board_cell_to->piece = new Pawn{ last_move.promoted_from,
                                                               last_move.player_side, board_cell_to->coord };
        break;
    case BISHOP:
        delete board_cell_to->piece;
        board_cell_to->piece = new Bishop{ last_move.promoted_from,
                                                                 last_move.player_side, board_cell_to->coord };
        break;
    case ROOK:
        delete board_cell_to->piece;
        board_cell_to->piece = new Rook{ last_move.promoted_from,
                                                               last_move.player_side, board_cell_to->coord };
        break;
    case KNIGHT:
        delete board_cell_to->piece;
        board_cell_to->piece = new Knight{ last_move.promoted_from,
                                                                 last_move.player_side, board_cell_to->coord };
        break;
    case KING:
        delete board_cell_to->piece;
        board_cell_to->piece = new King{ last_move.promoted_from,
                                                               last_move.player_side, board_cell_to->coord };
        break;
    case QUEEN:
        delete board_cell_to->piece;
        board_cell_to->piece = new Queen{ last_move.promoted_from,
                                                                last_move.player_side, board_cell_to->coord };
        break;
    default:
        break;
    }
    board_cell_to->piece->coord = last_move.from;
    board[last_move.from.x][last_move.from.y].piece = board_cell_to->piece;

    if (last_move.piece_taken != nullptr)
    {
        board_cell_to->piece = nullptr;
        Coord piece_taken_coord = last_move.piece_taken->coord;
        board[piece_taken_coord.x][piece_taken_coord.y].piece = last_move.piece_taken;
    }
    else
    {
        board_cell_to->piece = nullptr;
    }

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

    --move_number;
    if (past_moves.size() == 0) {
        std::cout << "ERROR - ATTEMPT TO UNDO MOVE WHEN IT IS THE FIRST MOVE OF THE MATCH" << std::endl;
    }
    past_moves.erase(past_moves.end() - 1);

    if (pop_past_possible_moves && past_possible_moves.size() > 0)
    {
        past_possible_moves.erase(past_possible_moves.end() - 1);
        possible_moves = past_possible_moves.back();
    }
}

BoardStructure::IsMoveLegalResult BoardStructure::is_move_legal(Move& possible_move)
{
    IsMoveLegalResult legal_and_check{};
    this->push_move(possible_move, false);
    legal_and_check.is_legal = !this->in_check();

    ++move_number;
    legal_and_check.causes_check = this->in_check();

    --move_number;
    this->pop_move(false);
    return legal_and_check;
}

void BoardStructure::add_possible_move(Coord move_to, Coord piece_position,
                                       Coord piece_taken_position, bool check_for_pin,
                                       bool is_pawn, bool is_castle)
{
    Move possible_move{piece_position, move_to,
        piece_taken_position, NOPIECE, is_castle, false};
    
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
    // clears whichever vector is about to have the new possible moves added to
    if (check_for_pin)
    {
        possible_moves.clear();
    }
    else
    {
        pin_moves.clear();
    }

    short int player_move_multiplier{ 1 };
    if (move_number % 2 == 1)
    {
        player_move_multiplier = -1;
    }

    for (int y = 0; y < BOARD_SIZE; ++y)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            BoardCell& board_cell = board[x][y];
            if (board_cell.piece != nullptr
                && board_cell.piece->player_side == move_number % 2)
            {
                board_cell.piece->add_possible_moves(*this,
                    player_move_multiplier, check_for_pin, board_cell);
            }
        }
    }

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

    if (check_for_pin) {
        past_possible_moves.push_back(possible_moves);
    }
}
