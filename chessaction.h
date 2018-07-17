//
// Created by xsy on 18-7-2.
//

#ifndef CHESSAGENT_CHESS_H
#define CHESSAGENT_CHESS_H

#include <opencv2/opencv.hpp>

#include "chesspiece.h"

class ChessAction {
private:
    // 558 x 620
    cv::Mat chess_board_;

    cv::Point top_left_;
    cv::Point top_right_;
    cv::Point bottom_left_;
    cv::Point bottom_right_;

    cv::Point positions_[10][9];

    ChessPiece* chess_pieces_[32];
public:
    ChessAction();

    virtual ~ChessAction();

    void reset_piece_position();

    void reverse_piece_position();

    int cv_add4c_mat_q(cv::Mat &dst, cv::Mat &scr, double scale);
    // for test
    void save_to_disk();
};


#endif //CHESSAGENT_CHESS_H
