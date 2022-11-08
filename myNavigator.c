#include "myNavigator.h"



// ##########################################################################################
// 
// -----------------------------------  ��������� �������  ----------------------------------
// 
// ##########################################################################################

// ��������� ���������� :
double startCoordinates[] 		 = { startLat, startLon, startAlt };

// ������� �������� ������������� ��������� :
#ifdef TRANSFORM_COORDS
const double latIntPartLimits[]  = { -1000000000.0, 1000000000.0 };
const double lonIntPartLimits[]  = { -1000000000.0, 1000000000.0 };
#else
const double latIntPartLimits[]  = { -9000.00000, 9000.00000 };
const double lonIntPartLimits[]  = { -18000.00000, 18000.00000 };
#endif
const double altIntPartLimits[]  = { 0.0, 99999.99 };

// ��������� ������� ������� :
const double kalmanFilterR[] 	 = { 50000.0, 50000.0, 50000.0 };

// ��������� �����-���� ������� :
const double alphaBetaFilterT[]  = { 1.0, 1.0, 1.0 };
const double alphaBetaFilterSp[] = { 3.0, 3.0, 3.0 };
const double alphaBetaFilterSn[] = { 3.0, 3.0, 3.0 };


const double gnss_a[] 		 	 = {GPS_A, 	   GLONASS_A,     GALILEO_A,     BEIDOU_A};
const double gnss_alpha[] 	 	 = {GPS_ALPHA, GLONASS_ALPHA, GALILEO_ALPHA, BEIDOU_ALPHA};

const double gnss_e_pow2[]	 	 = {2.0 * gnss_alpha[GPS]     - gnss_alpha[GPS]     * gnss_alpha[GPS],
									2.0 * gnss_alpha[GLONASS] - gnss_alpha[GLONASS] * gnss_alpha[GLONASS],
									2.0 * gnss_alpha[GALILEO] - gnss_alpha[GALILEO] * gnss_alpha[GALILEO],
									2.0 * gnss_alpha[BEIDOU]  - gnss_alpha[BEIDOU]  * gnss_alpha[BEIDOU]};

const double gnss_e_pow2_1[]	 = {1.0 - gnss_e_pow2[GPS],
									1.0 - gnss_e_pow2[GLONASS],
									1.0 - gnss_e_pow2[GALILEO],
									1.0 - gnss_e_pow2[BEIDOU]};

const double gnss_a_e_pow2[]	 = {0.5 * gnss_a[GPS]     * gnss_e_pow2[GPS],
									0.5 * gnss_a[GLONASS] * gnss_e_pow2[GLONASS],
									0.5 * gnss_a[GALILEO] * gnss_e_pow2[GALILEO],
									0.5 * gnss_a[BEIDOU]  * gnss_e_pow2[BEIDOU]};

// ����������� ���������� :
static uint16_t i;



