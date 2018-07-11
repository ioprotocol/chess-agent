#include <gtkmm.h>
#include <glibmm.h>

#include "application_utils.h"
#include "chesswindow.h"

int main(int argc, char *argv[]) {
    ChessWindow* window = nullptr;

    auto app = Gtk::Application::create(argc, argv, "com.github.chess.agent");
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(Hub::get_resource_glade_file());

    builder->get_widget_derived("appWindow",  window);
    return app->run(*window);
}