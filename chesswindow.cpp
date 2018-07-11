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
    refGlade->get_widget("option_test", p_option_test_);

    p_option_train_->signal_activate().connect(sigc::mem_fun(this, &ChessWindow::on_option_train_active));
    p_option_gen_resource_->signal_activate().connect(sigc::mem_fun(this, &ChessWindow::on_option_gen_resource_active));
    p_option_test_->signal_activate().connect(sigc::mem_fun(this, &ChessWindow::on_option_test_active));

    dispatcher_.connect(sigc::mem_fun(*this, &ChessWindow::on_worker_thread_finish));
}

void ChessWindow::on_option_train_active() {
    std::cout << "on_option_train_active" << std::endl;
    on_worker_thread_start();
}

void ChessWindow::on_option_gen_resource_active() {
    screen_shot_.generate_train_neg_img(screen_shot_.screen_shot_test());
    screen_shot_.generate_train_pos_img(screen_shot_.screen_shot_test());
    std::cout << "generate finish!" << std::endl;
}

void ChessWindow::on_worker_thread_start() {
    p_worker_thread_ = new std::thread(
            [this] {
                chess_worker_.run(this);
            });
}

void ChessWindow::on_worker_thread_finish() {
    std::cout << "on_worker_thread_finish" << std::endl;
}

void ChessWindow::notify() {
    dispatcher_.emit();
}

void ChessWindow::on_option_test_active() {
}
