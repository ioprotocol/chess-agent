//
// Created by xsy on 18-7-2.
//

#include "chessaction.h"

#include <glibmm.h>

#include "application_utils.h"

ChessAction::ChessAction() {
    this->chess_board_ = cv::imread(Glib::build_filename(Hub::get_resources_img_path(), "MAIN.png"), cv::IMREAD_COLOR);
    if (this->chess_board_.empty()) {
        g_assert_not_reached();
    }

    top_left_ = cv::Point(51, 53);
    top_right_ = cv::Point(507, 53);
    bottom_left_ = cv::Point(51, 567);
    bottom_right_ = cv::Point(507, 567);

    gdouble dx = (top_right_.x - top_left_.x) / 8.0;
    gdouble dy = (bottom_left_.y - top_left_.y) / 9.0;

    gint y = 0, x = 0;
    for( y = 0; y < 10; y++)
    {
        for(x = 0; x < 9; x++)
        {
            positions_[y][x].x = top_left_.x + dx * x;
            positions_[y][x].y = top_left_.y + dy * y;
        }
    }

    y = 0;
    /**
     * initialize the chess pieces
     */
    chess_pieces_[y++] = new ChessPiece("BR", 0, 0, "黑車");
    chess_pieces_[y++] = new ChessPiece("BR", 8, 0, "黑車");

    chess_pieces_[y++] = new ChessPiece("RR", 0, 9, "红車");
    chess_pieces_[y++] = new ChessPiece("RR", 8, 9, "红車");

    chess_pieces_[y++] = new ChessPiece("BN", 1, 0, "黑马");
    chess_pieces_[y++] = new ChessPiece("BN", 7, 0, "黑马");

    chess_pieces_[y++] = new ChessPiece("RN", 1, 9, "红马");
    chess_pieces_[y++] = new ChessPiece("RN", 7, 9, "红马");

    chess_pieces_[y++] = new ChessPiece("BB", 2, 0, "黑象");
    chess_pieces_[y++] = new ChessPiece("BB", 6, 0, "黑象");

    chess_pieces_[y++] = new ChessPiece("RB", 2, 9, "红象");
    chess_pieces_[y++] = new ChessPiece("RB", 6, 9, "红象");

    chess_pieces_[y++] = new ChessPiece("BA", 3, 0, "黑士");
    chess_pieces_[y++] = new ChessPiece("BA", 5, 0, "黑士");

    chess_pieces_[y++] = new ChessPiece("RA", 3, 9, "红士");
    chess_pieces_[y++] = new ChessPiece("RA", 5, 9, "红士");

    chess_pieces_[y++] = new ChessPiece("BK", 4, 0, "黑将");

    chess_pieces_[y++] = new ChessPiece("RK", 4, 9, "红将");

    chess_pieces_[y++] = new ChessPiece("BC", 1, 2, "黑炮");
    chess_pieces_[y++] = new ChessPiece("BC", 7, 2, "黑炮");

    chess_pieces_[y++] = new ChessPiece("RC", 1, 7, "红炮");
    chess_pieces_[y++] = new ChessPiece("RC", 7, 7, "红炮");

    chess_pieces_[y++] = new ChessPiece("BP", 0, 3, "黑卒");
    chess_pieces_[y++] = new ChessPiece("BP", 2, 3, "黑卒");
    chess_pieces_[y++] = new ChessPiece("BP", 4, 3, "黑卒");
    chess_pieces_[y++] = new ChessPiece("BP", 6, 3, "黑卒");
    chess_pieces_[y++] = new ChessPiece("BP", 8, 3, "黑卒");

    chess_pieces_[y++] = new ChessPiece("RP", 0, 6, "红卒");
    chess_pieces_[y++] = new ChessPiece("RP", 2, 6, "红卒");
    chess_pieces_[y++] = new ChessPiece("RP", 4, 6, "红卒");
    chess_pieces_[y++] = new ChessPiece("RP", 6, 6, "红卒");
    chess_pieces_[y  ] = new ChessPiece("RP", 8, 6, "红卒");
}

ChessAction::~ChessAction() {
    this->chess_board_.release();
    for(int i = 0; i < 32; i++) {
        delete chess_pieces_[i];
        chess_pieces_[i] = NULL;
    }
}

void ChessAction::save_to_disk() {
    int thickness = 2;
    gint y = 0, x = 0;
    for( y = 0; y < 9; y++) {
        for(x = 0; x < 8; x++) {
            cv::line(this->chess_board_, positions_[y][x], positions_[y+1][x+1], cv::Scalar( 0, 0, 255), thickness, cv::LINE_8);
            cv::line(this->chess_board_, positions_[y+1][x], positions_[y][x+1], cv::Scalar( 0, 0, 255), thickness, cv::LINE_8);
        }
    }
    cv::imwrite("/home/xsy/main.png", this->chess_board_);
}

