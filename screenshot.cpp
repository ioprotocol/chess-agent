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
#include "knndection.h"

ScreenShot::ScreenShot() :
        left_top_(0, 0), right_bottom_(0, 0), max_circle_radius_(0)
{
    p_detection_ = new KnnDection();
    gint i = 0;
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
    chess_position_type_[i  ] = Chess::B_CHE;
}

ScreenShot::~ScreenShot() {
    delete p_detection_;
}

cv::Mat ScreenShot::screen_shot() {
    gdk_threads_enter();
    Glib::RefPtr<Gdk::Display> display = Gdk::Display::get_default();
    if (!display) {
        std::cout << "default display is null" << std::endl;
    }

    Glib::RefPtr<Gdk::Screen> screen = display->get_default_screen();

    Glib::RefPtr<Gdk::Window> active_window = screen->get_active_window();
    Glib::RefPtr<Gdk::Window> root_window = screen->get_root_window();

    if (!active_window) {
        std::cout << "root window is null " << std::endl;
    }

    Gdk::Rectangle frame_extras;

    active_window->get_frame_extents(frame_extras);
    int x, y;
    active_window->get_origin(x, y);

    GdkPixbuf *pixbuf = gdk_pixbuf_get_from_window(root_window->gobj(), x, y, active_window->get_width(), active_window->get_height());

    cv::Mat mat = Hub::pixbuffer_to_mat(pixbuf);
    std::cout << Glib::DateTime::create_now_local().format("%y-%m-%d %H:%M:%S") << "@x:" << x << "y:" << y << "width:"
              << active_window->get_width() << "height:" << active_window->get_height() << std::endl;
    gdk_threads_leave();

    cv::imwrite(Glib::build_filename(Hub::get_resources_path(), "screen.jpg"), mat);
    return mat;
}

void ScreenShot::hough_detection_circle(cv::Mat &src, std::vector<cv::Vec3f> &circles) {
    cv::Mat src_gray;
    cvtColor( src, src_gray, cv::COLOR_BGR2GRAY);
    GaussianBlur( src_gray, src_gray, cv::Size(3, 3), 2, 2);
    HoughCircles( src_gray, circles, cv::HOUGH_GRADIENT, 1, 25, 208, 40, 15, 30);
#ifdef _TEST_STD_OUT
    cv::Mat test;
    src.copyTo(test);
    for (cv::Vec3f vf : circles) {
        cv::Point center(cvRound(vf[0]), cvRound(vf[1]));
        int radius = cvRound(vf[2]);
        cv::circle(test, center, radius, cv::Scalar(255, 0, 0), 2);
        cv::imwrite(Glib::build_filename(Hub::get_resources_path(), "test.jpg"), test);
    }
#endif
}

void ScreenShot::hough_detection_circle_single(cv::Mat &src, Circle &circle) {
    std::vector<cv::Vec3f> circles;
    hough_detection_circle(src, circles);

    cv::Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));
    int radius = cvRound(circles[0][2]);
    circle.set_center(center);
    circle.set_radius(radius);
}

#ifdef _TEST_STD_OUT
void print_circle_position(std::list<Circle> &circle_list) {
    std::cout << "total:" << circle_list.size() << std::endl;
    for (Circle circle : circle_list) {
        std::cout << Chess::point_to_position(circle.center()) << " , ";
    }
    std::cout << std::endl;
}
#endif

// 自动学习成功，成功识别了棋盘的位置信息,并标记了棋盘的左上坐标和右下坐标
#define DETECT_STUDY_SUCCESS                1
// 自动识别棋盘失败，可能原因是部分棋子未识别成功，建议调整hough_detection_circle的识别参数
#define DETECT_STUDY_FAILED                 2
// 识别的棋子信息太少，无法完成自动识别
#define DETECT_STUDY_CIRCLE_TO_LITTILE      3
// 自动训练失败，识别到的棋子信息不全，必须是32个棋子
#define DETECT_AUTOTRAIN_CIRCLE_LITTILE     4
// 自动训练失败，识别错误率太高，无法准确识别棋子
#define DETECT_AUTOTRAIN_ERR_RATE_HIGH      5
// 自动训练成功
#define DETECT_AUTOTRAIN_SUCCESS            6

