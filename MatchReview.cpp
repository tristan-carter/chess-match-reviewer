// MatchReview.cpp
#include <iostream>
#include <QVariant>


#include "MatchReview.h"
#include "PieceSquareTables.cpp"

MatchReview::MatchReview()
{
    match_board = BoardStructure{};
    match_moves = {};
}

void MatchReview::enter_user_side(PlayerSide entered_user_side) {
    user_side = entered_user_side;
}

QVariantList MatchReview::convert_board_to_QML_board() {
    QVariantList boardGUI_variant_list;
    for (short int y = BOARD_SIZE - 1; y > -1; y--) {
        QVariantList row{};
        for (short int x = 0; x < BOARD_SIZE; x++) {
            const BoardCell& board_cell = match_board.board[x][y];
            if (board_cell.piece == nullptr) {
                row.append("");
            }
            else {
                switch (board_cell.piece->piece_type)
                {
                case PAWN:
                    if (board_cell.piece->player_side == WHITE) { row.append("♙"); }
                    else { row.append("♟"); };
                    break;
                case ROOK:
                    if (board_cell.piece->player_side == WHITE) { row.append("♖"); }
                    else { row.append("♜"); };
                    break;
                case KNIGHT:
                    if (board_cell.piece->player_side == WHITE) { row.append("♘"); }
                    else { row.append("♞"); };
                    break;
                case BISHOP:
                    if (board_cell.piece->player_side == WHITE) { row.append("♗"); }
                    else { row.append("♝"); };
                    break;
                case QUEEN:
                    if (board_cell.piece->player_side == WHITE) { row.append("♕"); }
                    else { row.append("♛"); };
                    break;
                case KING:
                    if (board_cell.piece->player_side == WHITE) { row.append("♔"); }
                    else { row.append("♚"); };
                    break;
                }
            }
        }
        boardGUI_variant_list.append(row);
    }
    return boardGUI_variant_list;
}

QVariantList MatchReview::enter_move_from_match(short int x_to,
    short int y_to, short int x_from, short int y_from, PieceType promoting_to)
{
    Move move_entered{ Coord{x_from, y_from}, Coord{x_to, y_to} };
    if (promoting_to != NOPIECE) {
        move_entered.promotion_to = promoting_to;
    }

    bool is_entered_move_valid = match_board.push_move(move_entered);

    if (is_entered_move_valid) {
        match_moves.push_back(move_entered);
    }

    QVariantList boardGUI_variant_list = this->convert_board_to_QML_board();

    return boardGUI_variant_list;
}

QVariantList MatchReview::find_blunders()
{
    if (match_moves.size() == 0) {
        // user recieves a pop up saying no blunders could be found in their match
        return QVariantList();
    }
    
    for (short int i = 0; i < match_moves.size(); i++) {
        match_board.pop_move();
    }

    // evaluates the available moves the user had to choose from during 
    // each of their turns and compares the best available moves 
    // with the moves they chose to determine if they made any blunders
    QVariantList match_blunders = {};
    for (int move_number = 0; move_number < match_moves.size(); move_number++)
    {
        if (move_number % 2 == user_side) {
            Move best_move;
            short int best_move_eval = -9999;
            short int user_move_eval;
            std::vector<Move> current_possible_moves = match_board.possible_moves;
            for (Move move: current_possible_moves) {
                match_board.push_move(move);
                short int eval_score = -this->negamax_alpha_beta(match_board,
                    -9999, 9999, TREE_DEPTH);

                // finds the best alternative move
                if (eval_score > best_move_eval) {
                    best_move_eval = eval_score;
                    best_move = move;
                }

                // checks if this move was the one the user chose
                if (move == match_moves[move_number]) {
                    user_move_eval = eval_score;
                }
                match_board.pop_move();
            }

            if (best_move_eval - user_move_eval > 200) {

                QVariantMap blunder;
                blunder["blunder_from_x"] = match_moves[move_number].from.x;
                blunder["blunder_from_y"] = 7 - match_moves[move_number].from.y;
                blunder["blunder_to_x"] = match_moves[move_number].to.x;
                blunder["blunder_to_y"] = 7 - match_moves[move_number].to.y;

                blunder["best_from_x"] = best_move.from.x;
                blunder["best_from_y"] = 7 - best_move.from.y;
                blunder["best_to_x"] = best_move.to.x;
                blunder["best_to_y"] = 7 - best_move.to.y;

                blunder["boardBeforeBlunder"] = this->convert_board_to_QML_board();

                short int blunder_severity = (best_move_eval - user_move_eval) / 10;

                if (blunder_severity > 100) {
                    blunder_severity = 100;
                }
                blunder["severity"] = blunder_severity;


                match_blunders.append(blunder);
            }
        }

        match_board.push_move(match_moves[move_number]);
    }
    return match_blunders;
}

short int MatchReview::negamax_alpha_beta(BoardStructure& board,
                                          short int alpha, short int beta, short int treeDepthLeft)
{
    if (treeDepthLeft == 0) { return this->quiescence(board, alpha, beta); }

    short int best_value = -9999;

    PlayerSide side_to_maximise = static_cast <PlayerSide> (board.move_number % 2);

    std::vector<Move> current_possible_moves = board.possible_moves;
    for (Move &move : current_possible_moves)
    {
        board.push_move(move);

        short int score{};
        if (board.game_state != IN_PLAY)
        {
            if (board.game_state == DRAW)
            {
                score = 0;
            }
            else if (board.game_state == side_to_maximise)
            {
                score = 9999; // side to maximise score won
            }
            else
            {
                score = -9999; // side to minimise score won
            }
        }
        else
        {
            score = -this->negamax_alpha_beta(board, -beta, -alpha, treeDepthLeft - 1);
        }
        board.pop_move();
        if (score > best_value)
        {
            best_value = score;
            if (score > alpha) { alpha = score; }
        }
        if (score >= beta) { return score; }
    }
    return best_value;
}

