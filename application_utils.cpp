//
// Created by xushy on 18-7-2.
//
#include "application_utils.h"

#include <QDir>
#include <opencv2/imgcodecs.hpp>


cv::Point Chess::uint32_to_point(unsigned int position) {
    return cv::Point(position % 10000, position / 10000);
}

unsigned int  Chess::point_to_uint32(cv::Point point) {
    return point_to_uint32(point.x, point.y);
}

unsigned int  Chess::point_to_uint32(int x, int y) {
    return y * 10000 + x;
}

bool Chess::is_same_position(unsigned int  point1, unsigned int  point2) {
    return get_distance_by_position(point1, point2) < 6;
}

double Chess::get_distance_by_position(unsigned int  point1, unsigned int  point2) {
    int x1 = point1 % 10000;
    int y1 = point1 / 10000;
    int x2 = point2 % 10000;
    int y2 = point2 / 10000;
    double distance = sqrt(abs(x1 - x2) * abs(x1 - x2) + abs(y1 - y2) * abs(y1 - y2));
    return distance;
}

double Chess::get_distance_by_position(cv::Point point1, cv::Point point2) {
    return get_distance_by_position(point_to_uint32(point1), point_to_uint32(point2));
}

std::string Chess::get_chess_name(int type) {
    std::string prefix;
    std::string name;
    if (type >= 10) {
        prefix = "黑";
        type = type - 10;
    } else {
        prefix = "红";
    }
    switch (type) {
        case R_CHE:
            name = "车";
            break;
        case R_MA:
            name = "马";
            break;
        case R_XIANG:
            name = "相";
            break;
        case R_SHI:
            name = "士";
            break;
        case R_JIANG:
            name = "将";
            break;
        case R_PAO:
            name = "炮";
            break;
        case R_ZU:
            name = "卒";
            break;
    }
    return prefix + name;
}

QString Hub::current_dir() {
    QByteArray env = qgetenv(PWD);
    if (env.size() < 1) {
        return QDir::currentPath();
    }
    return QString(env);
}
