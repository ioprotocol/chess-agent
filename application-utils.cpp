//
// Created by xushy on 18-7-2.
//
#include "application-utils.h"

#define APPLICATION_PATH "APATH"

char *get_application_path()
{
    char *pathvar = getenv(APPLICATION_PATH);
    if (pathvar == NULL || strlen(pathvar) == 0) {
        char* dir = get_current_dir_name();
        return dir;
    } else {
        return pathvar;
    }
}