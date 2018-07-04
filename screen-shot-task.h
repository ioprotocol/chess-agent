//
// Created by xushy on 18-7-4.
//

#ifndef CHESSAGENT_SCREEN_SHOT_TASK_H
#define CHESSAGENT_SCREEN_SHOT_TASK_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

extern GTask* screen_shot_task;

extern GCancellable* screen_shot_task_cancellable;

void screen_shot_task_call_back(GObject *source_object, GAsyncResult *res, gpointer user_data);

void screen_shot_task_data_destroy(gpointer data);

void screen_shot_task_thread(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable);

void screen_shot_task_create(gpointer source_object, gint sleep_time);

void screen_shot_task_destory();

G_END_DECLS

#endif //CHESSAGENT_SCREEN_SHOT_TASK_H
