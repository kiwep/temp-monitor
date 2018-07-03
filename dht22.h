//
// dht22.h
// OrangePi Temperature Sensor Server
//
// Created by Peter Kovacs "kiwep" on 2018-07-03.
//

#pragma once
#include <stdbool.h>

bool read_dht22(float* temp, float* hum);
