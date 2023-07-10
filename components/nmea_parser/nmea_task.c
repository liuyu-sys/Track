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
gps_t gpsInfo;

uint8_t hour_check(uint8_t hour)
{
    if (hour == 24)
        return 0;
    else
        return hour;
}
void Reload_map()
{
    static uint8_t i = 0;
    if (i >= 2)
    {
        i = 0;
        if (ui_now_scr == UI_MAP_SCR && gpsInfo.valid)
            changeMap();
    }
    else
    {
        i++;
    }
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

    switch (event_id)
    {
    case GPS_UPDATE:
        gpsInfo = *(gps_t *)event_data;
        /* print information parsed from GPS statements */
        ESP_LOGD(TAG, "%d/%d/%d %d:%d:%d => \r\n"
                      "latitude   = %.05f°N\r\n"
                      "longitude = %.05f°E\r\n"
                      "altitude   = %.02fm\r\n"
                      "speed      = %fm/s\r\n"
                      "dop_h = %.03f , dop_p = %.03f, dop_v = %.03f\r\n",
                 gpsInfo.date.year + YEAR_BASE, gpsInfo.date.month, gpsInfo.date.day,
                 gpsInfo.tim.hour + TIME_ZONE, gpsInfo.tim.minute, gpsInfo.tim.second,
                 gpsInfo.latitude, gpsInfo.longitude, gpsInfo.altitude, gpsInfo.speed, gpsInfo.dop_h, gpsInfo.dop_p, gpsInfo.dop_v);

        gpsInfo.tim.hour = hour_check(gpsInfo.tim.hour);

        rec_event_handle(&gpsInfo);
        lv_event_send(ui_speed, LV_EVENT_VALUE_CHANGED, &gpsInfo);
        Reload_map();
        break;
    case GPS_UNKNOWN:
        /* print unknown statements */
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