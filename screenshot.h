//
// Created by xsy on 18-7-4.
//

#ifndef CHESSAGENT_SCREENSHOTTASK_H
#define CHESSAGENT_SCREENSHOTTASK_H

#include <opencv2/opencv.hpp>

#include "application_utils.h"
#include "detection.h"

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
// 象棋程序不是当前活动窗口
#define DETECT_WINDOW_IS_NOT_ACTIVE         7

class Circle {
private:
    cv::Point center_;
    int radius_;

public:
    Circle() {}

    Circle(cv::Point center, int radius) : center_(center), radius_(radius) {}

    inline cv::Point &center() {
        return center_;
    }

    inline int radius() {
        return radius_;
    }

    bool operator < (const Circle& other) const {
        if(abs(center_.y - other.center_.y) < 15) {
            return center_.x < other.center_.x;
        }
        return Chess::point_to_uint32(center_) < Chess::point_to_uint32(other.center_);
    }

    void set_center(const cv::Point &center) {
        center_.x = center.x;
        center_.y = center.y;
    }

    void set_radius(int radius) {
        radius_ = radius;
    }
};

class ScreenShot {
private:
    // 自动学习记住的棋盘坐标信息
    cv::Point left_top_;
    cv::Point right_bottom_;
    // 自动学习记住识别到最大的象棋半径，用于从屏幕截图中提取象棋
    int max_circle_radius_;
    // 标定的窗口尺寸，用于判断象棋程序是否是最活跃的窗口
    cv::Size chess_window_size_;

    Detection *p_detection_;
    // 初始棋盘对应的棋子类型，用于训练识别算法
    int chess_position_type_[32];
public:
    ScreenShot();

    virtual ~ScreenShot();

private:
    /**
     * 检测图像中的所有circle
     * @param src
     * @param circles
     */
    void hough_detection_circle(cv::Mat &src, std::vector<cv::Vec3f> &circles);

    /**
     * 只检测图像中的单个园
     * @param src
     * @param circle
     */
    void hough_detection_circle_single(cv::Mat &src, Circle &circle);

    /**
     * 自动学习识别棋盘坐标
     *
     * @param circle_list
     */
    void study(std::list<Circle> &circle_list);

    /**
     * 根据识别的圆形坐标自动识别棋子完整样本
     * @param circle_list
     * @param screen
     * @param smples
     */
    void grab_samles(std::list<Circle> &circle_list, cv::Mat &screen, std::list<Sample> &samle_list);
    /**
     * 棋盘位于开局阶段，自动训练识别算法
     * @param circle_list
     * @param screen
     * @return
     */
    int auto_train(std::list<Circle> &circle_list, cv::Mat &screen);

    /**
     * 屏幕坐标转象棋坐标
     * @param point
     * @return
     */
    int coordinate_screen_to_chess(cv::Point &point);

    /**
     * 象棋坐标转屏幕坐标
     * @param point
     * @return
     */
    void coordinate_chess_to_screen(int in, cv::Point &point);

    /**
     * 识别象棋的颜色
     *
     * @param sample
     * @return
     */
    int detect_chess_color(Sample &sample);
public:

    int detect_chess_position(std::map<unsigned int, int> *map, cv::Mat &screen);
};


#endif //CHESSAGENT_SCREENSHOTTASK_H
