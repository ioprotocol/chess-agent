//
// Created by xsy on 18-7-10.
//

#include "chess-worker.h"
#include <iostream>
#include "chess-window.h"

void ChessWorker::run(ChessWindow *chessWindow) {
    std::cout << "thread is run" << std::endl;
    usleep(3*1000*1000);
    chessWindow->notify();
}
