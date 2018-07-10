//
// Created by xsy on 18-7-2.
//

#include "chess.h"

#include <glibmm.h>

#include "application-utils.h"

Chess::Chess() {
    this->chessBoard = cv::imread(Glib::build_filename(get_resources_img_path(), "MAIN.png"), cv::IMREAD_COLOR);
    if (this->chessBoard.empty()) {
        g_assert_not_reached();
    }

    topLeft = cv::Point(51, 53);
    topRight = cv::Point(507, 53);
    bottomLeft = cv::Point(51, 567);
    bottomRight = cv::Point(507, 567);

    gdouble dx = (topRight.x - topLeft.x) / 8.0;
    gdouble dy = (bottomLeft.y - topLeft.y) / 9.0;

    gint y = 0, x = 0;
    for( y = 0; y < 10; y++)
    {
        for(x = 0; x < 9; x++)
        {
            positions[y][x].x = topLeft.x + dx * x;
            positions[y][x].y = topLeft.y + dy * y;
        }
    }

    y = 0;
    /**
     * initialize the chess pieces
     */
    chessPieces[y++] = new Piece("BR", 0, 0, "黑車");
    chessPieces[y++] = new Piece("BR", 8, 0, "黑車");

    chessPieces[y++] = new Piece("RR", 0, 9, "红車");
    chessPieces[y++] = new Piece("RR", 8, 9, "红車");

    chessPieces[y++] = new Piece("BN", 1, 0, "黑马");
    chessPieces[y++] = new Piece("BN", 7, 0, "黑马");

    chessPieces[y++] = new Piece("RN", 1, 9, "红马");
    chessPieces[y++] = new Piece("RN", 7, 9, "红马");

    chessPieces[y++] = new Piece("BB", 2, 0, "黑象");
    chessPieces[y++] = new Piece("BB", 6, 0, "黑象");

    chessPieces[y++] = new Piece("RB", 2, 9, "红象");
    chessPieces[y++] = new Piece("RB", 6, 9, "红象");

    chessPieces[y++] = new Piece("BA", 3, 0, "黑士");
    chessPieces[y++] = new Piece("BA", 5, 0, "黑士");

    chessPieces[y++] = new Piece("RA", 3, 9, "红士");
    chessPieces[y++] = new Piece("RA", 5, 9, "红士");

    chessPieces[y++] = new Piece("BK", 4, 0, "黑将");

    chessPieces[y++] = new Piece("RK", 4, 9, "红将");

    chessPieces[y++] = new Piece("BC", 1, 2, "黑炮");
    chessPieces[y++] = new Piece("BC", 7, 2, "黑炮");

    chessPieces[y++] = new Piece("RC", 1, 7, "红炮");
    chessPieces[y++] = new Piece("RC", 7, 7, "红炮");

    chessPieces[y++] = new Piece("BP", 0, 3, "黑卒");
    chessPieces[y++] = new Piece("BP", 2, 3, "黑卒");
    chessPieces[y++] = new Piece("BP", 4, 3, "黑卒");
    chessPieces[y++] = new Piece("BP", 6, 3, "黑卒");
    chessPieces[y++] = new Piece("BP", 8, 3, "黑卒");

    chessPieces[y++] = new Piece("RP", 0, 6, "红卒");
    chessPieces[y++] = new Piece("RP", 2, 6, "红卒");
    chessPieces[y++] = new Piece("RP", 4, 6, "红卒");
    chessPieces[y++] = new Piece("RP", 6, 6, "红卒");
    chessPieces[y  ] = new Piece("RP", 8, 6, "红卒");
}

Chess::~Chess() {
    this->chessBoard.release();
    for(int i = 0; i < 32; i++) {
        delete chessPieces[i];
        chessPieces[i] = NULL;
    }
}

void Chess::saveToDisk() {
    int thickness = 2;
    gint y = 0, x = 0;
    for( y = 0; y < 9; y++) {
        for(x = 0; x < 8; x++) {
            cv::line(this->chessBoard, positions[y][x], positions[y+1][x+1], cv::Scalar( 0, 0, 255), thickness, cv::LINE_8);
            cv::line(this->chessBoard, positions[y+1][x], positions[y][x+1], cv::Scalar( 0, 0, 255), thickness, cv::LINE_8);
        }
    }
    cv::imwrite("/home/xsy/main.png", this->chessBoard);
}

void Chess::resetPiecePosition() {
    gint y = 0;
    // che
    chessPieces[y++]->set_position(0, 0);
    chessPieces[y++]->set_position(8, 0);
    chessPieces[y++]->set_position(0, 9);
    chessPieces[y++]->set_position(8, 9);
    // ma
    chessPieces[y++]->set_position(1, 0);
    chessPieces[y++]->set_position(7, 0);
    chessPieces[y++]->set_position(1, 9);
    chessPieces[y++]->set_position(7, 9);
    // xiang
    chessPieces[y++]->set_position(2, 0);
    chessPieces[y++]->set_position(6, 0);
    chessPieces[y++]->set_position(2, 9);
    chessPieces[y++]->set_position(6, 9);
    // shi
    chessPieces[y++]->set_position(3, 0);
    chessPieces[y++]->set_position(5, 0);
    chessPieces[y++]->set_position(3, 9);
    chessPieces[y++]->set_position(5, 9);
    // jiang
    chessPieces[y++]->set_position(4, 0);
    chessPieces[y++]->set_position(4, 9);
    // pao
    chessPieces[y++]->set_position(1, 2);
    chessPieces[y++]->set_position(7, 2);
    chessPieces[y++]->set_position(1, 7);
    chessPieces[y++]->set_position(7, 7);
    // zu
    chessPieces[y++]->set_position(0, 3);
    chessPieces[y++]->set_position(2, 3);
    chessPieces[y++]->set_position(4, 3);
    chessPieces[y++]->set_position(6, 3);
    chessPieces[y++]->set_position(8, 3);

    chessPieces[y++]->set_position(0, 6);
    chessPieces[y++]->set_position(2, 6);
    chessPieces[y++]->set_position(4, 6);
    chessPieces[y++]->set_position(6, 6);
    chessPieces[y  ]->set_position(8, 6);
}

void Chess::reversePiecePosition() {
    for(gint i = 0; i < 32; i++) {
        chessPieces[i]->set_position(chessPieces[i]->get_col(), 9 - chessPieces[i]->get_row());
    }
}

GdkPixbuf * Chess::generateMat() {

    cv::Mat out = cv::Mat::zeros(558, 620, this->chessBoard.type());

    this->chessBoard.copyTo(out);

    for(gint i = 0; i < 32; i++) {
        cv::Point center = positions[chessPieces[i]->get_row()][chessPieces[i]->get_col()];

        cv::Rect rect = cv::Rect(center.x - 28, center.y - 28, 57, 57);
        cv::Mat roi = out(rect);

        if (chessPieces[i]->get_enable()) {
            if (chessPieces[i]->get_active()) {
                cvAdd4cMat_q(roi, chessPieces[i]->get_img_active(), 1);
            } else {
                cvAdd4cMat_q(roi, chessPieces[i]->get_img(), 1);
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

int Chess::cvAdd4cMat_q(cv::Mat &dst, cv::Mat &scr, double scale)
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
