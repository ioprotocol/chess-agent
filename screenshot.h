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

    void hough_detection_circle(cv::Mat &src, std::vector<cv::Vec3f> &circles);

    void study(std::list<Circle> &circle_list);
public:

    gint detect_chess_position(std::map<guint32, gint> &map);
};


#endif //CHESSAGENT_SCREENSHOTTASK_H
