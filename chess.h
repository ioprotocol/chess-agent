//
// Created by xsy on 18-7-2.
//

#ifndef CHESSAGENT_CHESS_H
#define CHESSAGENT_CHESS_H

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>

#include "piece.h"

class Chess {
private:
    // 558 x 620
    cv::Mat chessBoard;

    cv::Point topLeft;
    cv::Point topRight;
    cv::Point bottomLeft;
    cv::Point bottomRight;

    cv::Point positions[10][9];

    Piece* chessPieces[32];
public:
    Chess();

    virtual ~Chess();

    void resetPiecePosition();

    void reversePiecePosition();

    void generateMat();

    int cvAdd4cMat_q(cv::Mat &dst, cv::Mat &scr, double scale);
    // for test
    void saveToDisk();
};


#endif //CHESSAGENT_CHESS_H
