#pragma once

#include <math.h>
#include <stdio.h>
#include <stdint.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define X_PI (M_PI * 3000.0 / 180.0)
#define A (6378245.0)               // #长半轴
#define EE (0.00669342162296594323) // #偏心率平方

void PixelXYToTileXY(uint32_t pixelX, uint32_t pixelY, int screenWidth, int screenHeigh, uint32_t *tileX, uint32_t *tileY,
                     uint8_t *tileSubX, uint8_t *tileSubY, uint8_t *pSubX, uint8_t *pSubY);
void LatLongToPixelXY(float latitude, float longitude, uint8_t levelOfDetail, uint32_t *pixelX, uint32_t *pixelY);
void wgs84_to_gcj02(float *lng, float *lat);
double getDistance(double lon1, double lat1, double lon2, double lat2);
double calculateMean(int16_t data[], int n);
double calculateVariance(int16_t data[], int n);