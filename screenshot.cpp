//
// Created by xsy on 18-7-4.
//

#include "screenshot.h"

#include <fstream>

#include <opencv2/imgproc.hpp>
#include <iostream>
#include <time.h>

#include "application_utils.h"
#include "knndetection.h"

ScreenShot::ScreenShot() :
        left_top_(0, 0), right_bottom_(0, 0), max_circle_radius_(0), chess_window_size_(0, 0) {
    p_detection_ = new KnnDection();
    int i = 0;
    chess_position_type_[i++] = Chess::B_CHE;
    chess_position_type_[i++] = Chess::B_MA;
    chess_position_type_[i++] = Chess::B_XIANG;
    chess_position_type_[i++] = Chess::B_SHI;
    chess_position_type_[i++] = Chess::B_JIANG;
    chess_position_type_[i++] = Chess::B_SHI;
    chess_position_type_[i++] = Chess::B_XIANG;
    chess_position_type_[i++] = Chess::B_MA;
    chess_position_type_[i++] = Chess::B_CHE;
    chess_position_type_[i++] = Chess::B_PAO;
    chess_position_type_[i++] = Chess::B_PAO;
    chess_position_type_[i++] = Chess::B_ZU;
    chess_position_type_[i++] = Chess::B_ZU;
    chess_position_type_[i++] = Chess::B_ZU;
    chess_position_type_[i++] = Chess::B_ZU;
    chess_position_type_[i++] = Chess::B_ZU;
    chess_position_type_[i++] = Chess::R_ZU;
    chess_position_type_[i++] = Chess::R_ZU;
    chess_position_type_[i++] = Chess::R_ZU;
    chess_position_type_[i++] = Chess::R_ZU;
    chess_position_type_[i++] = Chess::R_ZU;
    chess_position_type_[i++] = Chess::B_PAO;
    chess_position_type_[i++] = Chess::B_PAO;
    chess_position_type_[i++] = Chess::B_CHE;
    chess_position_type_[i++] = Chess::B_MA;
    chess_position_type_[i++] = Chess::R_XIANG;
    chess_position_type_[i++] = Chess::R_SHI;
    chess_position_type_[i++] = Chess::R_JIANG;
    chess_position_type_[i++] = Chess::R_SHI;
    chess_position_type_[i++] = Chess::R_XIANG;
    chess_position_type_[i++] = Chess::B_MA;
    chess_position_type_[i] = Chess::B_CHE;
}

ScreenShot::~ScreenShot() {
    delete p_detection_;
}

void ScreenShot::hough_detection_circle(cv::Mat &src, std::vector<cv::Vec3f> &circles) {
    cv::Mat src_gray;
    cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
    GaussianBlur(src_gray, src_gray, cv::Size(3, 3), 2, 2);
    HoughCircles(src_gray, circles, cv::HOUGH_GRADIENT, 1, 25, 208, 40, 15, 30);
}

void ScreenShot::hough_detection_circle_single(cv::Mat &src, Circle &circle) {
    std::vector<cv::Vec3f> circles;
    hough_detection_circle(src, circles);

    if (circles.size() < 1) {
        qErrnoWarning("hough_detection_circle_single err");
        return;
    }
    if(circles.size() > 1) {
        std::cout << "more than one cirlce" << std::endl;
    }
    cv::Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));
    int radius = cvRound(circles[0][2]);
    circle.set_center(center);
    circle.set_radius(radius);
}

#ifdef _TEST_STD_OUT

void print_circle_position(std::list<Circle> &circle_list, cv::Mat &screen) {
    cv::Mat tmp;
    screen.copyTo(tmp);
    for (Circle circle : circle_list) {
        cv::circle(tmp, circle.center(), circle.radius(), cv::Scalar(255, 0, 0), 1);
    }
    cv::imwrite("D:\\test.jpg", tmp);
}

#endif

