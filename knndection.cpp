//
// Created by xsy on 18-7-13.
//

#include "knndection.h"
#include <giomm.h>
#include <glibmm.h>

#include "application_utils.h"

KnnDection::KnnDection() {
    std::string file_path = Glib::build_filename(Hub::get_resources_path(), "knn.xml");
    Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(file_path);

    if (file->query_exists()) {
        knn_model_ = cv::ml::StatModel::load<cv::ml::KNearest>(file_path);
    } else {
        knn_model_ = cv::ml::KNearest::create();
        knn_model_->setDefaultK(5);
        knn_model_->setIsClassifier(true);
    }
}

void KnnDection::train(std::list<Sample> &samples) {

    std::map<gint, gint> knn_sample_number_map_;
    cv::Mat src_data,src_labels;

    samples.sort();

    for (Sample sample : samples) {
        if (knn_sample_number_map_.find(sample.label()) != knn_sample_number_map_.end()) {
            knn_sample_number_map_[sample.label()] = knn_sample_number_map_[sample.label()] + 1;
        } else {
            knn_sample_number_map_[sample.label()] = 1;
        }
    }

    // 样本数量
    int train_num = knn_sample_number_map_.size() * MIN_SAMPLE_NUMBER;
    gint last_type = -1;

    for(Sample sample : samples) {
        if(last_type != sample.label() && last_type != -1) {
            int leave = knn_sample_number_map_.at(last_type);
            if(MIN_SAMPLE_NUMBER - leave > 0) {
                for(int i = 0; i < MIN_SAMPLE_NUMBER - leave; i++) {
                    src_data.push_back(format(sample.mat()));
                    src_labels.push_back(last_type);
                }
            }
        }
        if (last_type == samples.back().label()) {
            int leave = knn_sample_number_map_.at(last_type);
            if(MIN_SAMPLE_NUMBER - leave > 0) {
                for(int i = 0; i < MIN_SAMPLE_NUMBER - leave; i++) {
                    src_data.push_back(format(sample.mat()));
                    src_labels.push_back(last_type);
                }
            }
        }
        src_data.push_back(format(sample.mat()));
        src_labels.push_back(sample.label());
        last_type = sample.label();
    }

    src_data.convertTo(src_data, CV_32F); //uchar型转换为cv_32f

    cv::Mat trainData, trainLabels;
    trainData = src_data(cv::Range(0, train_num), cv::Range::all());
    trainLabels = src_labels(cv::Range(0, train_num), cv::Range::all());

    //使用KNN算法
    cv::Ptr<cv::ml::TrainData> tData = cv::ml::TrainData::create(trainData, cv::ml::ROW_SAMPLE, trainLabels);

    if (knn_model_->isTrained()) {
        knn_model_->train(tData, 1);
    } else {
        knn_model_->train(tData);
    }
}

cv::Mat KnnDection::format(cv::Mat &mat) {
    cv::Mat out;
    cv::inRange(mat, cv::Scalar(0,0,47), cv::Scalar(255,255,183), out);
    cv::threshold(out, out, 0, 255.0, CV_THRESH_BINARY_INV);
    cv::cvtColor(out, out, CV_BGR2GRAY);
    return out.reshape(0, 1);
}

gint KnnDection::predict(cv::Mat &mat) {
    cv::Mat tmp;
    cv::inRange(mat, cv::Scalar(0,0,47), cv::Scalar(255,255,183), tmp);
    cv::threshold(mat, tmp, 0, 255.0, CV_THRESH_BINARY_INV);
    tmp.convertTo(tmp, CV_32F);
    return (gint) knn_model_->predict(tmp.reshape(0, 1));
}

KnnDection::~KnnDection() {
    knn_model_->save(Glib::build_filename(Hub::get_resources_path(), "knn.xml"));
}

