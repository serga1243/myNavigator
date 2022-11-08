#pragma once

#include "myIncs.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "myNavigatorConfig.h"
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



#define GPS_A 						6378137.0
#define GPS_ALPHA					0.003352810664747480500

#define GLONASS_A 					6378136.5
#define GLONASS_ALPHA				0.003352819752979053200

#define GALILEO_A 					6378137.0
#define GALILEO_ALPHA				0.003352810664747480500

#define BEIDOU_A 					6378137.0
#define BEIDOU_ALPHA				0.003352810681182318800

enum GNSSs
{
	GPS		 	= 0,
	GLONASS		= 1,
	GALILEO		= 2,
	BEIDOU		= 3,
};

// ##########################################################################################
// 
// --------------------------------------  ����������  --------------------------------------
// 
// ##########################################################################################


// ������ �������� ���������� ��������������� �������� ���������� :
typedef struct PreviosPos
{
	double value[PREVIOS_POS_LEN];
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
#ifdef MYNAVIGATOR_DEBUG
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

#ifdef INCLUDE_VEL
typedef struct Velocity
{
	DecodedPos decodedPos;
} Velocity;
#endif

// ��� ���������� :
typedef struct Coordinates
{
#ifdef INCLUDE_LAT
	Coordinate lat;
#endif

#ifdef INCLUDE_LON
	Coordinate lon;
#endif

#ifdef INCLUDE_ALT
	Coordinate alt;
#endif

#ifdef INCLUDE_VEL
	Velocity   vel;
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
	uint8_t msg[MSG_MAX_LEN];
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
	uint16_t parId[MSG_MAX_LEN];
	uint16_t parIdLen;
	uint16_t id[7];
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
	double buffer[MEDIAN_FILTERING_BUFFER_LENGTH * MEDIAN_FILTERING_BUFFER_LENGTH];
	double normFunc[MEDIAN_FILTERING_BUFFER_LENGTH - 2];
	double normFuncSum;
	uint16_t bufferCntrs[MEDIAN_FILTERING_BUFFER_LENGTH - 2];
	int32_t bufferLens[MEDIAN_FILTERING_BUFFER_LENGTH - 2];
	int32_t filterCntrs[MEDIAN_FILTERING_BUFFER_LENGTH - 2];
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

#ifdef WRITE_COORDS_IN_FLASH
	WriteInFlash writeInFlash;
#endif

	uint8_t logInfo[LOG_INFO_ARR_LEN];
	uint16_t myNavigatorExeTime;
	volatile uint32_t filterMainCntr;
} MyNavigator;


// ��������� ������� :
void myNavigator(struct MyNavigator*);
void myNavigatorInit(struct MyNavigator*, struct FuncsPointrs*);
