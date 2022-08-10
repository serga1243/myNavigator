#include "utilitiesFuncs.h"

void isInvalidData(double* data, const double limits[], double* correctData)
{
	*data = (*data > limits[0]) && (*data < limits[1]) ? *data : *correctData;
	return;
}

int32_t intPower(uint16_t x, uint16_t n)
{
	if (n <= 0) return 1;
	if (n >= 1000000000) return 1000000000;
	return x * intPower(x, n - 1);
}


int32_t intFactorial(uint16_t n)
{
	if (n == 0) return 1;
	return n * intFactorial(n - 1);
}

double myfabs(double x)
{
	return x < 0.0 ? -x : x;
}