int ScreenShot::detect_chess_position(std::map<unsigned int, int>* map, cv::Mat &screen) {
    std::vector<cv::Vec3f> circle_vector;
    std::list<Circle> circle_list;

    if (chess_window_size_.width != 0) {
        if (chess_window_size_.width != screen.cols && chess_window_size_.height != screen.rows) {
            // chess window is not current active window
            std::cout << " Chess window is not active, can't get it's screen" << std::endl;
            return DETECT_WINDOW_IS_NOT_ACTIVE;
        }
    }

    hough_detection_circle(screen, circle_vector);

    for (cv::Vec3f vf : circle_vector) {
        cv::Point center(cvRound(vf[0]), cvRound(vf[1]));
        int radius = cvRound(vf[2]);

        if (left_top_.y == right_bottom_.y) {
            circle_list.push_back(Circle(center, radius));
        } else {
            // fiter out noise circle
            cv::Point p1(left_top_.x - 30, left_top_.y - 30);
            cv::Point p2(right_bottom_.x + 30, right_bottom_.y + 30);

            unsigned int low = Chess::point_to_uint32(p1);
            unsigned int high = Chess::point_to_uint32(p2);
            unsigned int value = Chess::point_to_uint32(center);

            if (value > low && value < high) {
                circle_list.push_back(Circle(center, radius));
            }
        }
    }

    if (circle_list.size() < 3) {
        return DETECT_AUTOTRAIN_CIRCLE_LITTILE;
    }

    circle_list.sort();
#ifdef _TEST_STD_OUT
    std::cout << "input circle size:" << circle_vector.size() << " filter noise:"
              << circle_vector.size() - circle_list.size() << std::endl;
    print_circle_position(circle_list, screen);
#endif
    if (left_top_.y == right_bottom_.y) {
        if (circle_list.size() < 32) {
            return DETECT_STUDY_CIRCLE_TO_LITTILE;
        }
        study(circle_list);
        if (left_top_.y == right_bottom_.y) {
            return DETECT_STUDY_FAILED;
        } else {
            chess_window_size_.height = screen.rows;
            chess_window_size_.width = screen.cols;
            return DETECT_STUDY_SUCCESS;
        }
    }
    if (!p_detection_->is_trained()) {
        return auto_train(circle_list, screen);
    }

    std::list<Sample> samle_list;
    grab_samles(circle_list, screen, samle_list);
    std::list<Sample>::iterator iter = samle_list.begin();
    for(iter = samle_list.begin(); iter != samle_list.end(); iter ++) {
        int type = p_detection_->predict(iter->mat());
        int pos = coordinate_screen_to_chess(iter->position());
        if (type >= 10) {
            type = type - detect_chess_color(*iter);
        }
        (*map)[pos] = type;
    }
    return 0;
}

/**
 * 自学习，自动识别棋盘的位置，并标定上左坐标和右下坐标
 *
 * @param circle_list
 */
void ScreenShot::study(std::list<Circle> &circle_list) {
    std::list<Circle>::iterator list_iter = circle_list.begin();
    cv::Point start_point(0, 0);
    int continues_cout = 0;

    while (list_iter != circle_list.end()) {
        max_circle_radius_ = MAX(max_circle_radius_, list_iter->radius());
        cv::Point p1 = list_iter->center();
        list_iter++;
        if (list_iter == circle_list.end()) {
            break;
        }
        cv::Point p2 = list_iter->center();
        list_iter++;
        if (list_iter == circle_list.end()) {
            break;
        }
        cv::Point p3 = list_iter->center();

        double d1 = Chess::get_distance_by_position(p1, p2);
        double d2 = Chess::get_distance_by_position(p2, p3);

        if (abs(d1 - d2) < 12) {
            if (start_point.x == 0 && start_point.y == 0) {
                start_point.x = p1.x;
                start_point.y = p1.y;
            }
            continues_cout++;
        } else {
            if (continues_cout == 7) {
                if (left_top_.x == 0 && left_top_.y == 0) {
                    left_top_.x = start_point.x;
                    left_top_.y = start_point.y;
                } else {
                    right_bottom_.x = p2.x;
                    right_bottom_.y = p2.y;
                    std::cout << "Study result: (x = "
                              << left_top_.x << ",y = " << left_top_.y << ")(x = "
                              << right_bottom_.x << ",y = " << right_bottom_.y << ")"
                              << " max_circle_radius_ = " << max_circle_radius_
                              << std::endl;
                    return;
                }
            } else {
                start_point.x = 0;
                start_point.y = 0;
            }
            continues_cout = 0;
        }

        list_iter--;
    }
}

