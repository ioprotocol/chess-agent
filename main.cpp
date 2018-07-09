#include <gtkmm.h>
#include <glibmm.h>

#include "application-utils.h"
#include "chess-window.h"

int main(int argc, char *argv[]) {
    ChessWindow* window = nullptr;
    gchar* glade_file = get_main_glade_file();

    auto app = Gtk::Application::create(argc, argv, "com.github.chess.agent");
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(glade_file);

    builder->get_widget_derived("appWindow",  window);

    g_free(glade_file);
    return app->run(*window);
}