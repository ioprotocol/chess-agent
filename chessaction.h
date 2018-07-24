//
// Created by xsy on 18-7-2.
//

#ifndef CHESSAGENT_CHESS_H
#define CHESSAGENT_CHESS_H

#include <opencv2/opencv.hpp>
#include <QPixmap>

class ChessAction {
private:
    // 558 x 620
    cv::Mat chess_board_;
    // 普通棋子
    cv::Mat normal_img[17];
    // 选中状态的棋子
    cv::Mat selected_img[17];

    cv::Point top_left_;
    cv::Point top_right_;
    cv::Point bottom_left_;
    cv::Point bottom_right_;

    cv::Point positions_[10][9];
private:
    int cv_add4c_mat_q(cv::Mat &dst, cv::Mat &scr, double scale);

public:
    ChessAction();

    virtual ~ChessAction();

    QPixmap generate_pixture(std::map<unsigned int, int> &chess_position_map);

    void save_to_disk();
};


#endif //CHESSAGENT_CHESS_H
