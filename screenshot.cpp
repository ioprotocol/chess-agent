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

    knn_model = cv::ml::StatModel::load<cv::ml::KNearest>(Glib::build_filename(Hub::get_resources_path(), "knn.xml"));
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

void ScreenShot::splitScreenImg(cv::Mat &mat, cv::Mat arrays[][9]) {
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

/*
gdouble ScreenShot::compareHist(const cv::Mat &mat1, const cv::Mat &mat2) {
    const int channels[] = { 0, 1 };
    const int histSize[] = { 50, 60 };
    float range1[] = {0, 255};
    float range2[] = {0, 180};
    const float *ranges[] = { range1, range2};

    cv::Mat hsv_test1;
    cv::Mat hsv_test2;

    cv::Mat hist_test1;
    cv::Mat hist_test2;

    cv::cvtColor(mat1, hsv_test1, cv::COLOR_BGR2HSV);
    cv::cvtColor(mat2, hsv_test2, cv::COLOR_BGR2HSV);

    cv::Mat src1[] = { hsv_test1 };
    cv::Mat src2[] = { hsv_test2 };


    cv::calcHist(src1, 1, channels, cv::Mat(), hist_test1, 1, histSize, &ranges[0], true, false);
    cv::calcHist(src2, 1, channels, cv::Mat(), hist_test2, 1, histSize, &ranges[0], true, false);

    cv::normalize(hist_test1, hist_test1, 0, hist_test1.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(hist_test2, hist_test2, 0, hist_test2.rows, cv::NORM_MINMAX, -1, cv::Mat());

    return cv::compareHist(hist_test1, hist_test2, cv::HISTCMP_CORREL);
}

void ScreenShot::matchTemplateTest(cv::Mat &src1) {
    cv::Mat src = src1(cv::Rect(0, 0, 200, 200));
//    cv::Mat src = tencentChessFeatures[0];
    cv::Mat tpl = tencentChessFeatures[0];
    cv::Mat result;

    int result_cols =  src.cols - tpl.cols + 1;
    int result_rows = src.rows - tpl.rows + 1;
    result.create( result_rows, result_cols, CV_32FC1 );

    int match_method = CV_TM_CCORR_NORMED;

    bool method_accepts_mask = (CV_TM_SQDIFF == match_method || match_method == CV_TM_CCORR_NORMED);

    matchTemplate( src, tpl, result, match_method);
    normalize( result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );

    double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
    cv::Point matchLoc;

    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );

    if( match_method  == cv::TM_SQDIFF || match_method == cv::TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
    else
    { matchLoc = maxLoc; }

    std::cout << "x=" << matchLoc.x << " y=" << matchLoc.y <<std::endl;
    return;
}
*/

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
        int idx = img_name.find("_", 2);
        std::string prefix = img_name.substr(2, idx);
        std::stringstream ss = std::stringstream(prefix);
        int value;
        ss >> value;

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
    float r = knn_model->predict(mat.reshape(0, 1));
    return (gint)r;
}

