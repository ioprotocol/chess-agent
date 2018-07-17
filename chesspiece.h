//
// Created by xushy on 18-7-2.
//

#ifndef CHESSAGENT_PIECE_H
#define CHESSAGENT_PIECE_H

#define ICON_TYPE ".png"

#include <opencv2/imgcodecs.hpp>
#include <QString>

class ChessPiece {
private:
    QString file_name_;
    // chess piece name
    QString chess_name_;
    // icon 57x57
    cv::Mat img_;
    cv::Mat img_active_;
    // from left to right 0 ～ 8
    int col_;
    // from up to down 0 ~ 9
    int row_;
    // where it is killed, this will be set to false
    bool enable_;
    // is last active step
    bool active_;
public:
    int get_col() const;

    int get_row() const;

    bool get_enable() const;

    bool get_active() const;

    cv::Mat &get_img();

    cv::Mat &get_img_active();

public:
    ChessPiece(QString file_name, int col, int row, QString chess_name);

    virtual ~ChessPiece();

    void set_position(int col, int row);
};


#endif //CHESSAGENT_PIECE_H
