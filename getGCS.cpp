#include "getGCS.h"

extern const double latIntPartLimits[];
extern const double lonIntPartLimits[];
extern const double altIntPartLimits[];

static unsigned short i;

void getGCS(struct MyNavigator* myNavigator)
{
	// ������
#ifdef includeLat

	// �������� ���
	myNavigator->coordinates.lat.decodedPos.value = 0;
	myNavigator->coordinates.lat.decodedPos.length = 0;
	myNavigator->coordinates.lat.decodedPos.floatLength = 0;
	myNavigator->coordinates.lat.decodedPos.intPosition = myNavigator->msgData.parId[myNavigator->msgData.id[0]] + 1;
	myNavigator->coordinates.lat.decodedPos.floatPosition = myNavigator->msgData.parId[myNavigator->msgData.id[0]] + 1;

	// �������� �� ��������, ��� ���� ���������� ������� � ��� ������ ���� ����������
	for (i = myNavigator->coordinates.lat.decodedPos.intPosition;
		i < myNavigator->msgData.parId[myNavigator->msgData.id[0] + 1];
		i++, myNavigator->coordinates.lat.decodedPos.length++)
	{
		// ���� ���������� �����
		if (myNavigator->msgIn.msg[i] == '.')
		{
			myNavigator->coordinates.lat.decodedPos.length--;
			myNavigator->coordinates.lat.decodedPos.floatPosition = i + 1;
			continue;
		}
		// ���� ����� ������ ���������
		else if (i >= msgMaxLen)
		{
			return;
		}
	}

	// ������� ��������������� ����� �� ������ � �������
	myNavigator->coordinates.lat.decodedPos.value = 
		char2double(&myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[0]] + 1],
		myNavigator->coordinates.lat.decodedPos.length);

	// ����� ������� ����� �� 10 � ������� ���� ����� �������
	myNavigator->coordinates.lat.decodedPos.floatLength = myNavigator->msgData.parId[myNavigator->msgData.id[0] + 1] -
		myNavigator->coordinates.lat.decodedPos.floatPosition;
	myNavigator->coordinates.lat.decodedPos.value *= pow(10, -myNavigator->coordinates.lat.decodedPos.floatLength);

	// ���� �� ����� � �� ������, �� ������ ���� �� -
	if (myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[1]] + 1] == 'S')
		myNavigator->coordinates.lat.decodedPos.value = -myNavigator->coordinates.lat.decodedPos.value;

#endif



	// �������
#ifdef includeLon

	// �������� ���
	myNavigator->coordinates.lon.decodedPos.value = 0;
	myNavigator->coordinates.lon.decodedPos.length = 0;
	myNavigator->coordinates.lon.decodedPos.floatLength = 0;
	myNavigator->coordinates.lon.decodedPos.intPosition = myNavigator->msgData.parId[myNavigator->msgData.id[2]] + 1;
	myNavigator->coordinates.lon.decodedPos.floatPosition = myNavigator->msgData.parId[myNavigator->msgData.id[2]] + 1;

	// �������� �� ��������, ��� ���� ���������� ������� � ��� ������ ���� ����������
	for (i = myNavigator->coordinates.lon.decodedPos.intPosition;
		i < myNavigator->msgData.parId[myNavigator->msgData.id[2] + 1];
		i++, myNavigator->coordinates.lon.decodedPos.length++)
	{
		// ���� ���������� �����
		if (myNavigator->msgIn.msg[i] == '.')
		{
			myNavigator->coordinates.lon.decodedPos.length--;
			myNavigator->coordinates.lon.decodedPos.floatPosition = i + 1;
			continue;
		}
		// ���� ����� ������ ���������
		else if (i >= msgMaxLen)
		{
			return;
		}
	}

	// ������� ��������������� ����� �� ������ � �������
	myNavigator->coordinates.lon.decodedPos.value =
		char2double(&myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[2]] + 1],
		myNavigator->coordinates.lon.decodedPos.length);

	// ����� ������� ����� �� 10 � ������� ���� ����� �������
	myNavigator->coordinates.lon.decodedPos.floatLength = myNavigator->msgData.parId[myNavigator->msgData.id[2] + 1] -
		myNavigator->coordinates.lon.decodedPos.floatPosition;
	myNavigator->coordinates.lon.decodedPos.value *= pow(10, -myNavigator->coordinates.lon.decodedPos.floatLength);

	// ���� �� ����� � �� ������, �� ������ ���� �� -
	if (myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[3]] + 1] == 'W')
		myNavigator->coordinates.lon.decodedPos.value = -myNavigator->coordinates.lon.decodedPos.value;	

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
		myNavigator->coordinates.alt.decodedPos.value = 0;
		myNavigator->coordinates.alt.decodedPos.length = 0;
		myNavigator->coordinates.alt.decodedPos.floatLength = 0;
		myNavigator->coordinates.alt.decodedPos.intPosition = myNavigator->msgData.parId[myNavigator->msgData.id[4]] + 1;
		myNavigator->coordinates.alt.decodedPos.floatPosition = myNavigator->msgData.parId[myNavigator->msgData.id[4]] + 1;

		// �������� �� ��������, ��� ���� ���������� ������� � ��� ������ ���� ����������
		for (i = myNavigator->coordinates.alt.decodedPos.intPosition;
			i < myNavigator->msgData.parId[myNavigator->msgData.id[4] + 1];
			i++, myNavigator->coordinates.alt.decodedPos.length++)
		{
			// ���� ���������� �����
			if (myNavigator->msgIn.msg[i] == '.')
			{
				myNavigator->coordinates.alt.decodedPos.length--;
				myNavigator->coordinates.alt.decodedPos.floatPosition = i + 1;
				continue;
			}
			// ���� ����� ������ ���������
			else if (i >= msgMaxLen)
			{
				return;
			}
		}

		// ������� ��������������� ����� �� ������ � �������
		myNavigator->coordinates.alt.decodedPos.value =
			char2double(&myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[4]] + 1],
			myNavigator->coordinates.alt.decodedPos.length);

		// ����� ������� ����� �� 10 � ������� ���� ����� �������
		myNavigator->coordinates.alt.decodedPos.floatLength = myNavigator->msgData.parId[myNavigator->msgData.id[4] + 1] -
			myNavigator->coordinates.alt.decodedPos.floatPosition;
		myNavigator->coordinates.alt.decodedPos.value *= pow(10, -myNavigator->coordinates.alt.decodedPos.floatLength);

	}

#else
	myNavigator->coordinates.alt.decodedPos.value = myNavigator->coordinates.alt.previosPos.value[previosPosLen - 1];
#endif

	return;
}