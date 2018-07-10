//
// Created by xushy on 18-7-2.
//

#ifndef CHESSAGENT_PIECE_H
#define CHESSAGENT_PIECE_H

#define ICON_TYPE ".png"

#include <glibmm.h>
#include <opencv2/imgcodecs.hpp>

class Piece {
private:
    Glib::ustring m_file_name;
    // chess piece name
    Glib::ustring m_chess_name;
    // icon 57x57
    cv::Mat m_img;
    cv::Mat m_img_active;
    // from left to right 0 ～ 8
    gint m_col;
    // from up to down 0 ~ 9
    gint m_row;
    // where it is killed, this will be set to false
    gboolean m_enable;
    // is last active step
    gboolean m_active;
public:
    gint get_col() const;

    gint get_row() const;

    gboolean get_enable() const;

    gboolean get_active() const;

    cv::Mat &get_img();

    cv::Mat &get_img_active();

public:
    Piece(std::string file_name, gint col, gint row, std::string chess_name);

    virtual ~Piece();

    void set_position(gint col, gint row);
};


#endif //CHESSAGENT_PIECE_H
