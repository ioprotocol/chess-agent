//
// Created by xushy on 18-7-2.
//

#ifndef CHESSAGENT_PIECE_H
#define CHESSAGENT_PIECE_H

#define ICON_TYPE ".png"

#include <glibmm.h>
#include <opencv2/imgcodecs.hpp>

class ChessPiece {
private:
    Glib::ustring file_name_;
    // chess piece name
    Glib::ustring chess_name_;
    // icon 57x57
    cv::Mat img_;
    cv::Mat img_active_;
    // from left to right 0 ～ 8
    gint col_;
    // from up to down 0 ~ 9
    gint row_;
    // where it is killed, this will be set to false
    gboolean enable_;
    // is last active step
    gboolean active_;
public:
    gint get_col() const;

    gint get_row() const;

    gboolean get_enable() const;

    gboolean get_active() const;

    cv::Mat &get_img();

    cv::Mat &get_img_active();

public:
    ChessPiece(std::string file_name, gint col, gint row, std::string chess_name);

    virtual ~ChessPiece();

    void set_position(gint col, gint row);
};


#endif //CHESSAGENT_PIECE_H
