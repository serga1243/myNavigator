// ����� ����� � ����� ������������� ������ ������

#include <iostream>
#include <ctime>
#include "myNavigator.h"

#define MSG true
using namespace std;

void someFunc(unsigned int a, unsigned int b, unsigned long long int c);

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
			myNavigatorInit(&mynavigator, &someFunc);
			isRestart = false;
		}

#ifdef MSG
		cout << "�� ������: " << ends;
#endif // myNavigator_DEBUG

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
			
			// �������, ��� ������� �� ������� ����� ����������� �������� 1 ���. :
			mynavigator.msgData.dt = 0.001 * (double)(rand() % 2000 + 10);
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
			cout << "     ����� � ����������� ��������� � ������������: " << mynavigator.msgData.dt << " ���." << ends;
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


// ������� ������������ ������ �� ���� ������ 
void someFunc(unsigned int a, unsigned int b, unsigned long long int c)
{
	short d = a + b + c;
}