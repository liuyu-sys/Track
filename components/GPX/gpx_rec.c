#include "gpx_rec.h"

#define CONCAT(str1, str2) str1##str2

static const char *TAG = "gpx_rec";
Recorder_t recorder;

char *get_gpx_fileName()
{
    // if (sdmmc_get_status(card) != ESP_OK)
    //     return "NO FILE";
    const char *directory = FILE_PATH; // 替换为实际的目录路径
    int num_files;
    char **filenames = get_gpx_files(directory, &num_files);
    char *res = (char *)malloc(20 * sizeof(char) * num_files);
    if (filenames == NULL)
    {
        printf("No GPX files found.\n");
    }
    else
    {
        printf("Found %d GPX files:\n", num_files);
        int offset = 0;
        for (int i = 0; i < num_files; i++)
        {
            if (i == (num_files - 1))
            {
                int cnt = snprintf(res + offset, 17 + 2, "%s", filenames[i]);
                offset += cnt;
                free(filenames[i]);
                break;
            }
            int cnt = snprintf(res + offset, 17 + 2, "%s\n", filenames[i]);
            offset += cnt;
            free(filenames[i]);
        }

        free(filenames);
    }
    return res;
}
char **get_gpx_files(const char *directory, int *num_files)
{
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    char **filenames = NULL;

    dir = opendir(directory);
    if (dir == NULL)
    {
        printf("Failed to open directory: %s\n", directory);
        return NULL;
    }

    // 遍历目录中的所有文件
    while ((entry = readdir(dir)) != NULL)
    {
        // 判断文件名是否以".gpx"结尾
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".gpx") != NULL)
        {
            count++;
            // 分配内存保存文件名
            filenames = (char **)realloc(filenames, count * sizeof(char *));
            filenames[count - 1] = strdup(entry->d_name);
        }
    }

    closedir(dir);
    *num_files = count;
    return filenames;
}

void createDirectoryIfNotExists(const char *path)
{
    struct stat st;
    if (stat(path, &st) == -1)
    {
        // 目录不存在，创建目录
        int mkdirResult = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
        if (mkdirResult == 0)
        {
            printf("Created directory: %s\n", path);
        }
        else
        {
            printf("Failed to create directory: %s\n", path);
        }
    }
    else if (S_ISDIR(st.st_mode))
    {
        // 目录已经存在
        printf("Directory already exists: %s\n", path);
    }
    else
    {
        // 路径存在但不是目录
        printf("Path is not a directory: %s\n", path);
    }
}

void rec_init()
{
    recorder.file_p = NULL;
    recorder.active = false;
    recorder.fileName = NULL;
    recorder.recInfo.time = 0;
    recorder.recInfo.avg_speed = 0;
    recorder.recInfo.cmd = RECORDER_CMD_NONE;
}
void Recorder_FileWriteString(FILE *file_p, const char *str)
{
    fprintf(file_p, "%s", str);
}

