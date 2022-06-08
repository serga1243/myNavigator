#include "myNavigator.h"

extern const double latIntPartLimits[];
extern const double lonIntPartLimits[];
extern const double altIntPartLimits[];
extern const double startCoordinates[];

static unsigned short i;

void myNavigator(struct MyNavigator* myNavigator)
{
	// Логи :
	myNavigator->logInfo[0] = 85;

	// Выходное сообщение :
	myNavigator->msgOut.len = myNavigator->msgIn.len < msgMaxLen ? myNavigator->msgIn.len : msgMaxLen;
	memcpy(myNavigator->msgOut.msg, myNavigator->msgIn.msg, myNavigator->msgOut.len);

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

		// инфо о стране спутниковой ГНСС :
		myNavigator->msgData.countryData[0] = myNavigator->msgOut.msg[1];
		myNavigator->msgData.countryData[1] = myNavigator->msgOut.msg[2];

		// идентификатор строки :
		myNavigator->msgData.idData[0] = myNavigator->msgOut.msg[3];
		myNavigator->msgData.idData[1] = myNavigator->msgOut.msg[4];
		myNavigator->msgData.idData[2] = myNavigator->msgOut.msg[5];

		// условие, что сообщение пришло верно :
		getXOR(&myNavigator->msgOut.msg[1], myNavigator->msgData.checkDataCond, myNavigator->msgOut.len - 6);

		// сравнение дешифрованной КС и полученной :
		if (myNavigator->msgData.checkDataCond[0] == myNavigator->msgData.checkData[0] &&
			myNavigator->msgData.checkDataCond[1] == myNavigator->msgData.checkData[1])
		{
			// Разделение сообщения на запятые и поиск их :
			myNavigator->msgData.parIdLen = findCommas(myNavigator);

			// Идентификатор строки :
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

	// Перевод координат из символьного вида в числовой :
	getGCS(myNavigator);

	// Перевод координат из геовида в прямоугольный :
	geo2decart(&myNavigator->coordinates.lat.decodedPos.value, &myNavigator->coordinates.lon.decodedPos.value);

	// Проверям, что декодированные координаты в реальном диапазоне :
	isInvalidData(&myNavigator->coordinates.lat.decodedPos.value, latIntPartLimits, &myNavigator->coordinates.lat.previosPos.value[previosPosLen - 1]);
	isInvalidData(&myNavigator->coordinates.lon.decodedPos.value, lonIntPartLimits, &myNavigator->coordinates.lon.previosPos.value[previosPosLen - 1]);
	isInvalidData(&myNavigator->coordinates.alt.decodedPos.value, altIntPartLimits, &myNavigator->coordinates.alt.previosPos.value[previosPosLen - 1]);



	// Фильтрация широты:
#ifdef includeLat

#if defined( DEBUG )

	kalmanFilter(&myNavigator->filters.kalmanFilter.lat, &myNavigator->coordinates.lat);
	myNavigator->coordinates.lat.filteredPos.allFilteredValues[0] = myNavigator->coordinates.lat.filteredPos.value;

	medianFilter(&myNavigator->filters.medianFilter.lat, &myNavigator->coordinates.lat);
	myNavigator->coordinates.lat.filteredPos.allFilteredValues[1] = myNavigator->coordinates.lat.filteredPos.value;

	minQuadFilter(&myNavigator->filters.minQuadFilter.lat, &myNavigator->coordinates.lat);
	myNavigator->coordinates.lat.filteredPos.allFilteredValues[2] = myNavigator->coordinates.lat.filteredPos.value;

	alphaBetaFilter(&myNavigator->filters.alphaBetaFilter.lat, &myNavigator->coordinates.lat);
	myNavigator->coordinates.lat.filteredPos.allFilteredValues[3] = myNavigator->coordinates.lat.filteredPos.value;

#elif defined( RELEASE )

#if defined(kalmanFiltering)
	// Фильтр Калмана :
	kalmanFilter(&myNavigator->filters.kalmanFilter.lat, &myNavigator->coordinates.lat);
	isInvalidData(&myNavigator->coordinates.lat.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lat.decodedPos.value);

#elif defined( medianFiltering )
	// Медианный фильтр :
	medianFilter(&myNavigator->filters.medianFilter.lat, &myNavigator->coordinates.lat);
	isInvalidData(&myNavigator->coordinates.lat.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lat.decodedPos.value);

#elif defined( minQuadFiltering )
	// Минимальный квадрат фильтр :
	minQuadFilter(&myNavigator->filters.minQuadFilter.lat, &myNavigator->coordinates.lat);
	isInvalidData(&myNavigator->coordinates.lat.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lat.decodedPos.value);

#elif defined( alphaBetaFiltering )
	// Альфа-бета фильтр :
	alphaBetaFilter(&myNavigator->filters.alphaBetaFilter.lat, &myNavigator->coordinates.lat);
	isInvalidData(&myNavigator->coordinates.lat.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lat.decodedPos.value);

#else
	// Если без фильтра :
	myNavigator->coordinates.lat.filteredPos.value = myNavigator->coordinates.lat.decodedPos.value;

#endif

#endif

#endif



	// Фильтрация долготы:
#ifdef includeLon

#if defined( DEBUG )

	kalmanFilter(&myNavigator->filters.kalmanFilter.lon, &myNavigator->coordinates.lon);
	myNavigator->coordinates.lon.filteredPos.allFilteredValues[0] = myNavigator->coordinates.lon.filteredPos.value;

	medianFilter(&myNavigator->filters.medianFilter.lon, &myNavigator->coordinates.lon);
	myNavigator->coordinates.lon.filteredPos.allFilteredValues[1] = myNavigator->coordinates.lon.filteredPos.value;

	minQuadFilter(&myNavigator->filters.minQuadFilter.lon, &myNavigator->coordinates.lon);
	myNavigator->coordinates.lon.filteredPos.allFilteredValues[2] = myNavigator->coordinates.lon.filteredPos.value;

	alphaBetaFilter(&myNavigator->filters.alphaBetaFilter.lon, &myNavigator->coordinates.lon);
	myNavigator->coordinates.lon.filteredPos.allFilteredValues[3] = myNavigator->coordinates.lon.filteredPos.value;

#elif defined( RELEASE )

#if defined( kalmanFiltering )
	// Фильтр Калмана :
	kalmanFilter(&myNavigator->filters.kalmanFilter.lon, &myNavigator->coordinates.lon);
	isInvalidData(&myNavigator->coordinates.lon.filteredPos.value, latIntPartLimits, &myNavigator->coordinates.lon.decodedPos.value);

#elif defined( medianFiltering )
	// Медианный фильтр :
	medianFilter(&myNavigator->filters.medianFilter.lon, &myNavigator->coordinates.lon);
	isInvalidData(&myNavigator->coordinates.lon.filteredPos.value, lonIntPartLimits, &myNavigator->coordinates.lon.decodedPos.value);

#elif defined( minQuadFiltering )
	// Минимальный квадрат фильтр :
	minQuadFilter(&myNavigator->filters.minQuadFilter.lon, &myNavigator->coordinates.lon);
	isInvalidData(&myNavigator->coordinates.lon.filteredPos.value, lonIntPartLimits, &myNavigator->coordinates.lon.decodedPos.value);

#elif defined( alphaBetaFiltering )
// Альфа-бета фильтр :
	alphaBetaFilter(&myNavigator->filters.alphaBetaFilter.lon, &myNavigator->coordinates.lon);
	isInvalidData(&myNavigator->coordinates.lon.filteredPos.value, lonIntPartLimits, &myNavigator->coordinates.lon.decodedPos.value);

#else
	// Если без фильтра :
	myNavigator->coordinates.lon.filteredPos.value = myNavigator->coordinates.lon.decodedPos.value;

#endif

#endif

#endif



	// Фильтрация высоты:
#ifdef includeAlt

#if defined( DEBUG )

	if (myNavigator->msgData.id[4] != 0)
	{
		kalmanFilter(&myNavigator->filters.kalmanFilter.alt, &myNavigator->coordinates.alt);
		myNavigator->coordinates.alt.filteredPos.allFilteredValues[0] = myNavigator->coordinates.alt.filteredPos.value;

		medianFilter(&myNavigator->filters.medianFilter.alt, &myNavigator->coordinates.alt);
		myNavigator->coordinates.alt.filteredPos.allFilteredValues[1] = myNavigator->coordinates.alt.filteredPos.value;

		minQuadFilter(&myNavigator->filters.minQuadFilter.alt, &myNavigator->coordinates.alt);
		myNavigator->coordinates.alt.filteredPos.allFilteredValues[2] = myNavigator->coordinates.alt.filteredPos.value;

		alphaBetaFilter(&myNavigator->filters.alphaBetaFilter.alt, &myNavigator->coordinates.alt);
		myNavigator->coordinates.alt.filteredPos.allFilteredValues[3] = myNavigator->coordinates.alt.filteredPos.value;
	}
	myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;

#elif defined( RELEASE )

#if defined( kalmanFiltering )
	// Фильтр Калмана :
	if (myNavigator->msgData.id[4] != 0)
	{
		kalmanFilter(&myNavigator->filters.kalmanFilter.alt, &myNavigator->coordinates.alt);
		isInvalidData(&myNavigator->coordinates.alt.filteredPos.value, altIntPartLimits, &myNavigator->coordinates.alt.decodedPos.value);
	}
	else
	{
		myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;
	}

#elif defined( medianFiltering )
	// Медианный фильтр :
	if (myNavigator->msgData.id[4] != 0)
	{
		medianFilter(&myNavigator->filters.medianFilter.alt, &myNavigator->coordinates.alt);
		isInvalidData(&myNavigator->coordinates.alt.filteredPos.value, altIntPartLimits, &myNavigator->coordinates.alt.decodedPos.value);
	}
	else
	{
		myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;
	}

#elif defined( minQuadFiltering )
	// Минимальный квадрат фильтр :
	if (myNavigator->msgData.id[4] != 0)
	{
		minQuadFilter(&myNavigator->filters.minQuadFilter.alt, &myNavigator->coordinates.alt);
		isInvalidData(&myNavigator->coordinates.alt.filteredPos.value, altIntPartLimits, &myNavigator->coordinates.alt.decodedPos.value);
	}
	else
	{
		myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;
	}

#elif defined( alphaBetaFiltering )
	// Альфа-бета фильтр :
	if (myNavigator->msgData.id[4] != 0)
	{
		alphaBetaFilter(&myNavigator->filters.alphaBetaFilter.alt, &myNavigator->coordinates.alt);
		isInvalidData(&myNavigator->coordinates.alt.filteredPos.value, altIntPartLimits, &myNavigator->coordinates.alt.decodedPos.value);
	}
	else
	{
		myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;
	}

#else
	// Если без фильтра :
	myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;

#endif

#endif

#endif



	// Перезапись массива с предыдущими координатами :
	overwritePrevPos(myNavigator);

	// Преобразование плоских прямоугольных координат в проекции Гаусса-Крюгера 
	// на эллипсоиде Красовского в геодезические координаты :
#ifdef TransformCoords

#if defined( DEBUG )

	decart2geo(&myNavigator->coordinates.lat.filteredPos.allFilteredValues[0], &myNavigator->coordinates.lon.filteredPos.allFilteredValues[0]);
	decart2geo(&myNavigator->coordinates.lat.filteredPos.allFilteredValues[1], &myNavigator->coordinates.lon.filteredPos.allFilteredValues[1]);
	decart2geo(&myNavigator->coordinates.lat.filteredPos.allFilteredValues[2], &myNavigator->coordinates.lon.filteredPos.allFilteredValues[2]);
	decart2geo(&myNavigator->coordinates.lat.filteredPos.allFilteredValues[3], &myNavigator->coordinates.lon.filteredPos.allFilteredValues[3]);

	decart2geo(&myNavigator->coordinates.lat.decodedPos.value, &myNavigator->coordinates.lon.decodedPos.value);
	myNavigator->coordinates.lat.filteredPos.value = myNavigator->coordinates.lat.decodedPos.value;
	myNavigator->coordinates.lon.filteredPos.value = myNavigator->coordinates.lon.decodedPos.value;

#elif defined( RELEASE )

	decart2geo(&myNavigator->coordinates.lat.filteredPos.value, &myNavigator->coordinates.lon.filteredPos.value);

#endif

#endif

	// Запись в постоянную память координат :
#if defined( WriteCoordsInFlash )
	writeInROM(&myNavigator->coordinates);
#endif

	// Перезапись сообщения с новыми координатами :
	changeMsg(myNavigator);

	// Вычисление новой КС для измененного сообщения :
	getXOR(&myNavigator->msgOut.msg[1], &myNavigator->msgOut.msg[myNavigator->msgOut.len - 4], myNavigator->msgOut.len - 6);

	return;
}


void myNavigatorInit(struct MyNavigator* myNavigator)
{
#ifdef DEBUG
	for (i = 0; i < 4; i++)
	{
		myNavigator->coordinates.lat.filteredPos.allFilteredValues[i] = 0;
		myNavigator->coordinates.lon.filteredPos.allFilteredValues[i] = 0;
		myNavigator->coordinates.alt.filteredPos.allFilteredValues[i] = 0;
	}
#endif

	for (i = 0; i < previosPosLen; i++)
	{
		myNavigator->coordinates.lat.previosPos.value[i] = startCoordinates[0];
		myNavigator->coordinates.lon.previosPos.value[i] = startCoordinates[1];
		myNavigator->coordinates.alt.previosPos.value[i] = startCoordinates[2];
	}

	for (i = 0; i < msgMaxLen; i++)
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

	for (i = 0; i < 6; i++)
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


inline void isInvalidData(double* data, const double limits[], double* correctData)
{
	*data = (*data > limits[0]) && (*data < limits[1]) ? *data : *correctData;
	return;
}
