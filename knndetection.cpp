//
// Created by xsy on 18-7-13.
//

#include "knndetection.h"

#include <QFile>
#include "applicationutils.h"

#define MODEL_FILE_NAME "/resources/knn.xml"

KnnDection::KnnDection() {
    QString knn_file_path = Hub::current_dir() + MODEL_FILE_NAME;
    QFile file(knn_file_path);

    if (file.exists()) {
        knn_model_ = cv::ml::StatModel::load<cv::ml::KNearest>(knn_file_path.toStdString());
    } else {
        knn_model_ = cv::ml::KNearest::create();
        knn_model_->setDefaultK(5);
        knn_model_->setIsClassifier(true);
    }
}

void KnnDection::train(std::list<Sample> &samples) {
    cv::Mat src_data,src_labels;
    samples.sort();

    int train_num = samples.size() * MIN_SAMPLE_NUMBER;

        for(int i = 0; i < MIN_SAMPLE_NUMBER; i++) {
            for (Sample sample : samples) {
                src_data.push_back(format(sample.mat()));
                src_labels.push_back(sample.label());
            }
        }

    src_data.convertTo(src_data, CV_32F);

    cv::Mat trainData, trainLabels;
    trainData = src_data(cv::Range(0, train_num), cv::Range::all());
    trainLabels = src_labels(cv::Range(0, train_num), cv::Range::all());

    cv::Ptr<cv::ml::TrainData> tData = cv::ml::TrainData::create(trainData, cv::ml::ROW_SAMPLE, trainLabels);

    if (knn_model_->isTrained()) {
        knn_model_->train(tData, 1);
    } else {
        knn_model_->train(tData);
    }
    knn_model_->save((Hub::current_dir() + MODEL_FILE_NAME).toStdString());
}

cv::Mat KnnDection::format(cv::Mat &mat) {
    cv::Mat out;
    cv::inRange(mat, cv::Scalar(0,0,47), cv::Scalar(255,255,183), out);
    cv::threshold(out, out, 0, 255.0, CV_THRESH_BINARY_INV);
    out.convertTo(out, CV_32F);
    return out.reshape(0, 1);
}

int KnnDection::predict(cv::Mat &mat) {
    return (int) knn_model_->predict(format(mat));
}

KnnDection::~KnnDection() {
    knn_model_->save((Hub::current_dir() + MODEL_FILE_NAME).toStdString());
}

bool KnnDection::is_trained() {
    return knn_model_->isTrained();
}

