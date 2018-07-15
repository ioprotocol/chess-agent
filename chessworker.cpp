//
// Created by xsy on 18-7-10.
//

#include "chessworker.h"
#include <iostream>
#include "chesswindow.h"

void ChessWorker::run(ChessWindow *chessWindow) {

    chessWindow->set_worker_running_flag(TRUE);

    std::cout << "thread is run" << std::endl;
    usleep(3*1000*1000);

    std::map<guint32, gint> chess_position;

    while (chessWindow->get_worker_running_flag()) {

        chess_position.clear();
        chessWindow->get_screen_shot().detect_chess_position(chess_position);
        print_chess_position(chess_position);

        usleep(1*1000*1000);
    }

    chessWindow->notify();
}

void ChessWorker::print_chess_position(std::map<guint32, gint> &posotion_map) {
    std::map<guint32, gint>::iterator it = posotion_map.begin();

    while (it != posotion_map.end()) {
        std::cout << "position:" << it->first << " type:" << Chess::get_chess_name(it->second) << std::endl;
        it ++;
    }
    std::cout << "************************************************************************" << std::endl;
}
