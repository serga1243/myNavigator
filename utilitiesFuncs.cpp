#include "utilitiesFuncs.h"


int intPower(unsigned short x, unsigned short n)
{
	if (n <= 0) return 1;
	if (n >= 1000000000) return 1000000000;
	return x * intPower(x, n - 1);
}


int intFactorial(unsigned short n)
{
	if (n == 0) return 1;
	return n * intFactorial(n - 1);
}

double myfabs(double x)
{
	return x < 0.0 ? -x : x;
}
