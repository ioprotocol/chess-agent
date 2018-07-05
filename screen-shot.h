//
// Created by xsy on 18-7-4.
//

#ifndef CHESSAGENT_SCREENSHOTTASK_H
#define CHESSAGENT_SCREENSHOTTASK_H

#include <opencv2/imgcodecs.hpp>

#include "application-utils.h"

class ScreenShot {
private:
private:
    gchar *tencent_chess_names[14];

    cv::Point topLeft;
    cv::Point topRight;
    cv::Point bottomLeft;
    cv::Point bottomRight;
    cv::Point positions[10][9];
    cv::Mat tencentChessFeatures[14];
public:
    ScreenShot();

    virtual ~ScreenShot();
public:
    cv::Mat shot();

    void split_screen_shot_img(cv::Mat &mat, cv::Mat arrays[10][9]);

    gdouble compareHist(const cv::Mat &mat1, const cv::Mat &mat2);

    ChessType chessType(const cv::Mat &mat);

    void compareTest(cv::Mat &mat);

    void matchTemplateTest(cv::Mat &src);
};


#endif //CHESSAGENT_SCREENSHOTTASK_H
