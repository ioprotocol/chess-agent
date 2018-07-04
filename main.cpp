#include <gtk/gtk.h>
#include <iostream>

#include <unistd.h>
#include "screen-shot.h"

GTask* task;
GCancellable* cancellable;

void task_call_back(GObject *source_object,
                  GAsyncResult *res,
                  gpointer user_data) {
    std::cout << "task call back" << std::endl;
}

void task_data_destroy(gpointer data) {
    std::cout << "task_data_destroy" << std::endl;
}

void thread_run(GTask           *task,
                gpointer         source_object,
                gpointer         task_data,
                GCancellable    *cancellable) {
    ScreenShot* screenShot = (ScreenShot*)task_data;
    while(true) {
        screenShot->run();
        usleep(3000*1000);
    }
    g_task_return_boolean(task, TRUE);
}

void button_clicked(GtkWidget *button, gpointer data)
{
    cancellable = g_cancellable_new();
    task = g_task_new(button, cancellable, task_call_back, data);

    g_task_set_task_data(task, new ScreenShot(), task_data_destroy);

    g_task_run_in_thread(task, thread_run);
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