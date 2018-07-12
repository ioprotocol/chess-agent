//
// Created by xsy on 18-7-4.
//

#include "screenshot.h"

#include <glibmm.h>
#include <gtkmm.h>
#include <fstream>

#include <opencv2/imgproc.hpp>
#include <iostream>
#include <gtk/gtk.h>
#include <time.h>

#include "application_utils.h"

ScreenShot::ScreenShot() {

    knn_sample_stat();

    top_left_ = cv::Point(35, 156);
    top_right_ = cv::Point(441, 156);
    bottom_left_ = cv::Point(35, 613);
    bottom_right_ = cv::Point(441, 613);

    gdouble dx = (top_right_.x - top_left_.x) / 8.0;
    gdouble dy = (bottom_left_.y - top_left_.y) / 9.0;

    mask_size_x_ = 48;
    mask_size_y_ = 48 * dy / dx;
    mask_redius_ = 18;

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
}

cv::Mat ScreenShot::screen_shot() {
    gdk_threads_enter();
    Glib::RefPtr<Gdk::Display> display = Gdk::Display::get_default();
    if (!display) {
        std::cout << "default display is null" << std::endl;
    }

    Glib::RefPtr<Gdk::Screen> screen = display->get_default_screen();

    Glib::RefPtr<Gdk::Window> window = screen->get_active_window();
    Glib::RefPtr<Gdk::Window> root_window = screen->get_root_window();

    if (!window) {
        std::cout << "root window is null " << std::endl;
    }

    Gdk::Rectangle frame_extras;

    window->get_frame_extents(frame_extras);
    int x, y;
    window->get_origin(x, y);

    GdkPixbuf *pixbuf = gdk_pixbuf_get_from_window(root_window->gobj(), x, y, window->get_width(), window->get_height());

    cv::Mat mat = Hub::pixbuffer_to_mat(pixbuf);
    std::cout << Glib::DateTime::create_now_local().format("%y-%m-%d %H:%M:%S") << "@x:" << x << "y:" << y << "width:"
              << window->get_width() << "height:" << window->get_height() << std::endl;
    gdk_threads_leave();
    return mat;
}

/**
 * KNN xunlian
 */
void ScreenShot::knn_train() {
    clock_t start_time = clock();
    knn_sample_stat();

    cv::Mat src_data,src_labels;
    cv::Mat gray;

    // 样本数量
    int sambles = knn_sample_number_map_.size();
    int train_num = sambles * MIN_SAMPLE_NUMBER;

    Glib::Dir sample_dir(Glib::build_filename(Hub::get_resources_path(), "dataset"));
    std::list<std::string> file_names(sample_dir.begin(), sample_dir.end());
    file_names.sort();

    gint last_type = -1;
    std::string prev_file_name;

    for(std::string file_name : file_names) {

        if (file_name.find("pos") == std::string::npos && file_name.find("neg") == std::string::npos) {
            continue;
        }

        int idx = file_name.find('_', 4);
        std::string prefix = file_name.substr(4, idx - 4);
        int value = std::stoi(prefix);

        if(last_type != value && last_type != -1) {
            int leave = knn_sample_number_map_.at(last_type);
            if(MIN_SAMPLE_NUMBER - leave > 0) {
                for(int i = 0; i < MIN_SAMPLE_NUMBER - leave; i++) {
                    cv::Mat src = cv::imread(Glib::build_filename(Hub::get_resources_path(), "dataset", prev_file_name));
                    cvtColor(src, gray, CV_BGR2GRAY);
                    src_data.push_back(gray.reshape(0, 1));
                    src_labels.push_back(last_type);
                    std::cout << "type:" << last_type << " file_name:" << prev_file_name << " repeat" << std::endl;
                }
            }
        }

        cv::Mat src = cv::imread(Glib::build_filename(Hub::get_resources_path(), "dataset", file_name));
        cvtColor(src, gray, CV_BGR2GRAY);
        src_data.push_back(gray.reshape(0, 1));
        src_labels.push_back(value);
        std::cout << "type:" << value << " file_name:" << file_name << std::endl;

        if (file_name == file_names.back()) {
            int leave = knn_sample_number_map_.at(value);
            if(MIN_SAMPLE_NUMBER - leave > 0) {
                for(int i = 0; i < MIN_SAMPLE_NUMBER - leave; i++) {
                    cv::Mat src = cv::imread(Glib::build_filename(Hub::get_resources_path(), "dataset", file_name));
                    cvtColor(src, gray, CV_BGR2GRAY);
                    src_data.push_back(gray.reshape(0, 1));
                    src_labels.push_back(value);
                    std::cout << "type:" << value << " file_name:" << file_name << " repeat" << std::endl;
                }
            }
        }

        last_type = value;
        prev_file_name = file_name;
    }

    src_data.convertTo(src_data, CV_32F); //uchar型转换为cv_32f

    cv::Mat trainData, trainLabels;
    trainData = src_data(cv::Range(0, train_num), cv::Range::all());
    trainLabels = src_labels(cv::Range(0, train_num), cv::Range::all());

    //使用KNN算法
    cv::Ptr<cv::ml::TrainData> tData = cv::ml::TrainData::create(trainData, cv::ml::ROW_SAMPLE, trainLabels);
    cv::Ptr<cv::ml::KNearest> model = cv::ml::KNearest::create();

    model->setDefaultK(5);
    model->setIsClassifier(true);
    model->train(tData);

    model->save(Glib::build_filename(Hub::get_resources_path(), "knn.xml"));
    model->clear();

    clock_t end_time = clock();

    knn_model_ = cv::ml::StatModel::load<cv::ml::KNearest>(Glib::build_filename(Hub::get_resources_path(), "knn.xml"));
    std::cout << "train finish @ cost:" <<end_time - start_time << std::endl;
}

