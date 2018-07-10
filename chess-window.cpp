//
// Created by xushy on 18-7-9.
//

#include "chess-window.h"

#include <iostream>

ChessWindow::ChessWindow(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade)
        : Gtk::ApplicationWindow(cobject),
          m_dispatcher(),
          m_worker_thread(nullptr),
          m_chess_worker(),
          m_screen_shot()
{
    refGlade->get_widget("m_option_train", m_option_train);
    refGlade->get_widget("m_option_gen_resource", m_option_gen_resource);

    m_option_train->signal_activate().connect(sigc::mem_fun(this, &ChessWindow::on_option_train_active));
    m_option_gen_resource->signal_activate().connect(sigc::mem_fun(this, &ChessWindow::on_option_gen_resource_active));


    m_dispatcher.connect(sigc::mem_fun(*this, &ChessWindow::on_worker_thread_finish));
}

void ChessWindow::on_option_train_active() {
    std::cout << "on_option_train_active" << std::endl;
    on_worker_thread_start();
}

void ChessWindow::on_option_gen_resource_active() {
    cv::Mat mat = m_screen_shot.screen_shot();
    std::cout << "on_option_gen_resource_active" << std::endl;
}

void ChessWindow::on_worker_thread_start() {
    m_worker_thread = new std::thread(
            [this] {
                m_chess_worker.run(this);
            });
}

void ChessWindow::on_worker_thread_finish() {
    std::cout << "on_worker_thread_finish" << std::endl;
}

void ChessWindow::notify() {
    m_dispatcher.emit();
}
