#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include "detectionthread.h"
#include "application_utils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    screenTimer = new QTimer(this);
    screenTimer->singleShot(3000, this, SLOT(screen_timer_timeout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actioncolor_triggered()
{
    qDebug() << "on_actioncolor_triggered";
}

void MainWindow::on_actionrun_triggered(bool checked)
{
    qDebug() << "on_actionrun_triggered" << checked;
    detectionThread.start();
}

void MainWindow::screen_timer_timeout()
{
    detectionThread.start();
}
