#include "findCommas.h"

static unsigned short parIdI;
static unsigned short i;

unsigned short findCommas(struct MyNavigator* myNavigator)
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