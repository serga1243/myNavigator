#include "getGCS.h"


extern const double latIntPartLimits[];
extern const double lonIntPartLimits[];
extern const double altIntPartLimits[];


void getGCS(struct MyNavigator* myNavigator, unsigned char logInfo[])
{
	unsigned short i = 0;

	// ������
#ifdef includeLat

	// �������� ���
	logInfo[0] = 6;
	myNavigator->coordinates.lat.decodedPos.value = 0;
	myNavigator->coordinates.lat.decodedPos.length = 0;
	myNavigator->coordinates.lat.decodedPos.floatLength = 0;
	myNavigator->coordinates.lat.decodedPos.intPosition = myNavigator->msgData.parId[myNavigator->msgData.id[0]] + 1;
	myNavigator->coordinates.lat.decodedPos.floatPosition = myNavigator->msgData.parId[myNavigator->msgData.id[0]] + 1;
	logInfo[0] = 5;

	// �������� �� ��������, ��� ���� ���������� ������� � ��� ������ ���� ����������
	for (i = myNavigator->coordinates.lat.decodedPos.intPosition;
		i < myNavigator->msgData.parId[myNavigator->msgData.id[0] + 1];
		i++, myNavigator->coordinates.lat.decodedPos.length++)
	{
		// ���� ���������� �����
		if (myNavigator->msgIn.msg[i] == '.')
		{
			//myNavigator->coordinates.lat.decodedPos.intLength = myNavigator->coordinates.lat.decodedPos.length--;
			myNavigator->coordinates.lat.decodedPos.floatPosition = i + 1;
			logInfo[0] = 4;
			continue;
		}
		// ���� ����� ������ ���������
		else if (i >= msgMaxLen)
		{
			logInfo[0] = 7;
			return;
		}
	}

	// ������� ��������������� ����� �� ������ � �������
	myNavigator->coordinates.lat.decodedPos.value = 
		(double)char2int(&myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[0]] + 1],
		myNavigator->coordinates.lat.decodedPos.length);
	logInfo[0] = 3;

	// ����� ������� ����� �� 10 � ������� ���� ����� �������
	myNavigator->coordinates.lat.decodedPos.floatLength = myNavigator->msgData.parId[myNavigator->msgData.id[0] + 1] -
		myNavigator->coordinates.lat.decodedPos.floatPosition;
	myNavigator->coordinates.lat.decodedPos.value /= (double)intPower(10, myNavigator->coordinates.lat.decodedPos.floatLength);
	logInfo[0] = 2;

	// ���� �� ����� � �� ������, �� ������ ���� �� -
	myNavigator->coordinates.lat.decodedPos.value = 
		myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[1]] + 1] == 'N' ?
		myfabs(myNavigator->coordinates.lat.decodedPos.value) :
		-myNavigator->coordinates.lat.decodedPos.value;
	logInfo[0] = 1;

	// ��������, ��� �������������� ���������� � �������� ���������
	isInvalidData(&myNavigator->coordinates.lat.decodedPos.value, latIntPartLimits, &myNavigator->coordinates.lat.previosPos.value[PreviosPosLen - 1]);
	logInfo[0] = 0;

#else
	myNavigator->coordinates.lat.decodedPos.value = myNavigator->coordinates.lat.previosPos.value[PreviosPosLen - 1];
	logInfo[0] = 0;
#endif



	// �������
