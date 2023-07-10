#ifndef __GPX_REC_H
#define __GPX_REC_H

#include "nmea_parser.h"
#include "stdio.h"
#include "GPX.h"
#include "esp_log.h"
#include "string.h"
#include "lvgl/lvgl.h"
#include "ui.h"
#include "cJSON.h"
#include "sdmmc_cmd.h"
#include "dirent.h"
#include "main.h"

#define RECORDER_GPX_TIME_FMT "%d-%02d-%02dT%02d:%02d:%02dZ"
#define FILE_PATH "/sdcard/track"
#define RECORDER_GPX_META_NAME "Track 1.0"
#define RECORDER_GPX_META_DESC "https://github.com/liuyu-sys"

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
    uint32_t time;
    float avg_speed;
} Recorder_Info_t;

typedef struct
{
    Recorder_Info_t recInfo;
    FILE *file_p;
    char *fileName;
    bool active;
} Recorder_t;

extern Recorder_t recorder;
char *get_gpx_fileName();
char **get_gpx_files(const char *directory, int *num_files);
void createDirectoryIfNotExists(const char *path);
void rec_init();
void rec_point(gps_t *gps_gpx);
void rec_start(gps_t *gps_gpx);
void Recorder_FileWriteString(FILE *file_p, const char *str);
void rec_stop();
void rec_event_handle(void *data);

#endif