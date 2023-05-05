// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.2.3
// LVGL version: 8.3.4
// Project name: track

#include "ui.h"
#include "ui_helpers.h"
#include "ui_comp.h"

uint32_t LV_EVENT_GET_COMP_CHILD;

typedef struct
{
    uint32_t child_idx;
    lv_obj_t *child;
} ui_comp_get_child_t;

lv_obj_t *ui_comp_get_child(lv_obj_t *comp, uint32_t child_idx)
{
    ui_comp_get_child_t info;
    info.child = NULL;
    info.child_idx = child_idx;
    lv_event_send(comp, LV_EVENT_GET_COMP_CHILD, &info);
    return info.child;
}

void get_component_child_event_cb(lv_event_t *e)
{
    lv_obj_t **c = lv_event_get_user_data(e);
    ui_comp_get_child_t *info = lv_event_get_param(e);
    info->child = c[info->child_idx];
}

void del_component_child_event_cb(lv_event_t *e)
{
    lv_obj_t **c = lv_event_get_user_data(e);
    lv_mem_free(c);
}

// COMPONENT statusBar

lv_obj_t *ui_statusBar_create(lv_obj_t *comp_parent)
{

    lv_obj_t *cui_statusBar;
    cui_statusBar = lv_obj_create(comp_parent);
    lv_obj_set_width(cui_statusBar, 240);
    lv_obj_set_height(cui_statusBar, 20);
    lv_obj_set_x(cui_statusBar, 0);
    lv_obj_set_y(cui_statusBar, -152);
    lv_obj_set_align(cui_statusBar, LV_ALIGN_CENTER);
    lv_obj_clear_flag(cui_statusBar, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(cui_statusBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_statusBar, lv_color_hex(0x707070), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_statusBar, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(cui_statusBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(cui_statusBar, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(cui_statusBar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(cui_statusBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(cui_statusBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *cui_Label7;
    cui_Label7 = lv_label_create(cui_statusBar);
    lv_obj_set_width(cui_Label7, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(cui_Label7, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(cui_Label7, LV_ALIGN_CENTER);
    lv_label_set_text(cui_Label7, "00 : 00");
    lv_obj_set_style_text_font(cui_Label7, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *cui_Label8;
    cui_Label8 = lv_label_create(cui_statusBar);
    lv_obj_set_width(cui_Label8, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(cui_Label8, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(cui_Label8, 0);
    lv_obj_set_y(cui_Label8, -2);
    lv_obj_set_align(cui_Label8, LV_ALIGN_LEFT_MID);
    lv_label_set_text(cui_Label8, "");
    lv_obj_set_style_text_font(cui_Label8, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_STATUSBAR_NUM);
    children[UI_COMP_STATUSBAR_STATUSBAR] = cui_statusBar;
    children[UI_COMP_STATUSBAR_LABEL7] = cui_Label7;
    children[UI_COMP_STATUSBAR_LABEL8] = cui_Label8;
    lv_obj_add_event_cb(cui_statusBar, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_statusBar, del_component_child_event_cb, LV_EVENT_DELETE, children);
    ui_comp_statusBar_create_hook(cui_statusBar);
    return cui_statusBar;
}
