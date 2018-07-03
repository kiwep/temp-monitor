//
// main.h
// OrangePi Temperature Sensor Server
//
// Created by Peter Kovacs "kiwep" on 2018-07-03.
//

#pragma once

typedef struct sensor_data {
  float external_temp;
  float internal_temp;
  float internal_hum;
  char update_time[21];
} sensor_data;
