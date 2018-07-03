//
// logging.c
// OrangePi Temperature Sensor Server
//
// Created by Peter Kovacs "kiwep" on 2018-07-03.
//

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "logging.h"

static FILE* f_logfile = NULL;

void open_logfile(char* path)
{
    if (path) {
        f_logfile = fopen(path, "a");
    }
    else {
        f_logfile = stdout;
    }
}

void close_logfile()
{
    if (f_logfile) fclose(f_logfile);
    f_logfile = NULL;
}

void logmsg(const char* format, ...)
{
    if (f_logfile == NULL) return;
    va_list args;
    va_start(args, format);
    char ts[23];
    time_t now;
    time(&now);
    strftime(ts, 23, "[%Y-%m-%d %H:%M:%S] ", localtime(&now));
    fprintf(f_logfile, ts);
    vfprintf(f_logfile, format, args);
    fprintf(f_logfile, "\n");
    fflush(f_logfile);
}
