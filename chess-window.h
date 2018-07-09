//
// Created by xushy on 18-7-9.
//

#ifndef CHESSAGENT_CHESSWINDOW_H
#define CHESSAGENT_CHESSWINDOW_H

#include <gtkmm.h>

class ChessWindow : public Gtk::ApplicationWindow {
public:
    ChessWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);

private:
    Gtk::MenuItem *m_option_train;
    Gtk::MenuItem *m_option_gen_resource;

public:
    void on_option_train_active();

    void on_option_gen_resource_active();
};


#endif //CHESSAGENT_CHESSWINDOW_H
