#include "myNavigator.h"

extern const double latIntPartLimits[];
extern const double lonIntPartLimits[];
extern const double altIntPartLimits[];
extern const double startCoordinates[];

void myNavigator(struct MyNavigator* myNavigator)
{
	// Логи :
	for (unsigned short i = 0; i < logInfoArrLen; i++)
	{
		myNavigator->logInfo[i] = 85;
	}

	// Выходное сообщение :
	myNavigator->msgOut.len = myNavigator->msgIn.len < msgMaxLen ? myNavigator->msgIn.len : msgMaxLen;
	memcpy(myNavigator->msgOut.msg, myNavigator->msgIn.msg, myNavigator->msgOut.len);
	myNavigator->logInfo[1] = 9;

	// Получение координат из сообщения :
	// Проверка сообщения на стандарт NMEA-0183 :
	if (myNavigator->msgOut.msg[0] == '$' &&
		myNavigator->msgOut.msg[myNavigator->msgOut.len - 1] == '\n' &&
		myNavigator->msgOut.msg[myNavigator->msgOut.len - 2] == '\r' &&
		myNavigator->msgOut.msg[myNavigator->msgOut.len - 5] == '*')
	{
		// контрольная сумма :
		myNavigator->msgData.checkData[0] = myNavigator->msgOut.msg[myNavigator->msgOut.len - 4];
		myNavigator->msgData.checkData[1] = myNavigator->msgOut.msg[myNavigator->msgOut.len - 3];
		myNavigator->logInfo[1] = 8;

		// инфо о стране спутниковой ГНСС :
		myNavigator->msgData.countryData[0] = myNavigator->msgOut.msg[1];
		myNavigator->msgData.countryData[1] = myNavigator->msgOut.msg[2];

		// идентификатор строки :
		myNavigator->msgData.idData[0] = myNavigator->msgOut.msg[3];
		myNavigator->msgData.idData[1] = myNavigator->msgOut.msg[4];
		myNavigator->msgData.idData[2] = myNavigator->msgOut.msg[5];

		// условие, что сообщение пришло верно :
		getXOR(&myNavigator->msgOut.msg[1], myNavigator->msgData.checkDataCond, myNavigator->msgOut.len - 6, &myNavigator->logInfo[2]);
		myNavigator->logInfo[1] = 7;

		// сравнение дешифрованной КС и полученной :
		if (myNavigator->msgData.checkDataCond[0] == myNavigator->msgData.checkData[0] &&
			myNavigator->msgData.checkDataCond[1] == myNavigator->msgData.checkData[1])
		{
			// Разделение сообщения на запятые и поиск их :
			myNavigator->msgData.parIdLen = findCommas(myNavigator, &myNavigator->logInfo[3]);
			myNavigator->logInfo[1] = 6;

			// Идентификатор строки :
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
	// Перевод координат из символьного вида в числовой :
	getGCS(myNavigator, &myNavigator->logInfo[5]);
	myNavigator->logInfo[1] = 4;

	// Преобразование геодезических координат в плоские прямоугольные координаты :
#ifdef TransformCoords
	geo2decart(&myNavigator->coordinates.lat.filteredPos.value, &myNavigator->coordinates.lon.filteredPos.value);
#endif



	// Фильтрация широты:
#ifdef includeLat

#if defined( DEBUG )

	kalmanFilter(&myNavigator->filters.kalmanFilter.lat, &myNavigator->coordinates.lat, &myNavigator->logInfo[13]);
	myNavigator->coordinates.lat.filteredPos.allFilteredValues[0] = myNavigator->coordinates.lat.filteredPos.value;

	medianFilter(&myNavigator->filters.medianFilter.lat, &myNavigator->coordinates.lat, &myNavigator->logInfo[13]);
	myNavigator->coordinates.lat.filteredPos.allFilteredValues[1] = myNavigator->coordinates.lat.filteredPos.value;

	minQuadFilter(&myNavigator->filters.minQuadFilter.lat, &myNavigator->coordinates.lat, &myNavigator->logInfo[13]);
	myNavigator->coordinates.lat.filteredPos.allFilteredValues[2] = myNavigator->coordinates.lat.filteredPos.value;

	alphaBetaFilter(&myNavigator->filters.alphaBetaFilter.lat, &myNavigator->coordinates.lat, &myNavigator->logInfo[13]);
	myNavigator->coordinates.lat.filteredPos.allFilteredValues[3] = myNavigator->coordinates.lat.filteredPos.value;

	myNavigator->coordinates.lat.filteredPos.value = myNavigator->coordinates.lat.decodedPos.value;

#elif defined( RELEASE )

#if defined(kalmanFiltering)
	// Фильтр Калмана :
	kalmanFilter(&myNavigator->filters.kalmanFilter.lat, &myNavigator->coordinates.lat, &myNavigator->logInfo[13]);
	isInvalidData(&myNavigator->coordinates.lat.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lat.decodedPos.value);

#elif defined( medianFiltering )
	// Медианный фильтр :
	medianFilter(&myNavigator->filters.medianFilter.lat, &myNavigator->coordinates.lat, &myNavigator->logInfo[13]);
	isInvalidData(&myNavigator->coordinates.lat.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lat.decodedPos.value);

#elif defined( minQuadFiltering )
	// Минимальный квадрат фильтр :
	minQuadFilter(&myNavigator->filters.minQuadFilter.lat, &myNavigator->coordinates.lat, &myNavigator->logInfo[13]);
	isInvalidData(&myNavigator->coordinates.lat.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lat.decodedPos.value);

#elif defined( alphaBetaFiltering )
	// Альфа-бета фильтр :
	alphaBetaFilter(&myNavigator->filters.alphaBetaFilter.lat, &myNavigator->coordinates.lat, &myNavigator->logInfo[13]);
	isInvalidData(&myNavigator->coordinates.lat.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lat.decodedPos.value);

#else
	// Если без фильтра :
	myNavigator->coordinates.lat.filteredPos.value = myNavigator->coordinates.lat.decodedPos.value;
	myNavigator->logInfo[13] = 0;

#endif

#endif

#else
	myNavigator->logInfo[13] = 0;
#endif



	// Фильтрация долготы:
#ifdef includeLon

#if defined( DEBUG )

	kalmanFilter(&myNavigator->filters.kalmanFilter.lon, &myNavigator->coordinates.lon, &myNavigator->logInfo[14]);
	myNavigator->coordinates.lon.filteredPos.allFilteredValues[0] = myNavigator->coordinates.lon.filteredPos.value;

	medianFilter(&myNavigator->filters.medianFilter.lon, &myNavigator->coordinates.lon, &myNavigator->logInfo[14]);
	myNavigator->coordinates.lon.filteredPos.allFilteredValues[1] = myNavigator->coordinates.lon.filteredPos.value;

	minQuadFilter(&myNavigator->filters.minQuadFilter.lon, &myNavigator->coordinates.lon, &myNavigator->logInfo[14]);
	myNavigator->coordinates.lon.filteredPos.allFilteredValues[2] = myNavigator->coordinates.lon.filteredPos.value;

	alphaBetaFilter(&myNavigator->filters.alphaBetaFilter.lon, &myNavigator->coordinates.lon, &myNavigator->logInfo[14]);
	myNavigator->coordinates.lon.filteredPos.allFilteredValues[3] = myNavigator->coordinates.lon.filteredPos.value;

	myNavigator->coordinates.lon.filteredPos.value = myNavigator->coordinates.lon.decodedPos.value;

#elif defined( RELEASE )

#if defined( kalmanFiltering )
	// Фильтр Калмана :
	kalmanFilter(&myNavigator->filters.kalmanFilter.lon, &myNavigator->coordinates.lon, &myNavigator->logInfo[14]);
	isInvalidData(&myNavigator->coordinates.lon.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lon.decodedPos.value);

#elif defined( medianFiltering )
	// Медианный фильтр :
	medianFilter(&myNavigator->filters.medianFilter.lon, &myNavigator->coordinates.lon, &myNavigator->logInfo[14]);
	isInvalidData(&myNavigator->coordinates.lon.filteredPos.value, lonIntPartLimits, &myNavigator->coordinates.lon.decodedPos.value);

#elif defined( minQuadFiltering )
	// Минимальный квадрат фильтр :
	minQuadFilter(&myNavigator->filters.minQuadFilter.lon, &myNavigator->coordinates.lon, &myNavigator->logInfo[14]);
	isInvalidData(&myNavigator->coordinates.lon.filteredPos.value, lonIntPartLimits, &myNavigator->coordinates.lon.decodedPos.value);

#elif defined( alphaBetaFiltering )
// Альфа-бета фильтр :
	alphaBetaFilter(&myNavigator->filters.alphaBetaFilter.lon, &myNavigator->coordinates.lon, &myNavigator->logInfo[14]);
	isInvalidData(&myNavigator->coordinates.lon.filteredPos.value, lonIntPartLimits, &myNavigator->coordinates.lon.decodedPos.value);

#else
	// Если без фильтра :
	myNavigator->coordinates.lon.filteredPos.value = myNavigator->coordinates.lon.decodedPos.value;
	myNavigator->logInfo[14] = 0;

#endif

#endif

#else
	myNavigator->logInfo[14] = 0;
#endif



	// Фильтрация высоты:
#ifdef includeAlt

#if defined( DEBUG )

	if (myNavigator->msgData.id[4] == 0)
	{
		kalmanFilter(&myNavigator->filters.kalmanFilter.alt, &myNavigator->coordinates.alt, &myNavigator->logInfo[15]);
		myNavigator->coordinates.alt.filteredPos.allFilteredValues[0] = myNavigator->coordinates.alt.filteredPos.value;

		medianFilter(&myNavigator->filters.medianFilter.alt, &myNavigator->coordinates.alt, &myNavigator->logInfo[15]);
		myNavigator->coordinates.alt.filteredPos.allFilteredValues[1] = myNavigator->coordinates.alt.filteredPos.value;

		minQuadFilter(&myNavigator->filters.minQuadFilter.alt, &myNavigator->coordinates.alt, &myNavigator->logInfo[15]);
		myNavigator->coordinates.alt.filteredPos.allFilteredValues[2] = myNavigator->coordinates.alt.filteredPos.value;

		alphaBetaFilter(&myNavigator->filters.alphaBetaFilter.alt, &myNavigator->coordinates.alt, &myNavigator->logInfo[15]);
		myNavigator->coordinates.alt.filteredPos.allFilteredValues[3] = myNavigator->coordinates.alt.filteredPos.value;
	}
	myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;

#elif defined( RELEASE )

#if defined( kalmanFiltering )
	// Фильтр Калмана :
	if (myNavigator->msgData.id[4] != 0)
	{
		kalmanFilter(&myNavigator->filters.kalmanFilter.alt, &myNavigator->coordinates.alt, &myNavigator->logInfo[15]);
		isInvalidData(&myNavigator->coordinates.alt.filteredPos.value, altIntPartLimits, &myNavigator->coordinates.alt.decodedPos.value);
	}
	else
	{
		myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;
		myNavigator->logInfo[15] = 0;
	}

#elif defined( medianFiltering )
	// Медианный фильтр :
	if (myNavigator->msgData.id[4] != 0)
	{
		medianFilter(&myNavigator->filters.medianFilter.alt, &myNavigator->coordinates.alt, &myNavigator->logInfo[15]);
		isInvalidData(&myNavigator->coordinates.alt.filteredPos.value, altIntPartLimits, &myNavigator->coordinates.alt.decodedPos.value);
	}
	else
	{
		myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;
		myNavigator->logInfo[15] = 0;
	}

#elif defined( minQuadFiltering )
	// Минимальный квадрат фильтр :
	if (myNavigator->msgData.id[4] != 0)
	{
		minQuadFilter(&myNavigator->filters.minQuadFilter.alt, &myNavigator->coordinates.alt, &myNavigator->logInfo[15]);
		isInvalidData(&myNavigator->coordinates.alt.filteredPos.value, altIntPartLimits, &myNavigator->coordinates.alt.decodedPos.value);
	}
	else
	{
		myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;
		myNavigator->logInfo[15] = 0;
	}

#elif defined( alphaBetaFiltering )
	// Альфа-бета фильтр :
	if (myNavigator->msgData.id[4] != 0)
	{
		alphaBetaFilter(&myNavigator->filters.alphaBetaFilter.alt, &myNavigator->coordinates.alt, &myNavigator->logInfo[15]);
		isInvalidData(&myNavigator->coordinates.alt.filteredPos.value, altIntPartLimits, &myNavigator->coordinates.alt.decodedPos.value);
	}
	else
	{
		myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;
		myNavigator->logInfo[15] = 0;
	}

#else
	// Если без фильтра :
	myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;
	myNavigator->logInfo[15] = 0;

#endif

#endif

#else
	myNavigator->logInfo[15] = 0;
#endif



	// Преобразование плоских прямоугольных координат в проекции Гаусса-Крюгера 
	// на эллипсоиде Красовского в геодезические координаты :
#ifdef TransformCoords
	decart2geo(&myNavigator->coordinates.lat.filteredPos.value, &myNavigator->coordinates.lon.filteredPos.value);
#endif

	// Запись в постоянную память координат :
#if defined( WriteCoordsInFlash )
	writeInROM(&myNavigator->coordinates);
#endif
	myNavigator->logInfo[1] = 3;

	// Перезапись массива с предыдущими координатами :
	overwritePrevPos(myNavigator, &myNavigator->logInfo[8]);
	myNavigator->logInfo[1] = 2;

	// Перезапись сообщения с новыми координатами :
	changeMsg(myNavigator, &myNavigator->logInfo[9]);
	myNavigator->logInfo[1] = 1;

	// Вычисление новой КС для измененного сообщения :
	getXOR(&myNavigator->msgOut.msg[1], &myNavigator->msgOut.msg[myNavigator->msgOut.len - 4], myNavigator->msgOut.len - 6, &myNavigator->logInfo[12]);
	myNavigator->logInfo[1] = 0;

	myNavigator->logInfo[0] = 0;
	return;
}


void myNavigatorInit(struct MyNavigator* myNavigator)
{
#ifdef DEBUG
	for (unsigned short i = 0; i < 4; i++)
	{
		myNavigator->coordinates.lat.filteredPos.allFilteredValues[i] = 0;
		myNavigator->coordinates.lon.filteredPos.allFilteredValues[i] = 0;
		myNavigator->coordinates.alt.filteredPos.allFilteredValues[i] = 0;
	}
#endif

	for (unsigned short i = 0; i < previosPosLen; i++)
	{
		myNavigator->coordinates.lat.previosPos.value[i] = startCoordinates[0];
		myNavigator->coordinates.lon.previosPos.value[i] = startCoordinates[1];
		myNavigator->coordinates.alt.previosPos.value[i] = startCoordinates[2];
	}

	for (unsigned short i = 0; i < msgMaxLen; i++)
	{
		myNavigator->msgIn.msg[i] = '0';
		myNavigator->msgOut.msg[i] = '0';
		myNavigator->msgData.parId[i] = 0;
	}
	myNavigator->msgIn.len = 0;
	myNavigator->msgOut.len = 0;
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

	for (unsigned short i = 0; i < 6; i++)
	{
		myNavigator->msgData.id[0] = 0;
	}

	myNavigator->coordinates.lat.decodedPos.value = 0;
	myNavigator->coordinates.lat.decodedPos.intPosition = 0;
	myNavigator->coordinates.lat.decodedPos.floatPosition = 0;
	myNavigator->coordinates.lat.decodedPos.length = 0;
	myNavigator->coordinates.lat.decodedPos.intLength = 0;
	myNavigator->coordinates.lat.decodedPos.floatLength = 0;
	myNavigator->coordinates.lat.filteredPos.value = 0;
	myNavigator->coordinates.lat.filteredPos.intLength = 0;
	myNavigator->coordinates.lat.filteredPos.floatLength = 0;

	myNavigator->coordinates.lon.decodedPos.value = 0;
	myNavigator->coordinates.lon.decodedPos.intPosition = 0;
	myNavigator->coordinates.lon.decodedPos.floatPosition = 0;
	myNavigator->coordinates.lon.decodedPos.length = 0;
	myNavigator->coordinates.lon.decodedPos.intLength = 0;
	myNavigator->coordinates.lon.decodedPos.floatLength = 0;
	myNavigator->coordinates.lon.filteredPos.value = 0;
	myNavigator->coordinates.lon.filteredPos.intLength = 0;
	myNavigator->coordinates.lon.filteredPos.floatLength = 0;

	myNavigator->coordinates.alt.decodedPos.value = 0;
	myNavigator->coordinates.alt.decodedPos.intPosition = 0;
	myNavigator->coordinates.alt.decodedPos.floatPosition = 0;
	myNavigator->coordinates.alt.decodedPos.length = 0;
	myNavigator->coordinates.alt.decodedPos.intLength = 0;
	myNavigator->coordinates.alt.decodedPos.floatLength = 0;
	myNavigator->coordinates.alt.filteredPos.value = 0;
	myNavigator->coordinates.alt.filteredPos.intLength = 0;
	myNavigator->coordinates.alt.filteredPos.floatLength = 0;

	// Инициализация медианного фильтра :
	medianFilterInit(&myNavigator->filters.medianFilter.lat, startCoordinates[0]);
	medianFilterInit(&myNavigator->filters.medianFilter.lon, startCoordinates[1]);
	medianFilterInit(&myNavigator->filters.medianFilter.alt, startCoordinates[2]);

	// Инициализацияя фильтра Калмана :
	kalmanFilterInit(&myNavigator->filters.kalmanFilter.lat, startCoordinates[0], kalmanFilterR[0]);
	kalmanFilterInit(&myNavigator->filters.kalmanFilter.lon, startCoordinates[1], kalmanFilterR[1]);
	kalmanFilterInit(&myNavigator->filters.kalmanFilter.alt, startCoordinates[2], kalmanFilterR[2]);

	// Инициализация лучшего фильтра :
	minQuadFilterInit(&myNavigator->filters.minQuadFilter.lat);
	minQuadFilterInit(&myNavigator->filters.minQuadFilter.lon);
	minQuadFilterInit(&myNavigator->filters.minQuadFilter.alt);

	// Инициализация альфа-бета фильтра :
	alphaBetaFilterInit(&myNavigator->filters.alphaBetaFilter.lat, alphaBetaFilterT[0], alphaBetaFilterSp[0], alphaBetaFilterSn[0]);
	alphaBetaFilterInit(&myNavigator->filters.alphaBetaFilter.lon, alphaBetaFilterT[1], alphaBetaFilterSp[1], alphaBetaFilterSn[1]);
	alphaBetaFilterInit(&myNavigator->filters.alphaBetaFilter.alt, alphaBetaFilterT[2], alphaBetaFilterSp[2], alphaBetaFilterSn[2]);

	return;
}