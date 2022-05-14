#include "changeMsg.h"
//#include <iostream>

void changeMsg(struct MyNavigator* myNavigator, unsigned char logInfo[])
{

	// Широта
#ifdef includeLat
	logInfo[0] = 8;
	overwriteStr(&myNavigator->coordinates.lat, myNavigator->msgOut.msg, myNavigator->msgData.parId[myNavigator->msgData.id[1]], &logInfo[0]);
	logInfo[0] = 1;

	// Меняем направление координаты в зависимости от знака координаты
	if (myNavigator->coordinates.lat.filteredPos.value < 0)
	{
		switch (myNavigator->msgOut.msg[myNavigator->msgData.parId[myNavigator->msgData.id[1]] + 1])
		{
		case 'N':
			myNavigator->msgOut.msg[myNavigator->msgData.parId[myNavigator->msgData.id[1]] + 1] = 'S';
			break;
		case 'S':
			myNavigator->msgOut.msg[myNavigator->msgData.parId[myNavigator->msgData.id[1]] + 1] = 'N';
			break;
		default:
			break;
		}
	}
	logInfo[0] = 0;
#else
	logInfo[0] = 0;
#endif


	// Долгота
#ifdef includeLon
	logInfo[1] = 8;
	overwriteStr(&myNavigator->coordinates.lon, myNavigator->msgOut.msg, myNavigator->msgData.parId[myNavigator->msgData.id[3]], &logInfo[1]);
	logInfo[1] = 1;

	// Меняем направление координаты в зависимости от знака координаты
	if (myNavigator->coordinates.lon.filteredPos.value < 0)
	{
		switch (myNavigator->msgOut.msg[myNavigator->msgData.parId[myNavigator->msgData.id[3]] + 1])
		{
		case 'E':
			myNavigator->msgOut.msg[myNavigator->msgData.parId[myNavigator->msgData.id[3]] + 1] = 'W';
			break;
		case 'W':
			myNavigator->msgOut.msg[myNavigator->msgData.parId[myNavigator->msgData.id[3]] + 1] = 'E';
			break;
		default:
			break;
		}
	}
	logInfo[1] = 0;
#else
	logInfo[1] = 0;
#endif


	// Высота
#ifdef includeAlt
	if (myNavigator->msgData.id[4] == 0)
	{
		logInfo[2] = 0;
	}
	else
	{
		logInfo[2] = 8;
		overwriteStr(&myNavigator->coordinates.alt, myNavigator->msgOut.msg, myNavigator->msgData.parId[myNavigator->msgData.id[5]], &logInfo[2]);
		logInfo[2] = 1;
	}
#else
	logInfo[2] = 0;
#endif
	
	return;
}


void overwriteStr(struct Coordinate* coordinate, char str[], unsigned short endPos, unsigned char* logInfo)
{
	unsigned short i = 0;
	unsigned short j = 0;
	unsigned short k = 0;
	short l = 0;
	double valueIntPart = 0;
	double valuefloatPartBig = 0;
	double valuefloatPartSmall = 0;
	char dataBufferInt[] = { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };
	char dataBufferFlo[] = { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };
	*logInfo = 7;

	valuefloatPartSmall = modf(myfabs(coordinate->filteredPos.value), &valueIntPart);
	*logInfo = 6;
	valuefloatPartBig = valuefloatPartSmall * pow(10, coordinate->decodedPos.floatLength + 2);
	*logInfo = 5;

	// Целая часть числа
	_itoa_s((int)valueIntPart, dataBufferInt, 12, 10);
	*logInfo = 4;
	coordinate->filteredPos.intLength = getNumLen(dataBufferInt);
	*logInfo = 3;

	// Дробная часть числа
	_itoa_s((int)valuefloatPartBig, dataBufferFlo, 12, 10);
	*logInfo = 2;
	coordinate->filteredPos.floatLength = getNumLen(dataBufferFlo) - 2;
	*logInfo = 1;

	for (i = coordinate->decodedPos.intPosition;
		i < endPos;
		i++)
	{
		if (str[i] == ',')
		{
			break;
		}
		else if (j < coordinate->filteredPos.intLength)
		{
			str[i] = dataBufferInt[j++];
			continue;
		}
		else if (j++ == coordinate->filteredPos.intLength)
		{
			str[i] = '.';
			continue;
		}
		else if (k < coordinate->filteredPos.floatLength)
		{
			if (valuefloatPartSmall < pow(10, (double)--l))
			{
				str[i] = '0';
				continue;
			}
			else
			{
				str[i] = dataBufferFlo[k++];
				continue;
			}
		}
		else 
		{
			str[i] = '0';
			continue;
		}

		if (i >= msgMaxLen) break;
	}
	*logInfo = 0;
}


unsigned short getNumLen(char dataBuffer[])
{
	unsigned short len = 0;
	for (unsigned short i = 0; i < 12; i++)
	{
		switch (dataBuffer[i])
		{
		case '0':
			len = i + 1;
			break;

		case '1':
			len = i + 1;
			break;

		case '2':
			len = i + 1;
			break;

		case '3':
			len = i + 1;
			break;

		case '4':
			len = i + 1;
			break;

		case '5':
			len = i + 1;
			break;

		case '6':
			len = i + 1;
			break;

		case '7':
			len = i + 1;
			break;

		case '8':
			len = i + 1;
			break;

		case '9':
			len = i + 1;
			break;

		default:
			i = 14;
			break;
		}
	}

	return len;
}