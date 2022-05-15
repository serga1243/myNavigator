#include "getGCS.h"


extern const double latIntPartLimits[];
extern const double lonIntPartLimits[];
extern const double altIntPartLimits[];


void getGCS(struct MyNavigator* myNavigator, unsigned char logInfo[])
{
	unsigned short i = 0;

	// Широта
#ifdef includeLat

	// Обнуляем все
	logInfo[0] = 6;
	myNavigator->coordinates.lat.decodedPos.value = 0;
	myNavigator->coordinates.lat.decodedPos.length = 0;
	myNavigator->coordinates.lat.decodedPos.floatLength = 0;
	myNavigator->coordinates.lat.decodedPos.intPosition = myNavigator->msgData.parId[myNavigator->msgData.id[0]] + 1;
	myNavigator->coordinates.lat.decodedPos.floatPosition = myNavigator->msgData.parId[myNavigator->msgData.id[0]] + 1;
	logInfo[0] = 5;

	// Проходим по символам, где были обнаружены запятые и где должна быть координата
	for (i = myNavigator->coordinates.lat.decodedPos.intPosition;
		i < myNavigator->msgData.parId[myNavigator->msgData.id[0] + 1];
		i++, myNavigator->coordinates.lat.decodedPos.length++)
	{
		// Если обнаружили точку
		if (myNavigator->msgIn.msg[i] == '.')
		{
			//myNavigator->coordinates.lat.decodedPos.intLength = myNavigator->coordinates.lat.decodedPos.length--;
			myNavigator->coordinates.lat.decodedPos.floatPosition = i + 1;
			logInfo[0] = 4;
			continue;
		}
		// Если пошли дальше сообщения
		else if (i >= msgMaxLen)
		{
			logInfo[0] = 7;
			return;
		}
	}

	// Перевод декодированного числа из целого в дробное
	myNavigator->coordinates.lat.decodedPos.value = 
		(double)char2int(&myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[0]] + 1],
		myNavigator->coordinates.lat.decodedPos.length);
	logInfo[0] = 3;

	// Делим дробное число на 10 в степени цифр после запятой
	myNavigator->coordinates.lat.decodedPos.floatLength = myNavigator->msgData.parId[myNavigator->msgData.id[0] + 1] -
		myNavigator->coordinates.lat.decodedPos.floatPosition;
	myNavigator->coordinates.lat.decodedPos.value /= (double)intPower(10, myNavigator->coordinates.lat.decodedPos.floatLength);
	logInfo[0] = 2;

	// Если не север и не восток, то меняем знак на -
	myNavigator->coordinates.lat.decodedPos.value = 
		myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[1]] + 1] == 'N' ?
		myfabs(myNavigator->coordinates.lat.decodedPos.value) :
		-myNavigator->coordinates.lat.decodedPos.value;
	logInfo[0] = 1;

	// Проверям, что декодированная координата в реальном диапазоне
	isInvalidData(&myNavigator->coordinates.lat.decodedPos.value, latIntPartLimits, &myNavigator->coordinates.lat.previosPos.value[PreviosPosLen - 1]);
	logInfo[0] = 0;

#else
	myNavigator->coordinates.lat.decodedPos.value = myNavigator->coordinates.lat.previosPos.value[PreviosPosLen - 1];
	logInfo[0] = 0;
#endif



	// Долгота
