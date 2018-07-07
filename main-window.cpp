//
// Created by xushy on 18-7-7.
//

#include "main-window.h"

#include <iostream>

void train(gpointer data);

void gen_train_resources(void);

void knn_test(void);

MainWindow::MainWindow() {
    mainWidget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /* give it the title */
    gtk_window_set_title(GTK_WINDOW(mainWidget), "Chess Agent");

    /* set default window size */
    gtk_window_set_default_size(GTK_WINDOW(mainWidget), 558, 620);
    /* connect the destroy signal of the window to gtk_main_quit
     * when the window is about to be destroyed we get a notification and
     * stop the main GTK+ loop
     */
    g_signal_connect(mainWidget, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    verticalBox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(mainWidget), verticalBox);

    menuBar = gtk_menu_bar_new();
    gtk_box_pack_start(GTK_BOX(verticalBox), menuBar, FALSE, FALSE, 0);

    menuBarOption = gtk_menu_item_new_with_label("选项");
    gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), menuBarOption);

    popMenuOption = gtk_menu_new();

    optionItemTrain = gtk_menu_item_new_with_label("训练");
    gtk_menu_shell_append(GTK_MENU_SHELL(popMenuOption), optionItemTrain);
    g_signal_connect_swapped(G_OBJECT(optionItemTrain),"activate",G_CALLBACK(train), this);

    optionItemGenRes = gtk_menu_item_new_with_label("生成样本");
    gtk_menu_shell_append(GTK_MENU_SHELL(popMenuOption), optionItemGenRes);
    optionItemTest = gtk_menu_item_new_with_label("测试");
    gtk_menu_shell_append(GTK_MENU_SHELL(popMenuOption), optionItemTest);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuBarOption), popMenuOption);

    g_signal_connect(mainWidget, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(mainWidget);
}

std::string MainWindow::getApplicationName() {
    return std::string("象棋先锋");
}

void train(gpointer data) {
    MainWindow* window = (MainWindow*) data;
    std::cout << window->getApplicationName() << std::endl;
}

void gen_train_resources(void) {

}

void knn_test(void) {

}
