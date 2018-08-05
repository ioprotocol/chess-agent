//
// Created by xushuyang on 2018/7/31.
//


#include "ChessSampling.h"
#include "applicationutils.h"

#include <QDebug>

ChessSampling::ChessSampling() : chessRect(QRect(0, 0, 0, 0)){}

QList<cv::Mat> ChessSampling::grabSample(cv::Mat &screen) {
    if (chessRect.width() == 0) {
        chessRect = QRect(Chess::detect_chess_board(screen));
        qDebug() << "detect chess board(" << chessRect.left() << "," << chessRect.top() << "," << chessRect.width()
                 << "," << chessRect.height() << ")";
    }

    QList<cv::Mat> result;

    if (chessRect.width() == 0) {
        return result;
    }

    QList<QRect> circles = filter(Chess::hough_detection_circle(screen));
    QSize size(48, 48);

    for (QRect circle : circles) {
        cv::Mat out;
        cv::Mat roi = screen(cv::Rect(circle.left() - size.width() / 2, circle.top() - size.width() / 2, size.width(),
                                      size.width()));
        cv::Mat mask = cv::Mat::zeros(size.width(), size.width(), screen.type());
        cv::circle(mask, cv::Point(24, 24), circle.width() - 4, CV_RGB(255, 255, 255), -1);
        roi.copyTo(out, mask);

        cv::inRange(out, cv::Scalar(0,0,56), cv::Scalar(132,130,193), out);
        cv::threshold(out, out, 0, 255.0, CV_THRESH_BINARY_INV);
        out.convertTo(out, CV_32F);

        result.push_back(out);
    }

    return result;
}

void ChessSampling::test(cv::Mat &screen) {
    QList<cv::Mat> list = grabSample(screen);
    QString path = Hub::current_dir();
    path.append("/resources/train/");
    for(cv::Mat mat : list) {
        QString name(path);
        name.append(QString::number(qrand())).append(".jpg");
        cv::imwrite(name.toStdString(), mat);
    }
}

QList<QRect> ChessSampling::filter(QList<QRect> list) {
    QRect extandRect = QRect(chessRect.left() - 10, chessRect.top() - 10, chessRect.width() + 10,
                             chessRect.height() + 10);
    QList<QRect> result;

    for (QRect rect : list) {
        if (extandRect.contains(rect.topLeft())) {
            result.push_back(rect);
        }
    }
    qDebug() << "input cicle size:" << list.size() << " filter:" << list.size() - result.size();
    return result;
}
