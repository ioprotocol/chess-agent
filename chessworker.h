//
// Created by xsy on 18-7-10.
//

#ifndef CHESSAGENT_CHESS_WORKER_H
#define CHESSAGENT_CHESS_WORKER_H

#include <gtkmm.h>
#include <thread>

class ChessWindow;

class ChessWorker {
private:
    void print_chess_position(std::map<gint, gint> &posotion_map);
public:
    void run(ChessWindow* window);
};


#endif //CHESSAGENT_CHESS_WORKER_H
