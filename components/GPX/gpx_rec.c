#include "gpx_rec.h"

#define RECORDER_GPX_TIME_FMT "%d-%02d-%02dT%02d:%02d:%02dZ"
#define FILE_PATH_FORMAT "/sdcard/TRK_%d%02d%02d_%02d%02d%02d.gpx"
#define RECORDER_GPX_META_NAME "Track 1.0"
#define RECORDER_GPX_META_DESC "https://github.com/liuyu-sys"

static const char *TAG = "gpx_rec";
Recorder_t recorder;

void rec_init()
{
    recorder.file_p = NULL;
    recorder.active = false;
    recorder.recInfo.time = 0;
    recorder.recInfo.cmd = RECORDER_CMD_NONE;
}
void Recorder_FileWriteString(FILE *file_p, const char *str)
{
    // LV_LOG_USER(str);
    fprintf(file_p, "%s", str);
}

void rec_start(gps_t *gps_gpx)
{
    ESP_LOGI(TAG, "Track record start");

    char filepath[512];
    snprintf(filepath, sizeof(filepath), FILE_PATH_FORMAT, gps_gpx->date.year + YEAR_BASE, gps_gpx->date.month, gps_gpx->date.day,
             gps_gpx->tim.hour + TIME_ZONE, gps_gpx->tim.minute, gps_gpx->tim.second);
    ESP_LOGE(TAG, "fileptah = %s", filepath);
    FILE *file_p = fopen("/sdcard/1.gpx", "w");
    if (file_p == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    recorder.file_p = file_p;
    ESP_LOGI(TAG, "Track file %s open success", filepath);

    setMetaName(RECORDER_GPX_META_NAME);
    setMetaDesc(RECORDER_GPX_META_DESC);
    setName(filepath);
    setDesc("");

    Recorder_FileWriteString(recorder.file_p, getOpen());
    Recorder_FileWriteString(recorder.file_p, getMetaData());
    Recorder_FileWriteString(recorder.file_p, getTrakOpen());
    Recorder_FileWriteString(recorder.file_p, getInfo());
    Recorder_FileWriteString(recorder.file_p, getTrakSegOpen());
    recorder.active = true;
    // return file_p;
}
void rec_point(gps_t *gps_gpx)
{
    if (recorder.active != true)
    {
        ESP_LOGE(TAG, "rec_point active error!");
        return;
    }
    ESP_LOGI(TAG, "Track recording...");
    ESP_LOGI(TAG, "%d/%d/%d %d:%d:%d => \r\n"
                  "\t\t\t\t\t\tlatitude   = %.05f°N\r\n"
                  "\t\t\t\t\t\tlongitude = %.05f°E\r\n"
                  "\t\t\t\t\t\taltitude   = %.02fm\r\n"
                  "\t\t\t\t\t\tspeed      = %fm/s",
             gps_gpx->date.year + YEAR_BASE, gps_gpx->date.month, gps_gpx->date.day,
             gps_gpx->tim.hour + TIME_ZONE, gps_gpx->tim.minute, gps_gpx->tim.second,
             gps_gpx->latitude, gps_gpx->longitude, gps_gpx->altitude, gps_gpx->speed);
    char timeBuf[64];
    snprintf(
        timeBuf, sizeof(timeBuf), "%d-%02d-%02dT%02d:%02d:%02dZ", gps_gpx->date.year + YEAR_BASE, gps_gpx->date.month, gps_gpx->date.day,
        gps_gpx->tim.hour + TIME_ZONE, gps_gpx->tim.minute, gps_gpx->tim.second);
    ESP_LOGI(TAG, "%s", timeBuf);
    char ele[10];
    snprintf(ele, sizeof(ele), "%.02f", gps_gpx->altitude);
    setEle(ele);
    setTime(timeBuf);

    char lon[15];
    snprintf(lon, sizeof(lon), "%.06f", gps_gpx->longitude);
    char lat[15];
    snprintf(lat, sizeof(lat), "%.06f", gps_gpx->latitude);
    char *gpxStr = getPt(GPX_TRKPT, lon, lat);

    Recorder_FileWriteString(recorder.file_p, gpxStr);
}

void rec_stop()
{

    ESP_LOGI(TAG, "Track record end");
    Recorder_FileWriteString(recorder.file_p, getTrakSegClose());

    Recorder_FileWriteString(recorder.file_p, getTrakClose());
    Recorder_FileWriteString(recorder.file_p, getClose());

    fclose(recorder.file_p);
    recorder.active = false;
    ESP_LOGI(TAG, "file closed");
}

void rec_event_handle(void *data)
{
    gps_t *gps_data = (gps_t *)data;
    switch (recorder.recInfo.cmd)
    {
    case RECORDER_CMD_NONE:
        ESP_LOGI(TAG, "recorder.active = %d", recorder.active);
        break;
    case RECORDER_CMD_START:
        if (recorder.active == false)
            rec_start(gps_data);
        if (recorder.active == true)
            rec_point(gps_data);
        break;
    case RECORDER_CMD_PAUSE:

        break;
    case RECORDER_CMD_CONTINUE:
        if (recorder.active == true)
            rec_point(gps_data);
        break;
    case RECORDER_CMD_STOP:
        rec_stop();
        recorder.recInfo.cmd = RECORDER_CMD_NONE;
        break;
    default:
        break;
    }
}