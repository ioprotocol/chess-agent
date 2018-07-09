//
// Created by xushy on 18-7-2.
//
#include "application-utils.h"

gchar* get_resources_img_path()
{
    gchar *application_path, *resource_path;
    const gchar* env_path;
    env_path = g_getenv(PWD);

    if(env_path == NULL || strlen(env_path) == 0)
    {
        application_path = g_get_current_dir();
    } else {
        application_path = g_strdup(env_path);
    }

    resource_path = g_build_filename(application_path, "resources", "img", NULL);

    g_free(application_path);

    return resource_path;
}

gchar* get_resources_path()
{
    gchar *application_path, *resource_path;
    const gchar* env_path;
    env_path = g_getenv(PWD);

    if(env_path == NULL || strlen(env_path) == 0)
    {
        application_path = g_get_current_dir();
    } else {
        application_path = g_strdup(env_path);
    }

    resource_path = g_build_filename(application_path, "resources", NULL);

    g_free(application_path);

    return resource_path;
}

gchar* get_main_glade_file()
{
    gchar *path, *file_name;
    path = get_resources_path();
    file_name = g_build_filename(path, "appMain.glade", NULL);
    g_free(path);
    return file_name;
}

