//
// Created by xsy on 18-7-4.
//

#ifndef CHESSAGENT_SCREENSHOTTASK_H
#define CHESSAGENT_SCREENSHOTTASK_H

#include <opencv2/opencv.hpp>
#include <QMap>

#include "applicationutils.h"
#include "detection.h"

// �Զ�ѧϰ�ɹ����ɹ�ʶ�������̵�λ����Ϣ,����������̵������������������
#define DETECT_STUDY_SUCCESS                1
// �Զ�ʶ������ʧ�ܣ�����ԭ���ǲ�������δʶ��ɹ����������hough_detection_circle��ʶ�����
#define DETECT_STUDY_FAILED                 2
// ʶ���������Ϣ̫�٣��޷�����Զ�ʶ��
#define DETECT_STUDY_CIRCLE_TO_LITTILE      3
// �Զ�ѵ��ʧ�ܣ�ʶ�𵽵�������Ϣ��ȫ��������32������
#define DETECT_AUTOTRAIN_CIRCLE_LITTILE     4
// �Զ�ѵ��ʧ�ܣ�ʶ�������̫�ߣ��޷�׼ȷʶ������
#define DETECT_AUTOTRAIN_ERR_RATE_HIGH      5
// �Զ�ѵ���ɹ�
#define DETECT_AUTOTRAIN_SUCCESS            6
// ��������ǵ�ǰ�����
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
    // �Զ�ѧϰ��ס������������Ϣ
    cv::Point left_top_;
    cv::Point right_bottom_;
    // �Զ�ѧϰ��סʶ����������뾶�����ڴ���Ļ��ͼ����ȡ����
    int max_circle_radius_;
    // �궨�Ĵ��ڳߴ磬�����ж���������Ƿ������Ծ�Ĵ���
    cv::Size chess_window_size_;

    Detection *p_detection_;
    // ��ʼ���̶�Ӧ���������ͣ�����ѵ��ʶ���㷨
    int chess_position_type_[32];
    // �����������
    QMap<quint32, quint32> prev_chess_snap;
public:
    ScreenShot();

    virtual ~ScreenShot();

private:
    /**
     * ���ͼ���е�����circle
     * @param src
     * @param circles
     */
    void hough_detection_circle(cv::Mat &src, std::vector<cv::Vec3f> &circles);

    /**
     * ֻ���ͼ���еĵ���԰
     * @param src
     * @param circle
     */
    bool hough_detection_circle_single(cv::Mat &src, Circle &circle);

    /**
     * �Զ�ѧϰʶ����������
     *
     * @param circle_list
     */
    void study(std::list<Circle> &circle_list);

    /**
     * ����ʶ���Բ�������Զ�ʶ��������������
     * @param circle_list
     * @param screen
     * @param smples
     */
    void grab_samles(std::list<Circle> &circle_list, cv::Mat &screen, std::list<Sample> &samle_list);
    /**
     * ����λ�ڿ��ֽ׶Σ��Զ�ѵ��ʶ���㷨
     * @param circle_list
     * @param screen
     * @return
     */
    int auto_train(std::list<Circle> &circle_list, cv::Mat &screen);

    /**
     * ��Ļ����ת��������
     * @param point
     * @return
     */
    int coordinate_screen_to_chess(cv::Point &point);

    /**
     * ��������ת��Ļ����
     * @param point
     * @return
     */
    void coordinate_chess_to_screen(int in, cv::Point &point);

    /**
     * ʶ���������ɫ
     *
     * @param sample
     * @return
     */
    int detect_chess_color(Sample &sample);
public:

    int detect_chess_position(std::map<unsigned int, int> *map, cv::Mat &screen);
};


#endif //CHESSAGENT_SCREENSHOTTASK_H