#ifdef includeLon

	// Обнуляем все
	logInfo[1] = 6;
	myNavigator->coordinates.lon.decodedPos.value = 0;
	myNavigator->coordinates.lon.decodedPos.length = 0;
	myNavigator->coordinates.lon.decodedPos.floatLength = 0;
	myNavigator->coordinates.lon.decodedPos.intPosition = myNavigator->msgData.parId[myNavigator->msgData.id[2]] + 1;
	myNavigator->coordinates.lon.decodedPos.floatPosition = myNavigator->msgData.parId[myNavigator->msgData.id[2]] + 1;
	logInfo[1] = 5;

	// Проходим по символам, где были обнаружены запятые и где должна быть координата
	for (i = myNavigator->coordinates.lon.decodedPos.intPosition;
		i < myNavigator->msgData.parId[myNavigator->msgData.id[2] + 1];
		i++, myNavigator->coordinates.lon.decodedPos.length++)
	{
		// Если обнаружили точку
		if (myNavigator->msgIn.msg[i] == '.')
		{
			//myNavigator->coordinates.lon.decodedPos.intLength = myNavigator->coordinates.lon.decodedPos.length--;
			myNavigator->coordinates.lon.decodedPos.floatPosition = i + 1;
			logInfo[1] = 4;
			continue;
		}
		// Если пошли дальше сообщения
		else if (i >= msgMaxLen)
		{
			logInfo[1] = 7;
			return;
		}
	}

	// Перевод декодированного числа из целого в дробное
	myNavigator->coordinates.lon.decodedPos.value =
		(double)char2int(&myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[2]] + 1],
		myNavigator->coordinates.lon.decodedPos.length);
	logInfo[1] = 3;

	// Делим дробное число на 10 в степени цифр после запятой
	myNavigator->coordinates.lon.decodedPos.floatLength = myNavigator->msgData.parId[myNavigator->msgData.id[2] + 1] -
		myNavigator->coordinates.lon.decodedPos.floatPosition;
	myNavigator->coordinates.lon.decodedPos.value /= (double)intPower(10, myNavigator->coordinates.lon.decodedPos.floatLength);
	logInfo[1] = 2;

	// Если не север и не восток, то меняем знак на -
	myNavigator->coordinates.lon.decodedPos.value =
		myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[3]] + 1] == 'E' ?
		myfabs(myNavigator->coordinates.lon.decodedPos.value) :
		-myNavigator->coordinates.lon.decodedPos.value;
	logInfo[1] = 1;

	// Проверям, что декодированная координата в реальном диапазоне
	isInvalidData(&myNavigator->coordinates.lon.decodedPos.value, lonIntPartLimits, &myNavigator->coordinates.lon.previosPos.value[PreviosPosLen - 1]);
	logInfo[1] = 0;

#else
	myNavigator->coordinates.lon.decodedPos.value = myNavigator->coordinates.lon.previosPos.value[PreviosPosLen - 1];
	logInfo[1] = 0;
#endif




	// Высота
#ifdef includeAlt

	if (myNavigator->msgData.id[4] == 0)
	{
		myNavigator->coordinates.alt.decodedPos.value = myNavigator->coordinates.alt.previosPos.value[PreviosPosLen - 1];
		logInfo[2] = 0;
	}
	else
	{
		// Обнуляем все
		logInfo[2] = 5;
		myNavigator->coordinates.alt.decodedPos.value = 0;
		myNavigator->coordinates.alt.decodedPos.length = 0;
		myNavigator->coordinates.alt.decodedPos.floatLength = 0;
		myNavigator->coordinates.alt.decodedPos.intPosition = myNavigator->msgData.parId[myNavigator->msgData.id[4]] + 1;
		myNavigator->coordinates.alt.decodedPos.floatPosition = myNavigator->msgData.parId[myNavigator->msgData.id[4]] + 1;
		logInfo[2] = 4;

		// Проходим по символам, где были обнаружены запятые и где должна быть координата
		for (i = myNavigator->coordinates.alt.decodedPos.intPosition;
			i < myNavigator->msgData.parId[myNavigator->msgData.id[4] + 1];
			i++, myNavigator->coordinates.alt.decodedPos.length++)
		{
			// Если обнаружили точку
			if (myNavigator->msgIn.msg[i] == '.')
			{
				//myNavigator->coordinates.alt.decodedPos.intLength = myNavigator->coordinates.alt.decodedPos.length--;
				myNavigator->coordinates.alt.decodedPos.floatPosition = i + 1;
				logInfo[2] = 3;
				continue;
			}
			// Если пошли дальше сообщения
			else if (i >= msgMaxLen)
			{
				logInfo[2] = 6;
				return;
			}
		}

		// Перевод декодированного числа из целого в дробное
		myNavigator->coordinates.alt.decodedPos.value =
			(double)char2int(&myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[4]] + 1],
			myNavigator->coordinates.alt.decodedPos.length);
		logInfo[2] = 2;

		// Делим дробное число на 10 в степени цифр после запятой
		myNavigator->coordinates.alt.decodedPos.floatLength = myNavigator->msgData.parId[myNavigator->msgData.id[4] + 1] -
			myNavigator->coordinates.alt.decodedPos.floatPosition;
		myNavigator->coordinates.alt.decodedPos.value /= (double)intPower(10, myNavigator->coordinates.alt.decodedPos.floatLength);
		logInfo[2] = 1;

		// Проверям, что декодированная координата в реальном диапазоне
		isInvalidData(&myNavigator->coordinates.alt.decodedPos.value, altIntPartLimits, &myNavigator->coordinates.alt.previosPos.value[PreviosPosLen - 1]);
		logInfo[2] = 0;
	}

#else
	myNavigator->coordinates.alt.decodedPos.value = myNavigator->coordinates.alt.previosPos.value[PreviosPosLen - 1];
	logInfo[2] = 0;
#endif

	return;
}