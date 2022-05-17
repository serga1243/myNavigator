#include <iostream>
#include "myNavigator.h"

#define MSG true
using namespace std;


int main()
{
	setlocale(LC_ALL, "Russian_Russia.1251");
	bool isRestart = true;
	char buf[msgMaxLen] = "", * p = NULL;

	 //���������� ������� :
	struct MyNavigator mynavigator;

	while (true)
	{
		// ����� ������ ������ :
		fflush(stdout);

		if (isRestart)
		{
			// ������������� ������� ���������� :
			myNavigatorInit(&mynavigator);

			// ������������� ���������� ������� :
			medianFilterInit(&mynavigator.filters.medianFilter.lat, startCoordinates[0]);
			medianFilterInit(&mynavigator.filters.medianFilter.lon, startCoordinates[1]);
			medianFilterInit(&mynavigator.filters.medianFilter.alt, startCoordinates[2]);

			// �������������� ������� ������� :
			kalmanFilterInit(&mynavigator.filters.kalmanFilter.lat, startCoordinates[0], kalmanFilterR[0]);
			kalmanFilterInit(&mynavigator.filters.kalmanFilter.lon, startCoordinates[1], kalmanFilterR[1]);
			kalmanFilterInit(&mynavigator.filters.kalmanFilter.alt, startCoordinates[2], kalmanFilterR[2]);

			// ������������� ������� ������� :
			minQuadFilterInit(&mynavigator.filters.minQuadFilter.lat);
			minQuadFilterInit(&mynavigator.filters.minQuadFilter.lon);
			minQuadFilterInit(&mynavigator.filters.minQuadFilter.alt);

			// ������������� �����-���� ������� :
			alphaBetaFilterInit(&mynavigator.filters.alphaBetaFilter.lat, alphaBetaFilterT[0], alphaBetaFilterSp[0], alphaBetaFilterSn[0]);
			alphaBetaFilterInit(&mynavigator.filters.alphaBetaFilter.lon, alphaBetaFilterT[1], alphaBetaFilterSp[1], alphaBetaFilterSn[1]);
			alphaBetaFilterInit(&mynavigator.filters.alphaBetaFilter.alt, alphaBetaFilterT[2], alphaBetaFilterSp[2], alphaBetaFilterSn[2]);

			isRestart = false;
		}

#ifdef MSG
		cout << "�� ������: " << ends;
#endif // DEBUG

		p = fgets(buf, msgMaxLen, stdin);
		if (p) {
			// ������� ������ �������� ������ :
			size_t len = strlen(buf);
			if (len != 0 && buf[len - 1] == '\n') {
				buf[len - 1] = '\0';
			}

			if (strncmp(buf, "stop", 4) == 0)
			{
				break;
			}
			if (strncmp(buf, "restart", 7) == 0)
			{
				cout << endl;
				isRestart = true;
				continue;
			}
			// ���������� ������ � ������� ������ :
			memcpy(mynavigator.msgIn.msg, buf, len);
			mynavigator.msgIn.len = (unsigned int)len;
			mynavigator.msgIn.msg[mynavigator.msgIn.len] = '\n';
			mynavigator.msgIn.msg[mynavigator.msgIn.len - 1] = '\r';
			mynavigator.msgIn.len++;

			myNavigator(&mynavigator);

#ifdef MSG // MSG
			cout << "� �������: " << ends;
#endif
			for (unsigned int i = 0; i < mynavigator.msgOut.len; i++)
			{
				cout << mynavigator.msgOut.msg[i] << ends;
			}
#ifdef MSG // MSG
			cout << "����     : " << ends;
			for (unsigned int i = 0; i < logInfoArrLen; i++)
			{
				cout << (int)mynavigator.logInfo[i] << " " << ends;
			}
			cout << "\n" << endl;
#endif
		}
#ifdef MSG
		else puts("�������� ������");
#endif // MSG
	}

	// �����
	return 0;
}









































