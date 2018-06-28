#include <gtk/gtk.h>

#include "screenshot-utils.h"
#include "screenshot-config.h"

ScreenshotConfig* screenshot_config;

void init_screen_config()
{
    screenshot_config = new ScreenshotConfig();
    screenshot_config->copy_to_clipboard = false;
    screenshot_config->take_window_shot = true;
    screenshot_config->take_area_shot = false;
    screenshot_config->include_border = false;
    screenshot_config->include_icc_profile = false;
    screenshot_config->include_pointer = false;
    screenshot_config->delay = 0;
}

int main(int argc,char *argv[]) {

    init_screen_config();

    GdkPixbuf* gdkPixbuf = screenshot_get_pixbuf(NULL);

    GtkWidget *window;
    GtkWidget *label;

    gtk_init(&argc, &argv);

    /* create the main, top level, window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    /* give it the title */
    gtk_window_set_title(GTK_WINDOW(window), "Hello World");

    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    /* connect the destroy signal of the window to gtk_main_quit
     * when the window is about to be destroyed we get a notification and
     * stop the main GTK+ loop
     */
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* create the "Hello, World" label */
    label = gtk_label_new("Hello, World");

    /* and insert it into the main window */
    gtk_container_add(GTK_CONTAINER(window), label);

    /* make sure that everything, window and label, are visible */
    gtk_widget_show_all(window);

    /* start the main loop, and let it rest until the application is closed */
    gtk_main();
    return 0;
}