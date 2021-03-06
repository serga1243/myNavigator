#include "changeMsg.h"

static unsigned short i;
static unsigned short j;
static unsigned short k;
static short l;
static short len;
static double valueIntPart;
static double valuefloatPartBig;
static double valuefloatPartSmall;
static char dataBufferInt[12];
static char dataBufferFlo[12];

void changeMsg(struct MyNavigator* myNavigator)
{

	// ??????
#ifdef includeLat
	overwriteStr(&myNavigator->coordinates.lat, myNavigator->msgIn.msg, myNavigator->msgData.parId[myNavigator->msgData.id[0] + 1]);

	// ?????? ??????????? ?????????? ? ??????????? ?? ????? ??????????
	if (myNavigator->coordinates.lat.filteredPos.value < 0.0)
	{
		switch (myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[1]] + 1])
		{
		case 'N':
			myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[1]] + 1] = 'S';
			break;
		case 'S':
			myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[1]] + 1] = 'N';
			break;
		default:
			break;
		}
	}
#endif


	// ???????
#ifdef includeLon
	overwriteStr(&myNavigator->coordinates.lon, myNavigator->msgIn.msg, myNavigator->msgData.parId[myNavigator->msgData.id[2] + 1]);

	// ?????? ??????????? ?????????? ? ??????????? ?? ????? ??????????
	if (myNavigator->coordinates.lon.filteredPos.value < 0.0)
	{
		switch (myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[3]] + 1])
		{
		case 'E':
			myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[3]] + 1] = 'W';
			break;
		case 'W':
			myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[3]] + 1] = 'E';
			break;
		default:
			break;
		}
	}
#endif


	// ??????
#ifdef includeAlt
	if (myNavigator->msgData.id[4] != 0)
	{
		overwriteStr(&myNavigator->coordinates.alt, myNavigator->msgIn.msg, myNavigator->msgData.parId[myNavigator->msgData.id[4] + 1]);
	}
#endif
	
	return;
}


void overwriteStr(struct Coordinate* coordinate, char str[], unsigned short endPos)
{
	i = 0;
	j = 0;
	k = 0;
	l = 0;

	valuefloatPartSmall = modf(myfabs(coordinate->filteredPos.value), &valueIntPart);
	valuefloatPartBig = valuefloatPartSmall * 1000000000.0;

	// ????? ????? ?????
#ifdef Cpp
	_itoa_s((int)valueIntPart, dataBufferInt, 12, 10);
#else
	itoa((int)valueIntPart, dataBufferInt, 10);
#endif
	coordinate->filteredPos.intLength = getNumLen(dataBufferInt);

	// ??????? ????? ?????
#ifdef Cpp
	_itoa_s((int)valuefloatPartBig, dataBufferFlo, 12, 10);
#else
	itoa((int)valuefloatPartBig, dataBufferFlo, 10);
#endif
	coordinate->filteredPos.floatLength = getNumLen(dataBufferFlo);

	for (i = coordinate->decodedPos.intPosition;
		i < endPos;
		i++)
	{
		if (i >= msgMaxLen) break;

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
			if (valuefloatPartSmall < pow(10.0, (double)--l))
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
	}
}


inline unsigned short getNumLen(char dataBuffer[])
{
	len = 0;
	for (i = 0; i < 12; i++)
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