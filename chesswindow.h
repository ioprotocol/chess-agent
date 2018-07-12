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
    Gtk::MenuItem *p_option_train_;
    Gtk::MenuItem *p_option_gen_resource_;
    Gtk::MenuItem *p_option_run_;
    Gtk::MenuItem *p_option_knn_stat_;
private:
    ScreenShot  screen_shot_;
    Glib::Dispatcher dispatcher_;
    std::thread* p_worker_thread_;
    ChessWorker chess_worker_;
private:
    // train the knn model
    void on_option_train_active();

    // generate train base data
    void on_option_gen_resource_active();

    void on_option_knn_stat_active();

    void on_option_run_active();

    void on_option_run_finish();

public:

    ScreenShot get_screen_shot();
};


#endif //CHESSAGENT_CHESSWINDOW_H
