// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.2.3
// LVGL version: 8.3.4
// Project name: track

#include "ui.h"
#include "nmea_parser.h"
#include <stdio.h>
#include <stdlib.h>

#define STRLEN 11

uint8_t recState = 0;

void now_speed_call(lv_event_t *e)
{
    gps_t *gps = (gps_t *)lv_event_get_param(e);
    char str[STRLEN];

    if (gps != NULL)
    {
        snprintf(str, 5, "%02d", (uint8_t)gps->speed);
        lv_label_set_text_fmt(ui_speed, "%s", str);

        lv_label_set_text_fmt(ui_comp_get_child(ui_statusBar, UI_COMP_STATUSBAR_LABEL7), "%02d : %02d", gps->tim.hour + TIME_ZONE, gps->tim.minute);
        lv_label_set_text_fmt(ui_comp_get_child(ui_statusBar2, UI_COMP_STATUSBAR_LABEL7), "%02d : %02d", gps->tim.hour + TIME_ZONE, gps->tim.minute);

        snprintf(str, 9, "%.05f", gps->latitude);
        lv_label_set_text_fmt(ui_latData, "%s°N", str);

        snprintf(str, 5, "%.02f", gps->altitude);
        lv_label_set_text_fmt(ui_altData, "%sm", str);

        snprintf(str, 9, "%.05f", gps->longitude);
        lv_label_set_text_fmt(ui_lonData, "%s°E", str);
    }
}

void rec_btn_style_set(lv_color_t bg_color, lv_color_t img_color, const void *img, lv_style_selector_t PART)
{
    lv_obj_set_style_bg_color(ui_statusBtn, bg_color, PART);
    lv_obj_set_style_bg_opa(ui_statusBtn, 255, PART);
    lv_obj_set_style_bg_img_src(ui_statusBtn, img, PART);
    lv_obj_set_style_bg_img_recolor(ui_statusBtn, img_color, PART);
    lv_obj_set_style_bg_img_recolor_opa(ui_statusBtn, 255, PART);
}
void set_text_statusBar(char *str)
{
    lv_label_set_text(ui_comp_get_child(ui_statusBar, UI_COMP_STATUSBAR_LABEL8), str);
    lv_label_set_text(ui_comp_get_child(ui_statusBar2, UI_COMP_STATUSBAR_LABEL8), str);
}

void ui_rec_update(lv_event_t *e)
{
    Recorder_Info_t Info = recorder.recInfo;
    lv_event_code_t event_code = lv_event_get_code(e);
    static float last_speed = 0;
    char str[20];
    int hour, min, sec;
    int trip = 0;
    if (LV_EVENT_VALUE_CHANGED == event_code)
    {
        last_speed = (float)atoi(lv_label_get_text(ui_speed));
        last_speed = (Info.avg_speed * (Info.time - 1) + last_speed) / Info.time;
        recorder.recInfo.avg_speed = last_speed;
        snprintf(str, sizeof(str), "%.01f", Info.avg_speed);
        lv_label_set_text_fmt(ui_avgData, "%skm/h", str);

        hour = Info.time / 3600;       // 计算小时数
        min = (Info.time % 3600) / 60; // 计算剩余分钟数
        sec = Info.time % 60;          // 计算剩余秒数
        if (hour > 0)
        {
            snprintf(str, sizeof(str), "%d: %02d: %02d", hour, min, sec);
        }
        else
        {
            snprintf(str, sizeof(str), "%02d: %02d", min, sec);
        }
        lv_label_set_text_fmt(ui_timeData, "%s", str);

        trip = last_speed * 0.2778 * Info.time; // 计算路程
        if (trip > 1000)
        {
            snprintf(str, sizeof(str), "%.02f km", (float)trip / 1000);
        }
        else
        {
            snprintf(str, sizeof(str), "%d m", trip);
        }
        lv_label_set_text_fmt(ui_tripData, "%s", str);
    }
}
void onRecord(bool longPress)
{
    switch (recState)
    {
    case 0: // 准备
        if (!longPress)
        {
            recState = 1;
            set_text_statusBar("REC");
            rec_btn_style_set(lv_color_hex(0xFBA414), lv_color_hex(0x2C2C2C), &ui_img_m_stop_png, LV_PART_MAIN);
            recorder.recInfo.cmd = RECORDER_CMD_START;
            xTimerReset(rec_timer, 0);
        }
        break;
    case 1: // REC
        if (!longPress)
        {
            recState = 2;
            set_text_statusBar("PUASE");
            rec_btn_style_set(lv_color_hex(0xFBA414), lv_color_hex(0x2C2C2C), &ui_img_m_start_png, LV_PART_MAIN);
            recorder.recInfo.cmd = RECORDER_CMD_PAUSE;
            recorder.active = false;
            xTimerStop(rec_timer, 0);
        }
        break;
    case 2: // 停止
        if (!longPress)
        {
            recState = 1;
            set_text_statusBar("REC");
            rec_btn_style_set(lv_color_hex(0xFBA414), lv_color_hex(0x2C2C2C), &ui_img_m_stop_png, LV_PART_MAIN);
            recorder.recInfo.cmd = RECORDER_CMD_CONTINUE;
            recorder.active = true;
            xTimerStart(rec_timer, 0);
        }
        else
        {
            recState = 0;
            set_text_statusBar("");
            rec_btn_style_set(lv_color_hex(0x2C2C2C), lv_color_hex(0xFBA414), &ui_img_m_start_png, LV_PART_MAIN);
            recorder.recInfo.cmd = RECORDER_CMD_STOP;
            xTimerStop(rec_timer, 0);
        }
        break;
    default:
        break;
    }
}