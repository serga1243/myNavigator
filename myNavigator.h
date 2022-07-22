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


// ����� � ����� ����� �������
// ��� ������� �� ����� ��������� :
#define myNavigator_RELEASE
//#define myNavigator_DEBUG

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
//#define kalmanFiltering

// ��������� ���������� ������� :
#define medianFiltering

// ��������� ������������� ������� :
//#define minQuadFiltering 

// ��������� �����-���� ������� :
//#define alphaBetaFiltering

// ��������� ���������� ������� :
#define medianFilteringBufferLength 6

// ������������ �������� ������� :
#define objectMaxSpeed 8000.0

#ifdef myNavigator_RELEASE

#undef WriteCoordsInFlash

#endif

#ifdef WriteCoordsInFlash

// ��������� ������ �� ����-������ ��� ������ ������
#define WriteInFlashStartAdress 0x08010000

// �������� ������ �� ����-������ ��� ������ ������
#define WriteInFlashEndAdress 0x080DFF9C

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
	double dt;
	double dT;
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
	unsigned short bufferCntrs[medianFilteringBufferLength - 2];
	int bufferLens[medianFilteringBufferLength - 2];
	int filterCntrs[medianFilteringBufferLength - 2];
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

	unsigned char logInfo[logInfoArrLen];
	unsigned short myNavigatorExeTime;
} MyNavigator;


// ��������� ������� :
void myNavigatorInit(struct MyNavigator*, struct FuncsPointrs);
void myNavigator(struct MyNavigator*);
