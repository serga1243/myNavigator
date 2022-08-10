#include "findCommas.h"

static uint16_t parIdI;
static uint16_t i;

uint16_t findCommas(struct MyNavigator* myNavigator)
{
	parIdI = 0;
	for (i = 0; i < msgMaxLen; i++)
	{
		if (myNavigator->msgIn.msg[i] == ',')
		{
			myNavigator->msgData.parId[parIdI++] = i;
		}
	}

	return parIdI;
}