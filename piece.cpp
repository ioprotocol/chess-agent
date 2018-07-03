//
// Created by xushy on 18-7-2.
//

#include "piece.h"
#include "application-utils.h"


Piece::Piece(const gchar* path, gint col, gint row, const gchar* name)
        : col(col), row(row) {
    gchar *resource_img_path, *file_name, *active_file_name, *abs_file_name, *abs_active_file_name;

    resource_img_path = get_resources_img_path();
    file_name = g_strconcat(path, ICON_TYPE, NULL);
    active_file_name = g_strconcat(path, "S", ICON_TYPE, NULL);

    abs_file_name = g_build_filename(resource_img_path, file_name, NULL);
    abs_active_file_name = g_build_filename(resource_img_path, active_file_name, NULL);

    this->img = cv::imread(abs_file_name, cv::IMREAD_COLOR);
    this->imgActive = cv::imread(abs_active_file_name, cv::IMREAD_COLOR);
    this->isActive = FALSE;
    this->isEnable = TRUE;

    if (this->img.empty()) {
        g_assert_not_reached();
    }
    if (this->imgActive.empty()) {
        g_assert_not_reached();
    }

    this->imgName = g_strdup(path);

    if(name != NULL)
    {
        this->name = g_strdup(name);
    }

    g_free(resource_img_path);
    g_free(file_name);
    g_free(abs_file_name);
    g_free(active_file_name);
    g_free(abs_active_file_name);
}

Piece::~Piece() {
    g_free(this->name);
    g_free(this->imgName);
    this->img.release();
}

gboolean Piece::isRed() {
    return this->imgName[0] == 'R';
}

void Piece::saveToDisk(const gchar *path) {
    cv::imwrite(path, this->img);
}

const cv::Mat &Piece::getImg() const {
    return img;
}

gint Piece::getCol() const {
    return col;
}

gint Piece::getRow() const {
    return row;
}

gchar *Piece::getName() const {
    return name;
}

gboolean Piece::getIsEnable() const {
    return isEnable;
}

void Piece::setImg(const cv::Mat &img) {
    Piece::img = img;
}

void Piece::setCol(gint col) {
    Piece::col = col;
}

void Piece::setRow(gint row) {
    Piece::row = row;
}

void Piece::setName(gchar *name) {
    Piece::name = name;
}

void Piece::setIsEnable(gboolean isEnable) {
    Piece::isEnable = isEnable;
}

gboolean Piece::getIsActive() const {
    return isActive;
}

void Piece::setIsActive(gboolean isActive) {
    Piece::isActive = isActive;
}

const cv::Mat &Piece::getImgActive() const {
    return imgActive;
}

void Piece::setImgActive(const cv::Mat &imgActive) {
    Piece::imgActive = imgActive;
}

void Piece::setPosition(gint col, gint row) {
    Piece::col = col;
    Piece::row = row;
}
