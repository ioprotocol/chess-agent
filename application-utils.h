//
// Created by xushy on 18-7-2.
//

#ifndef CHESSAGENT_APPLICATION_UTILS_H
#define CHESSAGENT_APPLICATION_UTILS_H

#include <gtk/gtk.h>
#include <gdk/gdkx.h>


#define PWD "EPWD"

G_BEGIN_DECLS

enum ChessType {
    B_CHE = 0, B_MA, B_XIANG, B_SHI, B_JIANG, B_PAO, B_ZU,
    R_CHE, R_MA, R_XIANG, R_SHI, R_JIANG, R_PAO, R_ZU,
    UN_KNOW
};
/**
 * Get current application img resources path
 * @return
 */
gchar* get_resources_img_path();

gchar* get_resources_path();

G_END_DECLS


#endif //CHESSAGENT_APPLICATION_UTILS_H
