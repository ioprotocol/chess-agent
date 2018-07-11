//
// Created by xsy on 18-7-4.
//

#include "screenshot.h"

#include <glibmm.h>
#include <fstream>

#include <opencv2/imgproc.hpp>
#include <iostream>

#include "screenshot-utils.h"
#include "screenshot-config.h"

ScreenShot::ScreenShot() {
    init_screen_config();

    mask_redius_ = 18;
    // width = 540 height = 607
    top_left_ = cv::Point(83, 144);
    top_right_ = cv::Point(623, 144);
    bottom_left_ = cv::Point(83, 751);
    bottom_right_ = cv::Point(623, 751);

    gdouble dx = (top_right_.x - top_left_.x) / 8.0;
    gdouble dy = (bottom_left_.y - top_left_.y) / 9.0;

    gint y = 0, x = 0;
    for( y = 0; y < 10; y++)
    {
        for(x = 0; x < 9; x++)
        {
            positions_[y][x].x = top_left_.x + dx * x;
            positions_[y][x].y = top_left_.y + dy * y;
        }
    }

    // mark black chess with initial postion
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 0), (gint) Chess::B_CHE));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 1), (gint) Chess::B_MA));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 2), (gint) Chess::B_XIANG));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 3), (gint) Chess::B_SHI));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 4), (gint) Chess::B_JIANG));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 5), (gint) Chess::B_SHI));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 6), (gint) Chess::B_XIANG));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 7), (gint) Chess::B_MA));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 8), (gint) Chess::B_CHE));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(2, 1), (gint) Chess::B_PAO));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(2, 7), (gint) Chess::B_PAO));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(3, 0), (gint) Chess::B_ZU));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(3, 2), (gint) Chess::B_ZU));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(3, 4), (gint) Chess::B_ZU));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(3, 6), (gint) Chess::B_ZU));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(3, 8), (gint) Chess::B_ZU));
    // mark red chess with initial postion
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(9, 0), (gint) Chess::B_CHE));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(9, 1), (gint) Chess::B_MA));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(9, 2), (gint) Chess::R_XIANG));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(9, 3), (gint) Chess::R_SHI));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(9, 4), (gint) Chess::R_JIANG));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(9, 5), (gint) Chess::R_SHI));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(9, 6), (gint) Chess::R_XIANG));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(9, 7), (gint) Chess::B_MA));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(9, 8), (gint) Chess::B_CHE));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(7, 1), (gint) Chess::B_PAO));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(7, 7), (gint) Chess::B_PAO));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(6, 0), (gint) Chess::R_ZU));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(6, 2), (gint) Chess::R_ZU));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(6, 4), (gint) Chess::R_ZU));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(6, 6), (gint) Chess::R_ZU));
    knn_chess_type_.insert(std::pair<gint, gint>(point_to_knn_type(6, 8), (gint) Chess::R_ZU));
    // mark blank board to knn type
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 0), 9000));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 1), 9001));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 2), 9001));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 4), 9001));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 6), 9001));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 7), 9001));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 3), 9002));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 5), 9003));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(0, 8), 9004));

    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(1, 0), 9005));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(1, 1), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(1, 2), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(1, 3), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(1, 4), 9007));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(1, 5), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(1, 6), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(1, 7), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(1, 8), 9008));

    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(2, 0), 9005));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(2, 1), 9009));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(2, 2), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(2, 3), 9010));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(2, 4), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(2, 5), 9011));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(2, 6), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(2, 7), 9009));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(2, 8), 9008));

    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(3, 0), 9012));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(3, 1), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(3, 2), 9009));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(3, 3), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(3, 4), 9009));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(3, 5), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(3, 6), 9009));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(3, 7), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(3, 8), 9013));

    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(4, 0), 9005));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(4, 1), 9014));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(4, 2), 9014));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(4, 3), 9014));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(4, 4), 9014));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(4, 5), 9014));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(4, 6), 9014));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(4, 7), 9014));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(4, 8), 9008));

    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(5, 0), 9005));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(5, 1), 9015));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(5, 2), 9015));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(5, 3), 9015));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(5, 4), 9015));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(5, 5), 9015));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(5, 6), 9015));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(5, 7), 9015));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(5, 8), 9008));

    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(6, 0), 9012));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(6, 1), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(6, 2), 9009));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(6, 3), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(6, 4), 9009));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(6, 5), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(6, 6), 9009));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(6, 7), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(6, 8), 9013));

    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(7, 0), 9005));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(7, 1), 9009));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(7, 2), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(7, 3), 9016));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(7, 4), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(7, 5), 9017));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(7, 6), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(7, 7), 9009));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(7, 8), 9008));

    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 0), 9005));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 1), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 2), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 3), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 4), 9007));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 5), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 6), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 7), 9006));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 8), 9008));

    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 0), 9018));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 1), 9019));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 2), 9019));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 3), 9020));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 4), 9019));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 5), 9021));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 6), 9019));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 7), 9019));
    knn_blank_type_.insert(std::pair<gint, gint>(point_to_knn_type(8, 8), 9022));

    knn_model_ = cv::ml::StatModel::load<cv::ml::KNearest>(Glib::build_filename(Hub::get_resources_path(), "knn.xml"));
}

