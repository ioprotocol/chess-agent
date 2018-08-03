#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QTimer>
#include <QThreadPool>
#include <QFutureWatcher>
#include <opencv2/opencv.hpp>
#include <Windows.h>

#include "chessaction.h"
#include "ChessSampling.h"

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

    void grab_window_timer_timeout();

    void worker_run_finish();
private:
    bool grab_window(cv::Mat &mat);

    QThreadPool workerThreadPool;
    QFutureWatcher<void> workerFutureWatcher;

    std::map<unsigned int, int> chess_position_map;
private:
    Ui::MainWindow *ui;
    QTimer *screenTimer;
    QTimer *grabWindowTimer;

    QString bindTitleKeyWord;
    HWND bindHWND;
    QRect bindWindowRect;

    ChessSampling chessSampling;
    ChessAction chessAction;
};

#endif // MAINWINDOW_H
