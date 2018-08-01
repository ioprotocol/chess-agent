//
// Created by xushuyang on 2018/7/31.
//


#include "ChessSampling.h"

#include <QDebug>

ChessSampling::ChessSampling(const QRect &chessRect) : chessRect(chessRect) {}

QList<cv::Mat> ChessSampling::grabSample(cv::Mat &screen) {
    QList<cv::Mat> result;
    float dx = this->chessRect.width() / 9.0;
    float dy = this->chessRect.height() / 10.0;

    for(int x = 0; x < 9; x++) {
        for(int y = 0; y < 10; y++) {
            cv::Rect rect(chessRect.left() + x * dx - 32, chessRect.top() + dy * y - 32, 64, 64);
            cv::Mat roi = screen(rect);
            QRect circle;
            if (houghDetectionCircle(roi, circle)) {

            } else {
                qDebug() << "detection circle failed, this img is blank or other err";
            }
        }
    }

    return result;
}