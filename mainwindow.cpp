#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
}
