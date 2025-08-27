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
    MatchReview();

    Q_INVOKABLE QVariantList enter_move_from_match(short int x_to,
        short int y_to, short int x_from, short int y_from, PieceType promoting_to);

    Q_INVOKABLE void enter_user_side(PlayerSide entered_user_side);

    Q_INVOKABLE QVariantList find_blunders();

    BoardStructure match_board;
private:

    QVariantList convert_board_to_QML_board();

    std::vector<Move> match_moves;
    PlayerSide user_side;
    const short int TREE_DEPTH{ 1 };

    // tree traversal methods
    short int negamax_alpha_beta(BoardStructure& board, short int alpha, short int beta, short int treeDepthLeft);
    short int quiescence(BoardStructure& board, short int alpha, short int beta);

    // leaf node evaluation methods
    short int static_evaluation(BoardStructure& board);
    short int static_in_play_evaluation(BoardStructure& board);
};
