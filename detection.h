//
// Created by xsy on 18-7-13.
//

#ifndef CHESSAGENT_DETECTION_H
#define CHESSAGENT_DETECTION_H

#include <glibmm.h>
#include <opencv2/opencv.hpp>

class Sample {
private:
    cv::Mat mat_;
    gint label_;
    cv::Point position_;
public:
    Sample(const cv::Mat &mat, gint label, cv::Point &position) : mat_(mat), label_(label), position_(position) {}

    bool operator < (const Sample& ti) const {
        return label_ < ti.label_;
    }

    inline cv::Mat &mat() {
        return mat_;
    }

    inline gint label() {
        return label_;
    }

    inline cv::Point &position() {
        return position_;
    }
};

class Detection {
public:
    /**
     * Train with generate sample
     */
    virtual void train(std::list<Sample> &samples) = 0;

    virtual gboolean is_trained() = 0;

    virtual gint predict(cv::Mat &mat) = 0;
};


#endif //CHESSAGENT_DETECTION_H
