// MatchReview.cpp
#include <iostream>
#include <QVariant>


#include "MatchReview.h"
#include "PieceSquareTables.cpp"

MatchReview::MatchReview()
{
    // Step 1 - initiates board and
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
            // const Piece* board_piece = board_cell.piece;
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

QVariantList MatchReview::enter_move_from_match(short int x_to, short int y_to, short int x_from, short int y_from, PieceType promoting_to)
{
    //
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

// in progress
QVariantList MatchReview::find_blunders()
{
    // undoes all moves made on board during entering of match
    for (short int i = 0; i < match_moves.size()-1; i++) {
        match_board.pop_move();
    }
    QVariantList match_blunders = {};
    for (int move_number = 0; move_number < match_moves.size(); move_number++)
    {
        if (move_number % 2 == user_side) {
            // evaluates all possible moves
            Move best_move;
            short int best_move_eval = -9999;
            short int user_move_eval;
            for (Move move: match_board.possible_moves) {
                match_board.push_move(move);
                short int eval_score = -this->negamax_alpha_beta(match_board,
                    -9999, 9999, TREE_DEPTH);

                match_board.pop_move();

                // finds best alternative move
                if (eval_score >= best_move_eval) {
                    best_move_eval = eval_score;
                    best_move = move;
                }

                // checks if this move was the move made by the user
                if (move == match_moves[move_number]) {
                    user_move_eval = eval_score;
                }
            }

            // determines whether user's move was a blunder
            if (best_move_eval - user_move_eval > 300) {
                // needs to store 4 pieces of information:
                // 1. the board just before the blunder is made as a QVariantList
                // 2. the coordinate of the square the piece moves to in the blunder
                // 3. the coordinate of the square the piece should have moved from and to for the best move
                // 4. a score from 1-100 as to how severe the blunder was
                short int blunder_from_x;
                short int blunder_from_y;
                short int blunder_to_x;
                short int blunder_to_y;

                short int best_from_x;
                short int best_from_y;
                short int best_to_x;
                short int best_to_y;
                QVariantMap blunder;
                blunder["boardBeforeBlunder"] = this->convert_board_to_QML_board();

                blunder["blunder_from_x"] = match_moves[move_number].from.x;
                blunder["blunder_from_y"] = match_moves[move_number].from.y;
                blunder["blunder_to_x"] = match_moves[move_number].to.x;
                blunder["blunder_to_y"] = match_moves[move_number].to.y;

                blunder["best_from_x"] = best_move.from.x;
                blunder["best_from_y"] = best_move.from.y;
                blunder["best_to_x"] = best_move.to.x;
                blunder["best_to_y"] = best_move.to.y;

                short int blunder_severity = (best_move_eval - user_move_eval) / 10;

                // validates that the blunder severity is not over its maximum value of 100
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
    // Step 1 - checks if the method has reached its maximum intended depth
    // (TREE_DEPTH) in the tree of possible board positions, if it has then the
    // quiescence algorithm is called which ensures that the position the
    // negamax algorithm has reached is stable.
    if (treeDepthLeft == 0) { return this->quiescence(board, alpha, beta); }

    // Step 2 - initiates the variable best_value which will be used to keep track of
    // the best possible move’s score. With the initial value being -9999 (less
    // than any move could possibly be scored) so that any possible move scores
    // higher than the initial value and replaces the initial value.
    short int best_value = -9999;

    // Step 3 - statically casts (a way to convert from one data type to another
    // in C++) the move number mod 2 (either 0 or 1) to the PlayerSide type.
    PlayerSide side_to_maximise = static_cast <PlayerSide> (board.move_number % 2);

    // Step 4 – iterates through each available legal move in the board position
    // the method was called on.
    std::vector<Move> current_possible_moves = board.possible_moves;
    for (Move &move : current_possible_moves)
    {
        // Step 5 - pushes the currently iterated move on the board so that the
        // resulting board position can be analysed to determine how good of a
        // move the move pushed was.
        bool is_legal = board.push_move(move);
        if (is_legal == false) {
            std::cout << "ERROR - INVALID MOVE" << std::endl;
        }

        // Step 6 – declares score in the scope necessary to be accessed in the
        // later steps. Then checks whether the match has ended after making an
        // available move, and if it did then will set the score variable equal
        // to 0 if it is a draw, -9999 if the match is lost for the current side
        // and 9999 if the match is won for the current side. If the match has
        // not ended, then will set the score variable equal to the negative of
        // what is returned by a recursive call of the negamax_alpha_beta method
        // (continues to branch deeper in the tree of possible moves), passing
        // the negative of alpha and beta.
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

        // Step 7 - undoes (pops) the move being evaluated which was pushed in step 5
        // so that the board is back how it was originally ready for the next
        // iteration to push a possible move.
        board.pop_move();

        // Step 8 – if the score from the currently iterated possible move is higher
        // than the current highest score (best_value) then this score replaces the
        // current score stored in best_value and checks if this score is also higher
        // than alpha, if it is then the score in alpha is also replaced by this score.
        if (score > best_value)
        {
            best_value = score;
            if (score > alpha) { alpha = score; }
        }

        // Step 9 - if the score from this currently iterated possible move is greater
        // than or equal to beta then negamax_alpha_beta returns this score to wherever
        // called the method.
        if (score >= beta) { return score; }
    }

    // Step 10 - returns the highest score out of all the possible moves iterated.
    return best_value;
}

short int MatchReview::quiescence(BoardStructure& board,
                                  short int alpha, short int beta)
{
    // Step 1 – statically evaluates the board position passed to this
    // method and makes this a constant so it can be accessed later on
    // in the method without the worry of it accidentally being changed
    // when it isn’t supposed to be. Also initiates the best_value variable
    // to the static evaluation score to keep track of the best possible
    // move’s score.
    const short int static_eval_score{ this->static_evaluation(board) };
    short int best_value = static_eval_score;

    // Step 2 - checks if the score found by the static evaluation in step 1
    // is greater than or equal to the beta value passed to this method, if
    // it is then this branch of the tree of possible moves is pruned (cut)
    // with this static evaluation being returned.
    if (static_eval_score >= beta)
    {
        return beta;
    }

    // Step 3 - initiates BIG_DELTA to 900 (the value of a queen in my program)
    // then checks if there is a pawn promotion available, if there is then adds
    // the maximum possible increase in value of a promotion of a piece (from a
    // pawn to a queen) which is 700.
    short int BIG_DELTA{ 900 };

    bool promotion__available = false;
    for (Move &move : board.possible_moves) {
        if (move.promotion_to != NOPIECE) {
            promotion__available = true;
            break;
        }
    }
    if (promotion__available) { BIG_DELTA += 800; }

    // Step 4 – checks if the static evaluation score is less than alpha minus
    // BIG_DELTA, if it is then returns alpha to wherever called this method.
    if (static_eval_score < alpha - BIG_DELTA) { return alpha; }

    // Step 5 – checks if alpha is less than the static evaluation score, if it
    // is then makes the alpha variable equal to the static evaluation score.
    if (alpha < static_eval_score) { alpha = static_eval_score; }

    // Step 6 - statically casts the move number mod 2 (either 0 or 1) to the
    // PlayerSide type.
    PlayerSide side_to_maximise = static_cast <PlayerSide> (board.move_number % 2);

    // Step 7 – iterates through each available legal move in the board position
    // the method was called on and checks for each iteration that the move
    // involves a piece capture (so only unstable positions are being branched down).
    std::vector<Move> current_possible_moves = board.possible_moves;
    for (Move &move : current_possible_moves)
    {
        if (move.piece_taken.x != -1)
        {
            // Step 8 -pushes the currently iterated move on the board so that the
            // resulting board position can be analysed.
            bool is_legal = board.push_move(move);
            if (is_legal == false) {
                std::cout << "ERROR - INVALID MOVE" << std::endl;
            }

            // Step 9 – declares score in the scope necessary to be accessed in the
            // later steps. Then checks whether the match has ended after making an
            // available move, and if it did then will set the score variable equal
            // to 0 if it is a draw, -9999 if the match is lost for the current side
            // and 9999 if the match is won for the current side. If the match has
            // not ended, then will set the score variable equal to the negative of
            // what is returned by a recursive call of the quiescence method, passing
            // the negative of alpha and beta.
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

            // Step 10 – undoes (pops) the move being evaluated which was pushed in
            // step 8 so that the board is back how it was originally ready for the next
            // iteration to push a possible move.
            board.pop_move();

            // Step 11 - if the score from this currently iterated possible move is
            // greater than or equal to beta then quiescence returns this score to
            // wherever called the method.
            if (score >= beta) { return score; }

            // Step 12 – if the score from this currently iterated possible move is
            // higher than the current highest score (best_value) then this score
            // replaces the current score stored in best_value.
            if (score > best_value) { best_value = score; }

            // Step 13 – if the score form this currently iterated possible move is
            // higher than alpha then alpha is made equal to this score.
            if (score > alpha) { alpha = score; }
        }
    }

    // Step 14 – returns alpha to wherever called this method.
    return alpha;
}

short int MatchReview::static_evaluation(BoardStructure& board)
{
    // Checks if the match is in play, if it is then the static
    // evaluation score is calculated by adding the scores
    // calculated by the actual_piece_weight method and the
    // basic_positional_weight method.
    if (board.game_state == IN_PLAY)
    {
        // match in play
        short int static_eval_score = this->static_in_play_evaluation(board);
        return static_eval_score;

    }
    // checks if the match was won by the side being evaluated for
    // in the tree whose current turn it is, in this case the method
    // returns 9999.
    else if (board.game_state == board.move_number % 2)
    {
        // side being evaluated for in the tree won
        return 9999;

    }
    // checks if the match is currently in the draw game state, if
    // it is then the methods returns 0.
    else if (board.game_state == DRAW)
    {
        // draw
        return 0;

    }
    // returns -9999 as the opposition to the side being evaluated
    // for in the tree won.
    else
    {
        // side being evaluated for in tree lost
        return -9999;
    }
}

short int MatchReview::static_in_play_evaluation(BoardStructure& board)
{
    // Step 1 – initiates the variable static_eval_score to 0
    // to keep track of the weights of the pieces on the current
    // player’s side minus the weights of the pieces of the opposition.
    short int static_eval_score { 0 };

    // Step 2 – sets the variables white_sign and black_sign to either
    // 1 or -1 depending on what colour the current player being
    // evaluated for is on. This is so that the weights of pieces owned
    // by the player being evaluated for are added to
    // static_eval_score whereas the weights of pieces owned
    // by the opposition are subtracted from static_eval_score.

    // initially assumed to be white side's turn
    short int white_sign { 1 };
    short int black_sign { -1 };

    // checks if it is in fact black side's turn
    if (board.move_number % 2 == 1)
    {
        white_sign = -1;
        black_sign = 1;
    }

    // Step 3 - iterates through each board cell in the board.
    for (int y = 0; y < BOARD_SIZE; ++y)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            BoardCell& board_cell = board.board[x][y];

            // Step 4 – checks if there is a piece in the currently iterated board cell and if there
            // is checks what type of piece it is with a switch statement and checks whether the
            // piece is white or black, the value of that piece added to the score at the position
            // in that piece’s type and side’s piece square table multiplied by the sign of its
            // piece colour (defined in step 2) and added to the static_eval_score variable.
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

    // Step 5 - returns the calculated static evaluation score.
    return static_eval_score;
}
