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

static void PixelXYToTileXY(unsigned long pixelX, unsigned long pixelY, int screenWidth, int screenHeigh, unsigned long *tileX, unsigned long *tileY,
                            uint8_t *tileSubX, uint8_t *tileSubY, uint8_t *pSubX, uint8_t *pSubY);
static void LatLongToPixelXY(double latitude, double longitude, int levelOfDetail, unsigned long *pixelX, unsigned long *pixelY);
void wgs84_to_gcj02(double *lng, double *lat);
static double getDistance(double lon1, double lat1, double lon2, double lat2);