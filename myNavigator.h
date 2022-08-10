#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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
#include "char2double.h"
#include "writeInROM.h"
#include "geo2decart.h"
#include "decart2geo.h"
#include "prePostFilter.h"

// Если язык c++, а не си
#define Cpp								

// Релиз с одним типом фильтра
// или отладка со всеми фильтрами :
#define myNavigator_RELEASE
//#define myNavigator_DEBUG

#define startLat 5010.88431
#define startLon 12805.33476
#define startAlt 100.0


#define TransformCoords					// если необходимо преобразовывать гео координаты в декартовы
#define WriteCoordsInFlash				// записывать ли координаты во флеш память?
#define msgMaxLen 1024					// максимальная длина сообщений
#define previosPosLen 20				// число хранимых предыдущих значений
#define logInfoArrLen 1					// длина массива логов

// Какие координаты фильтровать? :
#define includeLat
#define includeLon
#define includeAlt

// Параметры фильтра Калмана :
//#define kalmanFiltering

// Параметры медианного фильтра :
#define medianFiltering

// Параметры квадратичного фильтра :
//#define minQuadFiltering 

// Параметры альфа-бета фильтра :
//#define alphaBetaFiltering

// Начальный адресс во флеш-памяти для записи данных
#define WriteInFlashStartAdress 0x08010000

// Конечный адресс во флеш-памяти для записи данных
#define WriteInFlashEndAdress 0x080DFF9C

// Максимальная скорость объекта в м/с :
#define objectMaxSpeed 8000.0

// Параметры медианного фильтра :
#define medianFilteringBufferLength 6

#ifdef myNavigator_RELEASE

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
	uint16_t intPosition;
	uint16_t floatPosition;
	uint16_t length;
	uint16_t intLength;
	uint16_t floatLength;
} DecodedPos;

// Фильтрованная координата из сообщения :
typedef struct FilteredPos
{
	double value;
	uint16_t intLength;
	uint16_t floatLength;
	// Если тестируем все фильтры одновременно :
#ifdef myNavigator_DEBUG
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
#ifdef includeLat
	Coordinate lat;
#endif

#ifdef includeLon
	Coordinate lon;
#endif

#ifdef includeAlt
	Coordinate alt;
#endif
} Coordinates;


// ##########################################################################################
// 
// ---------------------------------  Сообщения с приемника  --------------------------------
// 
// ##########################################################################################


// Сообщения :
typedef struct Msg
{
	uint8_t msg[msgMaxLen];
	uint16_t len;
} Msg;

// Хранение :
typedef struct MsgData
{
	double dt;
	double dT;
	uint8_t checkData[2];
	uint8_t checkDataCond[2];
	uint8_t countryData[2];
	uint8_t idData[3];
	uint16_t parId[msgMaxLen];
	uint16_t parIdLen;
	uint16_t id[6];
} MsgData;


// ##########################################################################################
// 
// ---------------------------------- Указатели на функции ----------------------------------
// 
// ##########################################################################################

// Хранилище указателей на функции :
typedef struct FuncsPointrs
{
	void (*flashLockFunc)(void);
	void (*LEDaction)(uint8_t);
	void (*flashFunc)(uint32_t, uint32_t, uint64_t);
} FuncsPointrs;


// ##########################################################################################
// 
// -------------------------------------  Запись во флеш ------------------------------------
// 
// ##########################################################################################

// Данные для возможности записи во флеш :
typedef struct WriteInFlash
{
	uint32_t adress;
	bool isUnlockedFlash;
} WriteInFlash;


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
	uint16_t bufferCntrs[medianFilteringBufferLength - 2];
	int32_t bufferLens[medianFilteringBufferLength - 2];
	int32_t filterCntrs[medianFilteringBufferLength - 2];
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
	uint32_t Step;
	double Tob;
	double Predel;
	double Kb;
	double Ka;
	double Kg;
	double Az;
	double Vz;
	double fZ;
	double Sp;
	double Sn;
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
	Filters filters;
	Coordinates coordinates;
	MsgData msgData;
	FuncsPointrs funcsPointrs;

#ifdef WriteCoordsInFlash
	WriteInFlash writeInFlash;
#endif

	uint8_t logInfo[logInfoArrLen];
	uint16_t myNavigatorExeTime;
} MyNavigator;


// Прототипы функции :
void myNavigatorInit(struct MyNavigator*, struct FuncsPointrs);
void myNavigator(struct MyNavigator*);
