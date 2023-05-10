#ifndef _REC_TIMER_H
#define _REC_TIMER_H

// #include "driver/timer.h"
#include "rec_timer.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include "lvgl/lvgl.h"

extern TimerHandle_t rec_timer;
extern bool is_1sec_flag;
void rec_timer_callback(TimerHandle_t xTimer);
void rec_timer_init();
void rec_timer_deinit();
#endif