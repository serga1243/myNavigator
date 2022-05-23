#include <iostream>
#include "myNavigator.h"

#define MSG true
using namespace std;


int main()
{
	setlocale(LC_ALL, "Russian_Russia.1251");

	bool isRestart = true;
	char buf[msgMaxLen] = "", * p = NULL;

	double aa = 4535.3245677;
	double bb = 13423.234758697;
	geo2decart(&aa, &bb);
	decart2geo(&aa, &bb);

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
			// Записываем строку в входной массив :
			memcpy(mynavigator.msgIn.msg, buf, len);
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









