int ScreenShot::auto_train(std::list<Circle> &circle_list, cv::Mat &screen) {
    std::list<Sample> samle_list;

    if (circle_list.size() != 32) {
        return DETECT_AUTOTRAIN_CIRCLE_LITTILE;
    }

    grab_samles(circle_list, screen, samle_list);

    p_detection_->train(samle_list);
    // check is the train is correct
    std::list<Sample>::iterator iter = samle_list.begin();
    int wrong_num = 0;
    while (iter != samle_list.end()) {
        int type = p_detection_->predict(iter->mat());
        if (type != iter->label()) {
            std::cout << "type:" << type << "label:" << iter->label() << std::endl;
            wrong_num++;
        }
        iter++;
    }
    if (wrong_num > 0) {
        return DETECT_AUTOTRAIN_ERR_RATE_HIGH;
    }
    std::cout << "knn model train success" << std::endl;
    return DETECT_AUTOTRAIN_SUCCESS;
}

void ScreenShot::grab_samles(std::list<Circle> &circle_list, cv::Mat &screen, std::list<Sample> &samle_list) {
    int size = max_circle_radius_*2;
    int index = 0;
    std::list<Circle>::iterator list_iter = circle_list.begin();
    while (list_iter != circle_list.end()) {
        cv::Rect rect(list_iter->center().x - size/2, list_iter->center().y - size/2, size, size);
        cv::Mat roi = screen(rect);
        cv::Mat split = cv::Mat::zeros(roi.rows, roi.cols, screen.type());
        cv::Mat mask = cv::Mat::zeros(split.rows, split.cols, screen.type());

        Circle re_circle;
        hough_detection_circle_single(roi, re_circle);
        cv::circle(mask, re_circle.center(), re_circle.radius() - 5, CV_RGB(255, 255, 255), -1);
        roi.copyTo(split, mask);

        samle_list.push_back(Sample(split, chess_position_type_[index], list_iter->center()));
        list_iter++;
        index++;
    }
}

int ScreenShot::coordinate_screen_to_chess(cv::Point &point) {
    int y = 0, x = 0;
    int dx = 0, dy = 0;

    dx = (right_bottom_.x - left_top_.x) / 8;
    dy = (right_bottom_.y - left_top_.y) / 9;

    for (int i = 0; i < 9; i++) {
        if (abs(point.x - left_top_.x - dx * i) < 8) {
            x = i;
            break;
        }
    }
    for (int i = 0; i < 10; i++) {
        if (abs(point.y - left_top_.y - dy * i) < 8) {
            y = i;
        }
    }

    return Chess::point_to_uint32(x, y);
}

void ScreenShot::coordinate_chess_to_screen(int in, cv::Point &point) {
    int dx = 0, dy = 0;

    dx = (right_bottom_.x - left_top_.x) / 8;
    dy = (right_bottom_.y - left_top_.y) / 9;

    cv::Point pos = Chess::uint32_to_point(in);
    point.x = left_top_.x + dx * pos.x;
    point.y = left_top_.y + dy * pos.y;
}

int ScreenShot::detect_chess_color(Sample &sample) {
    cv::Mat roi = sample.mat();
    cv::Mat threshold;

    cv::inRange(roi, cv::Scalar(0,0,84), cv::Scalar(255,255,255), threshold);
    cv::threshold(threshold, threshold, 0, 255.0, CV_THRESH_BINARY);

    const int channels = threshold.channels();
    unsigned int cols = channels * threshold.cols;
    uchar *p;
    cv::Scalar scalar;

    int counter = 0;
    for (int j = 0; j < roi.rows; j++) {
        p = threshold.ptr<uchar>(j);
        for (int i = 0; i < cols; i++) {
            int filter = (int)p[i];
            if (filter == 0) {
                counter ++;
            }
        }
    }

    if (counter > 50) {
        // black
        return 0;
    }
    // else is red
    return 10;
}
