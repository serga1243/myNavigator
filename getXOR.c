#include "getXOR.h"

static uint8_t dataArr[4];
//static uint8_t hash;
//static uint16_t i;

void getXOR(uint8_t data[], uint8_t XORarray[], uint16_t length)
{
	/*
	hash = 0x00;
	i = 0;

	while (length--) {
		hash ^= data[i++];
	}
	*/
	register uint8_t hash = 0x00;
	register uint8_t *i = data;
	register uint16_t len = length;

	while (len--) { hash ^= *(i++); }

	// �������� ��
#ifdef Cpp
	_itoa_s(hash, (char*)dataArr, 4, 16);
#else
	itoa(hash, (char*)dataArr, 16);
#endif

	// �������������� ������ � ��
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

	// ���������� ������� 1 �������
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

	// ���������� ������� 2 �������
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

	return;
}
