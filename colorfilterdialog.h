#ifndef COLORFILTERDIALOG_H
#define COLORFILTERDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <opencv2/opencv.hpp>

namespace Ui {
class ColorFilterDialog;
}

class ColorFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColorFilterDialog(QWidget *parent = 0);
    ~ColorFilterDialog();

private slots:
    void on_pushButton_clicked();

    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_2_sliderReleased();

    void on_horizontalSlider_3_sliderReleased();

    void on_horizontalSlider_5_sliderReleased();

    void on_horizontalSlider_6_sliderReleased();

    void on_horizontalSlider_4_sliderReleased();

private:
    void update_img();
private:
    Ui::ColorFilterDialog *ui;
    cv::Mat mat1;
    cv::Mat mat2;
};

#endif // COLORFILTERDIALOG_H
