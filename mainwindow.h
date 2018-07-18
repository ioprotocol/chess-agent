#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "detectionthread.h"

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
    Ui::MainWindow *ui;
    DetectionThread detectionThread;
    QTimer *screenTimer;
};

#endif // MAINWINDOW_H
