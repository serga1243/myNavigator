#include "getGCS.h"

extern const double latIntPartLimits[];
extern const double lonIntPartLimits[];
extern const double altIntPartLimits[];

static uint16_t i;

void getGCS(struct MyNavigator* myNavigator)
{
	// ������
#ifdef includeLat

	// �������� ���
	myNavigator->coordinates.lat.decodedPos.value = 0.0;
	myNavigator->coordinates.lat.decodedPos.length = 0;
	myNavigator->coordinates.lat.decodedPos.floatLength = 0;
	myNavigator->coordinates.lat.decodedPos.intPosition = myNavigator->msgData.parId[myNavigator->msgData.id[0]] + 1;
	myNavigator->coordinates.lat.decodedPos.floatPosition = myNavigator->coordinates.lat.decodedPos.intPosition;

	// �������� �� ��������, ��� ���� ���������� ������� � ��� ������ ���� ����������
	for (i = myNavigator->coordinates.lat.decodedPos.intPosition;
		i < myNavigator->msgData.parId[myNavigator->msgData.id[0] + 1];
		i++, myNavigator->coordinates.lat.decodedPos.length++)
	{
		// ���� ����� ������ ���������
		if (i >= msgMaxLen) { break; }

		// ���� ���������� �����
		if (myNavigator->msgIn.msg[i] == '.')
		{
			myNavigator->coordinates.lat.decodedPos.length--;
			myNavigator->coordinates.lat.decodedPos.floatPosition = i + 1;
			continue;
		}
	}

	// ������� ��������������� ����� �� ������ � �������
	myNavigator->coordinates.lat.decodedPos.value = 
		char2double(&myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[0]] + 1],
		myNavigator->coordinates.lat.decodedPos.length);

	// ����� ������� ����� �� 10 � ������� ���� ����� �������
	myNavigator->coordinates.lat.decodedPos.floatLength = myNavigator->msgData.parId[myNavigator->msgData.id[0] + 1] -
		myNavigator->coordinates.lat.decodedPos.floatPosition;

	if (myNavigator->coordinates.lat.decodedPos.floatLength < 0)
		myNavigator->coordinates.lat.decodedPos.floatLength = 0;

	myNavigator->coordinates.lat.decodedPos.value *= pow(10.0, -myNavigator->coordinates.lat.decodedPos.floatLength);

	// ���� �� �����, �� ������ ���� �� -
	switch (myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[1]] + 1])
	{
	case 'N':
		break;

	case 'S':
		myNavigator->coordinates.lat.decodedPos.value = -myNavigator->coordinates.lat.decodedPos.value;
		break;

	default:
		myNavigator->coordinates.lat.decodedPos.value = myNavigator->coordinates.lat.previosPos.value[previosPosLen - 1];
		break;
	}

#endif



	// �������
