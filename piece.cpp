//
// Created by xushy on 18-7-2.
//

#include "piece.h"
#include "application-utils.h"

const cv::Mat &Piece::getImg() const {
    return img;
}

void Piece::setImg(const cv::Mat &img) {
    Piece::img = img;
}

gint Piece::getCol() const {
    return col;
}

void Piece::setCol(gint col) {
    Piece::col = col;
}

gint Piece::getRow() const {
    return row;
}

void Piece::setRow(gint row) {
    Piece::row = row;
}

const std::string &Piece::getName() const {
    return name;
}

void Piece::setName(const std::string &name) {
    Piece::name = name;
}

gboolean Piece::getIs_red() const {
    return is_red;
}

void Piece::setIs_red(gboolean is_red) {
    Piece::is_red = is_red;
}

Piece::Piece(const std::string &img_name, gint col, gint row, const std::string &name, gboolean is_red) : col(col),
                                                                                                 row(row), name(name),
                                                                                                 is_red(is_red) {
    char *application_path, *img_path, *file_name;
    application_path = get_application_path();
    img_path = g_build_filename(application_path, "resources", NULL);
    file_name = g_build_filename(img_path, img_name.data(), NULL);

    this->img = cv::imread(file_name);

//    g_free(application_path);
    g_free(img_path);
    g_free(file_name);
}
