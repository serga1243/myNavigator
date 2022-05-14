#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utilitiesFuncs.h"
#include "getGCS.h"
#include "getXOR.h"
#include "findCommas.h"
#include "chooseId.h"
#include "medianFilter.h"
#include "kalmanFilter.h"
#include "minQuadFilter.h"
#include "changeMsg.h"
#include "overwritePrevPos.h"
#include "char2int.h"

#define Cpp
#define msgMaxLen 1024
#define PreviosPosLen 3
#define logInfoArrLen 16
#define medianFilteringBufferLength 5

#define includeLat
#define includeLon
#define includeAlt

// Параметры фильтра Калмана
#define standartkalmanFilter
#define kalmanFiltering

//// Параметры медианного фильтра
//#define medianFiltering

//// Параметры квадратичного фильтра
//#define minQuadFiltering

#ifdef Cpp
const double startCoordinates[] = { 5010.88431, 12805.33476, 100.0 };
const double latIntPartLimits[] = { -9000.00000, 9000.00000 };
const double lonIntPartLimits[] = { -18000.00000, 18000.00000 };
const double altIntPartLimits[] = { 0, 99999.99 };
#endif


// Буффер хранения предыдущих нефильтрованных значений координаты
typedef struct PreviosPos
{
	double value[PreviosPosLen];
} PreviosPos;

// Декодированная координата из сообщения
typedef struct DecodedPos
{
	double value;
	unsigned short intPosition;
	unsigned short floatPosition;
	unsigned short length;
	unsigned short intLength;
	unsigned short floatLength;
} DecodedPos;

// Фильтрованная координата из сообщения
typedef struct FilteredPos
{
	double value;
	unsigned short intPosition;
	unsigned short floatPosition;
	unsigned short length;
	unsigned short intLength;
	unsigned short floatLength;
} FilteredPos;

// Один тип координат
typedef struct Coordinate
{
	PreviosPos previosPos;
	DecodedPos decodedPos;
	FilteredPos filteredPos;
} Coordinate;

// Все координаты
typedef struct Coordinates
{
	Coordinate lat;
	Coordinate lon;
	Coordinate alt;
} Coordinates;

// Сообщения
typedef struct Msg
{
	char msg[msgMaxLen];
	unsigned short len;
} Msg;

//
typedef struct MsgData
{
	char checkData[2];
	char checkDataCond[2];
	char countryData[2];
	char idData[3];
	unsigned short parId[msgMaxLen];
	unsigned short parIdLen;
	unsigned short id[6];
} MsgData;

//
typedef struct MinQuadFilterCoordinate
{
	double sumX = 0;
	double sumY = 0;
	double sumX2 = 0;
	double sumXY = 0;
	double a = 0;
	double b = 0;
} MinQuadFilterCoordinate;

//
typedef struct MinQuadFilter
{
	MinQuadFilterCoordinate lat;
	MinQuadFilterCoordinate lon;
	MinQuadFilterCoordinate alt;
} MinQuadFilter;

//
typedef struct MedianFilterCoordinate
{
	double buffer[medianFilteringBufferLength * medianFilteringBufferLength];
	double normFunc[medianFilteringBufferLength - 2];
	double normFuncSum;
	unsigned short bufferCntrs[medianFilteringBufferLength - 2];
	unsigned int bufferLens[medianFilteringBufferLength - 2];
	unsigned int filterCntrs[medianFilteringBufferLength - 2];
} MedianFilterCoordinate;

//
typedef struct MedianFilter
{
	MedianFilterCoordinate lat;
	MedianFilterCoordinate lon;
	MedianFilterCoordinate alt;
} MedianFilter;



typedef struct KalmanFilterCoordinate
{
	// Простой фильтр Калмана:
	double _err_measure;
	double _err_estimate;
	double _q;
	double _last_estimate;

	// Нормальный фильтр Калмана
	double R;
	double x_est[3];
	double p_est[9];
} KalmanFilterCoordinate;

typedef struct KalmanFilter
{
	KalmanFilterCoordinate lat;
	KalmanFilterCoordinate lon;
	KalmanFilterCoordinate alt;
} KalmanFilter;


//
typedef struct Filters
{
	MinQuadFilter minQuadFilter;
	MedianFilter medianFilter;
	KalmanFilter kalmanFilter;
} Filters;

//
typedef struct MyNavigator
{
	Msg msgIn;
	Msg msgOut;
	Filters filters;
	Coordinates coordinates;
	MsgData msgData;
	unsigned char logInfo[logInfoArrLen];
} MyNavigator;


void myNavigatorInit(struct MyNavigator*);
void myNavigator(struct MyNavigator*);