#ifdef includeLon

	// �������� ���
	logInfo[1] = 6;
	myNavigator->coordinates.lon.decodedPos.value = 0;
	myNavigator->coordinates.lon.decodedPos.length = 0;
	myNavigator->coordinates.lon.decodedPos.floatLength = 0;
	myNavigator->coordinates.lon.decodedPos.intPosition = myNavigator->msgData.parId[myNavigator->msgData.id[2]] + 1;
	myNavigator->coordinates.lon.decodedPos.floatPosition = myNavigator->msgData.parId[myNavigator->msgData.id[2]] + 1;
	logInfo[1] = 5;

	// �������� �� ��������, ��� ���� ���������� ������� � ��� ������ ���� ����������
	for (i = myNavigator->coordinates.lon.decodedPos.intPosition;
		i < myNavigator->msgData.parId[myNavigator->msgData.id[2] + 1];
		i++, myNavigator->coordinates.lon.decodedPos.length++)
	{
		// ���� ���������� �����
		if (myNavigator->msgIn.msg[i] == '.')
		{
			//myNavigator->coordinates.lon.decodedPos.intLength = myNavigator->coordinates.lon.decodedPos.length--;
			myNavigator->coordinates.lon.decodedPos.floatPosition = i + 1;
			logInfo[1] = 4;
			continue;
		}
		// ���� ����� ������ ���������
		else if (i >= msgMaxLen)
		{
			logInfo[1] = 7;
			return;
		}
	}

	// ������� ��������������� ����� �� ������ � �������
	myNavigator->coordinates.lon.decodedPos.value =
		(double)char2int(&myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[2]] + 1],
		myNavigator->coordinates.lon.decodedPos.length);
	logInfo[1] = 3;

	// ����� ������� ����� �� 10 � ������� ���� ����� �������
	myNavigator->coordinates.lon.decodedPos.floatLength = myNavigator->msgData.parId[myNavigator->msgData.id[2] + 1] -
		myNavigator->coordinates.lon.decodedPos.floatPosition;
	myNavigator->coordinates.lon.decodedPos.value /= (double)intPower(10, myNavigator->coordinates.lon.decodedPos.floatLength);
	logInfo[1] = 2;

	// ���� �� ����� � �� ������, �� ������ ���� �� -
	myNavigator->coordinates.lon.decodedPos.value =
		myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[3]] + 1] == 'E' ?
		myfabs(myNavigator->coordinates.lon.decodedPos.value) :
		-myNavigator->coordinates.lon.decodedPos.value;
	logInfo[1] = 1;

	// ��������, ��� �������������� ���������� � �������� ���������
	isInvalidData(&myNavigator->coordinates.lon.decodedPos.value, lonIntPartLimits, &myNavigator->coordinates.lon.previosPos.value[PreviosPosLen - 1]);
	logInfo[1] = 0;

#else
	myNavigator->coordinates.lon.decodedPos.value = myNavigator->coordinates.lon.previosPos.value[PreviosPosLen - 1];
	logInfo[1] = 0;
#endif




	// ������
#ifdef includeAlt

	if (myNavigator->msgData.id[4] == 0)
	{
		myNavigator->coordinates.alt.decodedPos.value = myNavigator->coordinates.alt.previosPos.value[PreviosPosLen - 1];
		logInfo[2] = 0;
	}
	else
	{
		// �������� ���
		logInfo[2] = 5;
		myNavigator->coordinates.alt.decodedPos.value = 0;
		myNavigator->coordinates.alt.decodedPos.length = 0;
		myNavigator->coordinates.alt.decodedPos.floatLength = 0;
		myNavigator->coordinates.alt.decodedPos.intPosition = myNavigator->msgData.parId[myNavigator->msgData.id[4]] + 1;
		myNavigator->coordinates.alt.decodedPos.floatPosition = myNavigator->msgData.parId[myNavigator->msgData.id[4]] + 1;
		logInfo[2] = 4;

		// �������� �� ��������, ��� ���� ���������� ������� � ��� ������ ���� ����������
		for (i = myNavigator->coordinates.alt.decodedPos.intPosition;
			i < myNavigator->msgData.parId[myNavigator->msgData.id[4] + 1];
			i++, myNavigator->coordinates.alt.decodedPos.length++)
		{
			// ���� ���������� �����
			if (myNavigator->msgIn.msg[i] == '.')
			{
				//myNavigator->coordinates.alt.decodedPos.intLength = myNavigator->coordinates.alt.decodedPos.length--;
				myNavigator->coordinates.alt.decodedPos.floatPosition = i + 1;
				logInfo[2] = 3;
				continue;
			}
			// ���� ����� ������ ���������
			else if (i >= msgMaxLen)
			{
				logInfo[2] = 6;
				return;
			}
		}

		// ������� ��������������� ����� �� ������ � �������
		myNavigator->coordinates.alt.decodedPos.value =
			(double)char2int(&myNavigator->msgIn.msg[myNavigator->msgData.parId[myNavigator->msgData.id[4]] + 1],
			myNavigator->coordinates.alt.decodedPos.length);
		logInfo[2] = 2;

		// ����� ������� ����� �� 10 � ������� ���� ����� �������
		myNavigator->coordinates.alt.decodedPos.floatLength = myNavigator->msgData.parId[myNavigator->msgData.id[4] + 1] -
			myNavigator->coordinates.alt.decodedPos.floatPosition;
		myNavigator->coordinates.alt.decodedPos.value /= (double)intPower(10, myNavigator->coordinates.alt.decodedPos.floatLength);
		logInfo[2] = 1;

		// ��������, ��� �������������� ���������� � �������� ���������
		isInvalidData(&myNavigator->coordinates.alt.decodedPos.value, altIntPartLimits, &myNavigator->coordinates.alt.previosPos.value[PreviosPosLen - 1]);
		logInfo[2] = 0;
	}

#else
	myNavigator->coordinates.alt.decodedPos.value = myNavigator->coordinates.alt.previosPos.value[PreviosPosLen - 1];
	logInfo[2] = 0;
#endif

	return;
}