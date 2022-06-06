#include "char2double.h"
#include "utilitiesFuncs.h"

static double val;
static double a;
static unsigned short i;
static unsigned short power;

double char2double(char str[], unsigned short strLen)
{
	val = 0;
	a = 0;

	for (i = 0, power = 0; i < strLen + 1; i++, power++)
	{
		switch (str[i])
		{
		case '1':
			a = 1;
			break;

		case '2':
			a = 2;
			break;

		case '3':
			a = 3;
			break;

		case '4':
			a = 4;
			break;

		case '5':
			a = 5;
			break;

		case '6':
			a = 6;
			break;

		case '7':
			a = 7;
			break;

		case '8':
			a = 8;
			break;

		case '9':
			a = 9;
			break;

		case '.':
			power--;
			continue;

		default:
			a = 0;
			break;
		}
		val += a * pow(10, (double)(strLen - power) - 1.0);
	}

	return val;
}