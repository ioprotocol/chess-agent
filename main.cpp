#include <gtk/gtk.h>
#include <iostream>

#include "screen-shot-task.h"

gboolean flag = FALSE;

void button_clicked(GtkWidget *button, gpointer data)
{
    if(!flag) {
        screen_shot_task_create(button, 10000);
        flag = TRUE;
    } else {
        screen_shot_task_destory();
        flag = FALSE;
    }

    std::cout << "thread_start" << std::endl;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;

    if(!g_thread_supported()) {
        g_thread_init(NULL);
    }
    gdk_threads_init();

    gtk_init(&argc, &argv);

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
    gtk_main ();
    return 0;
}