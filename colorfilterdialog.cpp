#include "colorfilterdialog.h"
#include "ui_colorfilterdialog.h"

ColorFilterDialog::ColorFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorFilterDialog)
{
    ui->setupUi(this);
}

ColorFilterDialog::~ColorFilterDialog()
{
    delete ui;
}

void ColorFilterDialog::on_pushButton_clicked()
{
    this->close();
}

// HR
void ColorFilterDialog::on_horizontalSlider_sliderReleased()
{

}
// HG
void ColorFilterDialog::on_horizontalSlider_2_sliderReleased()
{

}
// HB
void ColorFilterDialog::on_horizontalSlider_3_sliderReleased()
{

}
// LR
void ColorFilterDialog::on_horizontalSlider_5_sliderReleased()
{

}
// LG
void ColorFilterDialog::on_horizontalSlider_6_sliderReleased()
{

}
// LB
void ColorFilterDialog::on_horizontalSlider_4_sliderReleased()
{

}
