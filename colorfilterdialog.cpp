//
// Created by xushy on 18-7-16.
//

#include "colorfilterdialog.h"
#include "application_utils.h"

ColorFilterDialog::ColorFilterDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
        : Dialog(cobject), scale1_(0), scale2_(0), scale3_(0),scale4_(0), scale5_(0), scale6_(0)
{
    refGlade->get_widget("gtk_image1", p_image1);
    refGlade->get_widget("gtk_image2", p_image2);
    refGlade->get_widget("gtk_image3", p_image3);
    refGlade->get_widget("gtk_image4", p_image4);
    refGlade->get_widget("gkt_scale_l_h", p_scale_button1);
    refGlade->get_widget("gkt_scale_l_s", p_scale_button2);
    refGlade->get_widget("gkt_scale_l_v", p_scale_button3);
    refGlade->get_widget("gkt_scale_h_h", p_scale_button4);
    refGlade->get_widget("gkt_scale_h_s", p_scale_button5);
    refGlade->get_widget("gkt_scale_h_v", p_scale_button6);
    p_scale_button1->set_label("ScalarL_H");
    p_scale_button2->set_label("ScalarL_S");
    p_scale_button3->set_label("ScalarL_V");
    p_scale_button4->set_label("ScalarH_H");
    p_scale_button5->set_label("ScalarH_S");
    p_scale_button6->set_label("ScalarH_V");
    refGlade->get_widget("label_scalar", p_label);

    p_scale_button1->signal_value_changed().connect(sigc::mem_fun(this, &ColorFilterDialog::on_scale_button1_value_change));
    p_scale_button2->signal_value_changed().connect(sigc::mem_fun(this, &ColorFilterDialog::on_scale_button2_value_change));
    p_scale_button3->signal_value_changed().connect(sigc::mem_fun(this, &ColorFilterDialog::on_scale_button3_value_change));
    p_scale_button4->signal_value_changed().connect(sigc::mem_fun(this, &ColorFilterDialog::on_scale_button4_value_change));
    p_scale_button5->signal_value_changed().connect(sigc::mem_fun(this, &ColorFilterDialog::on_scale_button5_value_change));
    p_scale_button6->signal_value_changed().connect(sigc::mem_fun(this, &ColorFilterDialog::on_scale_button6_value_change));

    img1 = cv::imread(Glib::build_filename(Hub::get_resources_path(), "0.jpg"));
    img2 = cv::imread(Glib::build_filename(Hub::get_resources_path(), "8.jpg"));
    img3 = cv::imread(Glib::build_filename(Hub::get_resources_path(), "90000.jpg"));
    img4 = cv::imread(Glib::build_filename(Hub::get_resources_path(), "90008.jpg"));
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

void ColorFilterDialog::on_scale_button4_value_change(double value) {
    scale4_ = value * 255;
    update_scalar_label();
}

void ColorFilterDialog::on_scale_button5_value_change(double value) {
    scale5_ = value * 255;
    update_scalar_label();
}

void ColorFilterDialog::on_scale_button6_value_change(double value) {
    scale6_ = value * 255;
    update_scalar_label();
}

void ColorFilterDialog::update_scalar_label() {
    std::string msg;
    msg += " LH:" + std::to_string(scale1_);
    msg += " LS:" + std::to_string(scale2_);
    msg += " LV:" + std::to_string(scale3_);
    msg += " HH:" + std::to_string(scale4_);
    msg += " HS:" + std::to_string(scale5_);
    msg += " HV:" + std::to_string(scale6_);
    p_label->set_label(msg);

    cv::Mat tmp1, tmp2, tmp3, tmp4;

    threold_mat(img1, tmp1);
    threold_mat(img2, tmp2);
    threold_mat(img3, tmp3);
    threold_mat(img4, tmp4);

    cv::imwrite(Glib::build_filename(Hub::get_resources_path(), "img1.jpg"), img1);
    cv::imwrite(Glib::build_filename(Hub::get_resources_path(), "img2.jpg"), img2);
    cv::imwrite(Glib::build_filename(Hub::get_resources_path(), "img3.jpg"), img3);
    cv::imwrite(Glib::build_filename(Hub::get_resources_path(), "img4.jpg"), img4);

    p_image1->clear();
    p_image2->clear();
    p_image3->clear();
    p_image4->clear();

    p_image1->set(Gdk::Pixbuf::create_from_file(Glib::build_filename(Hub::get_resources_path(), "img1.jpg")));
    p_image2->set(Gdk::Pixbuf::create_from_file(Glib::build_filename(Hub::get_resources_path(), "img2.jpg")));
    p_image3->set(Gdk::Pixbuf::create_from_file(Glib::build_filename(Hub::get_resources_path(), "img3.jpg")));
    p_image4->set(Gdk::Pixbuf::create_from_file(Glib::build_filename(Hub::get_resources_path(), "img4.jpg")));
    p_image1->show();
    p_image2->show();
    p_image3->show();
    p_image4->show();
}

void ColorFilterDialog::threold_mat(cv::Mat &in, cv::Mat& out) {
    cv::inRange(in, cv::Scalar(scale1_, scale2_, scale3_), cv::Scalar(scale4_, scale5_, scale6_), out);
    cv::threshold(out, out, 0, 255.0, CV_THRESH_BINARY_INV);
}
