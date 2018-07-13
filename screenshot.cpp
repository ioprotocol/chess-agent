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

ScreenShot::ScreenShot() :
        left_top_(0, 0), right_bottom_(0, 0)
{
}

ScreenShot::~ScreenShot() {
}

cv::Mat ScreenShot::screen_shot() {
#ifndef _TEST_STD_OUT
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
    return mat;
#else
    cv::Mat mat = cv::imread(Glib::build_filename(Hub::get_resources_img_path(), "demo.jpg"));
    return mat;
#endif
}

void ScreenShot::hough_detection_circle(cv::Mat &src, std::vector<cv::Vec3f> &circles) {
    cv::Mat src_gray;
    cvtColor( src, src_gray, cv::COLOR_BGR2GRAY);
    GaussianBlur( src_gray, src_gray, cv::Size(3, 3), 2, 2);
    HoughCircles( src_gray, circles, cv::HOUGH_GRADIENT, 1, 25, 100, 35, 10, 30);
#ifdef _TEST_STD_OUT
    cv::Mat test;
    src.copyTo(test);
    for (cv::Vec3f vf : circles) {
        cv::Point center(cvRound(vf[0]), cvRound(vf[1]));
        int radius = cvRound(vf[2]);
        cv::circle(test, center, radius, cv::Scalar(255, 0, 0), 1);
        cv::imwrite(Glib::build_filename(Hub::get_resources_path(), "test.jpg"), test);
    }
#endif
}

#ifdef _TEST_STD_OUT

void print_circle_position(std::list<Circle> &circle_list) {
    for (Circle circle : circle_list) {
        std::cout << Chess::point_to_position(circle.center()) << " , ";
    }
    std::cout << std::endl;
}

#endif

gboolean ScreenShot::detect_chess_position(std::map<guint32, gint> &map) {
    cv::Mat screen;
    std::vector<cv::Vec3f> circle_vector;
    std::list<Circle> circle_list;

    screen = screen_shot();
    hough_detection_circle(screen, circle_vector);

    for (cv::Vec3f vf : circle_vector) {
        cv::Point center(cvRound(vf[0]), cvRound(vf[1]));
        int radius = cvRound(vf[2]);

        if (left_top_ == right_bottom_) {
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
    print_circle_position(circle_list);
#endif
    std::list<Circle>::iterator list_iter = circle_list.begin();
    cv::Point start_point(0, 0);
    gint continues_cout = 0;

    while (list_iter != circle_list.end()) {
        cv::Point p1 = list_iter->center();
        list_iter ++;
        if (list_iter == circle_list.end()) {
            break;
        }
        cv::Point p2 = list_iter->center();
        list_iter ++;
        if (list_iter == circle_list.end()) {
            break;
        }
        cv::Point p3 = list_iter->center();

        gdouble d1 = Chess::get_distance_by_position(p1, p2);
        gdouble d2 = Chess::get_distance_by_position(p2, p3);

        std::cout << "d1:" << d1 << " d2:" << d2 << std::endl;
        if(abs(d1 - d2) < 4) {
            if (start_point.x == 0 && start_point.y == 0) {
                start_point = p1;
            }
            continues_cout ++;
        } else {
            if (continues_cout == 7) {
                if (left_top_.x == 0 && left_top_.y == 0) {
                    left_top_ = start_point;
                } else {
                    right_bottom_ = p2;
                    break;
                }
            } else {
                start_point.x = 0;
                start_point.y = 0;
            }
            continues_cout = 0;
        }

        list_iter--;
    }

    return TRUE;
}
