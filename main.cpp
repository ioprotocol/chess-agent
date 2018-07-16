#include <gtkmm.h>
#include <glibmm.h>

#include "application_utils.h"
#include "chesswindow.h"

int main(int argc, char *argv[]) {
    ChessWindow* window = nullptr;

    if(!g_thread_supported()) {
        g_thread_init(NULL);
    }
    gdk_threads_init();

    auto app = Gtk::Application::create(argc, argv, "com.github.chess.agent");
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(Hub::get_resource_glade_file());

    builder->get_widget_derived("app_window",  window);
    return app->run(*window);
}