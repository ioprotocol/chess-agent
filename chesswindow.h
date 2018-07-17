//
// Created by xushy on 18-7-9.
//

#ifndef CHESSAGENT_CHESSWINDOW_H
#define CHESSAGENT_CHESSWINDOW_H

#include <gtkmm.h>
#include "chessworker.h"
#include "screenshot.h"

class ColorFilterDialog;

class ChessWindow : public Gtk::ApplicationWindow {
public:
    ChessWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);

    void notify();
private:
    Gtk::CheckMenuItem *p_option_run_;
    Gtk::MenuItem  *p_option_color_ranger_;
    Glib::RefPtr<Gtk::Builder> ref_glade_builder_;
private:
    ScreenShot  screen_shot_;
    Glib::Dispatcher dispatcher_;
    std::thread* p_worker_thread_;
    ChessWorker chess_worker_;
    gboolean worker_running_falg_;
    Glib::Threads::Mutex worker_mutex_;
    ColorFilterDialog *color_filter_dialog_;
private:
    void on_option_run_toggled();

    void on_option_color_ranger_active();

    void on_worker_thread_finish();

public:
    void set_worker_running_flag(gboolean flag);

    gboolean get_worker_running_flag();

    ScreenShot &get_screen_shot();
};


#endif //CHESSAGENT_CHESSWINDOW_H
