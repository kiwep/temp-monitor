//
// w1temp.h
// OrangePi Temperature Sensor Server
//
// Created by Peter Kovacs "kiwep" on 2018-07-03.
//

#pragma once
#include <stdbool.h>

bool read_w1_temp(const char *sensor_id, float* temp);