ScreenShot::~ScreenShot() {
    g_free(screenshot_config);
}

cv::Mat ScreenShot::screen_shot() {
    GOutputStream *outputStream;
    GdkPixbuf *gdkPixbuf;
    gpointer pixBuffer;
    gsize size;
    uchar *data;
    std::vector<int> params;

    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(100);

    outputStream = g_memory_output_stream_new_resizable();
    gdkPixbuf = screenshot_fallback_get_pixbuf(NULL);

    gdk_pixbuf_save_to_stream(gdkPixbuf, outputStream, "jpeg", NULL, NULL, "quality", "100", NULL);

    pixBuffer = g_memory_output_stream_get_data(G_MEMORY_OUTPUT_STREAM(outputStream));
    size = g_memory_output_stream_get_size(G_MEMORY_OUTPUT_STREAM(outputStream));

    data = (uchar *) pixBuffer;

    std::vector<uchar> vec_data(&data[0], &data[0] + size);

    cv::Mat mat = cv::imdecode(vec_data, cv::IMREAD_COLOR);

    g_object_unref(gdkPixbuf);
    g_object_unref(outputStream);

    std::cout << "screen shot @ " << Glib::DateTime::create_now_local().format("%Y-%m-%d %H:%M:%S") << std::endl;

    return mat;
}

/**
 * Just for dev test.
 *
 * @return
 */
cv::Mat ScreenShot::screen_shot_test() {
    return cv::imread(Glib::build_filename(Hub::get_resources_img_path(), "demo.jpeg"), cv::IMREAD_COLOR);
}

void ScreenShot::split_screen_img(cv::Mat &mat, cv::Mat **arrays) {
    cv::Mat mask = cv::Mat::zeros(48, 48, mat.type());
    cv::circle(mask, cv::Point(24, 24), 21, CV_RGB(255, 255, 255), -1);

    gint y = 0, x = 0;
    for( y = 0; y < 10; y++)
    {
        for(x = 0; x < 9; x++)
        {
            cv::Rect rect = cv::Rect(positions_[y][x].x - 24, positions_[y][x].y - 24, 48, 48);
            cv::Mat roi = mat(rect);
            cv::Mat split = cv::Mat::zeros(48, 48, mat.type());

            roi.copyTo(split, mask);
            arrays[y][x] = split;
            roi.release();
        }
    }
    mask.release();
}

/**
 * KNN xunlian
 */
void ScreenShot::knn_train() {
    // 样本数量
    int sambles = 90;
    // 样本有限，每个样本重复训练100次
    int repeat = 50;

    int train_num = sambles * repeat;

    cv::Mat src_data,src_labels;
    std::string img_name;
    cv::Mat gray;

    std::ifstream file_name_stream(Glib::build_filename(Hub::get_resources_path(), "train.txt"));
    for(int i = 0; i < sambles; i++) {
        getline(file_name_stream, img_name);
        int idx = img_name.find("_", 4);
        std::string prefix = img_name.substr(4, idx);
        int value = std::stoi(prefix);

        std::cout << img_name << ":" << value << std::endl;
        for(int o = 0; o < repeat; o++) {
            cv::Mat src = cv::imread(Glib::build_filename(Hub::get_resources_path(), "dataset", img_name));
            cvtColor(src, gray, CV_BGR2GRAY);
            src_data.push_back(gray.reshape(0, 1));
            src_labels.push_back(value);
        }
    }

    file_name_stream.close();

    src_data.convertTo(src_data, CV_32F); //uchar型转换为cv_32f

    cv::Mat trainData, trainLabels;
    trainData = src_data(cv::Range(0, train_num), cv::Range::all());
    trainLabels = src_labels(cv::Range(0, train_num), cv::Range::all());

    //使用KNN算法
    int K = 5;

    cv::Ptr<cv::ml::TrainData> tData = cv::ml::TrainData::create(trainData, cv::ml::ROW_SAMPLE, trainLabels);
    cv::Ptr<cv::ml::KNearest> model = cv::ml::KNearest::create();

    model->setDefaultK(K);
    model->setIsClassifier(true);
    model->train(tData);

    model->save(Glib::build_filename(Hub::get_resources_path(), "knn.xml"));
    model->clear();
    std::cout << "train finish:" << std::endl;
}

