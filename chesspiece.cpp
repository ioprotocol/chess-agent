//
// Created by xushy on 18-7-2.
//

#include "chesspiece.h"
#include "application_utils.h"

ChessPiece::ChessPiece(QString file_name, int col, int row, QString chess_name)
        : col_(col), row_(row), file_name_(file_name), chess_name_(chess_name)
{

    QString icon_file_name = Hub::current_dir().append("/resources/img/").append(file_name).append(ICON_TYPE);
    QString icon_active_file_name = Hub::current_dir().append("/resources/img/").append(file_name).append("S").append(
            ICON_TYPE);

    img_ = cv::imread(icon_file_name.toStdString(), cv::IMREAD_UNCHANGED);
    img_active_ = cv::imread(icon_active_file_name.toStdString(), cv::IMREAD_UNCHANGED);
    this->active_ = false;
    this->enable_ = true;

    if (this->img_.empty()) {
        qErrnoWarning("chess img load failed");
    }
    if (this->img_active_.empty()) {
        qErrnoWarning("chess img-active load failed");
    }
}

ChessPiece::~ChessPiece() {
}

void ChessPiece::set_position(int col, int row) {
    this->col_ = col;
    this->row_ = row;
}

int ChessPiece::get_col() const {
    return col_;
}

int ChessPiece::get_row() const {
    return row_;
}

bool ChessPiece::get_enable() const {
    return enable_;
}

bool ChessPiece::get_active() const {
    return active_;
}

cv::Mat &ChessPiece::get_img() {
    return img_;
}

cv::Mat &ChessPiece::get_img_active() {
    return img_active_;
}
