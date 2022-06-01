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

// ����� � ����� ����� �������
// ��� ������� �� ����� ��������� :
#define RELEASE
//#define DEBUG

#define TransformCoords					// ���� ���������� ��������������� ��� ���������� � ���������
#define WriteCoordsInFlash				// ���������� �� ���������� �� ���� ������?
#define Cpp								// ���� ���� c++, � �� ��
#define msgMaxLen 1024					// ������������ ����� ���������
#define previosPosLen 20				// ����� �������� ���������� ��������
#define logInfoArrLen 1					// ����� ������� �����

// ����� ���������� �����������? :
#define includeLat
#define includeLon
#define includeAlt

// ��������� ������� ������� :
#define kalmanFiltering

// ��������� ���������� ������� :
//#define medianFiltering

// ��������� ������������� ������� :
//#define minQuadFiltering 

// ��������� �����-���� ������� :
//#define alphaBetaFiltering

// ##########################################################################################
// 
// -----------------------------------  ��������� �������  ----------------------------------
// 
// ##########################################################################################

#ifdef Cpp

// ��������� ���������� :
#ifdef TransformCoords
const double startCoordinates[] = { 5561520.0722118802, 22434924.431076400, 100.0 };
#else
const double startCoordinates[] = { 5010.88431, 12805.33476, 100.0 };
#endif

// ������� �������� ������������� ��������� :
#ifdef TransformCoords
const double latIntPartLimits[] = { -100000000.0, 100000000.0 };
const double lonIntPartLimits[] = { -100000000.0, 100000000.0 };
#else
const double latIntPartLimits[] = { -9000.00000, 9000.00000 };
const double lonIntPartLimits[] = { -18000.00000, 18000.00000 };
#endif
const double altIntPartLimits[] = { 0.0, 99999.99 };

// ��������� ���������� ������� :
const unsigned int medianFilteringBufferLength = 5;

// ��������� ������� ������� :
const double kalmanFilterR[] = { 50000.0, 50000.0 , 50000.0 };

// ��������� �����-���� ������� :
const double alphaBetaFilterT[] = { 0.1, 0.1 , 0.1 };
const double alphaBetaFilterSp[] = { 0.1, 0.1, 0.1 };
const double alphaBetaFilterSn[] = { 3.0, 3.0, 3.0 };

#endif

#ifdef RELEASE

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
	unsigned short intPosition;
	unsigned short floatPosition;
	unsigned short length;
	unsigned short intLength;
	unsigned short floatLength;
} DecodedPos;

// ������������� ���������� �� ��������� :
typedef struct FilteredPos
{
	double value;
	unsigned short intLength;
	unsigned short floatLength;
	// ���� ��������� ��� ������� ������������ :
#ifdef DEBUG
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
	Coordinate lat;
	Coordinate lon;
	Coordinate alt;
} Coordinates;


// ##########################################################################################
// 
// ---------------------------------  ��������� � ���������  --------------------------------
// 
// ##########################################################################################


// ��������� :
typedef struct Msg
{
	char msg[msgMaxLen];
	unsigned short len;
} Msg;

// �������� :
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
	unsigned short bufferCntrs[medianFilteringBufferLength - 2];
	unsigned int bufferLens[medianFilteringBufferLength - 2];
	unsigned int filterCntrs[medianFilteringBufferLength - 2];
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
	Msg msgOut;
	Filters filters;
	Coordinates coordinates;
	MsgData msgData;
	unsigned char logInfo[logInfoArrLen];
} MyNavigator;


// ��������� ������� :
void myNavigatorInit(struct MyNavigator*);
void myNavigator(struct MyNavigator*);
inline void isInvalidData(double*, const double[], double*);