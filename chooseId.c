#include "chooseId.h"

inline void chooseId(struct MyNavigator* myNavigator)
{
	if (strncmp((const char *)myNavigator->msgData.idData, "PUB", 3) == 0)
	{
		if (myNavigator->msgData.parIdLen > 6)
		{
			myNavigator->msgData.id[0] = 2;
			myNavigator->msgData.id[1] = 3;
			myNavigator->msgData.id[2] = 4;
			myNavigator->msgData.id[3] = 5;
			myNavigator->msgData.id[4] = 6;
			myNavigator->msgData.id[5] = 0;
			myNavigator->msgData.id[6] = 0;
			myNavigator->logInfo[0] = 0;
			return;
		}
		else
		{
			myNavigator->logInfo[0] = 2;
			return;
		}
	}
	else if (strncmp((const char *)myNavigator->msgData.idData, "GGA", 3) == 0)
	{
		if (myNavigator->msgData.parIdLen > 8)
		{
			myNavigator->msgData.id[0] = 1;
			myNavigator->msgData.id[1] = 2;
			myNavigator->msgData.id[2] = 3;
			myNavigator->msgData.id[3] = 4;
			myNavigator->msgData.id[4] = 8;
			myNavigator->msgData.id[5] = 0;
			myNavigator->msgData.id[6] = 0;
			myNavigator->logInfo[0] = 0;
			return;
		}
		else
		{
			myNavigator->logInfo[0] = 2;
			return;
		}
	}
	else if (strncmp((const char *)myNavigator->msgData.idData, "GLL", 3) == 0)
	{
		if (myNavigator->msgData.parIdLen > 4)
		{
			if (myNavigator->msgIn.msg[myNavigator->msgData.parId[5]] == 'A')
			{
				myNavigator->msgData.id[0] = 0;
				myNavigator->msgData.id[1] = 1;
				myNavigator->msgData.id[2] = 2;
				myNavigator->msgData.id[3] = 3;
				myNavigator->msgData.id[4] = 0;
				myNavigator->msgData.id[5] = 4;
				myNavigator->msgData.id[6] = 0;
				myNavigator->logInfo[0] = 0;
			}
			else
			{
				myNavigator->logInfo[0] = 2;
			}
			return;
		}
		else
		{
			myNavigator->logInfo[0] = 2;
			return;
		}
	}
	else if (strncmp((const char *)myNavigator->msgData.idData, "RMA", 3) == 0)
	{
		if (myNavigator->msgData.parIdLen > 4)
		{
			myNavigator->msgData.id[0] = 1;
			myNavigator->msgData.id[1] = 2;
			myNavigator->msgData.id[2] = 3;
			myNavigator->msgData.id[3] = 4;
			myNavigator->msgData.id[4] = 0;
			myNavigator->msgData.id[5] = 0;
			myNavigator->msgData.id[6] = 7;
			myNavigator->logInfo[0] = 0;
			return;
		}
		else
		{
			myNavigator->logInfo[0] = 2;
			return;
		}
	}
	else if (strncmp((const char *)myNavigator->msgData.idData, "RMC", 3) == 0)
	{
		if (myNavigator->msgData.parIdLen > 5)
		{
			myNavigator->msgData.id[0] = 2;
			myNavigator->msgData.id[1] = 3;
			myNavigator->msgData.id[2] = 4;
			myNavigator->msgData.id[3] = 5;
			myNavigator->msgData.id[4] = 0;
			myNavigator->msgData.id[5] = 0;
			myNavigator->msgData.id[6] = 6;
			myNavigator->logInfo[0] = 0;
			return;
		}
		else
		{
			myNavigator->logInfo[0] = 2;
			return;
		}
	}
	/*else if (strncmp((const char *)myNavigator->msgData.idData, "WPL", 3) == 0)
	{
		if (myNavigator->msgData.parIdLen > 3)
		{
			myNavigator->msgData.id[0] = 0;
			myNavigator->msgData.id[1] = 1;
			myNavigator->msgData.id[2] = 2;
			myNavigator->msgData.id[3] = 3;
			myNavigator->msgData.id[4] = 0;
			myNavigator->msgData.id[5] = 0;
			myNavigator->logInfo[0] = 0;
			return;
		}
		else
		{
			myNavigator->logInfo[0] = 2;
			return;
		}
	}*/
	else if (strncmp((const char *)myNavigator->msgData.idData, "RMF", 3) == 0)
	{
		if (myNavigator->msgData.parIdLen > 8)
		{
			myNavigator->msgData.id[0] = 5;
			myNavigator->msgData.id[1] = 6;
			myNavigator->msgData.id[2] = 7;
			myNavigator->msgData.id[3] = 8;
			myNavigator->msgData.id[4] = 0;
			myNavigator->msgData.id[5] = 0;
			myNavigator->msgData.id[6] = 11;
			myNavigator->logInfo[0] = 0;
			return;
		}
		else
		{
			myNavigator->logInfo[0] = 2;
			return;
		}
	}
	else if (strncmp((const char *)myNavigator->msgData.idData, "RMI", 3) == 0)
	{
		if (myNavigator->msgData.parIdLen > 3)
		{
			myNavigator->msgData.id[0] = 0;
			myNavigator->msgData.id[1] = 1;
			myNavigator->msgData.id[2] = 2;
			myNavigator->msgData.id[3] = 3;
			myNavigator->msgData.id[4] = 0;
			myNavigator->msgData.id[5] = 0;
			myNavigator->msgData.id[6] = 0;
			myNavigator->logInfo[0] = 0;
			return;
		}
		else
		{
			myNavigator->logInfo[0] = 2;
			return;
		}
	}
	else
	{
		myNavigator->logInfo[0] = 1;
		return;
	}

	return;
}
