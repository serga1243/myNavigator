#include "findCommas.h"

unsigned short findCommas(struct MyNavigator* myNavigator, unsigned char* logInfo)
{
	unsigned short parIdI = 0;
	*logInfo = 1;
	for (unsigned short i = 0; i < msgMaxLen; i++)
	{
		if (myNavigator->msgIn.msg[i] == ',')
		{
			myNavigator->msgData.parId[parIdI++] = i;
		}
	}
	*logInfo = 0;

	return parIdI;
}