gint ScreenShot::knn_predict(cv::Mat &mat) {
    cv::inRange(mat, cv::Scalar(0,0,47), cv::Scalar(255,255,183), mat);
    cv::threshold(mat, mat, 0, 255.0, CV_THRESH_BINARY_INV);
    mat.convertTo(mat, CV_32F);
    float r = knn_model_->predict(mat.reshape(0, 1));
    return (gint)r;
}

void ScreenShot::generate_train_pos_img(cv::Mat &screen) {
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(100);

    Glib::Rand rand;

    std::string file_name;

    gint y = 0, x = 0;
    for( y = 0; y < 10; y++)
    {
        for(x = 0; x < 9; x++)
        {
            if (knn_chess_type_.count(point_to_knn_type(y, x)) == 1) {
                cv::Rect rect = cv::Rect(positions_[y][x].x - mask_size_x_/2 + 2, positions_[y][x].y - mask_size_y_/2 - 6, mask_size_x_, mask_size_y_);
                cv::Mat roi = screen(rect);

                std::vector<cv::Vec3f> circles;
                hough_detection_circle(roi, circles);
                if(circles.size() < 1) {
                    std::cout << "circles is null " << std::endl;
                    continue;
                }

                cv::Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));
                int radius = cvRound(circles[0][2]);

                if(radius <= 0 ) {
                    std::cout << "hough_detection_circle is null " << std::endl;
                    radius = mask_redius_;
                }

                cv::Mat split = cv::Mat::zeros(roi.rows, roi.cols, screen.type());
                cv::Mat mask = cv::Mat::zeros(split.rows, split.cols, screen.type());
                cv::circle(mask, center, radius - 4, CV_RGB(255, 255, 255), -1);

                roi.copyTo(split, mask);

                cv::Mat threshold;

                cv::inRange(split, cv::Scalar(0,0,47), cv::Scalar(255,255,183), threshold);
                cv::threshold(threshold, threshold, 0, 255.0, CV_THRESH_BINARY_INV);

                file_name.append("pos_").append(std::to_string(knn_chess_type_.at(point_to_knn_type(y, x))));
                file_name.append("_").append(std::to_string(y)).append(std::to_string(x));
                file_name.append("_").append(std::to_string(rand.get_int()));
                file_name.append(".jpg");
                file_name = Glib::build_filename(Hub::get_resources_path(), "dataset", file_name);

                cv::imwrite(file_name, threshold, params);
                file_name.clear();
            } else if (knn_blank_type_.count(point_to_knn_type(y, x)) == 1) {
                cv::Rect rect = cv::Rect(positions_[y][x].x - mask_size_x_/2, positions_[y][x].y - mask_size_y_/2, mask_size_x_, mask_size_y_);
                cv::Mat roi = screen(rect);
                cv::Mat split = cv::Mat::zeros(mask_size_x_, mask_size_y_, screen.type());

                cv::Mat mask = cv::Mat::zeros(mask_size_x_, mask_size_y_, screen.type());
                cv::circle(mask, cv::Point(mask_size_x_/2, mask_size_y_/2), mask_redius_, CV_RGB(255, 255, 255), -1);

                roi.copyTo(split, mask);

                cv::Mat threshold;

                cv::inRange(split, cv::Scalar(0,0,47), cv::Scalar(255,255,183), threshold);
                cv::threshold(threshold, threshold, 0, 255.0, CV_THRESH_BINARY_INV);

                file_name.append("neg_").append(std::to_string(knn_blank_type_.at(point_to_knn_type(y, x))));
                file_name.append("_").append(std::to_string(y)).append(std::to_string(x));
                file_name.append("_").append(std::to_string(rand.get_int()));
                file_name.append(".jpg");
                file_name = Glib::build_filename(Hub::get_resources_path(), "dataset", file_name);

                cv::imwrite(file_name, threshold, params);
                file_name.clear();
            }
        }
    }

}

