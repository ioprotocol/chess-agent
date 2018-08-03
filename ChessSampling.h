//
// Created by xushuyang on 2018/7/31.
//

#ifndef CHESSAGENT_CHESSSAMPLING_H
#define CHESSAGENT_CHESSSAMPLING_H

#include <QRect>
#include <QList>>
#include <opencv2/opencv.hpp>

class ChessSampling {
private:
    QRect chessRect;
public:
    ChessSampling();

public:
    QList<cv::Mat> grabSample(cv::Mat &screen);
};


#endif //CHESSAGENT_CHESSSAMPLING_H