// ##########################################################################################
// 
// ----------------------------------- ��������� ������� ------------------------------------
// 
// ##########################################################################################
void myNavigator(struct MyNavigator* myNavigator)
{
	// ���� :
	myNavigator->logInfo[0] = 85;

	// ����� ����� ����������� :
	myNavigator->msgData.dT += myNavigator->msgData.dt;

	// �������� ��������� :
	if (myNavigator->msgIn.len > MSG_MAX_LEN)
		myNavigator->msgIn.len = MSG_MAX_LEN;

	// ����� ����� ����������� :
#if defined( ALPHA_BETA_FILTERING ) || defined( MYNAVIGATOR_DEBUG )

	if (myNavigator->filters.alphaBetaFilter.lat.Step == 0)
		myNavigator->filters.alphaBetaFilter.lat.Tob = 0.0;
	if (myNavigator->filters.alphaBetaFilter.lon.Step == 0)
		myNavigator->filters.alphaBetaFilter.lon.Tob = 0.0;
	if (myNavigator->filters.alphaBetaFilter.alt.Step == 0)
		myNavigator->filters.alphaBetaFilter.alt.Tob = 0.0;

	myNavigator->filters.alphaBetaFilter.lat.Tob += myNavigator->msgData.dT;
	myNavigator->filters.alphaBetaFilter.lon.Tob += myNavigator->msgData.dT;
	myNavigator->filters.alphaBetaFilter.alt.Tob += myNavigator->msgData.dT;

#endif

	// ��������� ��������� �� ��������� :
	// �������� ��������� �� �������� NMEA-0183 :
	if (myNavigator->msgIn.msg[0] == '$' &&
		myNavigator->msgIn.msg[myNavigator->msgIn.len - 1] == '\n' &&
		myNavigator->msgIn.msg[myNavigator->msgIn.len - 2] == '\r' &&
		myNavigator->msgIn.msg[myNavigator->msgIn.len - 5] == '*')
	{
		// ����������� ����� :
		myNavigator->msgData.checkData[0] = myNavigator->msgIn.msg[myNavigator->msgIn.len - 4];
		myNavigator->msgData.checkData[1] = myNavigator->msgIn.msg[myNavigator->msgIn.len - 3];

		// ���� � ������ ����������� ���� :
		myNavigator->msgData.countryData[0] = myNavigator->msgIn.msg[1];
		myNavigator->msgData.countryData[1] = myNavigator->msgIn.msg[2];

		// ������������� ������ :
		myNavigator->msgData.idData[0] = myNavigator->msgIn.msg[3];
		myNavigator->msgData.idData[1] = myNavigator->msgIn.msg[4];
		myNavigator->msgData.idData[2] = myNavigator->msgIn.msg[5];

		// �������, ��� ��������� ������ ����� :
		getXOR(&myNavigator->msgIn.msg[1], myNavigator->msgData.checkDataCond, myNavigator->msgIn.len - 6);

		// ��������� ������������� �� � ���������� :
		if (myNavigator->msgData.checkDataCond[0] == myNavigator->msgData.checkData[0] &&
			myNavigator->msgData.checkDataCond[1] == myNavigator->msgData.checkData[1])
		{
			// ���������� ��������� �� ������� � ����� �� :
			myNavigator->msgData.parIdLen = findCommas(myNavigator);

			// ������������� ������ :
			chooseId(myNavigator);
			if (myNavigator->logInfo[0] != 0)
			{
				myNavigator->logInfo[0] = 3;
				return;
			}
		}
		else
		{
			myNavigator->logInfo[0] = 2;
			return;
		}
	}
	else
	{
		myNavigator->logInfo[0] = 1;
		return;
	}

	// �������������, ��� ������ ��������� � ������������ (���������� ������ ����������) (�����!) :
	(*myNavigator->funcsPointrs.LEDaction)(9);

	// ������� ��������� �� ����������� ���� � �������� :
	getGCS(myNavigator);

	// ������� ��������� �� ������� � ������������� :
#ifdef TRANSFORM_COORDS
	geo2decart(myNavigator, &myNavigator->coordinates.lat.decodedPos.value, &myNavigator->coordinates.lon.decodedPos.value, &myNavigator->coordinates.alt.decodedPos.value);
#endif

	// ��������, ��� �������������� ���������� � �������� ��������� :
	isInvalidData(&myNavigator->coordinates.lat.decodedPos.value, latIntPartLimits, &myNavigator->coordinates.lat.previosPos.value[PREVIOS_POS_LEN - 1]);
	isInvalidData(&myNavigator->coordinates.lon.decodedPos.value, lonIntPartLimits, &myNavigator->coordinates.lon.previosPos.value[PREVIOS_POS_LEN - 1]);
	isInvalidData(&myNavigator->coordinates.alt.decodedPos.value, altIntPartLimits, &myNavigator->coordinates.alt.previosPos.value[PREVIOS_POS_LEN - 1]);

	// ��������, ��� �������������� ���������� ����������� ������ (�� "������� �������") :
#ifdef TRANSFORM_COORDS
	prePostFilter(myNavigator, true);
#endif



	if (++myNavigator->filterMainCntr > PREVIOS_POS_LEN)
	{

		// ���������� ������ :
#ifdef INCLUDE_LAT

#if defined( MYNAVIGATOR_DEBUG )

		kalmanFilter(&myNavigator->filters.kalmanFilter.lat, &myNavigator->coordinates.lat);
		myNavigator->coordinates.lat.filteredPos.allFilteredValues[0] = myNavigator->coordinates.lat.filteredPos.value;

		medianFilter(&myNavigator->filters.medianFilter.lat, &myNavigator->coordinates.lat);
		myNavigator->coordinates.lat.filteredPos.allFilteredValues[1] = myNavigator->coordinates.lat.filteredPos.value;

		minQuadFilter(&myNavigator->filters.minQuadFilter.lat, &myNavigator->coordinates.lat);
		myNavigator->coordinates.lat.filteredPos.allFilteredValues[2] = myNavigator->coordinates.lat.filteredPos.value;

		alphaBetaFilter(myNavigator, &myNavigator->filters.alphaBetaFilter.lat, &myNavigator->coordinates.lat);
		myNavigator->coordinates.lat.filteredPos.allFilteredValues[3] = myNavigator->coordinates.lat.filteredPos.value;

#elif defined( MYNAVIGATOR_RELEASE )

#if defined(KALMAN_FILTERING)
		// ������ ������� :
		kalmanFilter(&myNavigator->filters.kalmanFilter.lat, &myNavigator->coordinates.lat);
		isInvalidData(&myNavigator->coordinates.lat.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lat.decodedPos.value);

#elif defined( MEDIAN_FILTERING )
		// ��������� ������ :
		medianFilter(&myNavigator->filters.medianFilter.lat, &myNavigator->coordinates.lat);
		isInvalidData(&myNavigator->coordinates.lat.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lat.decodedPos.value);

#elif defined( MIN_QUAD_FILTERING )
		// ����������� ������� ������ :
		minQuadFilter(&myNavigator->filters.minQuadFilter.lat, &myNavigator->coordinates.lat);
		isInvalidData(&myNavigator->coordinates.lat.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lat.decodedPos.value);

#elif defined( ALPHA_BETA_FILTERING )
		// �����-���� ������ :
		alphaBetaFilter(myNavigator, &myNavigator->filters.alphaBetaFilter.lat, &myNavigator->coordinates.lat);
		isInvalidData(&myNavigator->coordinates.lat.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lat.decodedPos.value);

#else
		// ���� ��� ������� :
		myNavigator->coordinates.lat.filteredPos.value = myNavigator->coordinates.lat.decodedPos.value;

#endif

#endif

#endif



		// ���������� ������� :
#ifdef INCLUDE_LON

#if defined( MYNAVIGATOR_DEBUG )

		kalmanFilter(&myNavigator->filters.kalmanFilter.lon, &myNavigator->coordinates.lon);
		myNavigator->coordinates.lon.filteredPos.allFilteredValues[0] = myNavigator->coordinates.lon.filteredPos.value;

		medianFilter(&myNavigator->filters.medianFilter.lon, &myNavigator->coordinates.lon);
		myNavigator->coordinates.lon.filteredPos.allFilteredValues[1] = myNavigator->coordinates.lon.filteredPos.value;

		minQuadFilter(&myNavigator->filters.minQuadFilter.lon, &myNavigator->coordinates.lon);
		myNavigator->coordinates.lon.filteredPos.allFilteredValues[2] = myNavigator->coordinates.lon.filteredPos.value;

		alphaBetaFilter(myNavigator, &myNavigator->filters.alphaBetaFilter.lon, &myNavigator->coordinates.lon);
		myNavigator->coordinates.lon.filteredPos.allFilteredValues[3] = myNavigator->coordinates.lon.filteredPos.value;

#elif defined( MYNAVIGATOR_RELEASE )

#if defined( KALMAN_FILTERING )
		// ������ ������� :
		kalmanFilter(&myNavigator->filters.kalmanFilter.lon, &myNavigator->coordinates.lon);
		isInvalidData(&myNavigator->coordinates.lon.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lon.decodedPos.value);

#elif defined( MEDIAN_FILTERING )
		// ��������� ������ :
		medianFilter(&myNavigator->filters.medianFilter.lon, &myNavigator->coordinates.lon);
		isInvalidData(&myNavigator->coordinates.lon.filteredPos.value, lonIntPartLimits, &myNavigator->coordinates.lon.decodedPos.value);

#elif defined( MIN_QUAD_FILTERING )
		// ����������� ������� ������ :
		minQuadFilter(&myNavigator->filters.minQuadFilter.lon, &myNavigator->coordinates.lon);
		isInvalidData(&myNavigator->coordinates.lon.filteredPos.value, lonIntPartLimits, &myNavigator->coordinates.lon.decodedPos.value);

#elif defined( ALPHA_BETA_FILTERING )
	// �����-���� ������ :
		alphaBetaFilter(myNavigator, &myNavigator->filters.alphaBetaFilter.lon, &myNavigator->coordinates.lon);
		isInvalidData(&myNavigator->coordinates.lon.filteredPos.value, lonIntPartLimits, &myNavigator->coordinates.lon.decodedPos.value);

#else
		// ���� ��� ������� :
		myNavigator->coordinates.lon.filteredPos.value = myNavigator->coordinates.lon.decodedPos.value;

#endif

#endif

#endif



		// ���������� ������ :
#ifdef INCLUDE_ALT

#if defined( MYNAVIGATOR_DEBUG )

		if (myNavigator->msgData.id[4] != 0)
		{
			kalmanFilter(&myNavigator->filters.kalmanFilter.alt, &myNavigator->coordinates.alt);
			myNavigator->coordinates.alt.filteredPos.allFilteredValues[0] = myNavigator->coordinates.alt.filteredPos.value;

			medianFilter(&myNavigator->filters.medianFilter.alt, &myNavigator->coordinates.alt);
			myNavigator->coordinates.alt.filteredPos.allFilteredValues[1] = myNavigator->coordinates.alt.filteredPos.value;

			minQuadFilter(&myNavigator->filters.minQuadFilter.alt, &myNavigator->coordinates.alt);
			myNavigator->coordinates.alt.filteredPos.allFilteredValues[2] = myNavigator->coordinates.alt.filteredPos.value;

			alphaBetaFilter(myNavigator, &myNavigator->filters.alphaBetaFilter.alt, &myNavigator->coordinates.alt);
			myNavigator->coordinates.alt.filteredPos.allFilteredValues[3] = myNavigator->coordinates.alt.filteredPos.value;
		}
		myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;

#elif defined( MYNAVIGATOR_RELEASE )

#if defined( KALMAN_FILTERING )
		// ������ ������� :
		if (myNavigator->msgData.id[4] != 0)
		{
			kalmanFilter(&myNavigator->filters.kalmanFilter.alt, &myNavigator->coordinates.alt);
			isInvalidData(&myNavigator->coordinates.alt.filteredPos.value, altIntPartLimits, &myNavigator->coordinates.alt.decodedPos.value);
		}
		else
		{
			myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;
		}

#elif defined( MEDIAN_FILTERING )
		// ��������� ������ :
		if (myNavigator->msgData.id[4] != 0)
		{
			medianFilter(&myNavigator->filters.medianFilter.alt, &myNavigator->coordinates.alt);
			isInvalidData(&myNavigator->coordinates.alt.filteredPos.value, altIntPartLimits, &myNavigator->coordinates.alt.decodedPos.value);
		}
		else
		{
			myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;
		}

#elif defined( MIN_QUAD_FILTERING )
		// ����������� ������� ������ :
		if (myNavigator->msgData.id[4] != 0)
		{
			minQuadFilter(&myNavigator->filters.minQuadFilter.alt, &myNavigator->coordinates.alt);
			isInvalidData(&myNavigator->coordinates.alt.filteredPos.value, altIntPartLimits, &myNavigator->coordinates.alt.decodedPos.value);
		}
		else
		{
			myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;
		}

#elif defined( ALPHA_BETA_FILTERING )
		// �����-���� ������ :
		if (myNavigator->msgData.id[4] != 0)
		{
			alphaBetaFilter(myNavigator, &myNavigator->filters.alphaBetaFilter.alt, &myNavigator->coordinates.alt);
			isInvalidData(&myNavigator->coordinates.alt.filteredPos.value, altIntPartLimits, &myNavigator->coordinates.alt.decodedPos.value);
		}
		else
		{
			myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;
		}

#else
		// ���� ��� ������� :
		myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;

#endif

#endif

#endif

	}
	else
	{
		// ���������� ������� � ����������� ������������ :
		overwritePrevPos(myNavigator);

		(*myNavigator->funcsPointrs.LEDaction)(10);
		myNavigator->logInfo[0] = 0;

		return;
	}



	// ���������� ������� � ����������� ������������ :
	overwritePrevPos(myNavigator);

	// ��������, ��� ������������� ���������� � ����������� ������ (�� "������� �������") :
#ifdef TRANSFORM_COORDS
	prePostFilter(myNavigator, false);

	// �������������� ������� ������������� ��������� � �������� ������-������� 
	// �� ���������� ����������� � ������������� ���������� :
#if defined( MYNAVIGATOR_DEBUG )

	decart2geo(myNavigator, &myNavigator->coordinates.lat.filteredPos.allFilteredValues[0], &myNavigator->coordinates.lon.filteredPos.allFilteredValues[0], &myNavigator->coordinates.alt.filteredPos.allFilteredValues[0]);
	decart2geo(myNavigator, &myNavigator->coordinates.lat.filteredPos.allFilteredValues[1], &myNavigator->coordinates.lon.filteredPos.allFilteredValues[1], &myNavigator->coordinates.alt.filteredPos.allFilteredValues[1]);
	decart2geo(myNavigator, &myNavigator->coordinates.lat.filteredPos.allFilteredValues[2], &myNavigator->coordinates.lon.filteredPos.allFilteredValues[2], &myNavigator->coordinates.alt.filteredPos.allFilteredValues[2]);
	decart2geo(myNavigator, &myNavigator->coordinates.lat.filteredPos.allFilteredValues[3], &myNavigator->coordinates.lon.filteredPos.allFilteredValues[3], &myNavigator->coordinates.alt.filteredPos.allFilteredValues[3]);

	decart2geo(myNavigator, &myNavigator->coordinates.lat.decodedPos.value, &myNavigator->coordinates.lon.decodedPos.value, &myNavigator->coordinates.alt.filteredPos.allFilteredValues[0]);
	myNavigator->coordinates.lat.filteredPos.value = myNavigator->coordinates.lat.decodedPos.value;
	myNavigator->coordinates.lon.filteredPos.value = myNavigator->coordinates.lon.decodedPos.value;

#elif defined( MYNAVIGATOR_RELEASE )
	decart2geo(myNavigator, &myNavigator->coordinates.lat.filteredPos.value, &myNavigator->coordinates.lon.filteredPos.value, &myNavigator->coordinates.alt.filteredPos.value);
#endif

#endif

	// ������ � ���������� ������ ��������� :
#if defined( WRITE_COORDS_IN_FLASH )
	writeInROM(myNavigator);
#endif

	// ���������� ��������� � ������ ������������ :
	changeMsg(myNavigator);

	// ���������� ����� �� ��� ����������� ��������� :
	getXOR(&myNavigator->msgIn.msg[1], &myNavigator->msgIn.msg[myNavigator->msgIn.len - 4], myNavigator->msgIn.len - 6);

	// �������������, ��� ������� ���������� ��������� ������� (����������� ������ ����������) :
	(*myNavigator->funcsPointrs.LEDaction)(10);

	myNavigator->msgData.dT = 0.0;

	myNavigator->logInfo[0] = 0;

	return;
}