void ScreenShot::generate_train_neg_img(cv::Mat &screen) {
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(100);

    cv::Mat mask = cv::Mat::zeros(mask_size_x_, mask_size_y_, screen.type());
    cv::circle(mask, cv::Point(mask_size_x_/2, mask_size_y_/2), mask_redius_, CV_RGB(255, 255, 255), -1);

    Glib::Rand rand;

    std::string file_name;

    gint y = 0, x = 0;
    for( y = 0; y < 10; y++)
    {
        for(x = 0; x < 9; x++)
        {
            if (knn_blank_type_.count(point_to_knn_type(y, x)) == 1) {
                cv::Rect rect = cv::Rect(positions_[y][x].x - mask_size_x_/2, positions_[y][x].y - mask_size_y_/2, mask_size_x_, mask_size_y_);
                cv::Mat roi = screen(rect);
                cv::Mat split = cv::Mat::zeros(mask_size_x_, mask_size_y_, screen.type());

                roi.copyTo(split, mask);

                cv::Mat threshold;

                cv::inRange(split, cv::Scalar(0,0,47), cv::Scalar(255,255,183), threshold);
                cv::threshold(threshold, threshold, 0, 255.0, CV_THRESH_BINARY_INV);

                file_name.append("neg_").append(std::to_string(knn_blank_type_.at(point_to_knn_type(y, x))));
                file_name.append("_").append(std::to_string(y)).append(std::to_string(x));
                file_name.append("_").append(std::to_string(rand.get_int()));
                file_name.append(".jpg");
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

void ScreenShot::generate_base_train_data(void) {
    cv::Mat pos_mat = cv::imread(Glib::build_filename(Hub::get_resources_img_path(), "demo.jpg"), cv::IMREAD_COLOR);
    cv::Mat neg_mat = cv::imread(Glib::build_filename(Hub::get_resources_img_path(), "blank.jpg"), cv::IMREAD_COLOR);
    generate_train_pos_img(pos_mat);
    generate_train_neg_img(neg_mat);
    generate_train_mark_img(pos_mat);
}

void ScreenShot::generate_train_mark_img(const cv::Mat &screen) {
    cv::Mat copy = cv::Mat::zeros(screen.cols, screen.rows, screen.type());
    screen.copyTo(copy);
    gint y = 0, x = 0;
    for (y = 0; y < 10; y++) {
        for (x = 0; x < 9; x++) {
            cv::Rect rect = cv::Rect(positions_[y][x].x - mask_size_x_/2, positions_[y][x].y - mask_size_y_/2, mask_size_x_, mask_size_y_);
            cv::rectangle(copy, rect, cv::Scalar(0, 255, 0), 1);
        }
    }
    cv::imwrite(Glib::build_filename(Hub::get_resources_path(), "dataset", "mark.jpg"), copy);
}

void ScreenShot::knn_predit_stat() {
    Glib::Dir sample_dir(Glib::build_filename(Hub::get_resources_path(), "dataset"));
    std::list<std::string> entries (sample_dir.begin(), sample_dir.end());

    int err = 0;
    int wright = 0;

    for (std::string file_name : entries) {
        if (file_name.find("pos") == std::string::npos && file_name.find("neg") == std::string::npos) {
            continue;
        }
        std::string sub_string = file_name.substr(4, file_name.find('_', 4) - 4);
        gint knn_type = std::stoi(sub_string);

        cv::Mat src = cv::imread(Glib::build_filename(Hub::get_resources_path(), "dataset", file_name));
        cv::cvtColor(src, src, CV_BGR2GRAY);
        src.convertTo(src, CV_32F);
        float r = knn_model_->predict(src.reshape(0, 1));
        gint predict_type = r;

        if (predict_type != knn_type) {
            std::cout << "file_name:" << file_name << " is err" << " type:" << knn_type << " err:" << predict_type << std::endl;
            err ++;
        } else {
            wright ++;
        }
    }

    std::cout << "right:" << wright << " err:" << err << std::endl;
}

void ScreenShot::knn_sample_stat() {
    knn_sample_number_map_.clear();

    Glib::Dir sample_dir(Glib::build_filename(Hub::get_resources_path(), "dataset"));
    std::list<std::string> entries (sample_dir.begin(), sample_dir.end());

    for (std::string file_name : entries) {
        if (file_name.find("pos") == std::string::npos && file_name.find("neg") == std::string::npos) {
            continue;
        }
        std::string sub_string = file_name.substr(4, file_name.find('_', 4) - 4);
        gint knn_type = std::stoi(sub_string);

        if (knn_sample_number_map_.find(knn_type) != knn_sample_number_map_.end()) {
            knn_sample_number_map_[knn_type] = knn_sample_number_map_[knn_type] + 1;
        } else {
            knn_sample_number_map_[knn_type] = 1;
        }
    }

#ifdef _TEST_STD_OUT
    std::map<gint, gint>::iterator it = knn_sample_number_map_.begin();
    while (it != knn_sample_number_map_.end()) {
        std::cout << it->first << ":" << it->second << std::endl;
        it ++;
    }
#endif
}

std::vector<cv::Vec3f> ScreenShot::hough_detection_circle(cv::Mat &src, std::vector<cv::Vec3f> &circles) {
    cv::Mat src_gray;
    cvtColor( src, src_gray, cv::COLOR_BGR2GRAY);
    GaussianBlur( src_gray, src_gray, cv::Size(3, 3), 2, 2);
    HoughCircles( src_gray, circles, cv::HOUGH_GRADIENT, 1, 25, 100, 40, 10, 30 );

#ifdef _TEST_STD_OUT
    cv::Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));
    int radius = cvRound(circles[0][2]);
    cv::Mat split = cv::Mat::zeros(src.rows, src.cols, src.type());
    src.copyTo(split);
    cv::circle(split, center, radius, CV_RGB(255, 255, 255), -1);

    Glib::Rand rand;
    cv::imwrite(Glib::build_filename("/home/xushy/CLionProjects/test", std::to_string(rand.get_int()) + ".jpg"), split);
#endif

    return circles;
}

void ScreenShot::detect_chess_position(cv::Mat &screen, std::map<gint, gint> &map) {
    cv::imwrite(Glib::build_filename("/home/xushy/CLionProjects/test", "demo.jpg"), screen);
    gint y = 0, x = 0;
    for (y = 0; y < 10; y++) {
        for (x = 0; x < 9; x++) {
            cv::Rect rect = cv::Rect(positions_[y][x].x - mask_size_x_ / 2 + 2,
                                     positions_[y][x].y - mask_size_y_ / 2 - 6, mask_size_x_, mask_size_y_);
            cv::Mat roi = screen(rect);

            cv::imwrite(Glib::build_filename("/home/xushy/CLionProjects/test", std::to_string(y*10 + x) + ".jpg"), roi);

            gint type = knn_predict(roi);

            if (type > 1000) {
                type = -1;
            }

            gint pos = point_to_knn_type(y, x);

            map[pos] = type;
        }
    }
}
