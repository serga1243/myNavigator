// Новая ветка с точно определенными типами данных

#include <iostream>
#include <ctime>
#include "myNavigator.h"

//#define FLASH_VOLTAGE_RANGE_1        0x00000000U  /*!< Device operating range: 1.8V to 2.1V                */
//#define FLASH_VOLTAGE_RANGE_2        0x00000001U  /*!< Device operating range: 2.1V to 2.7V                */
//#define FLASH_VOLTAGE_RANGE_3        0x00000002U  /*!< Device operating range: 2.7V to 3.6V                */
//#define FLASH_VOLTAGE_RANGE_4        0x00000003U  /*!< Device operating range: 2.7V to 3.6V + External Vpp */
#define MSG true
using namespace std;

void someFunc(void);
void someFunc(uint8_t);
void someFunc(uint32_t, uint32_t, uint64_t);



int32_t main()
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
			myNavigatorInit(&mynavigator, FuncsPointrs{&someFunc, &someFunc, &someFunc});
			isRestart = false;
		}

#ifdef MSG
		cout << "На фильтр: " << ends;
#endif // myNavigator_DEBUG

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
			memcpy(mynavigator.msgIn.msg, (uint8_t*)buf, len);
			mynavigator.msgIn.len = (uint32_t)len;
			mynavigator.msgIn.msg[mynavigator.msgIn.len] = '\n';
			mynavigator.msgIn.msg[mynavigator.msgIn.len - 1] = '\r';
			mynavigator.msgIn.len++;
			
			// Считаем, что разница во времени между сообщениями примерно 1 сек. :
			mynavigator.msgData.dt = 0.001 * (double)(rand() % 2000 + 10);
			myNavigator(&mynavigator);

#ifdef MSG // MSG
			cout << "С фильтра: " << ends;
#endif
			for (uint32_t i = 0; i < mynavigator.msgIn.len; i++)
			{
				cout << mynavigator.msgIn.msg[i] << ends;
			}
#ifdef MSG // MSG
			cout << "Логи     : " << ends;
			for (uint32_t i = 0; i < logInfoArrLen; i++)
			{
				cout << (int32_t)mynavigator.logInfo[i] << " " << ends;
			}
			cout << "     Время с предыдущего сообщения с координатами: " << mynavigator.msgData.dt << " сек." << ends;
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


// функция симулярующая запись во флеш память 
void someFunc(void) {}
void someFunc(uint8_t a) {}
void someFunc(uint32_t a, uint32_t b, uint64_t c) {}
