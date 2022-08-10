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

// ���� ���� c++, � �� ��
#define Cpp								

// ����� � ����� ����� �������
// ��� ������� �� ����� ��������� :
#define myNavigator_RELEASE
//#define myNavigator_DEBUG

#define startLat 5010.88431
#define startLon 12805.33476
#define startAlt 100.0


#define TransformCoords					// ���� ���������� ��������������� ��� ���������� � ���������
#define WriteCoordsInFlash				// ���������� �� ���������� �� ���� ������?
#define msgMaxLen 1024					// ������������ ����� ���������
#define previosPosLen 20				// ����� �������� ���������� ��������
#define logInfoArrLen 1					// ����� ������� �����

// ����� ���������� �����������? :
#define includeLat
#define includeLon
#define includeAlt

// ��������� ������� ������� :
//#define kalmanFiltering

// ��������� ���������� ������� :
#define medianFiltering

// ��������� ������������� ������� :
//#define minQuadFiltering 

// ��������� �����-���� ������� :
//#define alphaBetaFiltering

// ��������� ������ �� ����-������ ��� ������ ������
#define WriteInFlashStartAdress 0x08010000

// �������� ������ �� ����-������ ��� ������ ������
#define WriteInFlashEndAdress 0x080DFF9C

// ������������ �������� ������� � �/� :
#define objectMaxSpeed 8000.0

// ��������� ���������� ������� :
#define medianFilteringBufferLength 6

#ifdef myNavigator_RELEASE

#undef WriteCoordsInFlash

#endif




// ##########################################################################################
// 
// --------------------------------------  ����������  --------------------------------------
// 
// ##########################################################################################


// ������ �������� ���������� ��������������� �������� ���������� :
typedef struct PreviosPos
{
	double value[previosPosLen];
} PreviosPos;

// �������������� ���������� �� ��������� :
typedef struct DecodedPos
{
	double value;
	uint16_t intPosition;
	uint16_t floatPosition;
	uint16_t length;
	uint16_t intLength;
	uint16_t floatLength;
} DecodedPos;

// ������������� ���������� �� ��������� :
typedef struct FilteredPos
{
	double value;
	uint16_t intLength;
	uint16_t floatLength;
	// ���� ��������� ��� ������� ������������ :
#ifdef myNavigator_DEBUG
	double allFilteredValues[4];
#endif
} FilteredPos;

// ���� ��� ��������� :
typedef struct Coordinate
{
	PreviosPos previosPos;
	DecodedPos decodedPos;
	FilteredPos filteredPos;
} Coordinate;

// ��� ���������� :
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
// ---------------------------------  ��������� � ���������  --------------------------------
// 
// ##########################################################################################


// ��������� :
typedef struct Msg
{
	uint8_t msg[msgMaxLen];
	uint16_t len;
} Msg;

// �������� :
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
// ---------------------------------- ��������� �� ������� ----------------------------------
// 
// ##########################################################################################

// ��������� ���������� �� ������� :
typedef struct FuncsPointrs
{
	void (*flashLockFunc)(void);
	void (*LEDaction)(uint8_t);
	void (*flashFunc)(uint32_t, uint32_t, uint64_t);
} FuncsPointrs;


// ##########################################################################################
// 
// -------------------------------------  ������ �� ���� ------------------------------------
// 
// ##########################################################################################

// ������ ��� ����������� ������ �� ���� :
typedef struct WriteInFlash
{
	uint32_t adress;
	bool isUnlockedFlash;
} WriteInFlash;


// ##########################################################################################
// 
// ----------------------------------------  �������  ---------------------------------------
// 
// ##########################################################################################


// �������������� ������ ��� 1 ���������� :
typedef struct MinQuadFilterCoordinate
{
	double sumX;
	double sumY;
	double sumX2;
	double sumXY;
	double a;
	double b;
} MinQuadFilterCoordinate;

// �������������� ������ :
typedef struct MinQuadFilter
{
	MinQuadFilterCoordinate lat;
	MinQuadFilterCoordinate lon;
	MinQuadFilterCoordinate alt;
} MinQuadFilter;

// ��������� ������ ��� 1 ���������� :
typedef struct MedianFilterCoordinate
{
	double buffer[medianFilteringBufferLength * medianFilteringBufferLength];
	double normFunc[medianFilteringBufferLength - 2];
	double normFuncSum;
	uint16_t bufferCntrs[medianFilteringBufferLength - 2];
	int32_t bufferLens[medianFilteringBufferLength - 2];
	int32_t filterCntrs[medianFilteringBufferLength - 2];
} MedianFilterCoordinate;

// ��������� ������ :
typedef struct MedianFilter
{
	MedianFilterCoordinate lat;
	MedianFilterCoordinate lon;
	MedianFilterCoordinate alt;
} MedianFilter;

// ������ ������� ��� 1 ���������� :
typedef struct KalmanFilterCoordinate
{
	double R;
	double x_est[3];
	double p_est[9];
} KalmanFilterCoordinate;

// ������ ������� :
typedef struct KalmanFilter
{
	KalmanFilterCoordinate lat;
	KalmanFilterCoordinate lon;
	KalmanFilterCoordinate alt;
} KalmanFilter;

// �����-���� ������ ��� 1 ���������� :
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

// �����-���� ������ :
typedef struct AlphaBetaFilter
{
	AlphaBetaFilterCoordinate lat;
	AlphaBetaFilterCoordinate lon;
	AlphaBetaFilterCoordinate alt;
} AlphaBetaFilter;

// ��� ������� � 1 ����� :
typedef struct Filters
{
	AlphaBetaFilter alphaBetaFilter;
	MinQuadFilter minQuadFilter;
	MedianFilter medianFilter;
	KalmanFilter kalmanFilter;
} Filters;


// ##########################################################################################
// 
// ------------------------------------  ����� ���������  -----------------------------------
// 
// ##########################################################################################


// ��� ������ � 1 ����� :
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


// ��������� ������� :
void myNavigatorInit(struct MyNavigator*, struct FuncsPointrs);
void myNavigator(struct MyNavigator*);