short int MatchReview::quiescence(BoardStructure& board,
                                  short int alpha, short int beta)
{
    const short int static_eval_score{ this->static_evaluation(board) };
    short int best_value = static_eval_score;

    if (static_eval_score >= beta)
    {
        return beta;
    }

    // initiates BIG_DELTA to 900 (the value of a queen) if there is a pawn promotion
    // available then adds the maximum possible increase in value of a promotion of a piece (from a
    // pawn to a queen) which is 800
    short int BIG_DELTA{ 900 };

    bool promotion__available = false;
    for (Move &move : board.possible_moves) {
        if (move.promotion_to != NOPIECE) {
            promotion__available = true;
            break;
        }
    }
    if (promotion__available) { BIG_DELTA += 800; }

    if (static_eval_score < alpha - BIG_DELTA) { return alpha; }

    if (alpha < static_eval_score) { alpha = static_eval_score; }

    PlayerSide side_to_maximise = static_cast <PlayerSide> (board.move_number % 2);

    // branches down unstable positions
    std::vector<Move> current_possible_moves = board.possible_moves;
    for (Move &move : current_possible_moves)
    {
        if (move.piece_taken.x != -1)
        {
            board.push_move(move);

            short int score{};
            if (board.game_state != IN_PLAY)
            {
                if (board.game_state == DRAW)
                {
                    score = 0;
                }
                else if (board.game_state == side_to_maximise)
                {
                    score = 9999; // side to maximise score won
                }
                else
                {
                    score = -9999; // side to minimise score won
                }
            }
            else
            {
                score = -this->quiescence(board, -beta, -alpha);
            }

            board.pop_move();

            if (score >= beta) { return score; }

            if (score > best_value) { best_value = score; }

            if (score > alpha) { alpha = score; }
        }
    }

    return alpha;
}

short int MatchReview::static_evaluation(BoardStructure& board)
{
    if (board.game_state == IN_PLAY)
    {
        short int static_eval_score = this->static_in_play_evaluation(board);
        return static_eval_score;

    }
    else if (board.game_state == board.move_number % 2)
    {
        // side being evaluated for in the tree won
        return 9999;

    }
    else if (board.game_state == DRAW)
    {
        // draw
        return 0;

    }
    else
    {
        // side being evaluated for in tree lost
        return -9999;
    }
}

short int MatchReview::static_in_play_evaluation(BoardStructure& board)
{
    short int static_eval_score { 0 };

    // initially assumed to be white side's turn
    short int white_sign { 1 };
    short int black_sign { -1 };

    // checks if it is in fact black side's turn
    if (board.move_number % 2 == 1)
    {
        white_sign = -1;
        black_sign = 1;
    }

    // iterares through each board cell
    for (int y = 0; y < BOARD_SIZE; ++y)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            BoardCell& board_cell = board.board[x][y];

            if (board_cell.piece)
            {
                switch (board_cell.piece->piece_type)
                {
                case PAWN:
                    if (board_cell.piece->player_side == WHITE)
                    {
                        static_eval_score += (100 + white_pawn_square_table[x][y]) * white_sign;
                    }
                    else {
                        static_eval_score += (100 + black_pawn_square_table[x][y]) * black_sign;
                    }
                    break;

                case KNIGHT:
                    if (board_cell.piece->player_side == WHITE)
                    {
                        static_eval_score += (300 + white_knight_square_table[x][y]) * white_sign;
                    }
                    else {
                        static_eval_score += (300 + black_knight_square_table[x][y]) * black_sign;
                    }
                    break;
                case BISHOP:
                    if (board_cell.piece->player_side == WHITE)
                    {
                        static_eval_score += (300 + white_bishop_square_table[x][y]) * white_sign;
                    }
                    else {
                        static_eval_score += (300 + black_bishop_square_table[x][y]) * black_sign;
                    }
                    break;
                case ROOK:
                    if (board_cell.piece->player_side == WHITE)
                    {
                        static_eval_score += (500 + white_rook_square_table[x][y]) * white_sign;
                    }
                    else {
                        static_eval_score += (500 + black_rook_square_table[x][y]) * black_sign;
                    }
                    break;
                case QUEEN:
                    if (board_cell.piece->player_side == WHITE)
                    {
                        static_eval_score += (900 + white_queen_square_table[x][y]) * white_sign;
                    }
                    else {
                        static_eval_score += (900 + black_queen_square_table[x][y]) * black_sign;
                    }
                    break;
                case KING:
                    // only position is taken into account for kings as both sides must always have
                    // the same number of kings when the match is in play so they cancel out
                    if (board_cell.piece->player_side == WHITE)
                    {
                        static_eval_score += white_king_square_table[x][y] * white_sign;
                    }
                    else {
                        static_eval_score += black_king_square_table[x][y] * black_sign;
                    }
                    break;
                default:
                    std::cout << "ERROR - INVALID PIECE TYPE" << std::endl;
                    break;
                }
            }
        }
    }

    return static_eval_score;
}
