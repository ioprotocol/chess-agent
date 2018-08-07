//
// Created by xsy on 18-7-13.
//

#ifndef CHESSAGENT_DETECTION_H
#define CHESSAGENT_DETECTION_H

#include <opencv2/opencv.hpp>

class Sample {
private:
    cv::Mat mat_;
    int label_;
    cv::Point position_;
public:
    Sample(const cv::Mat &mat, int label, cv::Point &position) : mat_(mat), label_(label), position_(position) {}

    bool operator < (const Sample& ti) const {
        return label_ < ti.label_;
    }

    inline cv::Mat &mat() {
        return mat_;
    }

    inline int label() {
        return label_;
    }

    inline cv::Point &position() {
        return position_;
    }
};

class Detection {
public:
    virtual cv::Mat format(cv::Mat &mat) = 0;
    /**
     * Train with generate sample
     */
    virtual void train() = 0;

    virtual bool is_trained() = 0;

    virtual int predict(cv::Mat &mat) = 0;
};


#endif //CHESSAGENT_DETECTION_H
