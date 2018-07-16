//
// Created by xushy on 18-7-9.
//

#include "chesswindow.h"
#include <iostream>
#include "colorfilterdialog.h"

ChessWindow::ChessWindow(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade)
        : Gtk::ApplicationWindow(cobject),
          dispatcher_(),
          ref_glade(refGlade),
          p_worker_thread_(nullptr),
          worker_running_falg_(FALSE),
          chess_worker_(),
          worker_mutex_(),
          screen_shot_()
{
    refGlade->get_widget("option_run", p_option_run_);
    refGlade->get_widget("option_color_ranger", p_option_color_ranger_);

    p_option_run_->signal_toggled().connect(sigc::mem_fun(this, &ChessWindow::on_option_run_toggled));
    p_option_color_ranger_->signal_activate().connect(sigc::mem_fun(this, &ChessWindow::on_option_color_ranger_active));

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

void ChessWindow::on_option_color_ranger_active() {

    ref_glade->get_widget_derived("app_color_filter_dialog",  color_filter_dialog_);
    color_filter_dialog_->set_transient_for(*this);
    color_filter_dialog_->show_all();
}

