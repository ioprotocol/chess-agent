//
// Created by xushy on 18-7-9.
//

#include "chesswindow.h"
#include <iostream>

ChessWindow::ChessWindow(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade)
        : Gtk::ApplicationWindow(cobject),
          dispatcher_(),
          p_worker_thread_(nullptr),
          chess_worker_(),
          screen_shot_()
{
    refGlade->get_widget("option_train", p_option_train_);
    refGlade->get_widget("option_gen_resource", p_option_gen_resource_);
    refGlade->get_widget("option_run", p_option_run_);
    refGlade->get_widget("option_knn_stat", p_option_knn_stat_);

    p_option_train_->signal_activate().connect(sigc::mem_fun(this, &ChessWindow::on_option_train_active));
    p_option_gen_resource_->signal_activate().connect(sigc::mem_fun(this, &ChessWindow::on_option_gen_resource_active));
    p_option_run_->signal_activate().connect(sigc::mem_fun(this, &ChessWindow::on_option_run_active));
    p_option_knn_stat_->signal_activate().connect(sigc::mem_fun(this, &ChessWindow::on_option_knn_stat_active));

    dispatcher_.connect(sigc::mem_fun(*this, &ChessWindow::on_option_run_finish));
}

void ChessWindow::notify() {
    dispatcher_.emit();
}

void ChessWindow::on_option_train_active() {
    screen_shot_.knn_train();
    std::cout << "knn train finish !" << std::endl;
}

void ChessWindow::on_option_gen_resource_active() {
    screen_shot_.generate_base_train_data();
    std::cout << "generate finish!" << std::endl;
}

void ChessWindow::on_option_run_active() {
    p_worker_thread_ = new std::thread(
            [this] {
                chess_worker_.run(this);
            });
}

void ChessWindow::on_option_run_finish() {
    std::cout << "on_option_run_finish" << std::endl;
}

ScreenShot ChessWindow::get_screen_shot() {
    return ScreenShot();
}

void ChessWindow::on_option_knn_stat_active() {
    screen_shot_.knn_predit_stat();
}
