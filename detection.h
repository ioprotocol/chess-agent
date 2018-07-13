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
public:
    Sample(const cv::Mat &mat, gint label) : mat_(mat), label_(label) {}

    bool operator < (const Sample& ti) const {
        return label_ < ti.label_;
    }

    inline cv::Mat &mat() {
        return mat_;
    }

    inline gint label() {
        return label_;
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
