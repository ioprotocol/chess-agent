#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void on_actionRun_toggled(bool arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
