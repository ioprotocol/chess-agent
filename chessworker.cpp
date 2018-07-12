//
// Created by xsy on 18-7-10.
//

#include "chessworker.h"
#include <iostream>
#include "chesswindow.h"

void ChessWorker::run(ChessWindow *chessWindow) {
    std::cout << "thread is run" << std::endl;
    usleep(3*1000*1000);

    std::map<gint, gint> chess_position;

    while (true) {
        cv::Mat scrren = chessWindow->get_screen_shot().screen_shot();

        chessWindow->get_screen_shot().detect_chess_position(scrren, chess_position);

        print_chess_position(chess_position);

        usleep(1*1000*1000);
    }

    chessWindow->notify();
}

void ChessWorker::print_chess_position(std::map<gint, gint> &posotion_map) {
    std::map<gint, gint>::iterator it = posotion_map.begin();

    while (it != posotion_map.end()) {
        std::cout << "position:" << it->first << " type:" << it->second << std::endl;
        it ++;
    }
    std::cout << "************************************************************************" << std::endl;
}
