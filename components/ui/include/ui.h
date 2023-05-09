// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.2.3
// LVGL version: 8.3.4
// Project name: track

#ifndef _TRACK_UI_H
#define _TRACK_UI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"
#include "gpx_rec.h"
#include "ui_comp.h"
#include "ui_comp_hook.h"
#include "ui_events.h"
    void kmhAnim_Animation(lv_obj_t *TargetObject, int delay);
    void btnsAnim_Animation(lv_obj_t *TargetObject, int delay);
    extern lv_obj_t *ui_displayScr;
    extern lv_obj_t *ui_Dial;
    void ui_event_speed(lv_event_t *e);
    extern lv_obj_t *ui_speed;
    extern lv_obj_t *ui_kmh;
    extern lv_obj_t *ui_AVG;
    extern lv_obj_t *ui_Calorie;
    extern lv_obj_t *ui_Trip;
    extern lv_obj_t *ui_Time;
    extern lv_obj_t *ui_statusBar;
    extern lv_obj_t *ui_btns;
    extern lv_obj_t *ui_mapBtn;
    void ui_event_statusBtn(lv_event_t *e);
    extern lv_obj_t *ui_statusBtn;
    void ui_event_menuBtn(lv_event_t *e);
    extern lv_obj_t *ui_menuBtn;
    void ui_event_sysInfo(lv_event_t *e);
    extern lv_obj_t *ui_sysInfo;
    extern lv_obj_t *ui_statusBar2;
    extern lv_obj_t *ui_Panel2;
    extern lv_obj_t *ui_longitude;
    extern lv_obj_t *ui_lonData;
    extern lv_obj_t *ui_latitude;
    extern lv_obj_t *ui_latData;
    extern lv_obj_t *ui_altitude;
    extern lv_obj_t *ui_altData;
    void ui_event____initial_actions0(lv_event_t *e);
    extern lv_obj_t *ui____initial_actions0;

    LV_IMG_DECLARE(ui_img_map_png);     // assets\map.png
    LV_IMG_DECLARE(ui_img_m_start_png); // assets\m_start.png
    LV_IMG_DECLARE(ui_img_m_stop_png);  // assets\m_stop.png
    LV_IMG_DECLARE(ui_img_m_menu_png);  // assets\m_menu.png

    void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
