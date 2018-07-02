//
// Created by xushy on 18-7-2.
//

#ifndef CHESSAGENT_PIECE_H
#define CHESSAGENT_PIECE_H

#include <string>
#include <gtk/gtk.h>
#include "opencv2/imgcodecs.hpp"

class Piece {
private:
    // icon
    cv::Mat img;
    // from left to right 0 ～ 8
    gint col;
    // from up to down 0 ~ 9
    gint row;
    // chess piece name
    std::string name;
    gboolean is_red;
public:
    Piece(const std::string &img_name, gint col, gint row, const std::string &name, gboolean is_red);

    const cv::Mat &getImg() const;

    void setImg(const cv::Mat &img);

    gint getCol() const;

    void setCol(gint col);

    gint getRow() const;

    void setRow(gint row);

    const std::string &getName() const;

    void setName(const std::string &name);

    gboolean getIs_red() const;

    void setIs_red(gboolean is_red);
};


#endif //CHESSAGENT_PIECE_H
