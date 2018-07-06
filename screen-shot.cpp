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

ScreenShot::ScreenShot() {
    gchar *resource_img_path, *file_name;

    gchar* tem[] = {
        "OOS.png", "T_MA_B.jpg", "T_XIANG_B.jpg", "T_SHI_B.jpg", "T_JIANG_B.jpg", "T_PAO_B.jpg", "T_ZU_B.jpg",
                "T_CHE_R.jpg", "T_MA_R.jpg", "T_XIANG_R.jpg", "T_SHI_R.jpg", "T_JIANG_R.jpg", "T_PAO_R.jpg", "T_ZU_R.jpg"
    };

    for(int i = 0; i < 14; i++) {
        tencent_chess_names[i] = tem[i];
    }

    init_screen_config();

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

    for( y = 0; y < 14; y++) {
        file_name = g_build_filename(resource_img_path, tencent_chess_names[y], NULL);
        tencentChessFeatures[y] = cv::imread(file_name, cv::IMREAD_COLOR);
        if(tencentChessFeatures[y].empty()) {
            g_assert_not_reached();
        }
        g_free(file_name);
    }

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

ChessType ScreenShot::chessType(const cv::Mat &mat) {
    gint i = 0;
    gdouble ret = 0;
    for( i = 0; i < 14; i++) {
        ret = compareHist(tencentChessFeatures[i], mat);
        std::cout << tencent_chess_names[i] << ":" << abs(1.0 - ret) << std::endl;
        if (abs(1.0 - ret) < 0.001) {
            return ChessType(i);
        }
    }

    return UN_KNOW;
}

void ScreenShot::compareTest(cv::Mat &mat) {
    gdouble ret = 0;
    gint y = 0;
    gint x = 0;

    for(y = 0; y < 13; y++) {
        for(x = y; x < 14; x++) {
            ret = compareHist(tencentChessFeatures[y], tencentChessFeatures[x]);
            std::cout << tencent_chess_names[y] << " vs " << tencent_chess_names[x] << " = " << ret << " = " << 1 - ret<< std::endl;
        }
    }
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
