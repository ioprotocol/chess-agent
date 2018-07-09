//
// Created by xushy on 18-7-9.
//

#include "chess-window.h"

#include <iostream>

ChessWindow::ChessWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
    : Gtk::ApplicationWindow(cobject)
{
    refGlade->get_widget("m_option_train", m_option_train);
    refGlade->get_widget("m_option_gen_resource", m_option_gen_resource);

    m_option_train->signal_activate().connect(sigc::mem_fun(this, &ChessWindow::on_option_train_active));
    m_option_gen_resource->signal_activate().connect(sigc::mem_fun(this, &ChessWindow::on_option_gen_resource_active));
}

void ChessWindow::on_option_train_active() {
    std::cout << "on_option_train_active" << std::endl;
}

void ChessWindow::on_option_gen_resource_active() {
    std::cout << "on_option_gen_resource_active" << std::endl;
}
