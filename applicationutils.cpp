//
// Created by xushy on 18-7-2.
//
#include "applicationutils.h"

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

QString Chess::get_chess_name(int type) {
	int tmp = type;
    QString prefix;
    QString name;
    if (tmp >= 10) {
        prefix.append("��");
		tmp = tmp - 10;
    } else {
        prefix = "��";
    }
    switch (tmp) {
        case R_CHE:
            name = "��";
            break;
        case R_MA:
            name = "��";
            break;
        case R_XIANG:
            name = "��";
            break;
        case R_SHI:
            name = "ʿ";
            break;
        case R_JIANG:
            name = "��";
            break;
        case R_PAO:
            name = "��";
            break;
        case R_ZU:
            name = "��";
            break;
    }
    return prefix.append(name);
}

QString Hub::current_dir() {
    return qEnvironmentVariable(PWD, QDir::currentPath());
}
