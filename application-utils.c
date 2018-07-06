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

void  set_pixel_value(GdkPixbuf *pixbuf, int x, int y, int color)
{
    guchar *pixel;
    guchar *p;
    gint channel;
    gint rowstride;

    if(!pixbuf)   return ;

    channel = gdk_pixbuf_get_n_channels(pixbuf);
    rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    pixel = gdk_pixbuf_get_pixels(pixbuf);
    p = pixel+y*rowstride+x*channel;

    p[0] = (color & 0xFF000000) >>24;
    p[1] = (color & 0x00FF0000) >>16;
    p[2] = (color & 0x0000FF00) >>8;
}