// MatchReview.h
#pragma once
#include <vector>

#include <QObject>
#include <QVariant>

#include "BoardStructure.h"

class MatchReview : public QObject
{
    Q_OBJECT
public:
    // MatchReview class constructor
    MatchReview();

    // enter_move_from_match is called whenever the user enters a move from the match they would like to review,
    // this method pushes that move onto the match_moves attribute of this class and then returns the updated
    // board after that move has been made so the board GUI can be updated correctly.
    Q_INVOKABLE QVariantList enter_move_from_match(short int x_to,
        short int y_to, short int x_from, short int y_from, PieceType promoting_to);

    // enter_user_side is called when the user clicks which side they played in the match they will enter to be reviewed
    Q_INVOKABLE void enter_user_side(PlayerSide entered_user_side);

    // finds the worst moves made by the user then returns this to the frontend QML code as board positions to iterate
    // through which also come with info about which square to move to was the best move and which square the piece
    // just moved from
    Q_INVOKABLE QVariantList find_blunders();

    BoardStructure match_board;
private:
    struct Blunder {
        QVariantList boardBeforeBlunder;

        short int blunder_from_x;
        short int blunder_from_y;
        short int blunder_to_x;
        short int blunder_to_y;

        short int best_from_x;
        short int best_from_y;
        short int best_to_x;
        short int best_to_y;

        short int severity;
    };

    QVariantList convert_board_to_QML_board();

    std::vector<Move> match_moves;
    PlayerSide user_side;
    const short int TREE_DEPTH{ 2 };

    // tree traversal methods
    short int negamax_alpha_beta(BoardStructure& board, short int alpha, short int beta, short int treeDepthLeft);
    short int quiescence(BoardStructure& board, short int alpha, short int beta);

    // leaf node evaluation methods
    short int static_evaluation(BoardStructure& board);
    short int static_in_play_evaluation(BoardStructure& board);
};
