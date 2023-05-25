/*
 * @Descripttion :
 * @version      :
 * @Author       : Kevincoooool
 * @Date         : 2021-09-04 16:11:59
 * @LastEditors  : Kevincoooool
 * @LastEditTime : 2021-09-04 18:05:01
 * @FilePath     : \lvgl_v8\main\main.c
 */

#include "main.h"

SemaphoreHandle_t xGuiSemaphore;
TaskHandle_t gui_task_hand;
TaskHandle_t sd_card_task_hand;

void lv_tick_task(void *arg)
{
    (void)arg;
    lv_tick_inc(portTICK_PERIOD_MS);
}

void gui_task(void *arg)
{
    xGuiSemaphore = xSemaphoreCreateMutex();
    lv_init();          // lvgl内核初始化
    lvgl_driver_init(); // lvgl显示接口初始化
    /* Example for 1) */
    static lv_disp_draw_buf_t draw_buf;
    // 配置刷屏用的双缓存区大小,一般给分配的空间为 (CONFIG_LV_HOR_RES_MAX * 40~80)
    // DISP_BUF_SIZE 可通过menuconfig中的 CONFIG_CUSTOM_DISPLAY_BUFFER_SIZE 来配置。如不自定义,则默认为 (LV_HOR_RES_MAX * 40)
    lv_color_t *buf1 = (lv_color_t *)heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    lv_color_t *buf2 = (lv_color_t *)heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);

    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, DISP_BUF_SIZE); /*Initialize the display buffer*/

    static lv_disp_drv_t disp_drv;            /*A variable to hold the drivers. Must be static or global.*/
    lv_disp_drv_init(&disp_drv);              /*Basic initialization*/
    disp_drv.draw_buf = &draw_buf;            /*Set an initialized buffer*/
    disp_drv.flush_cb = disp_driver_flush;    /*Set a flush callback to draw to the display*/
    disp_drv.hor_res = CONFIG_LV_HOR_RES_MAX; /*Set the horizontal resolution in pixels*/
    disp_drv.ver_res = CONFIG_LV_VER_RES_MAX; /*Set the vertical resolution in pixels*/
    lv_disp_drv_register(&disp_drv);          /*Register the driver and save the created display objects*/

    /* Register an input device when enabled on the menuconfig */
#if CONFIG_LV_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.read_cb = touch_driver_read;
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register(&indev_drv);
#endif

    esp_register_freertos_tick_hook((esp_freertos_tick_cb_t)lv_tick_task);

    rec_timer_init();
    ui_init();

    while (1)
    {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(10));

        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {
            lv_timer_handler();
            xSemaphoreGive(xGuiSemaphore);
        }
    }
}

void my_log_output(const char *fmt, va_list args)
{
    FILE *log_file = fopen("/sdcard/example.log", "a");
    if (log_file)
    {
        vfprintf(log_file, fmt, args);
        fclose(log_file);
    }
}
sdmmc_card_t *card;
void sd_task(void *arg)
{
    vTaskDelay(pdMS_TO_TICKS(2000));
    static const char *TAG = "sd_fatfs_task";
    card = sd_card_fatfs_spi_init();
    while (!card)
    {
        // 验证错误,如果返回值为0,则SD卡初始化及FATFS挂载失败,重试
        ESP_LOGE(TAG, "Failed !! %d Retry!!", false);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        card = sd_card_fatfs_spi_init();
    }
    //  esp_log_set_vprintf(my_log_output);
    // int16_t accel[3], gyro[3];
    // 初始化 MPU6050/9250。检测 MPU6050/9250。并通过I2Cx总线写寄存器,来对功能进行配置。
    // i2c_mpu6050_init(I2C_NUM_0);
    while (true)
    {
        // i2c_mpu6050_get_accel_gyro(I2C_NUM_0, accel, gyro);
        // printf("Accel: x:%d\ty:%d\tz:%d\t, (x+y+z)/3 = %d ||\tGyro: x:%d\ty:%d\tz:%d\n", accel[0], accel[1], accel[2], (accel[0] + accel[1] + accel[2]) / 3, gyro[0], gyro[1], gyro[2]);

        // printf("Accel variance: %.2f, gyro variance: %.2f\n\n", calculateVariance(accel, 3), calculateVariance(gyro, 3));
        // UBaseType_t stackHigh_gui = uxTaskGetStackHighWaterMark(gui_task_hand);
        // UBaseType_t stackHigh_sd = uxTaskGetStackHighWaterMark(sd_card_task_hand);
        // printf("stackHigh_gui = %u, stackHigh_sd = %u\n", stackHigh_gui * sizeof(StackType_t), stackHigh_sd * sizeof(StackType_t));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
void app_main(void)
{
    nmea_init();
    xTaskCreatePinnedToCore(gui_task, "gui task", 1024 * 3, NULL, 1, &gui_task_hand, 0);
    xTaskCreate(sd_task, "sd_card task", 1024 * 3, NULL, 0, &sd_card_task_hand);
}
