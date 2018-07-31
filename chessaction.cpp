//
// Created by xsy on 18-7-2.
//

#include "chessaction.h"
#include "applicationutils.h"

ChessAction::ChessAction() {
    this->chess_board_ = cv::imread((Hub::current_dir() + "/resources/img/MAIN.png").toStdString(), cv::IMREAD_COLOR);
    if (this->chess_board_.empty()) {
        qErrnoWarning("chess board MAIN.png load failed");
    }

    top_left_ = cv::Point(51, 53);
    top_right_ = cv::Point(507, 53);
    bottom_left_ = cv::Point(51, 567);
    bottom_right_ = cv::Point(507, 567);

    double dx = (top_right_.x - top_left_.x) / 8.0;
    double dy = (bottom_left_.y - top_left_.y) / 9.0;

    int y = 0, x = 0;
    for( y = 0; y < 10; y++)
    {
        for(x = 0; x < 9; x++)
        {
            positions_[y][x].x = top_left_.x + dx * x;
            positions_[y][x].y = top_left_.y + dy * y;
        }
    }

    normal_img[0] = cv::imread(Hub::current_dir().append("/resources/img").append("/RR.png").toStdString(), cv::IMREAD_UNCHANGED);
    normal_img[1] = cv::imread(Hub::current_dir().append("/resources/img").append("/RN.png").toStdString(), cv::IMREAD_UNCHANGED);
    normal_img[2] = cv::imread(Hub::current_dir().append("/resources/img").append("/RB.png").toStdString(), cv::IMREAD_UNCHANGED);
    normal_img[3] = cv::imread(Hub::current_dir().append("/resources/img").append("/RA.png").toStdString(), cv::IMREAD_UNCHANGED);
    normal_img[4] = cv::imread(Hub::current_dir().append("/resources/img").append("/RK.png").toStdString(), cv::IMREAD_UNCHANGED);
    normal_img[5] = cv::imread(Hub::current_dir().append("/resources/img").append("/RC.png").toStdString(), cv::IMREAD_UNCHANGED);
    normal_img[6] = cv::imread(Hub::current_dir().append("/resources/img").append("/RP.png").toStdString(), cv::IMREAD_UNCHANGED);

    normal_img[10] = cv::imread(Hub::current_dir().append("/resources/img").append("/BR.png").toStdString(), cv::IMREAD_UNCHANGED);
    normal_img[11] = cv::imread(Hub::current_dir().append("/resources/img").append("/BN.png").toStdString(), cv::IMREAD_UNCHANGED);
    normal_img[12] = cv::imread(Hub::current_dir().append("/resources/img").append("/BB.png").toStdString(), cv::IMREAD_UNCHANGED);
    normal_img[13] = cv::imread(Hub::current_dir().append("/resources/img").append("/BA.png").toStdString(), cv::IMREAD_UNCHANGED);
    normal_img[14] = cv::imread(Hub::current_dir().append("/resources/img").append("/BK.png").toStdString(), cv::IMREAD_UNCHANGED);
    normal_img[15] = cv::imread(Hub::current_dir().append("/resources/img").append("/BC.png").toStdString(), cv::IMREAD_UNCHANGED);
    normal_img[16] = cv::imread(Hub::current_dir().append("/resources/img").append("/BP.png").toStdString(), cv::IMREAD_UNCHANGED);

    selected_img[0] = cv::imread(Hub::current_dir().append("/resources/img").append("/RRS.png").toStdString(), cv::IMREAD_UNCHANGED);
    selected_img[1] = cv::imread(Hub::current_dir().append("/resources/img").append("/RNS.png").toStdString(), cv::IMREAD_UNCHANGED);
    selected_img[2] = cv::imread(Hub::current_dir().append("/resources/img").append("/RBS.png").toStdString(), cv::IMREAD_UNCHANGED);
    selected_img[3] = cv::imread(Hub::current_dir().append("/resources/img").append("/RAS.png").toStdString(), cv::IMREAD_UNCHANGED);
    selected_img[4] = cv::imread(Hub::current_dir().append("/resources/img").append("/RKS.png").toStdString(), cv::IMREAD_UNCHANGED);
    selected_img[5] = cv::imread(Hub::current_dir().append("/resources/img").append("/RCS.png").toStdString(), cv::IMREAD_UNCHANGED);
    selected_img[6] = cv::imread(Hub::current_dir().append("/resources/img").append("/RPS.png").toStdString(), cv::IMREAD_UNCHANGED);

    selected_img[10] = cv::imread(Hub::current_dir().append("/resources/img").append("/BRS.png").toStdString(), cv::IMREAD_UNCHANGED);
    selected_img[11] = cv::imread(Hub::current_dir().append("/resources/img").append("/BNS.png").toStdString(), cv::IMREAD_UNCHANGED);
    selected_img[12] = cv::imread(Hub::current_dir().append("/resources/img").append("/BBS.png").toStdString(), cv::IMREAD_UNCHANGED);
    selected_img[13] = cv::imread(Hub::current_dir().append("/resources/img").append("/BAS.png").toStdString(), cv::IMREAD_UNCHANGED);
    selected_img[14] = cv::imread(Hub::current_dir().append("/resources/img").append("/BKS.png").toStdString(), cv::IMREAD_UNCHANGED);
    selected_img[15] = cv::imread(Hub::current_dir().append("/resources/img").append("/BCS.png").toStdString(), cv::IMREAD_UNCHANGED);
    selected_img[16] = cv::imread(Hub::current_dir().append("/resources/img").append("/BPS.png").toStdString(), cv::IMREAD_UNCHANGED);
}

ChessAction::~ChessAction() {
    this->chess_board_.release();
}

void ChessAction::save_to_disk() {
    int thickness = 2;
    int y = 0, x = 0;
    for( y = 0; y < 9; y++) {
        for(x = 0; x < 8; x++) {
            cv::line(this->chess_board_, positions_[y][x], positions_[y+1][x+1], cv::Scalar( 0, 0, 255), thickness, cv::LINE_8);
            cv::line(this->chess_board_, positions_[y+1][x], positions_[y][x+1], cv::Scalar( 0, 0, 255), thickness, cv::LINE_8);
        }
    }
    cv::imwrite("/home/xsy/main.png", this->chess_board_);
}

QPixmap ChessAction::generate_pixture(std::map<unsigned int, int> &chess_position_map) {
    cv::Mat out = cv::Mat::zeros(558, 620, this->chess_board_.type());
    this->chess_board_.copyTo(out);

    std::map<unsigned int, int>::iterator it = chess_position_map.begin();

    while (it != chess_position_map.end()) {
        qint32 type = it->second;
        if (type < 0 || type > 16 || (type > 6 && type < 10)) {
            qErrnoWarning("invalid chess type value");
        }
        cv::Point pt = Chess::uint32_to_point(it->first);
        cv::Point center = positions_[pt.y][pt.x];
        cv::Rect rect = cv::Rect(center.x - 28, center.y - 28, 57, 57);
        cv::Mat roi = out(rect);
        cv_add4c_mat_q(roi, normal_img[type], 1);
        it ++;
    }

    cv::imwrite("D:\\test.jpg", out);

    std::vector<uchar> data_encode;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(100);
    cv::imencode(".jpeg", out, data_encode, params);

    QPixmap pixmap;
    pixmap.loadFromData(data_encode.data(), data_encode.size(), "jpeg");
    return pixmap;
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
