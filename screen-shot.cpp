//
// Created by xsy on 18-7-4.
//

#include "screen-shot.h"

#include <unistd.h>
#include <time.h>
#include <fstream>

#include <gtk/gtk.h>

#include <opencv2/imgproc.hpp>

#include <iostream>

#include "screenshot-utils.h"
#include "screenshot-config.h"

ScreenShot::ScreenShot() {
    gchar *resource_img_path, *resource_path, *file_name;

    init_screen_config();

    resource_img_path = get_resources_img_path();
    resource_path = get_resources_path();

    topLeft = cv::Point(83, 144);
    topRight = cv::Point(623, 144);
    bottomLeft = cv::Point(83, 751);
    bottomRight = cv::Point(623, 751);

    gdouble dx = (topRight.x - topLeft.x) / 8.0;
    gdouble dy = (bottomLeft.y - topLeft.y) / 9.0;

    gint y = 0, x = 0;
    for( y = 0; y < 10; y++)
    {
        for(x = 0; x < 9; x++)
        {
            positions[y][x].x = topLeft.x + dx * x;
            positions[y][x].y = topLeft.y + dy * y;
        }
    }

    file_name = g_build_filename(resource_path, "knn.xml", NULL);
    knnModel = cv::ml::StatModel::load<cv::ml::KNearest>(file_name);

    g_free(resource_path);
    g_free(file_name);
    g_free(resource_img_path);
}

ScreenShot::~ScreenShot() {
    g_free(screenshot_config);
}

cv::Mat ScreenShot::shot() {
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

    time_t timep;
    time(&timep);
    std::cout << "screen shot" << asctime(gmtime(&timep)) << std::endl;

    return mat;
}

void ScreenShot::splitScreenImg(cv::Mat &mat, cv::Mat arrays[][9]) {
    cv::Mat mask = cv::Mat::zeros(48, 48, mat.type());
    cv::circle(mask, cv::Point(24, 24), 21, CV_RGB(255, 255, 255), -1);

    gint y = 0, x = 0;
    for( y = 0; y < 10; y++)
    {
        for(x = 0; x < 9; x++)
        {
            cv::Rect rect = cv::Rect(positions[y][x].x - 24, positions[y][x].y - 24, 48, 48);
            cv::Mat roi = mat(rect);
            cv::Mat split = cv::Mat::zeros(48, 48, mat.type());

            roi.copyTo(split, mask);
            arrays[y][x] = split;
            roi.release();
        }
    }
    mask.release();
}

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

/*
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
void ScreenShot::knnTrain() {
    int train_num = 50;
    cv::Mat src_data,src_labels;
    std::string img_name;
    cv::Mat gray;

    std::ifstream file_name_stream("/home/xsy/CLionProjects/svg-train/img.txt");
    for(int i = 0; i < 50; i++) {
        getline(file_name_stream, img_name);
        int value = (int)(img_name.at(0) - '0');
        std::cout << "image: " << img_name << "  " << value << std::endl;

        cv::Mat src = cv::imread("/home/xsy/CLionProjects/svg-train/img/" + img_name);
        cvtColor(src, gray, CV_BGR2GRAY);
        src_data.push_back(gray.reshape(0, 1));
        src_labels.push_back(value);
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

    model->save("/home/xsy/CLionProjects/knn.xml");
    model->clear();
}

gint ScreenShot::knnPredit(cv::Mat &mat) {
    cv::Mat tmp;
    cvtColor(mat, tmp, CV_BGR2GRAY);
    tmp.convertTo(tmp, CV_32F);
    float r = knnModel->predict(tmp.reshape(0, 1));
    return (gint)r;
}
