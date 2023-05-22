
idf_component_register(SRCS "rec_timer.c" "gpx_rec.c" "GPX.c" "gpx_parse.c"
                       INCLUDE_DIRS "include"
                       REQUIRES "fatfs" "lvgl" "nmea_parser" "freertos" "json" "expat" "main")
