#include <gtk/gtk.h>
#include "main-window.h"

int main(int argc, char *argv[]) {
    MainWindow* mainWindow;

    if (!g_thread_supported()) {
        g_thread_init(NULL);
    }
    gdk_threads_init();

    gtk_init(&argc, &argv);

    mainWindow = new MainWindow();

    gtk_main();
    return 0;
}