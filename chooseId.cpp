#include "chooseId.h"

void chooseId(struct MyNavigator* myNavigator)
{
	if (myNavigator->msgData.idData[0] == 'P' &&
		myNavigator->msgData.idData[1] == 'U' &&
		myNavigator->msgData.idData[2] == 'B')
	{
		if (myNavigator->msgData.parIdLen > 6)
		{
			myNavigator->msgData.id[0] = 2;
			myNavigator->msgData.id[1] = 3;
			myNavigator->msgData.id[2] = 4;
			myNavigator->msgData.id[3] = 5;
			myNavigator->msgData.id[4] = 6;
			myNavigator->msgData.id[5] = 0;
			myNavigator->logInfo[0] = 0;
			return;
		}
		else
		{
			myNavigator->logInfo[0] = 2;
			return;
		}
	}
	else if (myNavigator->msgData.idData[0] == 'G' &&
		myNavigator->msgData.idData[1] == 'G' &&
		myNavigator->msgData.idData[2] == 'A')
	{
		if (myNavigator->msgData.parIdLen > 8)
		{
			myNavigator->msgData.id[0] = 1;
			myNavigator->msgData.id[1] = 2;
			myNavigator->msgData.id[2] = 3;
			myNavigator->msgData.id[3] = 4;
			myNavigator->msgData.id[4] = 8;
			myNavigator->msgData.id[5] = 0;
			myNavigator->logInfo[0] = 0;
			return;
		}
		else
		{
			myNavigator->logInfo[0] = 2;
			return;
		}
	}
	else if (myNavigator->msgData.idData[0] == 'G' &&
		myNavigator->msgData.idData[1] == 'L' &&
		myNavigator->msgData.idData[2] == 'L')
	{
		if (myNavigator->msgData.parIdLen > 4)
		{
			myNavigator->msgData.id[0] = 0;
			myNavigator->msgData.id[1] = 1;
			myNavigator->msgData.id[2] = 2;
			myNavigator->msgData.id[3] = 3;
			myNavigator->msgData.id[4] = 0;
			myNavigator->msgData.id[5] = 4;
			myNavigator->logInfo[0] = 0;
			return;
		}
		else
		{
			myNavigator->logInfo[0] = 2;
			return;
		}
	}
	else if (myNavigator->msgData.idData[0] == 'R' &&
		myNavigator->msgData.idData[1] == 'M' &&
		myNavigator->msgData.idData[2] == 'A')
	{
		if (myNavigator->msgData.parIdLen > 4)
		{
			myNavigator->msgData.id[0] = 1;
			myNavigator->msgData.id[1] = 2;
			myNavigator->msgData.id[2] = 3;
			myNavigator->msgData.id[3] = 4;
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
	}
	else if (myNavigator->msgData.idData[0] == 'R' &&
		myNavigator->msgData.idData[1] == 'M' &&
		myNavigator->msgData.idData[2] == 'C')
	{
		if (myNavigator->msgData.parIdLen > 5)
		{
			myNavigator->msgData.id[0] = 2;
			myNavigator->msgData.id[1] = 3;
			myNavigator->msgData.id[2] = 4;
			myNavigator->msgData.id[3] = 5;
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
	}
	else if (myNavigator->msgData.idData[0] == 'W' &&
		myNavigator->msgData.idData[1] == 'P' &&
		myNavigator->msgData.idData[2] == 'L')
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
	}
	else if (myNavigator->msgData.idData[0] == 'R' &&
		myNavigator->msgData.idData[1] == 'M' &&
		myNavigator->msgData.idData[2] == 'F')
	{
		if (myNavigator->msgData.parIdLen > 8)
		{
			myNavigator->msgData.id[0] = 5;
			myNavigator->msgData.id[1] = 6;
			myNavigator->msgData.id[2] = 7;
			myNavigator->msgData.id[3] = 8;
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
	}
	else if (myNavigator->msgData.idData[0] == 'R' &&
		myNavigator->msgData.idData[1] == 'M' &&
		myNavigator->msgData.idData[2] == 'I')
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
	}
	else
	{
		myNavigator->logInfo[0] = 1;
		return;
	}

	return;
}