//
// main.c
// OrangePi Temperature Sensor Server
//
// Created by Peter Kovacs "kiwep" on 2018-07-03.
//

#include <stdio.h>
#include <evhtp.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <wiringPi.h>

#include "main.h"
#include "dht22.h"
#include "w1temp.h"
#include "logging.h"

#define QUOTE(...) #__VA_ARGS__

const char* w1_sensor_id = "28-01131a393320";
const int http_port = 8012;

const char* json_response = QUOTE({
  "sensors": {
    "dht22": {
      "temperature": %.1f,
      "humidity": %.1f
    },
    "ds18b20": {
      "temperature": %.1f
    }
  },
  "updated": "%s"
}\n);

pthread_mutex_t mutex;
volatile static sensor_data data;

void time_get_now(char* buff, size_t buff_len)
{
    time_t now;
    time(&now);
    strftime(buff, buff_len, "%FT%TZ", gmtime(&now));
}

void* update_thread(void* d)
{
    float ext_temp = 0.0f;
    float in_temp = 0.0f;
    float in_hum = 0.0f;

    if (wiringPiSetup() == -1) exit(1);
    logmsg("Sensor reader thread started");

    while (1) {
      if (read_w1_temp(w1_sensor_id, &ext_temp) && read_dht22(&in_temp, &in_hum)) {
            pthread_mutex_lock(&mutex);
            data.external_temp = ext_temp;
            data.internal_temp = in_temp;
            data.internal_hum = in_hum;
            time_get_now((char *)&data.update_time[0], 21);
            pthread_mutex_unlock(&mutex);
        }
        sleep(7);
    }
}

void testcb(evhtp_request_t* req, void* a)
{
    evhtp_headers_add_header(req->headers_out, evhtp_header_new("Content-Type", "text/json", 0, 0));
    pthread_mutex_lock(&mutex);
    evbuffer_add_printf(req->buffer_out, json_response, data.internal_temp, data.internal_hum, data.external_temp, data.update_time);
    pthread_mutex_unlock(&mutex);
    evhtp_send_reply(req, EVHTP_RES_OK);
}

int main(int argc, char* argv[])
{
    pthread_t t1;

    if (argc > 1 && strcmp(argv[1], "-log") == 0) {
        char* p = strrchr(argv[0], '/');
        if (p) p[0] = '\0';
        char log_file_path[255];
        snprintf(log_file_path, 255, "%s/temp-mon.log", argv[0]);
        open_logfile(log_file_path);
    }
    else {
        open_logfile(NULL);
    }

    evbase_t* evbase = event_base_new();
    evhtp_t* htp = evhtp_new(evbase, NULL);

    data.external_temp = 0.0f;
    data.internal_temp = 0.0f;
    data.internal_hum = 0.0f;

    pthread_create(&t1, NULL, update_thread, NULL);

    evhtp_set_cb(htp, "/sensor", testcb, NULL);
    evhtp_bind_socket(htp, "0.0.0.0", http_port, 1024);

    logmsg("HTTP Server started on port %d", http_port);
    event_base_loop(evbase, 0);

    close_logfile();
    return 0;
}