void rec_start(gps_t *gps_gpx)
{
    /* 打印记录开始 */
    ESP_LOGI(TAG, "Track record start");
    /* 当GPS数据不可用，发送错误给LVGL事件回调函数并退出函数*/
    if (gps_gpx->valid == false)
    {
        ESP_LOGE(TAG, "valid error !!");
        lv_event_send(ui_statusBtn, LV_EVENT_KEY, 0);
        return;
    }
    /* 初始化文件名 */
    char filepath[50];
    /* 以时间为文件名 */
    snprintf(filepath, sizeof(filepath), FILE_PATH "/%d%02d%02d_%02d%02d.gpx", gps_gpx->date.year + YEAR_BASE, gps_gpx->date.month, gps_gpx->date.day,
             gps_gpx->tim.hour + TIME_ZONE, gps_gpx->tim.minute);
    /* 检测gpx存放的固定路径是否存在，不存在就创建路径 */
    createDirectoryIfNotExists(FILE_PATH);

    FILE *file_p = fopen(filepath, "w");
    /* 新建文件错误 则打印日志并且发送错误给LVGL事件回调函数 */
    if (file_p == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
        lv_event_send(ui_statusBtn, LV_EVENT_KEY, 0);
        return;
    }
    recorder.fileName = filepath;
    recorder.file_p = file_p;
    /* 新建文件成功并写入字符 */
    ESP_LOGI(TAG, "Track file %s open success", filepath);
    setMetaName(RECORDER_GPX_META_NAME); // 软件版本
    setMetaDesc(RECORDER_GPX_META_DESC); // 产品描述
    setName(filepath);                   // 设置文件名并写入
    setDesc("");
    /* 写入设置好的固定xml格式 */
    Recorder_FileWriteString(recorder.file_p, getOpen());
    Recorder_FileWriteString(recorder.file_p, getMetaData());
    Recorder_FileWriteString(recorder.file_p, getTrakOpen());
    Recorder_FileWriteString(recorder.file_p, getInfo());
    Recorder_FileWriteString(recorder.file_p, getTrakSegOpen());
    recorder.active = true;
}
void rec_point(gps_t *gps_gpx)
{
    /* 文件指针不存在时，发送错误给LVGL事件回调函数  */
    if (recorder.file_p == NULL)
    {
        ESP_LOGE(TAG, "rec_point active error!");
        lv_event_send(ui_statusBtn, LV_EVENT_KEY, 0);
        return;
    }
    if (gps_gpx->valid == false)
    {
        ESP_LOGI(TAG, "vaild is false...");
        return;
    }
    /* 打印记录轨迹点 */
    ESP_LOGI(TAG, "Track recording...");
    char timeBuf[64];
    /* 将时间信息保存到字符串 */
    snprintf(
        timeBuf, sizeof(timeBuf), "%d-%02d-%02dT%02d:%02d:%02dZ", gps_gpx->date.year + YEAR_BASE, gps_gpx->date.month, gps_gpx->date.day,
        gps_gpx->tim.hour + TIME_ZONE, gps_gpx->tim.minute, gps_gpx->tim.second);
    ESP_LOGI(TAG, "%s", timeBuf);
    /* 将海拔信息保存到字符串 */
    char ele[10];
    snprintf(ele, sizeof(ele), "%.02f", gps_gpx->altitude);
    setEle(ele);
    setTime(timeBuf);
    /* 将经纬度信息保存到字符串 */
    char lon[15];
    snprintf(lon, sizeof(lon), "%.06f", gps_gpx->longitude);
    char lat[15];
    snprintf(lat, sizeof(lat), "%.06f", gps_gpx->latitude);
    /* 将设置好的字符串信息保存到文件中 */
    Recorder_FileWriteString(recorder.file_p, getPt(GPX_TRKPT, lon, lat));
}

void rec_stop()
{
    /* 文件指针不存在时，发送错误给LVGL事件回调函数 */
    if (recorder.file_p == NULL)
    {
        ESP_LOGE(TAG, "rec_stop file_p == NULL");
        lv_event_send(ui_statusBtn, LV_EVENT_KEY, 0);
        return;
    }
    ESP_LOGI(TAG, "Track record end");
    /* 写入文件结束的固定字符到文件 */
    Recorder_FileWriteString(recorder.file_p, getTrakSegClose());
    Recorder_FileWriteString(recorder.file_p, getTrakClose());
    Recorder_FileWriteString(recorder.file_p, getClose());
    /* 关闭文件 */
    fclose(recorder.file_p);
    /* 关闭记录状态 */
    recorder.active = false;
    ESP_LOGI(TAG, "file closed");
}

void rec_event_handle(void *data)
{
    gps_t *gps_data = (gps_t *)data;
    switch (recorder.recInfo.cmd)
    {
    case RECORDER_CMD_NONE:
        /* 空闲状态 */
        ESP_LOGD(TAG, "recorder.active = %d", recorder.active);
        break;
    case RECORDER_CMD_START:
        if (recorder.active == false)
            rec_start(gps_data);
        else if (recorder.active == true && recorder.recInfo.time % 2 == 0)
            rec_point(gps_data);
        break;
    case RECORDER_CMD_PAUSE:

        break;
    case RECORDER_CMD_CONTINUE:
        if (recorder.active == true && recorder.recInfo.time % 2 == 0)
            rec_point(gps_data);
        break;
    case RECORDER_CMD_STOP:
        rec_stop();
        rec_init();
        recorder.recInfo.cmd = RECORDER_CMD_NONE;
        break;
    default:
        break;
    }
}