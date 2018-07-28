//
// Created by xushuyang on 2018/7/27.
//

#include "cvanndetection.h"

CvANNDection::CvANNDection() {
    model = cv::ml::ANN_MLP::create();
    // ÿһ����Ԫ����
    int ar[]={1,2,3};
    // ����������
    cv::Mat layerSizes(1,3,CV_32S,ar);

    model->setLayerSizes(layerSizes);

    //�����
    model->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM);
    //MLP��ѵ������
    model->setTrainMethod(cv::ml::ANN_MLP::BACKPROP,0.1,0.9);

    //ѵ��ģ��
//    cv::Ptr<cv::ml::TrainData> trainData = cv::ml::TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);
//    model->train(trainData);

//    Ptr<ANN_MLP> bp = ANN_MLP::load<ANN_MLP>("*.xml");

    // https://blog.csdn.net/akadiao/article/details/79236458
}

void CvANNDection::train(std::list<Sample> &samples) {

}

bool CvANNDection::is_trained() {
    return false;
}

int CvANNDection::predict(cv::Mat &mat) {
    return 0;
}

cv::Mat CvANNDection::format(cv::Mat &mat) {
    return cv::Mat();
}