gint ScreenShot::detect_chess_position(std::map<guint32, gint> &map) {
    cv::Mat screen;
    std::vector<cv::Vec3f> circle_vector;
    std::list<Circle> circle_list;

    screen = screen_shot();
    hough_detection_circle(screen, circle_vector);

    for (cv::Vec3f vf : circle_vector) {
        cv::Point center(cvRound(vf[0]), cvRound(vf[1]));
        int radius = cvRound(vf[2]);

        if (left_top_.y == right_bottom_.y) {
            circle_list.push_back(Circle(center, radius));
        } else {
            // fiter out noise circle
            cv::Point p1(left_top_.x - 10, left_top_.y - 10);
            cv::Point p2(right_bottom_.x + 10, right_bottom_.y + 10);

            guint32 low = Chess::point_to_position(p1);
            guint32 high = Chess::point_to_position(p2);
            guint32 value = Chess::point_to_position(center);

            if (value > low && value < high) {
                circle_list.push_back(Circle(center, radius));
            }
        }
    }

    circle_list.sort();
#ifdef _TEST_STD_OUT
    std::cout << "input circle size:" << circle_vector.size() << " filter noise:" << circle_vector.size() - circle_list.size() << std::endl;
    print_circle_position(circle_list);
#endif
    if (left_top_.y == right_bottom_.y) {
        if(circle_list.size() < 32) {
            return DETECT_STUDY_CIRCLE_TO_LITTILE;
        }
        study(circle_list);
        if (left_top_.y == right_bottom_.y) {
            return DETECT_STUDY_FAILED;
        } else {
            return DETECT_STUDY_SUCCESS;
        }
    }
    if (!p_detection_->is_trained()) {
        return auto_train(circle_list, screen);
    }

    std::list<Sample> samle_list;

    grab_samles(circle_list, screen, samle_list);

    std::list<Sample>::iterator iter = samle_list.begin();
    gint wrong_num = 0;
    while (iter != samle_list.end()) {
        gint type = p_detection_->predict(iter->mat());
        if (type != iter->label()) {
            std::cout << "type:" << type << "label:" << iter->label() << std::endl;
            wrong_num ++;
        }
        iter ++;
    }
    std::cout << "wrong num:" << wrong_num << std::endl;

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
    gint continues_cout = 0;

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

        gdouble d1 = Chess::get_distance_by_position(p1, p2);
        gdouble d2 = Chess::get_distance_by_position(p2, p3);

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
                    std::cout << "Study result: ("
                              << left_top_.x << ",y" << left_top_.y << ")("
                              << right_bottom_.x << "," << right_bottom_.y << ")"
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

gint ScreenShot::auto_train(std::list<Circle> &circle_list, cv::Mat &screen) {
                                                                         std::list<Sample> samle_list;

                                                                         if (circle_list.size() != 32) {
                                                                         return DETECT_AUTOTRAIN_CIRCLE_LITTILE;
                                                                         }

                                                                         grab_samles(circle_list, screen, samle_list);

                                                                         p_detection_->train(samle_list);
                                                                         // check is the train is correct
                                                                         std::list<Sample>::iterator iter = samle_list.begin();
                                                                         gint wrong_num = 0;
                                                                         while (iter != samle_list.end()) {
                                                                         gint type = p_detection_->predict(iter->mat());
                                                                         if (type != iter->label()) {
                                                                         std::cout << "type:" << type << "label:" << iter->label() << std::endl;
                                                                         wrong_num ++;
                                                                         }
                                                                         iter ++;
                                                                         }
                                                                         if (wrong_num > 0) {
                                                                         return DETECT_AUTOTRAIN_ERR_RATE_HIGH;
                                                                         }
                                                                         std::cout << "knn model train success" << std::endl;
                                                                         return DETECT_AUTOTRAIN_SUCCESS;
                                                                         }

void ScreenShot::grab_samles(std::list<Circle> &circle_list, cv::Mat &screen, std::list<Sample> &samle_list) {
    gint index = 0;
    std::list<Circle>::iterator list_iter = circle_list.begin();
    while (list_iter != circle_list.end()) {
        cv::Rect rect(list_iter->center().x - 24, list_iter->center().y - 24, 48, 48);
        cv::Mat roi = screen(rect);
        cv::Mat split = cv::Mat::zeros(roi.rows, roi.cols, screen.type());
        cv::Mat mask = cv::Mat::zeros(split.rows, split.cols, screen.type());

        Circle re_circle;
        hough_detection_circle_single(roi, re_circle);
        cv::circle(mask, re_circle.center(), re_circle.radius() - 5, CV_RGB(255, 255, 255), -1);
        roi.copyTo(split, mask);

        samle_list.push_back(Sample(split, chess_position_type_[index], list_iter->center()));
        list_iter++;
        index ++;
    }
}
