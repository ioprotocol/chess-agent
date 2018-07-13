//
// Created by xushy on 18-7-9.
//

#include "chesswindow.h"
#include <iostream>

ChessWindow::ChessWindow(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade)
        : Gtk::ApplicationWindow(cobject),
          dispatcher_(),
          p_worker_thread_(nullptr),
          worker_running_falg_(FALSE),
          chess_worker_(),
          worker_mutex_(),
          screen_shot_()
{
    refGlade->get_widget("option_run", p_option_run_);

    p_option_run_->signal_toggled().connect(sigc::mem_fun(this, &ChessWindow::on_option_run_toggled));

    dispatcher_.connect(sigc::mem_fun(*this, &ChessWindow::on_worker_thread_finish));
}

void ChessWindow::notify() {
    dispatcher_.emit();
}

void ChessWindow::on_option_run_toggled() {
    if (!p_option_run_->get_active()) {
        set_worker_running_flag(FALSE);

    } else {
        p_worker_thread_ = new std::thread(
                [this] {
                    chess_worker_.run(this);
                });
    }
}

void ChessWindow::on_worker_thread_finish() {
    std::cout << "on_worker_thread_finish" << std::endl;
}

ScreenShot &ChessWindow::get_screen_shot() {
    return screen_shot_;
}

void ChessWindow::set_worker_running_flag(gboolean flag) {
    worker_mutex_.lock();
    worker_running_falg_ = flag;
    worker_mutex_.unlock();
}

gboolean ChessWindow::get_worker_running_flag() {
    gboolean flag;
    worker_mutex_.lock();
    flag = worker_running_falg_;
    worker_mutex_.unlock();
    return flag;
}

