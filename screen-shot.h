//
// Created by xsy on 18-7-4.
//

#ifndef CHESSAGENT_SCREENSHOTTASK_H
#define CHESSAGENT_SCREENSHOTTASK_H

#include <opencv2/imgcodecs.hpp>

class ScreenShot {
private:
private:
    cv::Point topLeft;
    cv::Point topRight;
    cv::Point bottomLeft;
    cv::Point bottomRight;
    cv::Point positions[10][9];
    cv::Mat bChe;
    cv::Mat rChe;
public:
    ScreenShot();

    virtual ~ScreenShot();
public:
    cv::Mat shot();

    void split_screen_shot_img(cv::Mat &mat, cv::Mat arrays[10][9]);

    double compareHist(cv::Mat &mat1, cv::Mat &mat2);

    double isBChe(cv::Mat &mat);
};


#endif //CHESSAGENT_SCREENSHOTTASK_H
