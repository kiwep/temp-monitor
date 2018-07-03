//
// w1temp.c
// OrangePi Temperature Sensor Server
//
// Created by Peter Kovacs "kiwep" on 2018-07-03.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "w1temp.h"
#include "logging.h"

const char* w1_slave_path = "/sys/bus/w1/devices/%s/w1_slave";

bool read_w1_temp(const char *sensor_id, float* temp) {
    char name_buff[50];
    snprintf(name_buff, sizeof(name_buff) - 1, w1_slave_path, sensor_id);
    FILE *f = fopen(name_buff, "r");
    if (f) {
        char file_buff[120];
        char temp_buff[6] = {0, 0, 0, 0, 0, 0};
        bool ret = false;
        size_t nread;
        if ((nread = fread(file_buff, 1, sizeof(file_buff), f)) > 0) {
            strncpy(temp_buff, &file_buff[nread - 6], 5);
            float t = (float)atof(temp_buff) / 1000.0f;
            // printf("temp: %f\n", t);
            *temp = t;
            ret = true;
        }
        else {
            logmsg("Error: Failed to read 1-wire slave '%s'!", sensor_id);
        }

        fclose(f);
        return ret;
    }

    logmsg("Error: failed to open 1-wire slave '%s'!", sensor_id);

    return false;
}
