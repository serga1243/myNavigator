#include "findCommas.h"

static uint16_t parIdI;
static uint16_t i;

inline uint16_t findCommas(struct MyNavigator* myNavigator)
{
	parIdI = 0;
	for (i = 0; i < MSG_MAX_LEN; i++)
	{
		if (myNavigator->msgIn.msg[i] == ',')
		{
			myNavigator->msgData.parId[parIdI++] = i;
		}
	}

	return parIdI;
}
