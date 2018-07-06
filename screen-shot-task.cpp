//
// Created by xushy on 18-7-4.
//
#include "screen-shot-task.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

#include "screen-shot.h"
#include "application-utils.h"

GTask* screen_shot_task;

GCancellable* screen_shot_task_cancellable;

gint screen_shot_sleep_time = 1000;

ScreenShot* screenShot;

/**
 *
 * @param source_object
 * @param res
 * @param user_data
 */
void screen_shot_task_call_back(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    std::cout << "screen shot task has stop" << std::endl;
    g_object_unref(screen_shot_task_cancellable);
    delete screenShot;
}

/**
 *
 * @param data
 */
void screen_shot_task_data_destroy(gpointer data) {
    std::cout << "screen_shot_task_data_destroy" << std::endl;
}


/**
 * Just for dev test.
 *
 * @return
 */
cv::Mat load_screen_shot_for_test() {
    cv::Mat mat;
    gchar* name;
    gchar* img_path;

    img_path = get_resources_img_path();
    name = g_build_filename(img_path, "demo.jpeg", NULL);

    mat = cv::imread(name, cv::IMREAD_COLOR);

    g_free(name);
    g_free(img_path);

    return mat;
}

void output_disk_for_test(cv::Mat arrays[10][9]) {
    gchar* path;
    gchar str[8];

    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(100);

    gint y = 0, x = 0;
    for( y = 0; y < 10; y++)
    {
        for(x = 0; x < 9; x++)
        {
            sprintf(&str[0], "%d_%d.jpg", y, x);
            path = g_build_filename("/home/xushy/CLionProjects/dataset", str, NULL);

            cv::imwrite(path, arrays[y][x], params);

            g_free(path);
        }
    }
}

void output_disk_for_dataset(cv::Mat mat, gint y, gint x) {
    gchar* path;
    gchar str[20];
    str[0] = '\0';

    // che
    if(y == 0 && x == 0) {
        sprintf(&str[0], "1_%d_che.jpg", 1);
    }
    if(y == 0 && x == 8) {
        sprintf(&str[0], "1_%d_che.jpg", 2);
    }
    if(y == 9 && x == 0) {
        sprintf(&str[0], "1_%d_che.jpg", 3);
    }
    if(y == 9 && x == 8) {
        sprintf(&str[0], "1_%d_che.jpg", 4);
    }
    // ma
    if(y == 0 && x == 1) {
        sprintf(&str[0], "2_%d_ma.jpg", 1);
    }
    if(y == 0 && x == 7) {
        sprintf(&str[0], "2_%d_ma.jpg", 2);
    }
    if(y == 9 && x == 1) {
        sprintf(&str[0], "2_%d_ma.jpg", 3);
    }
    if(y == 9 && x == 7) {
        sprintf(&str[0], "2_%d_ma.jpg", 4);
    }
    // xiang b
    if(y == 0 && x == 2) {
        sprintf(&str[0], "3_%d_xiang_b.jpg", 1);
    }
    if(y == 0 && x == 6) {
        sprintf(&str[0], "3_%d_xiang_b.jpg", 2);
    }
    // xiang r
    if(y == 9 && x == 2) {
        sprintf(&str[0], "4_%d_xiang_r.jpg", 1);
    }
    if(y == 9 && x == 6) {
        sprintf(&str[0], "4_%d_xiang_r.jpg", 2);
    }
    // shi b
    if(y == 0 && x == 3) {
        sprintf(&str[0], "5_%d_shi_b.jpg", 1);
    }
    if(y == 0 && x == 5) {
        sprintf(&str[0], "5_%d_shi_b.jpg", 2);
    }
    // shi r
    if(y == 9 && x == 3) {
        sprintf(&str[0], "6_%d_shi_r.jpg", 1);
    }
    if(y == 9 && x == 5) {
        sprintf(&str[0], "6_%d_shi_r.jpg", 2);
    }
    // jiang b
    if(y == 0 && x == 4) {
        sprintf(&str[0], "7_%d_jiang_b.jpg", 1);
    }
    // jiang r
    if(y == 9 && x == 4) {
        sprintf(&str[0], "8_%d_jiang_r.jpg", 1);
    }
    // pao b
    if(y == 2 && x == 1) {
        sprintf(&str[0], "9_%d_pao_b.jpg", 1);
    }
    if(y == 2 && x == 7) {
        sprintf(&str[0], "9_%d_pao_b.jpg", 2);
    }
    // pao r
    if(y == 7 && x == 1) {
        sprintf(&str[0], "9_%d_pao_r.jpg", 3);
    }
    if(y == 7 && x == 7) {
        sprintf(&str[0], "9_%d_pao_r.jpg", 4);
    }
    // zu b
    if(y == 3 && x == 0) {
        sprintf(&str[0], "10_%d_zu_b.jpg", 1);
    }
    if(y == 3 && x == 2) {
        sprintf(&str[0], "10_%d_zu_b.jpg", 2);
    }
    if(y == 3 && x == 4) {
        sprintf(&str[0], "10_%d_zu_b.jpg", 3);
    }
    if(y == 3 && x == 6) {
        sprintf(&str[0], "10_%d_zu_b.jpg", 4);
    }
    if(y == 3 && x == 8) {
        sprintf(&str[0], "10_%d_zu_b.jpg", 5);
    }
    // zu r
    if(y == 6 && x == 0) {
        sprintf(&str[0], "11_%d_zu_r.jpg", 1);
    }
    if(y == 6 && x == 2) {
        sprintf(&str[0], "11_%d_zu_r.jpg", 2);
    }
    if(y == 6 && x == 4) {
        sprintf(&str[0], "11_%d_zu_r.jpg", 3);
    }
    if(y == 6 && x == 6) {
        sprintf(&str[0], "11_%d_zu_r.jpg", 4);
    }
    if(y == 6 && x == 8) {
        sprintf(&str[0], "11_%d_zu_r.jpg", 5);
    }
    if(str[0] == '\0') {
        sprintf(&str[0], "%d_%d_blank.jpg", y*1000 + x * 100, y*10 + x);
    }

    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(100);

    cv::Mat threshold;

    cv::inRange(mat, cv::Scalar(0,0,47), cv::Scalar(255,255,183), threshold);
    cv::threshold(threshold, threshold, 0, 255.0, CV_THRESH_BINARY_INV);

    path = g_build_filename("/home/xushy/CLionProjects/dataset", str, NULL);

    cv::imwrite(path, threshold, params);
    g_free(path);
}

