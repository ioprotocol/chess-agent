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

double Chess::get_distance_by_position(QPoint point1, QPoint point2) {
    double distance = sqrt(abs(point1.x() - point2.x()) * abs(point1.x() - point2.x()) + abs(point1.y() - point2.y()) * abs(point1.y() - point2.y()));
    return distance;
}

QString Chess::get_chess_name(int type) {
	int tmp = type;
    QString prefix;
    QString name;
    if (tmp >= 10) {
        prefix.append("黑");
		tmp = tmp - 10;
    } else {
        prefix = "红";
    }
    switch (tmp) {
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
    return prefix.append(name);
}

QRect Chess::detect_chess_board(cv::Mat &screen) {
    QRect rect(0, 0, 0, 0);

    QPoint start_point(0, 0);
    int continues_cout = 0;

    QList<QRect> list = hough_detection_circle(screen);

    if (list.size() < 32) {
        qErrnoWarning("detect_chess_board failed for circle is too little!");
        return rect;
    }

    QList<QRect>::iterator list_iter = list.begin();

    while (list_iter != list.end()) {
        QPoint p1 = list_iter->topLeft();
        list_iter++;
        if (list_iter == list.end()) {
            break;
        }
        QPoint p2 = list_iter->topLeft();
        list_iter++;
        if (list_iter == list.end()) {
            break;
        }
        QPoint p3 = list_iter->topLeft();

        double d1 = get_distance_by_position(p1, p2);
        double d2 = get_distance_by_position(p2, p3);

        if (abs(d1 - d2) < 12) {
            if (start_point.x() == 0 && start_point.y() == 0) {
                start_point = p1;
            }
            continues_cout++;
        } else {
            if (continues_cout == 7) {
                if (rect.width() == 0 && rect.height() == 0) {
                    rect.setTopLeft(start_point);
                } else {
                    rect.setWidth(p2.x() - start_point.x());
                    rect.setHeight(p2.y() - start_point.y());
                    qDebug() << "detect_chess_board successfully ";
                    break;
                }
            } else {
                start_point.setX(0);
                start_point.setY(0);
            }
            continues_cout = 0;
        }

        list_iter--;
    }

    return rect;
}

QList<QRect> Chess::hough_detection_circle(cv::Mat &src) {
    QList<QRect> circles;
    cv::Mat src_gray;
    std::vector<cv::Vec3f> cirs;
    cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
    GaussianBlur(src_gray, src_gray, cv::Size(3, 3), 2, 2);
    HoughCircles(src_gray, cirs, cv::HOUGH_GRADIENT, 1, 25, 208, 40, 20, 45);
    if (cirs.size() > 0) {
        for (cv::Vec3f vec3f : cirs) {
            int radius = cvRound(vec3f[2]);
            circles.push_back(QRect(vec3f[0], vec3f[1], radius, radius));
        }
    }
    return circles;
}

bool Chess::hough_detection_single_circle(cv::Mat &src, QRect &out) {
    QList<QRect> rects = hough_detection_circle(src);
    if (rects.size() < 1) {
        return false;
    }
    out = rects.front();
    return true;
}

QString Hub::current_dir() {
    return qEnvironmentVariable(PWD, QDir::currentPath());
}
