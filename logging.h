//
// logging.h
// OrangePi Temperature Sensor Server
//
// Created by Peter Kovacs "kiwep" on 2018-07-03.
//

#pragma once

void open_logfile(char* path);
void close_logfile();

void logmsg(const char* format, ...);
