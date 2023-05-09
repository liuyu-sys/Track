#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "lvgl/demos/lv_demos.h"
#include "lvgl_helpers.h"
#include "lvgl/lvgl.h"

#include "esp_freertos_hooks.h"
#include "nmea_task.h"
#include "esp_log.h"
#include <string.h>
#include "driver/gpio.h"

#include "sd_card_fatfs.h"
#include "ui.h"
#include "gpx_rec.h"
#include "nmea_parser.h"

void create_demo_application(void);
void gui_task(void *arg);
void lv_tick_task(void *arg);

#endif