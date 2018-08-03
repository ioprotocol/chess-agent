//
// Created by xushuyang on 2018/7/31.
//


#include "ChessSampling.h"
#include "applicationutils.h"

#include <QDebug>

ChessSampling::ChessSampling() : chessRect(QRect(0, 0, 0, 0)){}

QList<cv::Mat> ChessSampling::grabSample(cv::Mat &screen) {
    if (chessRect.width() == 0) {
        qDebug() << "detect chess board";
        chessRect = QRect(Chess::detect_chess_board(screen));
    }

    QList<cv::Mat> result;

    if (chessRect.width() == 0) {
        return result;
    }

    float dx = this->chessRect.width() / 9.0;
    float dy = this->chessRect.height() / 10.0;

    for(int x = 0; x < 9; x++) {
        for(int y = 0; y < 10; y++) {
            cv::Rect rect(chessRect.left() + x * dx - 32, chessRect.top() + dy * y - 32, 64, 64);
            cv::Mat roi = screen(rect);
            QRect circle;
            if (Chess::hough_detection_single_circle(roi, circle)) {
                QString path = Hub::current_dir();
                path.append("/resources/train/");
                path.append(QString::number(qrand())).append(".jpg");
                qDebug() << path;
                cv::imwrite(path.toStdString(), roi);
            } else {
                qDebug() << "detection circle failed, this img is blank or other err";
            }
        }
    }

    return result;
}

void ChessSampling::test(cv::Mat &screen) {
    grabSample(screen);
}
