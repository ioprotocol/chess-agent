//
// Created by xsy on 18-7-4.
//

#ifndef CHESSAGENT_SCREENSHOTTASK_H
#define CHESSAGENT_SCREENSHOTTASK_H

#include <glibmm.h>
#include <opencv2/opencv.hpp>

#include "application_utils.h"
#include "detection.h"

class Circle {
private:
    cv::Point center_;
    gint radius_;

public:
    Circle() {}

    Circle(cv::Point center, gint radius) : center_(center), radius_(radius) {}

    inline cv::Point &center() {
        return center_;
    }

    inline gint radius() {
        return radius_;
    }

    bool operator < (const Circle& other) const {
        if(abs(center_.y - other.center_.y) < 15) {
            return center_.x < other.center_.x;
        }
        return Chess::point_to_position(center_) < Chess::point_to_position(other.center_);
    }

    void set_center(const cv::Point &center) {
        center_.x = center.x;
        center_.y = center.y;
    }

    void set_radius(gint radius) {
        radius_ = radius;
    }
};

class ScreenShot {
private:
    cv::Point left_top_;
    cv::Point right_bottom_;
    Detection *p_detection_;
    gint chess_position_type_[32];
public:
    ScreenShot();

    virtual ~ScreenShot();

private:
    cv::Mat screen_shot();

    /**
     * 检测图像中的所有circle
     * @param src
     * @param circles
     */
    void hough_detection_circle(cv::Mat &src, std::vector<cv::Vec3f> &circles);

    /**
     * 只检测图像中的单个园
     * @param src
     * @param circle
     */
    void hough_detection_circle_single(cv::Mat &src, Circle &circle);

    /**
     * 自动学习识别棋盘坐标
     *
     * @param circle_list
     */
    void study(std::list<Circle> &circle_list);

    /**
     * 根据识别的圆形坐标自动识别棋子完整样本
     * @param circle_list
     * @param screen
     * @param smples
     */
    void grab_samles(std::list<Circle> &circle_list, cv::Mat &screen, std::list<Sample> &samle_list);
    /**
     * 棋盘位于开局阶段，自动训练识别算法
     * @param circle_list
     * @param screen
     * @return
     */
    gint auto_train(std::list<Circle> &circle_list, cv::Mat &screen);
public:

    gint detect_chess_position(std::map<guint32, gint> &map);
};


#endif //CHESSAGENT_SCREENSHOTTASK_H
