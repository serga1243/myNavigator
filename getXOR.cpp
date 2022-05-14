#include "getXOR.h"

void getXOR(struct MyNavigator* myNavigator, unsigned char* logInfo)
{
	*logInfo = 6;
	unsigned short length = myNavigator->msgIn.len - 6;
	char dataArr[] = { '\0', '\0', '\0', '\0' };
	unsigned char hash = 0x00;
	unsigned short i = 1;
	*logInfo = 5;

	// Вычисление хэш-функции
	while (length--) 
	{
		hash ^= myNavigator->msgIn.msg[i];
		i++;
	}
	*logInfo = 4;

	// Получаем КС
#ifdef Cpp
	_itoa_s(hash, dataArr, 4, 16);
#else
	itoa(hash, dataArr, 16);
#endif
	*logInfo = 3;

	// Перезаписываем массив с КС
	if (dataArr[1] != '0' &&
		dataArr[1] != '1' &&
		dataArr[1] != '2' &&
		dataArr[1] != '3' &&
		dataArr[1] != '4' &&
		dataArr[1] != '5' &&
		dataArr[1] != '6' &&
		dataArr[1] != '7' &&
		dataArr[1] != '8' &&
		dataArr[1] != '9' &&
		dataArr[1] != 'a' &&
		dataArr[1] != 'b' &&
		dataArr[1] != 'c' &&
		dataArr[1] != 'd' &&
		dataArr[1] != 'e' &&
		dataArr[1] != 'f')
	{
		myNavigator->msgData.checkDataCond[0] = '0';
		myNavigator->msgData.checkDataCond[1] = dataArr[0];
	}
	else
	{
		myNavigator->msgData.checkDataCond[0] = dataArr[0];
		myNavigator->msgData.checkDataCond[1] = dataArr[1];
	}
	*logInfo = 2;

	// Исправляем регистр 1 символа
	switch (myNavigator->msgData.checkDataCond[0])
	{
	case 'a':
		myNavigator->msgData.checkDataCond[0] = 'A';
		break;

	case 'b':
		myNavigator->msgData.checkDataCond[0] = 'B';
		break;

	case 'c':
		myNavigator->msgData.checkDataCond[0] = 'C';
		break;

	case 'd':
		myNavigator->msgData.checkDataCond[0] = 'D';
		break;

	case 'e':
		myNavigator->msgData.checkDataCond[0] = 'E';
		break;

	case 'f':
		myNavigator->msgData.checkDataCond[0] = 'F';
		break;

	default:
		break;
	}
	*logInfo = 1;

	// Исправляем регистр 2 символа
	switch (myNavigator->msgData.checkDataCond[1])
	{
	case 'a':
		myNavigator->msgData.checkDataCond[1] = 'A';
		break;

	case 'b':
		myNavigator->msgData.checkDataCond[1] = 'B';
		break;

	case 'c':
		myNavigator->msgData.checkDataCond[1] = 'C';
		break;

	case 'd':
		myNavigator->msgData.checkDataCond[1] = 'D';
		break;

	case 'e':
		myNavigator->msgData.checkDataCond[1] = 'E';
		break;

	case 'f':
		myNavigator->msgData.checkDataCond[1] = 'F';
		break;

	default:
		break;
	}
	*logInfo = 0;

	return;
}