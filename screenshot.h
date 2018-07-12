//
// Created by xsy on 18-7-4.
//

#ifndef CHESSAGENT_SCREENSHOTTASK_H
#define CHESSAGENT_SCREENSHOTTASK_H

#include <glibmm.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

#include "application_utils.h"

// w:529 x h:800
#define SCREEN_WIDTH  529
#define SCREEN_HEIGHT 800

#define MIN_SAMPLE_NUMBER 50

class ScreenShot {
private:
    cv::Point top_left_;
    cv::Point top_right_;
    cv::Point bottom_left_;
    cv::Point bottom_right_;

    gint mask_size_x_;
    gint mask_size_y_;
    gint mask_redius_;

    cv::Point positions_[10][9];

    std::map<gint, gint> knn_chess_type_;
    std::map<gint, gint> knn_blank_type_;
    std::map<gint, gint> knn_sample_number_map_;

    cv::Ptr<cv::ml::KNearest> knn_model_;
public:
    ScreenShot();

    virtual ~ScreenShot();

private:
    /**
     * Generate positive knn train data to `pwd`/resources/dataset
     */
    void generate_train_pos_img(cv::Mat &screen);

    // generate negative knn train data to `pwd`/resources/dataset
    void generate_train_neg_img(cv::Mat &screen);

    void generate_train_mark_img(const cv::Mat &screen);

    gint point_to_knn_type(gint y, gint x);

    void knn_sample_stat();

    std::vector<cv::Vec3f> hough_detection_circle(cv::Mat &src, std::vector<cv::Vec3f> &circles);

    gint knn_predict(cv::Mat &mat);
public:
    cv::Mat screen_shot();

    void generate_base_train_data(void);

    void knn_train();

    void knn_predit_stat();

    /**
     * 检测象棋的位置
     * @param map
     */
    void detect_chess_position(cv::Mat &screen, std::map<gint, gint> &map);
};


#endif //CHESSAGENT_SCREENSHOTTASK_H
