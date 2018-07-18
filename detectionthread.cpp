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

    while (true) {
        QDesktopWidget *desk = QApplication::desktop();
        QScreen *screen = QGuiApplication::primaryScreen();
//        QWidget* activeWidget = QApplication::activeWindow();

        QPixmap p = screen->grabWindow(desk->winId(), 0, 0);
        QImage image = p.toImage();
        image.save(Hub::current_dir() + "/resources/screen.jpg", "jpeg", 100);
        qDebug() << "screen grab success";
        sleep(3);
    }

}
