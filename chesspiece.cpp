//
// Created by xushy on 18-7-2.
//

#include "chesspiece.h"
#include "application_utils.h"

ChessPiece::ChessPiece(std::string file_name, gint col, gint row, std::string chess_name)
        : col_(col), row_(row), file_name_(file_name), chess_name_(chess_name)
{

    std::string abs_file_name = Glib::build_filename(Hub::get_resources_img_path(), file_name_, std::string(ICON_TYPE));
    std::string abs_active_file_name = Glib::build_filename(Hub::get_resources_img_path(), file_name_, std::string("S"),
                                                        std::string(ICON_TYPE));

    img_ = cv::imread(abs_file_name, cv::IMREAD_UNCHANGED);
    img_active_ = cv::imread(abs_active_file_name, cv::IMREAD_UNCHANGED);
    this->active_ = FALSE;
    this->enable_ = TRUE;

    if (this->img_.empty()) {
        g_assert_not_reached();
    }
    if (this->img_active_.empty()) {
        g_assert_not_reached();
    }
}

ChessPiece::~ChessPiece() {
}

void ChessPiece::set_position(gint col, gint row) {
    this->col_ = col;
    this->row_ = row;
}

gint ChessPiece::get_col() const {
    return col_;
}

gint ChessPiece::get_row() const {
    return row_;
}

gboolean ChessPiece::get_enable() const {
    return enable_;
}

gboolean ChessPiece::get_active() const {
    return active_;
}

cv::Mat &ChessPiece::get_img() {
    return img_;
}

cv::Mat &ChessPiece::get_img_active() {
    return img_active_;
}
