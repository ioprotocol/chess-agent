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
void output_samples_to_disk(std::list<Sample> &samples) {
    Glib::Rand rand;
    for (Sample sample : samples) {
        cv::imwrite(Glib::build_filename(Hub::get_resources_path(), "test", "p1", std::to_string(sample.label()) + "_" + std::to_string(rand.get_int()) + ".jpg"), sample.mat());
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
    knn_model_->save(Glib::build_filename(Hub::get_resources_path(), "knn.xml"));
}

cv::Mat KnnDection::format(cv::Mat &mat) {
    cv::Mat out;
    cv::inRange(mat, cv::Scalar(0,0,47), cv::Scalar(255,255,183), out);
    cv::threshold(out, out, 0, 255.0, CV_THRESH_BINARY_INV);
    out.convertTo(out, CV_32F);
    return out.reshape(0, 1);
}

gint KnnDection::predict(cv::Mat &mat) {
    return (gint) knn_model_->predict(format(mat));
}

KnnDection::~KnnDection() {
    knn_model_->save(Glib::build_filename(Hub::get_resources_path(), "knn.xml"));
}

gboolean KnnDection::is_trained() {
    return knn_model_->isTrained();
}

