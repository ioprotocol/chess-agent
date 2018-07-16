//
// Created by xushy on 18-7-16.
//

#include "colorfilterdialog.h"

ColorFilterDialog::ColorFilterDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
        : Dialog(cobject), scale1_(0), scale2_(0), scale3_(0)
{
    refGlade->get_widget("gtk_image1", p_image1);
    refGlade->get_widget("gtk_image2", p_image2);
    refGlade->get_widget("gtk_image3", p_image3);
    refGlade->get_widget("gtk_image4", p_image4);
    refGlade->get_widget("gkt_scale_h", p_scale_button1);
    refGlade->get_widget("gkt_scale_s", p_scale_button2);
    refGlade->get_widget("gkt_scale_v", p_scale_button3);
    p_scale_button1->set_label("Scalar_H");
    p_scale_button2->set_label("Scalar_S");
    p_scale_button3->set_label("Scalar_V");
    refGlade->get_widget("label_scalar", p_label);

    p_scale_button1->signal_value_changed().connect(sigc::mem_fun(this, &ColorFilterDialog::on_scale_button1_value_change));
    p_scale_button2->signal_value_changed().connect(sigc::mem_fun(this, &ColorFilterDialog::on_scale_button2_value_change));
    p_scale_button3->signal_value_changed().connect(sigc::mem_fun(this, &ColorFilterDialog::on_scale_button3_value_change));
}

void ColorFilterDialog::on_scale_button1_value_change(double value) {
    scale1_ = value * 255;
    update_scalar_label();
}

void ColorFilterDialog::on_scale_button2_value_change(double value) {
    scale2_ = value * 255;
    update_scalar_label();
}

void ColorFilterDialog::on_scale_button3_value_change(double value) {
    scale3_ = value * 255;
    update_scalar_label();
}

void ColorFilterDialog::update_scalar_label() {
    std::string msg;
    msg += " H:" + std::to_string(scale1_);
    msg += " S:" + std::to_string(scale2_);
    msg += " V:" + std::to_string(scale3_);
    p_label->set_label(msg);
}
