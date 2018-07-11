//
// Created by xsy on 18-7-4.
//

#ifndef CHESSAGENT_SCREENSHOTTASK_H
#define CHESSAGENT_SCREENSHOTTASK_H

#include <glibmm.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

#include "application_utils.h"

class ScreenShot {
private:
    cv::Point top_left_;
    cv::Point top_right_;
    cv::Point bottom_left_;
    cv::Point bottom_right_;
    cv::Point positions_[10][9];
    cv::Ptr<cv::ml::KNearest> knn_model;
public:
    ScreenShot();

    virtual ~ScreenShot();
public:
    cv::Mat screen_shot();

    cv::Mat screen_shot_test();
    /**
     * Generate knn train data to `pwd`/resources/dataset
     *
     * @param screen
     */
    void output_disk_by_screen_shot(cv::Mat screen);

    void splitScreenImg(cv::Mat &mat, cv::Mat arrays[][9]);

    void knn_train();

    gint knn_predit(cv::Mat &mat);
};


#endif //CHESSAGENT_SCREENSHOTTASK_H
