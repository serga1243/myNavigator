#include "overwritePrevPos.h"

void overwritePrevPos(struct MyNavigator* myNavigator, unsigned char* logInfo)
{
	*logInfo = 1;
	for (unsigned short i = 0; i < previosPosLen; i++)
	{
		if (i < previosPosLen - 1)
		{
#ifdef includeLat
			myNavigator->coordinates.lat.previosPos.value[i] = myNavigator->coordinates.lat.previosPos.value[i + 1];
#endif

#ifdef includeLon
			myNavigator->coordinates.lon.previosPos.value[i] = myNavigator->coordinates.lon.previosPos.value[i + 1];
#endif

#ifdef includeAlt
			myNavigator->coordinates.alt.previosPos.value[i] = myNavigator->coordinates.alt.previosPos.value[i + 1];
#endif
		}
		else
		{
#ifdef includeLat
			myNavigator->coordinates.lat.previosPos.value[i] = myNavigator->coordinates.lat.decodedPos.value;
#endif

#ifdef includeLon
			myNavigator->coordinates.lon.previosPos.value[i] = myNavigator->coordinates.lon.decodedPos.value;
#endif

#ifdef includeAlt
			myNavigator->coordinates.alt.previosPos.value[i] = myNavigator->coordinates.alt.decodedPos.value;
#endif
		}
	}
	*logInfo = 0;

	return;
}