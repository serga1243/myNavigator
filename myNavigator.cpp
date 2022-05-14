#include "myNavigator.h"

extern const double latIntPartLimits[];
extern const double lonIntPartLimits[];
extern const double altIntPartLimits[];
extern const double startCoordinates[];

void myNavigator(struct MyNavigator* myNavigator)
{
	// ���� :
	for (unsigned short i = 0; i < logInfoArrLen; i++)
	{
		myNavigator->logInfo[i] = 85;
	}

	// �������� ��������� :
	myNavigator->msgOut.len = myNavigator->msgIn.len < msgMaxLen ? myNavigator->msgIn.len : msgMaxLen;
	for (unsigned short i = 0; i < myNavigator->msgOut.len; i++)
	{
		myNavigator->msgOut.msg[i] = myNavigator->msgIn.msg[i];
	}
	myNavigator->logInfo[1] = 9;

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
		myNavigator->logInfo[1] = 8;

		// ���� � ������ ����������� ���� :
		myNavigator->msgData.countryData[0] = myNavigator->msgIn.msg[1];
		myNavigator->msgData.countryData[1] = myNavigator->msgIn.msg[2];

		// ������������� ������ :
		myNavigator->msgData.idData[0] = myNavigator->msgIn.msg[3];
		myNavigator->msgData.idData[1] = myNavigator->msgIn.msg[4];
		myNavigator->msgData.idData[2] = myNavigator->msgIn.msg[5];

		// �������, ��� ��������� ������ ����� :
		getXOR(myNavigator, &myNavigator->logInfo[2]);
		myNavigator->logInfo[1] = 7;

		// ��������� ������������� �� � ���������� :
		if (myNavigator->msgData.checkDataCond[0] == myNavigator->msgData.checkData[0] &&
			myNavigator->msgData.checkDataCond[1] == myNavigator->msgData.checkData[1])
		{
			// ���������� ��������� �� ������� � ����� �� :
			myNavigator->msgData.parIdLen = findCommas(myNavigator, &myNavigator->logInfo[3]);
			myNavigator->logInfo[1] = 6;

			// ������������� ������ :
			chooseId(myNavigator, &myNavigator->logInfo[4]);
			myNavigator->logInfo[1] = 5;
			if (myNavigator->logInfo[4] != 0)
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
	// ������� ��������� �� ����������� ���� � ������������� :
	getGCS(myNavigator, &myNavigator->logInfo[5]);
	myNavigator->logInfo[1] = 4;

	// ���������� ������:
#ifdef includeLat

#if defined( kalmanFiltering )
	// ������ ������� :
	kalmanFilter(&myNavigator->filters.kalmanFilter.lat, &myNavigator->coordinates.lat, &myNavigator->logInfo[13]);
	isInvalidData(&myNavigator->coordinates.lat.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lat.decodedPos.value);

#elif defined( medianFiltering )
	// ��������� ������ :
	medianFilter(&myNavigator->filters.medianFilter.lat, &myNavigator->coordinates.lat, &myNavigator->logInfo[13]);
	isInvalidData(&myNavigator->coordinates.lat.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lat.decodedPos.value);

#elif defined( minQuadFiltering )
	// ����������� ������� ������ :
	minQuadFilter(&myNavigator->filters.minQuadFilter.lat, &myNavigator->coordinates.lat, &myNavigator->logInfo[13]);
	isInvalidData(&myNavigator->coordinates.lat.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lat.decodedPos.value);

#else
	// ���� ��� ������� :
	myNavigator->coordinates.lat.filteredPos.value = myNavigator->coordinates.lat.decodedPos.value;
	myNavigator->logInfo[13] = 0;

#endif

#else
	myNavigator->logInfo[13] = 0;
#endif



	// ���������� �������:
#ifdef includeLon

#if defined( kalmanFiltering )
	// ������ ������� :
	kalmanFilter(&myNavigator->filters.kalmanFilter.lon, &myNavigator->coordinates.lon, &myNavigator->logInfo[14]);
	isInvalidData(&myNavigator->coordinates.lon.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lon.decodedPos.value);

#elif defined( medianFiltering )
	// ��������� ������ :
	medianFilter(&myNavigator->filters.medianFilter.lon, &myNavigator->coordinates.lon, &myNavigator->logInfo[14]);
	isInvalidData(&myNavigator->coordinates.lon.filteredPos.value, lonIntPartLimits, &myNavigator->coordinates.lon.decodedPos.value);

#elif defined( minQuadFiltering )
	// ����������� ������� ������ :
	minQuadFilter(&myNavigator->filters.minQuadFilter.lon, &myNavigator->coordinates.lon, &myNavigator->logInfo[14]);
	isInvalidData(&myNavigator->coordinates.lon.filteredPos.value, lonIntPartLimits, &myNavigator->coordinates.lon.decodedPos.value);

#else
	// ���� ��� ������� :
	myNavigator->coordinates.lon.filteredPos.value = myNavigator->coordinates.lon.decodedPos.value;
	myNavigator->logInfo[14] = 0;

#endif

#else
	myNavigator->logInfo[14] = 0;
#endif



	// ���������� ������:
#ifdef includeLat

#if defined( kalmanFiltering )
	// ������ ������� :
	myNavigator->logInfo[15] = 0;
	if (myNavigator->msgData.id[4] != 0)
	{
		kalmanFilter(&myNavigator->filters.kalmanFilter.alt, &myNavigator->coordinates.alt, &myNavigator->logInfo[15]);
		isInvalidData(&myNavigator->coordinates.alt.filteredPos.value, altIntPartLimits, &myNavigator->coordinates.alt.decodedPos.value);
	}

#elif defined( medianFiltering )
	// ��������� ������ :
	myNavigator->logInfo[15] = 0;
	if (myNavigator->msgData.id[4] != 0)
	{
		medianFilter(&myNavigator->filters.medianFilter.alt, &myNavigator->coordinates.alt, &myNavigator->logInfo[15]);
		isInvalidData(&myNavigator->coordinates.alt.filteredPos.value, altIntPartLimits, &myNavigator->coordinates.alt.decodedPos.value);
	}

#elif defined( minQuadFiltering )
	// ����������� ������� ������ :
	myNavigator->logInfo[15] = 0;
	if (myNavigator->msgData.id[4] != 0)
	{
		minQuadFilter(&myNavigator->filters.minQuadFilter.alt, &myNavigator->coordinates.alt, &myNavigator->logInfo[15]);
		isInvalidData(&myNavigator->coordinates.alt.filteredPos.value, altIntPartLimits, &myNavigator->coordinates.alt.decodedPos.value);
	}

#else
	// ���� ��� ������� :
	myNavigator->logInfo[15] = 0;
	if (myNavigator->msgData.id[4] != 0)
	{
		myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;
		myNavigator->logInfo[15] = 0;
	}

#endif

#else
	myNavigator->logInfo[15] = 0;
#endif

	myNavigator->logInfo[1] = 3;

	// ���������� ������� � ����������� ������������ :
	overwritePrevPos(myNavigator, &myNavigator->logInfo[8]);
	myNavigator->logInfo[1] = 2;

	// ���������� ��������� � ������ ������������ :
	changeMsg(myNavigator, &myNavigator->logInfo[9]);
	myNavigator->logInfo[1] = 1;

	// ���������� ����� �� ��� ����������� ��������� :
	getXOR(myNavigator, &myNavigator->logInfo[12]);
	myNavigator->logInfo[1] = 0;

	myNavigator->logInfo[0] = 0;
	return;
}


void myNavigatorInit(struct MyNavigator* myNavigator)
{
	for (unsigned short i = 0; i < PreviosPosLen; i++)
	{
		myNavigator->coordinates.lat.previosPos.value[i] = startCoordinates[0];
		myNavigator->coordinates.lon.previosPos.value[i] = startCoordinates[1];
		myNavigator->coordinates.alt.previosPos.value[i] = startCoordinates[2];
	}
	
}