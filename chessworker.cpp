//
// Created by xsy on 18-7-10.
//

#include "chessworker.h"
#include <iostream>
#include "chesswindow.h"

void ChessWorker::run(ChessWindow *chessWindow) {
    std::cout << "thread is run" << std::endl;
    usleep(3*1000*1000);
    chessWindow->get_screen_shot().screen_shot();
    chessWindow->notify();
}
