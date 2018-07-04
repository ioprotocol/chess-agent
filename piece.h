//
// Created by xushy on 18-7-2.
//

#ifndef CHESSAGENT_PIECE_H
#define CHESSAGENT_PIECE_H

#define ICON_TYPE ".png"

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <opencv2/imgcodecs.hpp>

class Piece {
private:
    gchar* imgName;
    // icon 57x57
    cv::Mat img;
    cv::Mat imgActive;
    // from left to right 0 ～ 8
    gint col;
    // from up to down 0 ~ 9
    gint row;
    // chess piece name
    gchar* name;
    // where it is killed, this will be set to false
    gboolean isEnable;
    // is last active step
    gboolean isActive;
public:
    Piece(const gchar* path, gint col, gint row, const gchar* name);

    virtual ~Piece();

    gboolean isRed();

    void saveToDisk(const gchar* path);

    cv::Mat &getImg();

    gint getCol() const;

    gint getRow() const;

    gchar *getName() const;

    gboolean getIsEnable() const;

    void setImg(const cv::Mat &img);

    void setCol(gint col);

    void setRow(gint row);

    void setPosition(gint col, gint row);

    void setName(gchar *name);

    void setIsEnable(gboolean isEnable);

    gboolean getIsActive() const;

    void setIsActive(gboolean isActive);

    cv::Mat &getImgActive();

    void setImgActive(const cv::Mat &imgActive);
};


#endif //CHESSAGENT_PIECE_H
