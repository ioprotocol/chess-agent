//
// Created by xushy on 18-7-2.
//

#ifndef CHESSAGENT_APPLICATION_UTILS_H
#define CHESSAGENT_APPLICATION_UTILS_H

#include <QString>
#include <QRect>
#include <QList>
#include <opencv2/opencv.hpp>

#define PWD "EPWD"

#define _TEST_STD_OUT

namespace Hub {
    QString current_dir();
}

namespace Chess {
    enum Type {
        B_CHE = 10, B_MA = 11, B_XIANG = 12, B_SHI = 13, B_JIANG = 14, B_PAO = 15, B_ZU = 16,
        R_CHE = 0, R_MA = 1, R_XIANG = 2, R_SHI = 3, R_JIANG = 4, R_PAO = 5, R_ZU = 6
    };

    bool sort_circle(QRect &rt1, QRect &rt2);

    QString get_chess_name(int type);

    unsigned int point_to_uint32(cv::Point point);

    unsigned int point_to_uint32(int x, int y);

    cv::Point uint32_to_point(unsigned int position);

    double get_distance_by_position(QPoint point1, QPoint point2);

    QRect detect_chess_board(cv::Mat &screen);

    QList<QRect> hough_detection_circle(cv::Mat &src);

    bool hough_detection_single_circle(cv::Mat &src, QRect &out);

}
#endif //CHESSAGENT_APPLICATION_UTILS_H
