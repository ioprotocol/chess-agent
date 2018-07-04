//
// Created by xsy on 18-7-4.
//

#include "screen-shot.h"

#include <unistd.h>
#include <time.h>

#include <gtk/gtk.h>

#include <opencv2/imgproc.hpp>

#include <iostream>

#include "screenshot-utils.h"
#include "screenshot-config.h"
#include "application-utils.h"

ScreenShot::ScreenShot() {
    init_screen_config();

    gchar *resource_img_path, *file_name;

    resource_img_path = get_resources_img_path();

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

    file_name = g_build_filename(resource_img_path, "T_CHE_B.jpg", NULL);
    bChe = cv::imread(file_name, 1);
    g_free(file_name);

    file_name = g_build_filename(resource_img_path, "T_CHE_R.jpg", NULL);
    rChe = cv::imread(file_name, 1);
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

void ScreenShot::split_screen_shot_img(cv::Mat &mat, cv::Mat arrays[10][9]) {
    cv::Mat mask = cv::Mat::zeros(48, 48, mat.type());
    cv::circle(mask, cv::Point(24, 24), 22, CV_RGB(255, 255, 255), -1);

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

double ScreenShot::compareHist(cv::Mat &mat1, cv::Mat &mat2) {
    cv::Mat hsv_test1;
    cv::Mat hsv_test2;

    cv::Mat hist_test1;
    cv::Mat hist_test2;

    const int channels = 0;
    const int histSize = 256;
    float range[] = {0, 255};
    const float *ranges[] = {range};

    cv::cvtColor(mat1, hsv_test1, cv::COLOR_BGR2HSV);
    cv::cvtColor(mat2, hsv_test2, cv::COLOR_BGR2HSV);

    cv::calcHist(&hsv_test1, 1, &channels, cv::Mat(), hist_test1, 1, &histSize, &ranges[0], true, false);
    cv::normalize(hsv_test1, hsv_test1, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
    cv::calcHist(&hsv_test2, 1, &channels, cv::Mat(), hist_test2, 1, &histSize, &ranges[0], true, false);
    cv::normalize(hsv_test2, hsv_test2, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    double v = cv::compareHist(hist_test1, hist_test2, cv::HISTCMP_CORREL);
    return v;
}

double ScreenShot::isBChe(cv::Mat &mat) {
    return compareHist(mat, this->bChe);
}
