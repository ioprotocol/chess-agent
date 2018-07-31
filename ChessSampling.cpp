//
// Created by xushuyang on 2018/7/31.
//

#include "ChessSampling.h"


ChessSampling::ChessSampling(const QRect &chessRect) : chessRect(chessRect) {}

QList<cv::Mat> ChessSampling::grabSample(cv::Mat &screen) {
    QList<cv::Mat> result;
    float dx = this->chessRect.width() / 9.0;
    float dy = this->chessRect.height() / 10.0;

    for(int x = 0; x < 9; x++) {
        for(int y = 0; y < 10; y++) {
            cv::Rect rect(chessRect.left() + x * dx - 32, chessRect.top() + dy * y - 32, 64, 64);
            cv::Mat roi = screen(rect);
        }
    }

    return result;
}

void ChessSampling::houghDetectionCircle(cv::Mat &src, std::vector<cv::Vec3f> &circles) {
    cv::Mat src_gray;
    cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
    GaussianBlur(src_gray, src_gray, cv::Size(3, 3), 2, 2);
    HoughCircles(src_gray, circles, cv::HOUGH_GRADIENT, 1, 25, 208, 40, 15, 30);
}

bool ChessSampling::houghDetectionCircle(cv::Mat &src, QRect &circle) {
    std::vector<cv::Vec3f> circles;
    houghDetectionCircle(src, circles);

    if (circles.size() < 1) {
        qErrnoWarning("hough_detection_circle_single err");
        return false;
    }
    if(circles.size() > 1) {
        std::cout << "more than one cirlce" << std::endl;
        return false;
    }
    QPoint center(cvRound(circles[0][0]), cvRound(circles[0][1]));
    int radius = cvRound(circles[0][2]);
    circle.setTopLeft(center);
    circle.setWidth(radius);
    circle.setHeight(radius);
    return true;
}