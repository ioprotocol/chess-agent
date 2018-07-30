#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThreadPool>
#include <QFuture>
#include <QtConcurrent>
#include <QApplication>
#include <QDesktopWidget>
#include <QPixmap>
#include <QScreen>
#include <QList>
#include <QWindow>
#include <QBuffer>
#include <QDebug>

#include "colorfilterdialog.h"
#include "application_utils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    bindHWND(NULL),
    bindTitleKeyWord("4399"),
    bindWindowRect(QRect(0, 0, 0, 0))
{
    ui->setupUi(this);
	
    screenTimer = new QTimer(this);
    grabWindowTimer = new QTimer(this);
	
    connect(screenTimer, SIGNAL(timeout()), this, SLOT(screen_timer_timeout()));
    connect(grabWindowTimer, SIGNAL(timeout()), this, SLOT(grab_window_timer_timeout()));
	
    connect(&workerFutureWatcher, SIGNAL(finished()), this, SLOT(worker_run_finish()));
    this->setFixedSize(600, 700);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actioncolor_triggered()
{
    ColorFilterDialog dlg(this);
    dlg.exec();
}

void MainWindow::on_actionrun_triggered(bool checked)
{
    if(checked) {
        screenTimer->setInterval(3000);
        screenTimer->start();
        grabWindowTimer->setInterval(3000);
        grabWindowTimer->start();
    } else {
        screenTimer->stop();
        grabWindowTimer->stop();
    }
}

void MainWindow::screen_timer_timeout()
{
    cv::Mat screen;
    if (grab_window(screen)) {
        if (!workerFutureWatcher.isRunning()) {
            chess_position_map.clear();
            QFuture<int> future = QtConcurrent::run(&workerThreadPool,
                    &screenShot, &ScreenShot::detect_chess_position, &chess_position_map, screen);
            workerFutureWatcher.setFuture(future);
        } else {
            qDebug() << "task queue is full";
        }
    }
}

void MainWindow::worker_run_finish() {
    int result = workerFutureWatcher.result();

    qDebug() << "worker_run_finish, result:" << result;

    switch (result) {
        case DETECT_STUDY_SUCCESS:
            break;
        case DETECT_STUDY_FAILED:
            break;
        case DETECT_STUDY_CIRCLE_TO_LITTILE:
            break;
        case DETECT_AUTOTRAIN_CIRCLE_LITTILE:
            break;
        case DETECT_AUTOTRAIN_ERR_RATE_HIGH:
            break;
        case DETECT_AUTOTRAIN_SUCCESS:
            break;
        case DETECT_WINDOW_IS_NOT_ACTIVE:
            break;
    }

    if(result == 0) {
        QPixmap pixmap = chessAction.generate_pixture(chess_position_map);
//        QPixmap pixmap;
//        pixmap.load("D:\\test.jpg");
        ui->mainImg->setPixmap(pixmap);
    }
}

bool MainWindow::grab_window(cv::Mat &mat)
{
    if (bindWindowRect.width() < 100) {
        return false;
    }

    QDesktopWidget *desk = QApplication::desktop();
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap p = screen->grabWindow(desk->winId(), bindWindowRect.left(), bindWindowRect.top(),
            bindWindowRect.width(), bindWindowRect.height());

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::ReadWrite);

    p.save(&buffer, "jpeg", 100);


    uchar* data = (uchar *) byteArray.data();
    std::vector<uchar> vec_data(&data[0], &data[0] + byteArray.size());
    mat = cv::imdecode(vec_data, cv::IMREAD_COLOR);
    return true;
}


HWND get_active_windows(QString keyWord) {
    char ss[200] = {0};
    int h = 0, w = 0;
    RECT rect;
    HWND hd = GetDesktopWindow();

    hd = GetWindow(hd, GW_CHILD);
    while (hd != NULL)
    {
        GetWindowRect(hd, &rect);
        w = rect.right - rect.left;
        h = rect.bottom - rect.top;

        memset(ss, 0, 200);
        GetWindowText(hd, (LPSTR) &ss[0], 200);
        if(w > 300 && h > 300) {
            QString title = QString::fromLocal8Bit(ss);
            if (title.contains(keyWord)) {
                return hd;
            }
        }
        hd = GetNextWindow(hd, GW_HWNDNEXT);
    }
    return NULL;
}

void MainWindow::grab_window_timer_timeout()
{
    if(bindHWND == NULL) {
        bindHWND = get_active_windows(bindTitleKeyWord);
        if(bindHWND != NULL) {
            qDebug() << "bind " << bindTitleKeyWord << " HWND success";
        } else {
            qDebug() << bindTitleKeyWord << "'s is not start, can't find the HWND ";
        }
    }
    if(bindHWND != NULL) {
        RECT rect;
        GetWindowRect(bindHWND, &rect);
        if (rect.left < 0 && rect.top < 0) {
            qDebug() << bindTitleKeyWord << " is not visible in primary screen !";
        } else {
            bindWindowRect.setLeft(rect.left);
            bindWindowRect.setRight(rect.right);
            bindWindowRect.setTop(rect.top);
            bindWindowRect.setBottom(rect.bottom);
            qDebug() << bindTitleKeyWord << " rect is -> left:" << bindWindowRect.left()
                     << " top:" << bindWindowRect.top()
                     << " width:" << bindWindowRect.width()
                     << " height:" << bindWindowRect.height();
        }
    }
}



