#include "myNavigator.h"

// ##########################################################################################
// 
// -----------------------------------  Начальные условия  ----------------------------------
// 
// ##########################################################################################

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

// Параметры фильтра Калмана :
const double kalmanFilterR[] = { 50000.0, 50000.0, 50000.0 };

// Параметры альфа-бета фильтра :
const double alphaBetaFilterT[] = { 1.0, 1.0 , 1.0 };
const double alphaBetaFilterSp[] = { 3.0, 3.0, 3.0 };
const double alphaBetaFilterSn[] = { 3.0, 3.0, 3.0 };

// Статическая переменная :
static unsigned short i;



void myNavigator(struct MyNavigator* myNavigator)
{
	// Логи :
	myNavigator->logInfo[0] = 85;

	// Время между сообщениями :
	myNavigator->msgData.dT += myNavigator->msgData.dt;

	// Выходное сообщение :
	myNavigator->msgOut.len = myNavigator->msgIn.len < msgMaxLen ? myNavigator->msgIn.len : msgMaxLen;
	memcpy(myNavigator->msgOut.msg, myNavigator->msgIn.msg, myNavigator->msgOut.len);

	// Если альфа-бета фильтр :
#if defined( alphaBetaFiltering ) || defined( myNavigator_DEBUG )

	if (myNavigator->filters.alphaBetaFilter.lat.Step == 0 &&
		myNavigator->filters.alphaBetaFilter.lon.Step == 0 &&
		myNavigator->filters.alphaBetaFilter.alt.Step == 0)
	{
		myNavigator->filters.alphaBetaFilter.lat.Tob = 0;
		myNavigator->filters.alphaBetaFilter.lon.Tob = 0;
		myNavigator->filters.alphaBetaFilter.alt.Tob = 0;
	}

	myNavigator->filters.alphaBetaFilter.lat.Tob += myNavigator->msgData.dT;
	myNavigator->filters.alphaBetaFilter.lon.Tob += myNavigator->msgData.dT;
	myNavigator->filters.alphaBetaFilter.alt.Tob += myNavigator->msgData.dT;

#endif

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

	// Проверям, что декодированные координаты принадлежат стробу (не "слишком большие") :
	prePostFilter(myNavigator, true);



	// Фильтрация широты :
#ifdef includeLat

#if defined( myNavigator_DEBUG )

	kalmanFilter(&myNavigator->filters.kalmanFilter.lat, &myNavigator->coordinates.lat);
	myNavigator->coordinates.lat.filteredPos.allFilteredValues[0] = myNavigator->coordinates.lat.filteredPos.value;

	medianFilter(&myNavigator->filters.medianFilter.lat, &myNavigator->coordinates.lat);
	myNavigator->coordinates.lat.filteredPos.allFilteredValues[1] = myNavigator->coordinates.lat.filteredPos.value;

	minQuadFilter(&myNavigator->filters.minQuadFilter.lat, &myNavigator->coordinates.lat);
	myNavigator->coordinates.lat.filteredPos.allFilteredValues[2] = myNavigator->coordinates.lat.filteredPos.value;

	alphaBetaFilter(&myNavigator->filters.alphaBetaFilter.lat, &myNavigator->coordinates.lat);
	myNavigator->coordinates.lat.filteredPos.allFilteredValues[3] = myNavigator->coordinates.lat.filteredPos.value;

#elif defined( myNavigator_RELEASE )

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



	// Фильтрация долготы :
#ifdef includeLon

#if defined( myNavigator_DEBUG )

	kalmanFilter(&myNavigator->filters.kalmanFilter.lon, &myNavigator->coordinates.lon);
	myNavigator->coordinates.lon.filteredPos.allFilteredValues[0] = myNavigator->coordinates.lon.filteredPos.value;

	medianFilter(&myNavigator->filters.medianFilter.lon, &myNavigator->coordinates.lon);
	myNavigator->coordinates.lon.filteredPos.allFilteredValues[1] = myNavigator->coordinates.lon.filteredPos.value;

	minQuadFilter(&myNavigator->filters.minQuadFilter.lon, &myNavigator->coordinates.lon);
	myNavigator->coordinates.lon.filteredPos.allFilteredValues[2] = myNavigator->coordinates.lon.filteredPos.value;

	alphaBetaFilter(&myNavigator->filters.alphaBetaFilter.lon, &myNavigator->coordinates.lon);
	myNavigator->coordinates.lon.filteredPos.allFilteredValues[3] = myNavigator->coordinates.lon.filteredPos.value;

#elif defined( myNavigator_RELEASE )

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



	// Фильтрация высоты :
#ifdef includeAlt

#if defined( myNavigator_DEBUG )

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

#elif defined( myNavigator_RELEASE )

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

	// Проверям, что фильтрованные координаты в принадлежат стробу (не "слишком большие") :
	prePostFilter(myNavigator, false);

	// Преобразование плоских прямоугольных координат в проекции Гаусса-Крюгера 
	// на эллипсоиде Красовского в геодезические координаты :
#ifdef TransformCoords

#if defined( myNavigator_DEBUG )

	decart2geo(&myNavigator->coordinates.lat.filteredPos.allFilteredValues[0], &myNavigator->coordinates.lon.filteredPos.allFilteredValues[0]);
	decart2geo(&myNavigator->coordinates.lat.filteredPos.allFilteredValues[1], &myNavigator->coordinates.lon.filteredPos.allFilteredValues[1]);
	decart2geo(&myNavigator->coordinates.lat.filteredPos.allFilteredValues[2], &myNavigator->coordinates.lon.filteredPos.allFilteredValues[2]);
	decart2geo(&myNavigator->coordinates.lat.filteredPos.allFilteredValues[3], &myNavigator->coordinates.lon.filteredPos.allFilteredValues[3]);

	decart2geo(&myNavigator->coordinates.lat.decodedPos.value, &myNavigator->coordinates.lon.decodedPos.value);
	myNavigator->coordinates.lat.filteredPos.value = myNavigator->coordinates.lat.decodedPos.value;
	myNavigator->coordinates.lon.filteredPos.value = myNavigator->coordinates.lon.decodedPos.value;

#elif defined( myNavigator_RELEASE )

	decart2geo(&myNavigator->coordinates.lat.filteredPos.value, &myNavigator->coordinates.lon.filteredPos.value);

#endif

#endif

	// Запись в постоянную память координат :
#if defined( WriteCoordsInFlash )
	writeInROM(myNavigator);
#endif

	// Перезапись сообщения с новыми координатами :
	changeMsg(myNavigator);

	// Вычисление новой КС для измененного сообщения :
	getXOR(&myNavigator->msgOut.msg[1], &myNavigator->msgOut.msg[myNavigator->msgOut.len - 4], myNavigator->msgOut.len - 6);

	myNavigator->msgData.dT = 0;

	return;
}


void myNavigatorInit(struct MyNavigator* myNavigator, void (*flashFunc)(uint32_t, uint32_t, uint64_t))
{
#ifdef myNavigator_DEBUG
	for (i = 0; i < 4; i++)
	{
		myNavigator->coordinates.lat.filteredPos.allFilteredValues[i] = 0;
		myNavigator->coordinates.lon.filteredPos.allFilteredValues[i] = 0;
		myNavigator->coordinates.alt.filteredPos.allFilteredValues[i] = 0;
	}
#endif

#ifdef WriteCoordsInFlash
	myNavigator->writeInFlash.adress = WriteInFlashStartAdress;
	myNavigator->writeInFlash.flashFunc = flashFunc;
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

	myNavigator->myNavigatorExeTime = 0;

	myNavigator->msgIn.len = 0;
	myNavigator->msgOut.len = 0;
	myNavigator->msgData.dt = 0;
	myNavigator->msgData.dT = 0;
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