/**
 *
 * @param task
 * @param source_object
 * @param task_data
 * @param cancellable
 */
void screen_shot_task_thread(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable) {
    ScreenShot* screenShot = (ScreenShot*)task_data;
    while(true) {
        if (g_cancellable_is_cancelled(cancellable)) {
            break;
        }
        // 1. take current active window picture
//        cv::Mat screen_shot = screenShot->shot();
        cv::Mat screen_shot_img = load_screen_shot_for_test();

        // 2. split screen shot img
        cv::Mat split_img_array[10][9];
        screenShot->splitScreenImg(screen_shot_img, split_img_array);

        // 3. analise split img
        for(int y = 0; y < 10; y++) {
            for(int x = 0; x < 9; x++) {
//                ChessType chessType = screenShot->chessType(split_img_array[y][x]);
//                std::cout << "pos:y=" << y << " x=" << x << " type:" << chessType << std::endl;
//                output_disk_for_dataset(split_img_array[y][x], y , x);
                std::cout << "pos:y=" << y << " x=" << x << " type:" << screenShot->knnPredit(split_img_array[y][x]) << std::endl;
            }
        }
//        screenShot->knnTrain();

        screen_shot_img.release();
        usleep(screen_shot_sleep_time*1000);
    }

    g_task_return_boolean(task, TRUE);
}

/**
 * start the screen shot task
 *
 * @param source_object
 */
void screen_shot_task_create(gpointer source_object, gint sleep_time) {
    screen_shot_sleep_time = sleep_time;
    screen_shot_task_cancellable = g_cancellable_new();
    screenShot = new ScreenShot();
    screen_shot_task = g_task_new(source_object, screen_shot_task_cancellable, screen_shot_task_call_back, NULL);
    g_task_set_task_data(screen_shot_task, screenShot, screen_shot_task_data_destroy);
    g_task_run_in_thread(screen_shot_task, screen_shot_task_thread);
}

/**
 * Stop the screen shot task
 *
 */
void screen_shot_task_destory() {
    g_cancellable_cancel(screen_shot_task_cancellable);
}

