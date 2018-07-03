//
// dht22.c
// OrangePi Temperature Sensor Server
//
// Created by Peter Kovacs "kiwep" on 2018-07-03.
// Original code from: http://www.piprojects.xyz/temperature-humidity-sensor-orange-pi/
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <wiringPi.h>

#include "dht22.h"
#include "logging.h"

#define MAX_TIMINGS 185
#define DHT_PIN 7

int data[5] = { 0, 0, 0, 0, 0 };

bool read_dht22(float* temp, float* hum)
{
    for (int try_cnt = 0; try_cnt < 15; try_cnt++) {

        uint8_t laststate = HIGH;
        uint8_t counter = 0;
        uint8_t j = 0, i;

        data[0] = data[1] = data[2] = data[3] = data[4] = 0;

        /* pull pin down for 18 milliseconds */
        pinMode(DHT_PIN, OUTPUT);
        digitalWrite(DHT_PIN, LOW);
        delay(18);

        /* prepare to read the pin */
        pinMode(DHT_PIN, INPUT);

        /* detect change and read data */
        for (i = 0; i < MAX_TIMINGS; i++) {
            counter = 0;
            while (digitalRead(DHT_PIN) == laststate) {
                counter++;
                delayMicroseconds(1);
                if (counter == 255) {
                    break;
                }
            }
            laststate = digitalRead(DHT_PIN);

            if (counter == 255) break;

            /* ignore first 3 transitions */
            if ((i >= 4) && (i % 2 == 0)) {
                /* shove each bit into the storage bytes */
                data[j / 8] <<= 1;
                if (counter > 16) data[j / 8] |= 1;
                j++;
            }
        }

        /* check we read 40 bits (8bit x 5 ) + verify checksum in the last byte */
        if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
            float h = (float)((data[0] << 8) + data[1]) / 10;
            if (h > 100) {
                h = data[0];    // for DHT11
            }
            float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
            if (c > 125) {
                c = data[2];    // for DHT11
            }
            if (data[2] & 0x80) {
                c = -c;
            }

            *temp = c;
            *hum = h;

            return true;
        }

        delay(550);
    }

    logmsg("Error: DHT22 sensor reading failed!");

    return false;
}
