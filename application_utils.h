//
// Created by xushy on 18-7-2.
//

#ifndef CHESSAGENT_APPLICATION_UTILS_H
#define CHESSAGENT_APPLICATION_UTILS_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <gdk-pixbuf/gdk-pixbuf-core.h>

#define PWD "EPWD"

#define _TEST_STD_OUT

namespace Hub {

    std::string get_application_path();

    std::string get_resources_path();

    std::string get_resources_img_path();

    std::string get_resource_glade_file();

    cv::Mat pixbuffer_to_mat(GdkPixbuf* gdkPixbuf);
}

namespace Chess {
    enum Type {
        B_CHE = 10, B_MA = 11, B_XIANG = 12, B_SHI = 13, B_JIANG = 14, B_PAO = 15, B_ZU = 16,
        R_CHE = 0, R_MA = 1, R_XIANG = 2, R_SHI = 3, R_JIANG = 4, R_PAO = 5, R_ZU = 6
    };

    guint32 point_to_position(cv::Point point);

    guint32 point_to_position(gint x, gint y);

    cv::Point position_to_point(guint32 position);

    gboolean is_same_position(guint32 point1, guint32 point2);

    gdouble get_distance_by_position(guint32 point1, guint32 point2);

    gdouble get_distance_by_position(cv::Point point1, cv::Point point2);
}
#endif //CHESSAGENT_APPLICATION_UTILS_H
