//
// Created by xushy on 18-7-2.
//

#include "piece.h"
#include "application-utils.h"

Piece::Piece(std::string file_name, gint col, gint row, std::string chess_name)
        : m_col(col), m_row(row), m_file_name(file_name), m_chess_name(chess_name)
{

    std::string abs_file_name = Glib::build_filename(get_resources_img_path(), m_file_name, std::string(ICON_TYPE));
    std::string abs_active_file_name = Glib::build_filename(get_resources_img_path(), m_file_name, std::string("S"),
                                                        std::string(ICON_TYPE));

    m_img = cv::imread(abs_file_name, cv::IMREAD_UNCHANGED);
    m_img_active = cv::imread(abs_active_file_name, cv::IMREAD_UNCHANGED);
    this->m_active = FALSE;
    this->m_enable = TRUE;

    if (this->m_img.empty()) {
        g_assert_not_reached();
    }
    if (this->m_img_active.empty()) {
        g_assert_not_reached();
    }
}

Piece::~Piece() {
}

void Piece::set_position(gint col, gint row) {
    this->m_col = col;
    this->m_row = row;
}

gint Piece::get_col() const {
    return m_col;
}

gint Piece::get_row() const {
    return m_row;
}

gboolean Piece::get_enable() const {
    return m_enable;
}

gboolean Piece::get_active() const {
    return m_active;
}

cv::Mat &Piece::get_img() {
    return m_img;
}

cv::Mat &Piece::get_img_active() {
    return m_img_active;
}
