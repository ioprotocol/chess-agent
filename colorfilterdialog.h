//
// Created by xushy on 18-7-16.
//

#ifndef CHESSAGENT_COLORFILTERDIALOG_H
#define CHESSAGENT_COLORFILTERDIALOG_H

#include <gtkmm.h>

class ColorFilterDialog : public Gtk::Dialog {

private:
    Gtk::Image* p_image1;
    Gtk::Image* p_image2;
    Gtk::Image* p_image3;
    Gtk::Image* p_image4;
    Gtk::ScaleButton* p_scale_button1;
    Gtk::ScaleButton* p_scale_button2;
    Gtk::ScaleButton* p_scale_button3;
    Gtk::Label* p_label;

    gdouble scale1_;
    gdouble scale2_;
    gdouble scale3_;
private:
    void on_scale_button1_value_change(double value);
    void on_scale_button2_value_change(double value);
    void on_scale_button3_value_change(double value);
    void update_scalar_label();
public:
    ColorFilterDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
};


#endif //CHESSAGENT_COLORFILTERDIALOG_H
