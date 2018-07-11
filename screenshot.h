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

    gint mask_redius_;

    cv::Point positions_[10][9];

    std::map<gint, gint> knn_chess_type_;
    std::map<gint, gint> knn_blank_type_;

    cv::Ptr<cv::ml::KNearest> knn_model_;
public:
    ScreenShot();

    virtual ~ScreenShot();
public:
    cv::Mat screen_shot();

    cv::Mat screen_shot_test();
    /**
     * Generate positive knn train data to `pwd`/resources/dataset
     *
     * @param screen
     */
    void generate_train_pos_img(cv::Mat screen);

    // generate negative knn train data to `pwd`/resources/dataset
    void generate_train_neg_img(cv::Mat screen);

    void split_screen_img(cv::Mat &mat, cv::Mat **arrays);

    gint point_to_knn_type(gint y, gint x);

    void knn_train();

    gint knn_predit(cv::Mat &mat);
};


#endif //CHESSAGENT_SCREENSHOTTASK_H
