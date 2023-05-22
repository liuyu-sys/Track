#include "rec_timer.h"
#include "gpx_rec.h"
#include <stdio.h>
#include <stdlib.h>

TimerHandle_t rec_timer = NULL;

void rec_timer_callback(TimerHandle_t xTimer)
{
    recorder.recInfo.time++;
    lv_event_send(ui_timeData, LV_EVENT_VALUE_CHANGED, NULL);
}

void rec_timer_init()
{
    rec_init();
    static const char *TAG = "rec_timer_init";
    ESP_LOGI(TAG, "rec_timer_init");
    const TickType_t timer_interval = 1000 / portTICK_PERIOD_MS; // 定时器中断时间为 1 秒钟
    rec_timer = xTimerCreate("Timer", timer_interval, pdTRUE, NULL, rec_timer_callback);
}

void rec_timer_deinit()
{
    xTimerDelete(rec_timer, 0);
    rec_timer = NULL;
}