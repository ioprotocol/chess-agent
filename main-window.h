//
// Created by xushy on 18-7-7.
//

#ifndef CHESSAGENT_MAINWIDGET_H
#define CHESSAGENT_MAINWIDGET_H

#include <gtk/gtk.h>
#include <iostream>

class MainWindow {
private:
    GtkWidget *mainWidget;
    GtkWidget *verticalBox;

    GtkWidget *menuBar;
    GtkWidget *menuBarOption;

    GtkWidget *popMenuOption;

    GtkWidget *optionItemTrain;
    GtkWidget *optionItemGenRes;
    GtkWidget *optionItemTest;

public:
    MainWindow();

    std::string getApplicationName();
};


#endif //CHESSAGENT_MAINWIDGET_H
