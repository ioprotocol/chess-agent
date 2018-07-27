//
// Created by xushuyang on 2018/7/27.
//

#ifndef CHESSAGENT_CVANNDECTION_H
#define CHESSAGENT_CVANNDECTION_H

#include <opencv2/opencv.hpp>

#include "detection.h"

class CvANNDection : public Detection {
public:
    CvANNDection();

private:
    cv::Ptr<cv::ml::ANN_MLP> model;
public:
    void train(std::list<Sample> &samples) override;

    bool is_trained() override;

    int predict(cv::Mat &mat) override;
};


#endif //CHESSAGENT_CVANNDECTION_H
