//
// Created by xushuyang on 2018/7/27.
//

#include "cvanndetection.h"

#include <QString>
#include "applicationutils.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>

CvANNDection::CvANNDection() {

    QString knn_file_path = Hub::current_dir().append("/resources/ann.xml");
    QFile file(knn_file_path);

    if (file.exists()) {
        model = cv::ml::StatModel::load<cv::ml::ANN_MLP>(knn_file_path.toStdString());
    } else {
        model = cv::ml::ANN_MLP::create();
        // 每一层神经元数量
        int ar[]={1,2,3};
        // 三层神经网络
        cv::Mat layerSizes(1,3,CV_32S,ar);
        model->setLayerSizes(layerSizes);
        //激活函数
        model->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM);
        //MLP的训练方法
        model->setTrainMethod(cv::ml::ANN_MLP::BACKPROP,0.1,0.9);
    }

    // https://blog.csdn.net/akadiao/article/details/79236458
}

void CvANNDection::train() {
    QString knn_file_path = Hub::current_dir().append("/resources/ann.xml");

    cv::Mat src_data,src_labels;

    QString path = Hub::current_dir().append("/resources/train/");
    int train_num = 0;

//    for(int i = 0; i < MIN_SAMPLE_NUMBER; i++) {
//        for (Sample sample : samples) {
//            src_data.push_back(format(sample.mat()));
//            src_labels.push_back(sample.label());
//        }
//    }

    src_data.convertTo(src_data, CV_32F);

    cv::Mat trainData, trainLabels;
    trainData = src_data(cv::Range(0, train_num), cv::Range::all());
    trainLabels = src_labels(cv::Range(0, train_num), cv::Range::all());

    cv::Ptr<cv::ml::TrainData> tData = cv::ml::TrainData::create(trainData, cv::ml::ROW_SAMPLE, trainLabels);

    model->train(tData);
    model->save(knn_file_path.toStdString());
}

bool CvANNDection::is_trained() {
    return model->isTrained();
}

int CvANNDection::predict(cv::Mat &mat) {
    return 0;
}

cv::Mat CvANNDection::format(cv::Mat &mat) {
    return cv::Mat();
}