#ifdef includeLon

	// �������� ���
	myNavigator->coordinates.lon.decodedPos.value = 0.0;
	myNavigator->coordinates.lon.decodedPos.length = 0;
	myNavigator->coordinates.lon.decodedPos.floatLength = 0;
	myNavigator->coordinates.lon.decodedPos.intPosition = myNavigator->msgData.parId[myNavigator->msgData.id[2]] + 1;
	myNavigator->coordinates.lon.decodedPos.floatPosition = myNavigator->coordinates.lon.decodedPos.intPosition;

	// �������� �� ��������, ��� ���� ���������� ������� � ��� ������ ���� ����������
	for (i = myNavigator->coordinates.lon.decodedPos.intPosition;
		i < myNavigator->msgData.parId[myNavigator->msgData.id[2] + 1];
		i++, myNavigator->coordinates.lon.decodedPos.length++)
	{
		// ���� ����� ������ ���������
		if (i >= msgMaxLen) { break; }

		// ���� ���������� �����
		if (myNavigator->msgIn.msg[i] == '.')
		{
			myNavigator->coordinates.lon.decodedPos.length--;
			myNavigator->coordinates.lon.decodedPos.floatPosition = i + 1;
			continue;
		}
	}

	// ������� ��������������� ����� �� ������ � �������
	myNavigator->coordinates.lon.decodedPos.value =
		char2double(&myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[2]] + 1],
		myNavigator->coordinates.lon.decodedPos.length);

	// ����� ������� ����� �� 10 � ������� ���� ����� �������
	myNavigator->coordinates.lon.decodedPos.floatLength = myNavigator->msgData.parId[myNavigator->msgData.id[2] + 1] -
		myNavigator->coordinates.lon.decodedPos.floatPosition;

	if (myNavigator->coordinates.lon.decodedPos.floatLength < 0)
		myNavigator->coordinates.lon.decodedPos.floatLength = 0;

	myNavigator->coordinates.lon.decodedPos.value *= pow(10.0, -myNavigator->coordinates.lon.decodedPos.floatLength);

	// ���� �� ������, �� ������ ���� �� -
	switch (myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[3]] + 1])
	{
	case 'E':
		break;

	case 'W':
		myNavigator->coordinates.lat.decodedPos.value = -myNavigator->coordinates.lat.decodedPos.value;
		break;

	default:
		myNavigator->coordinates.lat.decodedPos.value = myNavigator->coordinates.lat.previosPos.value[previosPosLen - 1];
		break;
	}

#endif




	// ������
#ifdef includeAlt

	if (myNavigator->msgData.id[4] == 0)
	{
		myNavigator->coordinates.alt.decodedPos.value = myNavigator->coordinates.alt.previosPos.value[previosPosLen - 1];
	}
	else
	{
		// �������� ���
		myNavigator->coordinates.alt.decodedPos.value = 0.0;
		myNavigator->coordinates.alt.decodedPos.length = 0;
		myNavigator->coordinates.alt.decodedPos.floatLength = 0;
		myNavigator->coordinates.alt.decodedPos.intPosition = myNavigator->msgData.parId[myNavigator->msgData.id[4]] + 1;
		myNavigator->coordinates.alt.decodedPos.floatPosition = myNavigator->coordinates.alt.decodedPos.intPosition;

		// �������� �� ��������, ��� ���� ���������� ������� � ��� ������ ���� ����������
		for (i = myNavigator->coordinates.alt.decodedPos.intPosition;
			i < myNavigator->msgData.parId[myNavigator->msgData.id[4] + 1];
			i++, myNavigator->coordinates.alt.decodedPos.length++)
		{
			// ���� ����� ������ ���������
			if (i >= msgMaxLen) { break; }

			// ���� ���������� �����
			if (myNavigator->msgIn.msg[i] == '.')
			{
				myNavigator->coordinates.alt.decodedPos.length--;
				myNavigator->coordinates.alt.decodedPos.floatPosition = i + 1;
				continue;
			}
		}

		// ������� ��������������� ����� �� ������ � �������
		myNavigator->coordinates.alt.decodedPos.value =
			char2double(&myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[4]] + 1],
			myNavigator->coordinates.alt.decodedPos.length);

		// ����� ������� ����� �� 10 � ������� ���� ����� �������
		myNavigator->coordinates.alt.decodedPos.floatLength = myNavigator->msgData.parId[myNavigator->msgData.id[4] + 1] -
			myNavigator->coordinates.alt.decodedPos.floatPosition;

		if (myNavigator->coordinates.alt.decodedPos.floatLength < 0)
			myNavigator->coordinates.alt.decodedPos.floatLength = 0;

		myNavigator->coordinates.alt.decodedPos.value *= pow(10.0, -myNavigator->coordinates.alt.decodedPos.floatLength);

	}

#else
	myNavigator->coordinates.alt.decodedPos.value = myNavigator->coordinates.alt.previosPos.value[previosPosLen - 1];
#endif

	return;
}