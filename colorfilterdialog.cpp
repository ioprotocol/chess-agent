#include "colorfilterdialog.h"
#include "ui_colorfilterdialog.h"

#include <QDebug>

ColorFilterDialog::ColorFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorFilterDialog)
{
    ui->setupUi(this);
    mat1 = cv::imread("D:\\1.jpg");
    mat2 = cv::imread("D:\\2.jpg");
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
    update_img();
}
// HG
void ColorFilterDialog::on_horizontalSlider_2_sliderReleased()
{
    update_img();
}
// HB
void ColorFilterDialog::on_horizontalSlider_3_sliderReleased()
{
    update_img();
}
// LR
void ColorFilterDialog::on_horizontalSlider_5_sliderReleased()
{
    update_img();
}
// LG
void ColorFilterDialog::on_horizontalSlider_6_sliderReleased()
{
    update_img();
}
// LB
void ColorFilterDialog::on_horizontalSlider_4_sliderReleased()
{
    update_img();
}

void ColorFilterDialog::update_img() {
    int hr = ui->horizontalSlider->value();
    int hg = ui->horizontalSlider_2->value();
    int hb = ui->horizontalSlider_3->value();
    int lr = ui->horizontalSlider_5->value();
    int lg = ui->horizontalSlider_6->value();
    int lb = ui->horizontalSlider_4->value();

    qDebug() << "hr:" << hr << " hg:" << hg << " hb:" << hb << " lr:" << lr << " lg:" << lg << " lb:" << lb;

    cv::Scalar high(hr, hg, hb);
    cv::Scalar low(lr, lg, lb);

    cv::Mat thr1, thr2;

    cv::inRange(mat1, low, high, thr1);
    cv::inRange(mat2, low, high, thr2);

    cv::medianBlur(thr1, thr1, 1);
    cv::medianBlur(thr2, thr2, 1);

    cv::threshold(thr1, thr1, 0, 255.0, CV_THRESH_BINARY_INV);
    thr1.convertTo(thr1, CV_32F);

    cv::threshold(thr2, thr2, 0, 255.0, CV_THRESH_BINARY_INV);
    thr2.convertTo(thr2, CV_32F);

    std::vector<uchar> data1;
    std::vector<uchar> data2;

    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(100);

    cv::imencode(".jpeg", thr1, data1, params);
    cv::imencode(".jpeg", thr2, data2, params);

    QPixmap pixmap1, pixmap2;
    pixmap1.loadFromData(data1.data(), data1.size(), "jpeg");
    pixmap2.loadFromData(data2.data(), data2.size(), "jpeg");

    ui->img1->setPixmap(pixmap1);
    ui->img2->setPixmap(pixmap2);
}