gint ScreenShot::knn_predit(cv::Mat &mat) {
    cv::inRange(mat, cv::Scalar(0,0,47), cv::Scalar(255,255,183), mat);
    cv::threshold(mat, mat, 0, 255.0, CV_THRESH_BINARY_INV);
//    cvtColor(mat, mat, CV_BGR2GRAY);
    mat.convertTo(mat, CV_32F);
    float r = knn_model_->predict(mat.reshape(0, 1));
    return (gint)r;
}

void ScreenShot::generate_train_pos_img(cv::Mat screen) {
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(100);

    cv::Mat mask = cv::Mat::zeros(48, 48, screen.type());
    cv::circle(mask, cv::Point(24, 24), mask_redius_, CV_RGB(255, 255, 255), -1);

    std::string file_name;

    gint y = 0, x = 0;
    for( y = 0; y < 10; y++)
    {
        for(x = 0; x < 9; x++)
        {
            if (knn_chess_type_.count(point_to_knn_type(y, x)) == 1) {
                cv::Rect rect = cv::Rect(positions_[y][x].x - 24, positions_[y][x].y - 24, 48, 48);
                cv::Mat roi = screen(rect);
                cv::Mat split = cv::Mat::zeros(48, 48, screen.type());

                roi.copyTo(split, mask);

                cv::Mat threshold;

                cv::inRange(split, cv::Scalar(0,0,47), cv::Scalar(255,255,183), threshold);
                cv::threshold(threshold, threshold, 0, 255.0, CV_THRESH_BINARY_INV);

                file_name.append("pos_").append(std::to_string(knn_chess_type_.at(point_to_knn_type(y, x))));
                file_name.append("_").append(std::to_string(y)).append(std::to_string(x));
                file_name.append(".jpg");
                std::cout << file_name << std::endl;
                file_name = Glib::build_filename(Hub::get_resources_path(), "dataset", file_name);

                cv::imwrite(file_name, threshold, params);
                file_name.clear();
            }
        }
    }

}

void ScreenShot::generate_train_neg_img(cv::Mat screen) {
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(100);

    cv::Mat mask = cv::Mat::zeros(48, 48, screen.type());
    cv::circle(mask, cv::Point(24, 24), mask_redius_, CV_RGB(255, 255, 255), -1);

    std::string file_name;

    gint y = 0, x = 0;
    for( y = 0; y < 10; y++)
    {
        for(x = 0; x < 9; x++)
        {
            if (knn_blank_type_.count(point_to_knn_type(y, x)) == 1) {
                cv::Rect rect = cv::Rect(positions_[y][x].x - 24, positions_[y][x].y - 24, 48, 48);
                cv::Mat roi = screen(rect);
                cv::Mat split = cv::Mat::zeros(48, 48, screen.type());

                roi.copyTo(split, mask);

                cv::Mat threshold;

                cv::inRange(split, cv::Scalar(0,0,47), cv::Scalar(255,255,183), threshold);
                cv::threshold(threshold, threshold, 0, 255.0, CV_THRESH_BINARY_INV);

                file_name.append("neg_").append(std::to_string(knn_blank_type_.at(point_to_knn_type(y, x))));
                file_name.append("_").append(std::to_string(y)).append(std::to_string(x));
                file_name.append(".jpg");
                std::cout << file_name << std::endl;
                file_name = Glib::build_filename(Hub::get_resources_path(), "dataset", file_name);

                cv::imwrite(file_name, threshold, params);
                file_name.clear();
            }
        }
    }
}

gint ScreenShot::point_to_knn_type(gint y, gint x) {
    return y * 10 + x;
}
