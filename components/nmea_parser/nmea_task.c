#include "nmea_task.h"
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "lvgl/lvgl.h"
#include "ui.h"

static const char *TAG = "gps_nmea";
gps_t *gps = NULL;
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
        gps = (gps_t *)event_data;
        /* print information parsed from GPS statements */

        ESP_LOGI(TAG, "%d/%d/%d %d:%d:%d => \r\n"
                      "\t\t\t\t\t\tlatitude   = %.05f°N\r\n"
                      "\t\t\t\t\t\tlongitude = %.05f°E\r\n"
                      "\t\t\t\t\t\taltitude   = %.02fm\r\n"
                      "\t\t\t\t\t\tspeed      = %fm/s",
                 gps->date.year + YEAR_BASE, gps->date.month, gps->date.day,
                 gps->tim.hour + TIME_ZONE, gps->tim.minute, gps->tim.second,
                 gps->latitude, gps->longitude, gps->altitude, gps->speed);

        lv_event_send(ui_latData, LV_EVENT_VALUE_CHANGED, NULL);
        lv_event_send(ui_lonData, LV_EVENT_VALUE_CHANGED, NULL);
        lv_event_send(ui_speed, LV_EVENT_VALUE_CHANGED, NULL);
        lv_event_send(ui_altData, LV_EVENT_VALUE_CHANGED, NULL);
        break;
    case GPS_UNKNOWN:
        /* print unknown statements */
        ESP_LOGD(TAG, "Unknown statement:%s", (char *)event_data);
        break;
    default:
        break;
    }
}

void nmea_task(void *arg)
{
    /* NMEA parser configuration */
    nmea_parser_config_t config = NMEA_PARSER_CONFIG_DEFAULT();
    /* init NMEA parser library */
    nmea_parser_handle_t nmea_hdl = nmea_parser_init(&config);
    /* register event handler for NMEA parser library */
    nmea_parser_add_handler(nmea_hdl, gps_event_handler, NULL);

    vTaskDelete(NULL);
}