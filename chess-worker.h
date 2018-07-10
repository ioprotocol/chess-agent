//
// Created by xsy on 18-7-10.
//

#ifndef CHESSAGENT_CHESS_WORKER_H
#define CHESSAGENT_CHESS_WORKER_H

#include <gtkmm.h>
#include <thread>

class ChessWindow;

class ChessWorker {
public:
    void run(ChessWindow* window);
};


#endif //CHESSAGENT_CHESS_WORKER_H
