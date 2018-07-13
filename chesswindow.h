//
// Created by xushy on 18-7-9.
//

#ifndef CHESSAGENT_CHESSWINDOW_H
#define CHESSAGENT_CHESSWINDOW_H

#include <gtkmm.h>
#include "chessworker.h"
#include "screenshot.h"

class ChessWindow : public Gtk::ApplicationWindow {
public:
    ChessWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);

    void notify();
private:
    Gtk::CheckMenuItem *p_option_run_;
private:
    ScreenShot  screen_shot_;
    Glib::Dispatcher dispatcher_;
    std::thread* p_worker_thread_;
    ChessWorker chess_worker_;
    gboolean worker_running_falg_;
    Glib::Threads::Mutex worker_mutex_;
private:
    void on_option_run_toggled();

    void on_worker_thread_finish();

public:
    void set_worker_running_flag(gboolean flag);

    gboolean get_worker_running_flag();

    ScreenShot get_screen_shot();
};


#endif //CHESSAGENT_CHESSWINDOW_H
