//
// Created by xsy on 18-7-18.
//

#include "detectionthread.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QPixmap>
#include <QScreen>
#include <QList>
#include <QWindow>
#include <QDebug>

#include "application_utils.h"

void DetectionThread::run() {
    QDesktopWidget *desk = QApplication::desktop();
    QScreen *screen = QGuiApplication::primaryScreen();
    QWidget* activeWidget = QApplication::activeWindow();
    QWindowList list = QApplication::allWindows();

    qDebug() << "list:" << list.size();
    for(int i = 0; i < list.size(); i ++) {
        QWindow* window = list.at(i);
        qDebug() << window->title() << " w:" << window->width() << " h:" << window->height();
    }

    while (true) {
        sleep(3);
        if (activeWidget == nullptr) {
            continue;
        }
        QPixmap p = screen->grabWindow(desk->winId(), activeWidget->pos().x(), activeWidget->pos().y(), activeWidget->width(), activeWidget->height());
        QImage image = p.toImage();
        image.save(Hub::current_dir() + "/resources/screen.jpg", "jpeg");
        qDebug() << "screen grab success";
    }
}
