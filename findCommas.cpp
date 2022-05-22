#include "findCommas.h"

static unsigned short parIdI;
static unsigned short i;

unsigned short findCommas(struct MyNavigator* myNavigator, unsigned char* logInfo)
{
	parIdI = 0;
	*logInfo = 1;
	for (i = 0; i < msgMaxLen; i++)
	{
		if (myNavigator->msgIn.msg[i] == ',')
		{
			myNavigator->msgData.parId[parIdI++] = i;
		}
	}
	*logInfo = 0;

	return parIdI;
}