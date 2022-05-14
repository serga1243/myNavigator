#include "getXOR.h"

void getXOR(char data[], char XORarray[], unsigned short length, unsigned char* logInfo)
{
	char dataArr[] = { '\0', '\0', '\0', '\0' };
	unsigned char hash = 0x00;
	unsigned short i = 0;
	*logInfo = 5;
	// Вычисление хэш-функции
	while (length--) {
		hash ^= data[i];
		i++;
	}
	*logInfo = 4;

	// Получаем КС
	_itoa_s(hash, dataArr, 4, 16);
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
		XORarray[0] = '0';
		XORarray[1] = dataArr[0];
	}
	else
	{
		XORarray[0] = dataArr[0];
		XORarray[1] = dataArr[1];
	}
	*logInfo = 2;

	// Исправляем регистр 1 символа
	switch (XORarray[0])
	{
	case 'a':
		XORarray[0] = 'A';
		break;

	case 'b':
		XORarray[0] = 'B';
		break;

	case 'c':
		XORarray[0] = 'C';
		break;

	case 'd':
		XORarray[0] = 'D';
		break;

	case 'e':
		XORarray[0] = 'E';
		break;

	case 'f':
		XORarray[0] = 'F';
		break;

	default:
		break;
	}
	*logInfo = 1;

	// Исправляем регистр 2 символа
	switch (XORarray[1])
	{
	case 'a':
		XORarray[1] = 'A';
		break;

	case 'b':
		XORarray[1] = 'B';
		break;

	case 'c':
		XORarray[1] = 'C';
		break;

	case 'd':
		XORarray[1] = 'D';
		break;

	case 'e':
		XORarray[1] = 'E';
		break;

	case 'f':
		XORarray[1] = 'F';
		break;

	default:
		break;
	}
	*logInfo = 0;

	return;
}