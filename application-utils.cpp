//
// Created by xushy on 18-7-2.
//
#include "application-utils.h"

#include <glibmm.h>

std::string get_application_path() {
    std::string base_path = Glib::getenv(PWD);

    if(base_path.empty()) {
        base_path = Glib::get_current_dir();
    }
    return base_path;
}

std::string get_resources_img_path()
{
    return Glib::build_filename(get_application_path(), "resources", "img");
}

std::string get_resources_path()
{
    return Glib::build_filename(get_application_path(), "resources");
}

std::string get_resource_glade_file()
{
    return Glib::build_filename(get_application_path(), "resources", "app-main.glade");
}