void ScreenShot::output_disk_by_screen_shot(cv::Mat screen) {
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(100);

    cv::Mat mask = cv::Mat::zeros(48, 48, screen.type());
    cv::circle(mask, cv::Point(24, 24), 21, CV_RGB(255, 255, 255), -1);

    std::string file_name;

    gint y = 0, x = 0;
    for( y = 0; y < 10; y++)
    {
        for(x = 0; x < 9; x++)
        {
            cv::Rect rect = cv::Rect(positions_[y][x].x - 24, positions_[y][x].y - 24, 48, 48);
            cv::Mat roi = screen(rect);
            cv::Mat split = cv::Mat::zeros(48, 48, screen.type());

            roi.copyTo(split, mask);

            /*
            // che
            if(y == 0 && x == 0) {
                file_name.append("a_1_1_che");
            }
            if(y == 0 && x == 8) {
                file_name.append("a_1_2_che");
            }
            if(y == 9 && x == 0) {
                file_name.append("a_1_3_che");
            }
            if(y == 9 && x == 8) {
                file_name.append("a_1_4_che");
            }
            // ma
            if(y == 0 && x == 1) {
                file_name.append("a_2_1_ma");
            }
            if(y == 0 && x == 7) {
                file_name.append("a_2_2_ma");
            }
            if(y == 9 && x == 1) {
                file_name.append("a_2_3_ma");
            }
            if(y == 9 && x == 7) {
                file_name.append("a_2_4_ma");
            }
            // xiang b
            if(y == 0 && x == 2) {
                file_name.append("a_3_1_xiang_b");
            }
            if(y == 0 && x == 6) {
                file_name.append("a_3_2_xiang_b");
            }
            // xiang r
            if(y == 9 && x == 2) {
                sprintf(&str[0], "a_4_%d_xiang_r.jpg", 1);
            }
            if(y == 9 && x == 6) {
                sprintf(&str[0], "a_4_%d_xiang_r.jpg", 2);
            }
            // shi b
            if(y == 0 && x == 3) {
                sprintf(&str[0], "a_5_%d_shi_b.jpg", 1);
            }
            if(y == 0 && x == 5) {
                sprintf(&str[0], "a_5_%d_shi_b.jpg", 2);
            }
            // shi r
            if(y == 9 && x == 3) {
                sprintf(&str[0], "a_6_%d_shi_r.jpg", 1);
            }
            if(y == 9 && x == 5) {
                sprintf(&str[0], "a_6_%d_shi_r.jpg", 2);
            }
            // jiang b
            if(y == 0 && x == 4) {
                sprintf(&str[0], "a_7_%d_jiang_b.jpg", 1);
            }
            // jiang r
            if(y == 9 && x == 4) {
                sprintf(&str[0], "a_8_%d_jiang_r.jpg", 1);
            }
            // pao b
            if(y == 2 && x == 1) {
                sprintf(&str[0], "a_9_%d_pao_b.jpg", 1);
            }
            if(y == 2 && x == 7) {
                sprintf(&str[0], "a_9_%d_pao_b.jpg", 2);
            }
            // pao r
            if(y == 7 && x == 1) {
                sprintf(&str[0], "a_9_%d_pao_r.jpg", 3);
            }
            if(y == 7 && x == 7) {
                sprintf(&str[0], "a_9_%d_pao_r.jpg", 4);
            }
            // zu b
            if(y == 3 && x == 0) {
                sprintf(&str[0], "a_10_%d_zu_b.jpg", 1);
            }
            if(y == 3 && x == 2) {
                sprintf(&str[0], "a_10_%d_zu_b.jpg", 2);
            }
            if(y == 3 && x == 4) {
                sprintf(&str[0], "a_10_%d_zu_b.jpg", 3);
            }
            if(y == 3 && x == 6) {
                sprintf(&str[0], "a_10_%d_zu_b.jpg", 4);
            }
            if(y == 3 && x == 8) {
                sprintf(&str[0], "a_10_%d_zu_b.jpg", 5);
            }
            // zu r
            if(y == 6 && x == 0) {
                sprintf(&str[0], "a_11_%d_zu_r.jpg", 1);
            }
            if(y == 6 && x == 2) {
                sprintf(&str[0], "a_11_%d_zu_r.jpg", 2);
            }
            if(y == 6 && x == 4) {
                sprintf(&str[0], "a_11_%d_zu_r.jpg", 3);
            }
            if(y == 6 && x == 6) {
                sprintf(&str[0], "a_11_%d_zu_r.jpg", 4);
            }
            if(y == 6 && x == 8) {
                sprintf(&str[0], "a_11_%d_zu_r.jpg", 5);
            }
            */

            cv::Mat threshold;

            cv::inRange(split, cv::Scalar(0,0,47), cv::Scalar(255,255,183), threshold);
            cv::threshold(threshold, threshold, 0, 255.0, CV_THRESH_BINARY_INV);

            file_name = Glib::build_filename(Hub::get_resources_path(), "dataset", file_name, ".jpg");

            cv::imwrite(file_name, threshold, params);
        }
    }

}