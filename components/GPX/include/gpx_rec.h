#ifndef __GPX_REC_H
#define __GPX_REC_H

#include "nmea_parser.h"
#include "stdio.h"
#include "GPX.h"
#include "esp_log.h"

typedef enum
{
    RECORDER_CMD_START = 0,
    RECORDER_CMD_PAUSE,
    RECORDER_CMD_CONTINUE,
    RECORDER_CMD_STOP,
    RECORDER_CMD_NONE,
} Recorder_Cmd_t;

typedef struct
{
    Recorder_Cmd_t cmd;
    uint16_t time;
} Recorder_Info_t;

typedef struct
{
    Recorder_Info_t recInfo;
    FILE *file_p;
    bool active;
} Recorder_t;

extern Recorder_t recorder;

void rec_init();
void rec_point(gps_t *gps_gpx);
void rec_start(gps_t *gps_gpx);
void Recorder_FileWriteString(FILE *file_p, const char *str);
void rec_stop();
void rec_event_handle(void *data);

#endif