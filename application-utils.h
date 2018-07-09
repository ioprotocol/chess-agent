//
// Created by xushy on 18-7-2.
//

#ifndef CHESSAGENT_APPLICATION_UTILS_H
#define CHESSAGENT_APPLICATION_UTILS_H

#include <gtk/gtk.h>
#include <gdk/gdkx.h>


#define PWD "EPWD"

G_BEGIN_DECLS

/**
 *
 * @return
 */
gchar* get_resources_path();

/**
 * Get current application img resources path
 * @return
 */
gchar* get_resources_img_path();

gchar* get_main_glade_file();
G_END_DECLS


#endif //CHESSAGENT_APPLICATION_UTILS_H
