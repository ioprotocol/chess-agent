#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QTimer>
#include <QThreadPool>
#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actioncolor_triggered();

    void on_actionrun_triggered(bool checked);

    void screen_timer_timeout();
private:
    cv::Mat grab_window();

    QThreadPool wokerThreadPool;

    QMap<quint32, qint32> detect_chess_position(cv::Mat &mat);
private:
    Ui::MainWindow *ui;
    QTimer *screenTimer;
};

#endif // MAINWINDOW_H
