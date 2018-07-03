#include <gtk/gtk.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include "screenshot-utils.h"
#include "screenshot-config.h"

#include "chess.h"

void* screen_shot(void* arg) {
    for (int i = 0; i < 5; i++) {
        GdkPixbuf *gdkPixbuf = screenshot_fallback_get_pixbuf(NULL);
        save_pixbuf_to_jpeg_file(gdkPixbuf);
        g_object_unref(gdkPixbuf);
        std::cout << "sceen:" << i << std::endl;
        sleep(5);
    }
}

void button_clicked(GtkWidget *button, gpointer data) {
    /*
    pthread_t id;
    int ret = pthread_create(&id, NULL, screen_shot, NULL);
    if(ret) {
        std::cout << "Create pthread error!" << std::endl;
        return;
    }
    pthread_join(id, NULL);
    */
    GdkPixbuf *pixbuf = NULL;
    Chess* chess = new Chess();
    pixbuf = chess->generateMat();
    delete chess;
    gdk_pixbuf_save(pixbuf, "/home/xushy/main1.jpg", "jpeg", NULL, "quality", "100", NULL);
    g_object_unref(pixbuf);

}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;

    gtk_init(&argc, &argv);
    init_screen_config();

    /* create the main, top level, window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    /* give it the title */
    gtk_window_set_title(GTK_WINDOW(window), "Chinese Chess");

    /* set default window size */
    gtk_window_set_default_size(GTK_WINDOW(window), 558, 620);

    /* connect the destroy signal of the window to gtk_main_quit
     * when the window is about to be destroyed we get a notification and
     * stop the main GTK+ loop
     */
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


    button = gtk_button_new_with_label("button");
    g_signal_connect(GTK_WIDGET(button), "clicked", G_CALLBACK(button_clicked), NULL);

    /* and insert it into the main window */
    gtk_container_add(GTK_CONTAINER(window), button);

    /* make sure that everything, window and label, are visible */
    gtk_widget_show_all(window);


    /* start the main loop, and let it rest until the application is closed */
    gtk_main();
    return 0;
}