void ChessAction::reset_piece_position() {
    gint y = 0;
    // che
    chess_pieces_[y++]->set_position(0, 0);
    chess_pieces_[y++]->set_position(8, 0);
    chess_pieces_[y++]->set_position(0, 9);
    chess_pieces_[y++]->set_position(8, 9);
    // ma
    chess_pieces_[y++]->set_position(1, 0);
    chess_pieces_[y++]->set_position(7, 0);
    chess_pieces_[y++]->set_position(1, 9);
    chess_pieces_[y++]->set_position(7, 9);
    // xiang
    chess_pieces_[y++]->set_position(2, 0);
    chess_pieces_[y++]->set_position(6, 0);
    chess_pieces_[y++]->set_position(2, 9);
    chess_pieces_[y++]->set_position(6, 9);
    // shi
    chess_pieces_[y++]->set_position(3, 0);
    chess_pieces_[y++]->set_position(5, 0);
    chess_pieces_[y++]->set_position(3, 9);
    chess_pieces_[y++]->set_position(5, 9);
    // jiang
    chess_pieces_[y++]->set_position(4, 0);
    chess_pieces_[y++]->set_position(4, 9);
    // pao
    chess_pieces_[y++]->set_position(1, 2);
    chess_pieces_[y++]->set_position(7, 2);
    chess_pieces_[y++]->set_position(1, 7);
    chess_pieces_[y++]->set_position(7, 7);
    // zu
    chess_pieces_[y++]->set_position(0, 3);
    chess_pieces_[y++]->set_position(2, 3);
    chess_pieces_[y++]->set_position(4, 3);
    chess_pieces_[y++]->set_position(6, 3);
    chess_pieces_[y++]->set_position(8, 3);

    chess_pieces_[y++]->set_position(0, 6);
    chess_pieces_[y++]->set_position(2, 6);
    chess_pieces_[y++]->set_position(4, 6);
    chess_pieces_[y++]->set_position(6, 6);
    chess_pieces_[y  ]->set_position(8, 6);
}

void ChessAction::reverse_piece_position() {
    for(gint i = 0; i < 32; i++) {
        chess_pieces_[i]->set_position(chess_pieces_[i]->get_col(), 9 - chess_pieces_[i]->get_row());
    }
}

GdkPixbuf * ChessAction::generate_mat() {

    cv::Mat out = cv::Mat::zeros(558, 620, this->chess_board_.type());

    this->chess_board_.copyTo(out);

    for(gint i = 0; i < 32; i++) {
        cv::Point center = positions_[chess_pieces_[i]->get_row()][chess_pieces_[i]->get_col()];

        cv::Rect rect = cv::Rect(center.x - 28, center.y - 28, 57, 57);
        cv::Mat roi = out(rect);

        if (chess_pieces_[i]->get_enable()) {
            if (chess_pieces_[i]->get_active()) {
                cv_add4c_mat_q(roi, chess_pieces_[i]->get_img_active(), 1);
            } else {
                cv_add4c_mat_q(roi, chess_pieces_[i]->get_img(), 1);
            }
        }
    }

    std::vector<uchar> data_encode;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(100);
    cv::imencode(".jpeg", out, data_encode, params);

    GBytes* buffer = g_bytes_new(data_encode.data(), data_encode.size());

    GInputStream* inputStream = g_memory_input_stream_new_from_bytes(buffer);
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_stream(inputStream, NULL, NULL);

    g_bytes_unref(buffer);
    g_object_unref(inputStream);
    out.release();

    return pixbuf;
}

int ChessAction::cv_add4c_mat_q(cv::Mat &dst, cv::Mat &scr, double scale)
{
    if (dst.channels() != 3 || scr.channels() != 4)
    {
        return true;
    }
    if (scale < 0.01)
        return false;
    std::vector<cv::Mat>scr_channels;
    std::vector<cv::Mat>dstt_channels;
    split(scr, scr_channels);
    split(dst, dstt_channels);
    CV_Assert(scr_channels.size() == 4 && dstt_channels.size() == 3);

    if (scale < 1)
    {
        scr_channels[3] *= scale;
        scale = 1;
    }
    for (int i = 0; i < 3; i++)
    {
        dstt_channels[i] = dstt_channels[i].mul(255.0 / scale - scr_channels[3], scale / 255.0);
        dstt_channels[i] += scr_channels[i].mul(scr_channels[3], scale / 255.0);
    }
    merge(dstt_channels, dst);
    return true;
}
