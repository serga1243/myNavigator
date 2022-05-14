#include <iostream>
#include "myNavigator.h"

#define MSG true
using namespace std;


int main()
{
	setlocale(LC_ALL, "Russian_Russia.1251");
	bool isRestart = true;
	char buf[msgMaxLen] = "", * p = NULL;

	 //Объявление фильтра :
	struct MyNavigator mynavigator;

	while (true)
	{
		// Сброс буфера вывода :
		fflush(stdout);

		if (isRestart)
		{
			// Инициализация функции фильтрации :
			myNavigatorInit(&mynavigator);

			// Инициализация медианного фильтра :
			medianFilterInit(&mynavigator.filters.medianFilter.lat, startCoordinates[0]);
			medianFilterInit(&mynavigator.filters.medianFilter.lon, startCoordinates[1]);
			medianFilterInit(&mynavigator.filters.medianFilter.alt, startCoordinates[2]);

			// Инициализацияя фильтра Калмана :
			kalmanFilterInit(&mynavigator.filters.kalmanFilter.lat, startCoordinates[0]);
			kalmanFilterInit(&mynavigator.filters.kalmanFilter.lon, startCoordinates[1]);
			kalmanFilterInit(&mynavigator.filters.kalmanFilter.alt, startCoordinates[2]);

			// Инициализация лучшего фильтра :
			minQuadFilterInit(&mynavigator.filters.minQuadFilter.lat);
			minQuadFilterInit(&mynavigator.filters.minQuadFilter.lon);
			minQuadFilterInit(&mynavigator.filters.minQuadFilter.alt);

			isRestart = false;
		}

#ifdef MSG
		cout << "На фильтр: " << ends;
#endif // DEBUG

		p = fgets(buf, msgMaxLen, stdin);
		if (p) {
			// Удаляем символ перевода строки :
			size_t len = strlen(buf);
			if (len != 0 && buf[len - 1] == '\n') {
				buf[len - 1] = '\0';
			}

			if (buf[0] == 's' && buf[1] == 't' && buf[2] == 'o' && buf[3] == 'p')
			{
				break;
			}
			if (buf[0] == 'r' && buf[1] == 'e' && buf[2] == 's' && buf[3] == 't' && buf[4] == 'a' && buf[5] == 'r' && buf[6] == 't')
			{
				cout << endl;
				isRestart = true;
				continue;
			}
			// Записываем строку в входной массив :
			for (unsigned int i = 0; i < (unsigned int)len; i++)
			{
				mynavigator.msgIn.msg[i] = buf[i];
			}
			mynavigator.msgIn.len = (unsigned int)len;
			mynavigator.msgIn.msg[mynavigator.msgIn.len] = '\n';
			mynavigator.msgIn.msg[mynavigator.msgIn.len - 1] = '\r';
			mynavigator.msgIn.len++;

			myNavigator(&mynavigator);

#ifdef MSG // MSG
			cout << "С фильтра: " << ends;
#endif
			for (unsigned int i = 0; i < mynavigator.msgOut.len; i++)
			{
				cout << mynavigator.msgOut.msg[i] << ends;
			}
#ifdef MSG // MSG
			cout << "Логи     : " << ends;
			for (unsigned int i = 0; i < logInfoArrLen; i++)
			{
				cout << (int)mynavigator.logInfo[i] << " " << ends;
			}
			cout << "\n" << endl;
#endif
		}
#ifdef MSG
		else puts("Возникла ошибка");
#endif // MSG
	}

	// Выход
	return 0;
}









































