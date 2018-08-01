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

    /**
     * 根据象棋的枚举类型值获取中文名称
     *
     * @param type
     * @return
     */
    QString get_chess_name(int type);

    /**
     * 将象棋二维坐标转为一维坐标，方便计算
     *
     * @param point
     * @return
     */
    unsigned int point_to_uint32(cv::Point point);

    /**
     * 将象棋二维坐标转为一维坐标，方便计算
     *
     * @param x
     * @param y
     * @return
     */
    unsigned int point_to_uint32(int x, int y);

    /**
     * 将一维坐标转为二维坐标
     *
     * @param position
     * @return
     */
    cv::Point uint32_to_point(unsigned int position);

    /**
     * 计算两点间的距离
     *
     * @param point1
     * @param point2
     * @return
     */
    double get_distance_by_position(QPoint point1, QPoint point2);

    /**
     * 通过开局位置，监测棋盘坐标
     *
     * @param screen
     * @return
     */
    QRect detect_chess_board(cv::Mat &screen);

    /**
     * 检测图像中所有的圆形
     *
     * @param src
     * @param circles
     */
    QList<QRect> hough_detection_circle(cv::Mat &src);
}
#endif //CHESSAGENT_APPLICATION_UTILS_H
