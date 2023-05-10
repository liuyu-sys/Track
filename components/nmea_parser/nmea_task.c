#include "nmea_task.h"
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "lvgl/lvgl.h"
#include "ui.h"
#include "gpx_rec.h"
#include "rec_timer.h"

static const char *TAG = "gps_nmea";

uint8_t hour_check(uint8_t hour)
{
    if (hour == 24)
        return 0;
    else
        return hour;
}

/**
 * @brief GPS Event Handler
 *
 * @param event_handler_arg handler specific arguments
 * @param event_base event base, here is fixed to ESP_NMEA_EVENT
 * @param event_id event id
 * @param event_data event specific arguments
 */
void gps_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    // int speed_data[11] = {10, 11, 12, 13, 14, 15, 12, 16, 12, 17, 14};
    // static uint32_t cnt = 0;
    gps_t *gps = NULL;
    switch (event_id)
    {
    case GPS_UPDATE:
        gps = (gps_t *)event_data;
        /* print information parsed from GPS statements */
        ESP_LOGD(TAG, "%d/%d/%d %d:%d:%d => \r\n"
                      "\t\t\t\t\t\tlatitude   = %.05f°N\r\n"
                      "\t\t\t\t\t\tlongitude = %.05f°E\r\n"
                      "\t\t\t\t\t\taltitude   = %.02fm\r\n"
                      "\t\t\t\t\t\tspeed      = %fm/s",
                 gps->date.year + YEAR_BASE, gps->date.month, gps->date.day,
                 gps->tim.hour + TIME_ZONE, gps->tim.minute, gps->tim.second,
                 gps->latitude, gps->longitude, gps->altitude, gps->speed);
        gps->tim.hour = hour_check(gps->tim.hour);
        // gps->speed = speed_data[cnt++];
        // if (cnt == 11)
        // {
        //     cnt = 10;
        // }
        rec_event_handle(gps);
        lv_event_send(ui_speed, LV_EVENT_VALUE_CHANGED, gps);
        break;
    case GPS_UNKNOWN:
        /* print unknown statements */
        ESP_LOGD(TAG, "Unknown statement:%s", (char *)event_data);
        break;
    default:
        break;
    }
}

void nmea_init(void)
{
    /* NMEA parser configuration */
    nmea_parser_config_t config = NMEA_PARSER_CONFIG_DEFAULT();
    /* init NMEA parser library */
    nmea_parser_handle_t nmea_hdl = nmea_parser_init(&config);
    /* register event handler for NMEA parser library */
    nmea_parser_add_handler(nmea_hdl, gps_event_handler, NULL);
}