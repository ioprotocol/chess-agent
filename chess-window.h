//
// Created by xushy on 18-7-9.
//

#ifndef CHESSAGENT_CHESSWINDOW_H
#define CHESSAGENT_CHESSWINDOW_H

#include <gtkmm.h>
#include "chess-worker.h"
#include "screen-shot.h"

class ChessWindow : public Gtk::ApplicationWindow {
public:
    ChessWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);

    void notify();
private:
    Gtk::MenuItem *m_option_train;
    Gtk::MenuItem *m_option_gen_resource;
private:
    ScreenShot m_screen_shot;
    Glib::Dispatcher m_dispatcher;
    std::thread* m_worker_thread;
    ChessWorker m_chess_worker;
private:
    void on_option_train_active();

    void on_option_gen_resource_active();

    void on_worker_thread_start();

    void on_worker_thread_finish();
};


#endif //CHESSAGENT_CHESSWINDOW_H
