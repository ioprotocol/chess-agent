/* screenshot-utils.h - common functions for GNOME Screenshot
 *
 * Copyright (C) 2001-2006  Jonathan Blandford <jrb@alum.mit.edu>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 */

#ifndef __SCREENSHOT_UTILS_H__
#define __SCREENSHOT_UTILS_H__

#include <gtk/gtk.h>
#include <gdk/gdkx.h>

G_BEGIN_DECLS
GdkPixbuf*  screenshot_fallback_get_pixbuf(GdkRectangle *rectangle);

GdkPixbuf*  screenshot_get_pixbuf(GdkRectangle *rectangle);

void        init_screen_config(void);
/**
 * save screen shot bitmap into /home/xsy/.cache/chess dir
 */
void		save_pixbuf_to_jpeg_file(GdkPixbuf *);

G_END_DECLS

#endif /* __SCREENSHOT_UTILS_H__ */
