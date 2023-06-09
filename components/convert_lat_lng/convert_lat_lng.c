#include "convert_lat_lng.h"

#define CONSTRAIN(value, minValue, maxValue) ((value < minValue) ? (minValue) : ((value > maxValue) ? (maxValue) : (value)))

unsigned long mKeyPath;
unsigned long mKeyName;
uint8_t mKeyExt;
uint8_t pixelSubX = 0, pixelSubY = 0;
const unsigned long EarthRadius = 6378137;
const double MinLatitude = -85.05112878;
const double MaxLatitude = 85.05112878;
const double MinLongitude = -180.0;
const double MaxLongitude = 180.0;
const uint8_t MaxLevel = 14;
const uint8_t MinLevel = 2;

// 计算经度（longitude）对应的瓦片坐标 X
double calculateTileX(double longitude, int zoom)
{
    return ((longitude + 180.0) / 360.0 * pow(2, zoom));
}

// 计算纬度（latitude）对应的瓦片坐标 Y
int calculateTileY(double latitude, int zoom)
{
    double latRad = latitude * M_PI / 180.0;
    return ((1.0 - log(tan(latRad) + 1.0 / cos(latRad)) / M_PI) / 2.0 * pow(2, zoom));
}

static double Clip(double n, double minValue, double maxValue)
{
    return CONSTRAIN(n, minValue, maxValue);
}

static unsigned long ClipULONG(unsigned long n, unsigned long minValue, unsigned long maxValue)
{
    return CONSTRAIN(n, minValue, maxValue);
}

static unsigned long MapSize(int levelOfDetail)
{
    unsigned long seed = 256;
    return seed << levelOfDetail;
}

float transformlng(float lng, float lat)
{
    float ret = 300.0 + lng + 2.0 * lat + 0.1 * lng * lng + 0.1 * lng * lat + 0.1 * sqrt(fabs(lng));
    ret += (20.0 * sin(6.0 * lng * M_PI) + 20.0 * sin(2.0 * lng * M_PI)) * 2.0 / 3.0;
    ret += (20.0 * sin(lng * M_PI) + 40.0 * sin(lng / 3.0 * M_PI)) * 2.0 / 3.0;
    ret += (150.0 * sin(lng / 12.0 * M_PI) + 300.0 * sin(lng / 30.0 * M_PI)) * 2.0 / 3.0;
    return ret;
}

float transformlat(float lng, float lat)
{
    float ret = -100.0 + 2.0 * lng + 3.0 * lat + 0.2 * lat * lat + 0.1 * lng * lat + 0.2 * sqrt(fabs(lng));
    ret += (20.0 * sin(6.0 * lng * M_PI) + 20.0 * sin(2.0 * lng * M_PI)) * 2.0 / 3.0;
    ret += (20.0 * sin(lat * M_PI) + 40.0 * sin(lat / 3.0 * M_PI)) * 2.0 / 3.0;
    ret += (160.0 * sin(lat / 12.0 * M_PI) + 320.0 * sin(lat * M_PI / 30.0)) * 2.0 / 3.0;
    return ret;
}
void wgs84_to_gcj02(float *lng, float *lat)
{
    float dlat, dlng, magic, sqrtmagic, radlat;
    dlat = transformlat(*lng - 105.0, *lat - 35.0);
    dlng = transformlng(*lng - 105.0, *lat - 35.0);
    radlat = *lat / 180.0 * M_PI;
    magic = sin(radlat);
    magic = 1 - EE * magic * magic;
    sqrtmagic = sqrt(magic);
    dlat = (dlat * 180.0) / ((A * (1 - EE)) / (magic * sqrtmagic) * M_PI);
    dlng = (dlng * 180.0) / (A / sqrtmagic * cos(radlat) * M_PI);
    *lat = *lat + dlat;
    *lng = *lng + dlng;
}
void LatLongToPixelXY(float latitude, float longitude, uint8_t levelOfDetail, uint32_t *pixelX, uint32_t *pixelY)
{
    /* 限制经纬度范围 */
    latitude = Clip(latitude, MinLatitude, MaxLatitude);  
    longitude = Clip(longitude, MinLongitude, MaxLongitude);

    float x = (longitude + 180.0) / 360.0;
    float sinLatitude = sin(latitude * M_PI / 180.0);
    float y = 0.5 - log((1.0 + sinLatitude) / (1.0 - sinLatitude)) / (4.0 * M_PI);

    uint32_t mapSize = MapSize(levelOfDetail);
    *pixelX = ClipULONG(x * mapSize + 0.5, 0, mapSize - 1);
    *pixelY = ClipULONG(y * mapSize + 0.5, 0, mapSize - 1);
}
void PixelXYToTileXY(uint32_t pixelX, uint32_t pixelY, int screenWidth, int screenHeigh, uint32_t *tileX, uint32_t *tileY,
                     uint8_t *tileSubX, uint8_t *tileSubY, uint8_t *pSubX, uint8_t *pSubY)
{
    *tileX = pixelX / 256;
    *tileY = pixelY / 256;
    *tileSubX = pixelX % 256 / screenWidth;
    *tileSubY = pixelY % 256 / screenHeigh;
    *pSubX = pixelX % 256 % screenWidth;
    *pSubY = pixelY % 256 % screenHeigh;
}

/* 默认地球半径,赤道半径(单位m) */
#define EARTH_RADIUS1 6371000

/**
 * 转化为弧度(rad)
 */
static double rad(double d)
{
    return (d * M_PI / 180.0);
}
/**
 * @param lon1 第一点的精度
 * @param lat1 第一点的纬度
 * @param lon2 第二点的精度
 * @param lat2 第二点的纬度
 * @return 返回的距离，单位m
 * */

double getDistance(double lon1, double lat1, double lon2, double lat2)
{
    double radLat1 = rad(lat1);
    double radLat2 = rad(lat2);
    double a = radLat1 - radLat2;
    double b = rad(lon1) - rad(lon2);
    double s = 2 * asin(sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2)));
    s = s * EARTH_RADIUS1;
    s = round(s * 10000) / 10000;
    return s;
}

double calculateMean(int16_t data[], int n)
{
    double sum = 0.0;
    for (int i = 0; i < n; i++)
    {
        sum += data[i];
    }
    return (sum / n);
}

double calculateVariance(int16_t data[], int n)
{
    double mean = calculateMean(data, n);
    double sumOfSquaredDiff = 0.0;

    for (int i = 0; i < n; i++)
    {
        double diff = data[i] - mean;
        sumOfSquaredDiff += diff * diff;
    }

    return (sumOfSquaredDiff / n);
}
