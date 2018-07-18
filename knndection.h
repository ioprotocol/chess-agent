//
// Created by xsy on 18-7-13.
//

#ifndef CHESSAGENT_KNNDECTION_H
#define CHESSAGENT_KNNDECTION_H

#include "detection.h"

#define MIN_SAMPLE_NUMBER 50

class KnnDection : public Detection {
private:
public:
    KnnDection();

    virtual ~KnnDection();

private:
    cv::Ptr<cv::ml::KNearest> knn_model_;

    /**
     * Format the source image to a standard mat.
     *
     * @param mat
     * @return
     */
    cv::Mat format(cv::Mat &mat);
public:
    void train(std::list<Sample> &samples) override;

    bool is_trained() override;

    int predict(cv::Mat &mat) override;
};


#endif //CHESSAGENT_KNNDECTION_H
