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
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    screenTimer = new QTimer(this);
    connect(screenTimer, SIGNAL(timeout()), this, SLOT(screen_timer_timeout()));

    qRegisterMetaType<QMap<quint32,qint32>>("QMap<quint32,qint32>");
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
        screenTimer->setInterval(1000);
        screenTimer->start();
    } else {
        screenTimer->stop();
    }
}

void MainWindow::screen_timer_timeout()
{
    qDebug() << "screen_timer_timeout";
    cv::Mat mat = grab_window();
    QFuture<QMap<quint32, qint32>> future = QtConcurrent::run(&wokerThreadPool, &MainWindow::detect_chess_position, mat);
}

QMap<quint32, qint32> MainWindow::detect_chess_position(cv::Mat &mat)
{
    QMap<quint32, qint32> map;
    return map;
}


cv::Mat MainWindow::grab_window()
{
    QDesktopWidget *desk = QApplication::desktop();
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap p = screen->grabWindow(desk->winId());

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::ReadWrite);

    p.save(&buffer, "jpeg", 100);

    uchar* data = (uchar *) byteArray.data();
    std::vector<uchar> vec_data(&data[0], &data[0] + byteArray.size());
    cv::Mat mat = cv::imdecode(vec_data, cv::IMREAD_COLOR);
    return mat;
}

