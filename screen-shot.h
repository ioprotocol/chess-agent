//
// Created by xsy on 18-7-4.
//

#ifndef CHESSAGENT_SCREENSHOTTASK_H
#define CHESSAGENT_SCREENSHOTTASK_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

#include "application-utils.h"

class ScreenShot {
private:
private:
    cv::Point topLeft;
    cv::Point topRight;
    cv::Point bottomLeft;
    cv::Point bottomRight;
    cv::Point positions[10][9];
    cv::Ptr<cv::ml::KNearest> knnModel;
public:
    ScreenShot();

    virtual ~ScreenShot();
public:
    cv::Mat shot();

    void splitScreenImg(cv::Mat &mat, cv::Mat arrays[][9]);

    gdouble compareHist(const cv::Mat &mat1, const cv::Mat &mat2);

//    void matchTemplateTest(cv::Mat &src);

    void knnTrain();

    gint knnPredit(cv::Mat &mat);
};


#endif //CHESSAGENT_SCREENSHOTTASK_H
