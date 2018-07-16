//
// Created by xushy on 18-7-16.
//

#ifndef CHESSAGENT_COLORFILTERDIALOG_H
#define CHESSAGENT_COLORFILTERDIALOG_H

#include <gtkmm.h>
#include <opencv2/opencv.hpp>

class ColorFilterDialog : public Gtk::Dialog {

private:
    Gtk::Image* p_image1;
    Gtk::Image* p_image2;
    Gtk::Image* p_image3;
    Gtk::Image* p_image4;
    Gtk::ScaleButton* p_scale_button1;
    Gtk::ScaleButton* p_scale_button2;
    Gtk::ScaleButton* p_scale_button3;
    Gtk::ScaleButton* p_scale_button4;
    Gtk::ScaleButton* p_scale_button5;
    Gtk::ScaleButton* p_scale_button6;
    Gtk::Label* p_label;

    gdouble scale1_;
    gdouble scale2_;
    gdouble scale3_;

    gdouble scale4_;
    gdouble scale5_;
    gdouble scale6_;

    cv::Mat img1;
    cv::Mat img2;
    cv::Mat img3;
    cv::Mat img4;

    Glib::RefPtr<Gdk::Pixbuf> pixbuf1;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf2;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf3;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf4;
private:
    void on_scale_button1_value_change(double value);
    void on_scale_button2_value_change(double value);
    void on_scale_button3_value_change(double value);
    void on_scale_button4_value_change(double value);
    void on_scale_button5_value_change(double value);
    void on_scale_button6_value_change(double value);
    void update_scalar_label();
    void threold_mat(cv::Mat &in, cv::Mat &out);
public:
    ColorFilterDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
};


#endif //CHESSAGENT_COLORFILTERDIALOG_H