void myNavigatorInit(struct MyNavigator* myNavigator, struct FuncsPointrs* funcsPointrs)
{
	myNavigator->filterMainCntr = 0;

#ifdef MYNAVIGATOR_DEBUG
	for (i = 0; i < 4; i++)
	{
		myNavigator->coordinates.lat.filteredPos.allFilteredValues[i] = 0;
		myNavigator->coordinates.lon.filteredPos.allFilteredValues[i] = 0;
		myNavigator->coordinates.alt.filteredPos.allFilteredValues[i] = 0;
	}
#endif

#ifdef WRITE_COORDS_IN_FLASH
	myNavigator->writeInFlash.adress = WRITE_IN_FLASH_START_ADRESS;
	myNavigator->writeInFlash.isUnlockedFlash = true;
#endif

	myNavigator->funcsPointrs.flashLockFunc = funcsPointrs->flashLockFunc;
	myNavigator->funcsPointrs.LEDaction = funcsPointrs->LEDaction;
	myNavigator->funcsPointrs.flashFunc = funcsPointrs->flashFunc;

#ifdef TRANSFORM_COORDS
	geo2decart(myNavigator, &startCoordinates[0], &startCoordinates[1], &startCoordinates[2]);
#endif

	for (i = 0; i < PREVIOS_POS_LEN; i++)
	{
		myNavigator->coordinates.lat.previosPos.value[i] = startCoordinates[0];
		myNavigator->coordinates.lon.previosPos.value[i] = startCoordinates[1];
		myNavigator->coordinates.alt.previosPos.value[i] = startCoordinates[2];
	}

	for (i = 0; i < MSG_MAX_LEN; i++)
	{
		myNavigator->msgIn.msg[i] = '0';
		myNavigator->msgIn.msg[i] = '0';
		myNavigator->msgData.parId[i] = 0;
	}

	myNavigator->myNavigatorExeTime = 0;

	myNavigator->msgIn.len = 0;
	myNavigator->msgIn.len = 0;
	myNavigator->msgData.dt = 0;
	myNavigator->msgData.dT = 0.0;
	myNavigator->msgData.parIdLen = 0;

	myNavigator->msgData.checkData[0] = '0';
	myNavigator->msgData.checkData[1] = '0';
	myNavigator->msgData.checkDataCond[0] = '0';
	myNavigator->msgData.checkDataCond[1] = '0';
	myNavigator->msgData.countryData[0] = '0';
	myNavigator->msgData.countryData[1] = '0';
	myNavigator->msgData.idData[0] = '0';
	myNavigator->msgData.idData[1] = '0';
	myNavigator->msgData.idData[2] = '0';

	for (i = 0; i < 7; i++)
	{
		myNavigator->msgData.id[i] = 0;
	}

	myNavigator->coordinates.lat.decodedPos.value = 0.0;
	myNavigator->coordinates.lat.decodedPos.intPosition = 0;
	myNavigator->coordinates.lat.decodedPos.floatPosition = 0;
	myNavigator->coordinates.lat.decodedPos.length = 0;
	myNavigator->coordinates.lat.decodedPos.intLength = 0;
	myNavigator->coordinates.lat.decodedPos.floatLength = 0;
	myNavigator->coordinates.lat.filteredPos.value = 0.0;
	myNavigator->coordinates.lat.filteredPos.intLength = 0;
	myNavigator->coordinates.lat.filteredPos.floatLength = 0;

	myNavigator->coordinates.lon.decodedPos.value = 0.0;
	myNavigator->coordinates.lon.decodedPos.intPosition = 0;
	myNavigator->coordinates.lon.decodedPos.floatPosition = 0;
	myNavigator->coordinates.lon.decodedPos.length = 0;
	myNavigator->coordinates.lon.decodedPos.intLength = 0;
	myNavigator->coordinates.lon.decodedPos.floatLength = 0;
	myNavigator->coordinates.lon.filteredPos.value = 0.0;
	myNavigator->coordinates.lon.filteredPos.intLength = 0;
	myNavigator->coordinates.lon.filteredPos.floatLength = 0;

	myNavigator->coordinates.alt.decodedPos.value = 0.0;
	myNavigator->coordinates.alt.decodedPos.intPosition = 0;
	myNavigator->coordinates.alt.decodedPos.floatPosition = 0;
	myNavigator->coordinates.alt.decodedPos.length = 0;
	myNavigator->coordinates.alt.decodedPos.intLength = 0;
	myNavigator->coordinates.alt.decodedPos.floatLength = 0;
	myNavigator->coordinates.alt.filteredPos.value = 0.0;
	myNavigator->coordinates.alt.filteredPos.intLength = 0;
	myNavigator->coordinates.alt.filteredPos.floatLength = 0;

	// ������������� ���������� ������� :
	medianFilterInit(&myNavigator->filters.medianFilter.lat, startCoordinates[0]);
	medianFilterInit(&myNavigator->filters.medianFilter.lon, startCoordinates[1]);
	medianFilterInit(&myNavigator->filters.medianFilter.alt, startCoordinates[2]);

	// �������������� ������� ������� :
	kalmanFilterInit(&myNavigator->filters.kalmanFilter.lat, startCoordinates[0], kalmanFilterR[0]);
	kalmanFilterInit(&myNavigator->filters.kalmanFilter.lon, startCoordinates[1], kalmanFilterR[1]);
	kalmanFilterInit(&myNavigator->filters.kalmanFilter.alt, startCoordinates[2], kalmanFilterR[2]);

	// ������������� ������� ������� :
	minQuadFilterInit(&myNavigator->filters.minQuadFilter.lat);
	minQuadFilterInit(&myNavigator->filters.minQuadFilter.lon);
	minQuadFilterInit(&myNavigator->filters.minQuadFilter.alt);

	// ������������� �����-���� ������� :
	alphaBetaFilterInit(&myNavigator->filters.alphaBetaFilter.lat, alphaBetaFilterT[0], alphaBetaFilterSp[0], alphaBetaFilterSn[0]);
	alphaBetaFilterInit(&myNavigator->filters.alphaBetaFilter.lon, alphaBetaFilterT[1], alphaBetaFilterSp[1], alphaBetaFilterSn[1]);
	alphaBetaFilterInit(&myNavigator->filters.alphaBetaFilter.alt, alphaBetaFilterT[2], alphaBetaFilterSp[2], alphaBetaFilterSn[2]);

	return;
}

