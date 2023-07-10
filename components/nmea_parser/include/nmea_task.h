#ifndef __NMEA_TASK_H
#define __NMEA_TASK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "nmea_parser.h"
    void gps_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
    void nmea_init(void);

    extern gps_t gpsInfo;
#ifdef __cplusplus
}
#endif

#endif
