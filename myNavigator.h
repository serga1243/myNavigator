#pragma once

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "utilitiesFuncs.h"
#include "getGCS.h"
#include "getXOR.h"
#include "findCommas.h"
#include "chooseId.h"
#include "medianFilter.h"
#include "kalmanFilter.h"
#include "minQuadFilter.h"
#include "alphaBetaFilter.h"
#include "changeMsg.h"
#include "overwritePrevPos.h"
#include "char2int.h"
#include "writeInROM.h"
#include "geo2decart.h"
#include "decart2geo.h"

// Релиз с одним типом фильтра
// или отладка со всеми фильтрами :
#define RELEASE
//#define DEBUG

#define TransformCoords					// если необходимо преобразовывать гео координаты в декартовы
#define WriteCoordsInFlash				// записывать ли координаты во флеш память?
#define Cpp								// если язык c++, а не си
#define msgMaxLen 1024					// максимальная длина сообщений
#define previosPosLen 20				// число хранимых предыдущих значений
#define logInfoArrLen 1					// длина массива логов

// Какие координаты фильтровать? :
#define includeLat
#define includeLon
#define includeAlt

// Параметры фильтра Калмана :
#define kalmanFiltering

// Параметры медианного фильтра :
//#define medianFiltering

// Параметры квадратичного фильтра :
//#define minQuadFiltering 

// Параметры альфа-бета фильтра :
//#define alphaBetaFiltering

// ##########################################################################################
// 
// -----------------------------------  Начальные условия  ----------------------------------
// 
// ##########################################################################################

#ifdef Cpp

// Начальные координаты :
#ifdef TransformCoords
const double startCoordinates[] = { 5561520.0722118802, 22434924.431076400, 100.0 };
#else
const double startCoordinates[] = { 5010.88431, 12805.33476, 100.0 };
#endif

// Пределы значений геодезических координат :
#ifdef TransformCoords
const double latIntPartLimits[] = { -100000000.0, 100000000.0 };
const double lonIntPartLimits[] = { -100000000.0, 100000000.0 };
#else
const double latIntPartLimits[] = { -9000.00000, 9000.00000 };
const double lonIntPartLimits[] = { -18000.00000, 18000.00000 };
#endif
const double altIntPartLimits[] = { 0.0, 99999.99 };

// Параметры медианного фильтра :
const unsigned int medianFilteringBufferLength = 5;

// Параметры фильтра Калмана :
const double kalmanFilterR[] = { 50000.0, 50000.0 , 50000.0 };

// Параметры альфа-бета фильтра :
const double alphaBetaFilterT[] = { 0.1, 0.1 , 0.1 };
const double alphaBetaFilterSp[] = { 0.1, 0.1, 0.1 };
const double alphaBetaFilterSn[] = { 3.0, 3.0, 3.0 };

#endif

#ifdef RELEASE

#undef WriteCoordsInFlash

#endif


// ##########################################################################################
// 
// --------------------------------------  Координаты  --------------------------------------
// 
// ##########################################################################################


// Буффер хранения предыдущих нефильтрованных значений координаты :
typedef struct PreviosPos
{
	double value[previosPosLen];
} PreviosPos;

// Декодированная координата из сообщения :
typedef struct DecodedPos
{
	double value;
	unsigned short intPosition;
	unsigned short floatPosition;
	unsigned short length;
	unsigned short intLength;
	unsigned short floatLength;
} DecodedPos;

// Фильтрованная координата из сообщения :
typedef struct FilteredPos
{
	double value;
	unsigned short intLength;
	unsigned short floatLength;
	// Если тестируем все фильтры одновременно :
#ifdef DEBUG
	double allFilteredValues[4];
#endif
} FilteredPos;

// Один тип координат :
typedef struct Coordinate
{
	PreviosPos previosPos;
	DecodedPos decodedPos;
	FilteredPos filteredPos;
} Coordinate;

// Все координаты :
typedef struct Coordinates
{
	Coordinate lat;
	Coordinate lon;
	Coordinate alt;
} Coordinates;


// ##########################################################################################
// 
// ---------------------------------  Сообщения с приемника  --------------------------------
// 
// ##########################################################################################


// Сообщения :
typedef struct Msg
{
	char msg[msgMaxLen];
	unsigned short len;
} Msg;

// Хранение :
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


// ##########################################################################################
// 
// ----------------------------------------  Фильтры  ---------------------------------------
// 
// ##########################################################################################


// Статистический фильтр для 1 координаты :
typedef struct MinQuadFilterCoordinate
{
	double sumX;
	double sumY;
	double sumX2;
	double sumXY;
	double a;
	double b;
} MinQuadFilterCoordinate;

// Статистический фильтр :
typedef struct MinQuadFilter
{
	MinQuadFilterCoordinate lat;
	MinQuadFilterCoordinate lon;
	MinQuadFilterCoordinate alt;
} MinQuadFilter;

// Медианный фильтр для 1 координаты :
typedef struct MedianFilterCoordinate
{
	double buffer[medianFilteringBufferLength * medianFilteringBufferLength];
	double normFunc[medianFilteringBufferLength - 2];
	double normFuncSum;
	unsigned short bufferCntrs[medianFilteringBufferLength - 2];
	unsigned int bufferLens[medianFilteringBufferLength - 2];
	unsigned int filterCntrs[medianFilteringBufferLength - 2];
} MedianFilterCoordinate;

// Медианный фильтр :
typedef struct MedianFilter
{
	MedianFilterCoordinate lat;
	MedianFilterCoordinate lon;
	MedianFilterCoordinate alt;
} MedianFilter;

// Фильтр Калмана для 1 координаты :
typedef struct KalmanFilterCoordinate
{
	double R;
	double x_est[3];
	double p_est[9];
} KalmanFilterCoordinate;

// Фильтр Калмана :
typedef struct KalmanFilter
{
	KalmanFilterCoordinate lat;
	KalmanFilterCoordinate lon;
	KalmanFilterCoordinate alt;
} KalmanFilter;

// Альфа-бета фильтр для 1 координаты :
typedef struct AlphaBetaFilterCoordinate
{
	unsigned int Step;
	double Tob;
	double Predel;
	double Kb;
	double Ka;
	double Kg;
	double Az;
	double Vz;
	double fZ;
} AlphaBetaFilterCoordinate;

// Альфа-бета фильтр :
typedef struct AlphaBetaFilter
{
	AlphaBetaFilterCoordinate lat;
	AlphaBetaFilterCoordinate lon;
	AlphaBetaFilterCoordinate alt;
} AlphaBetaFilter;

// Все фильтры в 1 месте :
typedef struct Filters
{
	AlphaBetaFilter alphaBetaFilter;
	MinQuadFilter minQuadFilter;
	MedianFilter medianFilter;
	KalmanFilter kalmanFilter;
} Filters;


// ##########################################################################################
// 
// ------------------------------------  Общая структура  -----------------------------------
// 
// ##########################################################################################


// Все данные в 1 месте :
typedef struct MyNavigator
{
	Msg msgIn;
	Msg msgOut;
	Filters filters;
	Coordinates coordinates;
	MsgData msgData;
	unsigned char logInfo[logInfoArrLen];
} MyNavigator;


// Прототипы функции :
void myNavigatorInit(struct MyNavigator*);
void myNavigator(struct MyNavigator*);
inline void isInvalidData(double*, const double[], double*);