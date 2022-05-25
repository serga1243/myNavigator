#include "overwritePrevPos.h"

static unsigned short i;

void overwritePrevPos(struct MyNavigator* myNavigator)
{
#ifdef includeLat
	memcpy(myNavigator->coordinates.lat.previosPos.value, &myNavigator->coordinates.lat.previosPos.value[1], (previosPosLen - 1) * 8);
	myNavigator->coordinates.lat.previosPos.value[previosPosLen - 1] = myNavigator->coordinates.lat.decodedPos.value;
#endif

#ifdef includeLon
	memcpy(myNavigator->coordinates.lon.previosPos.value, &myNavigator->coordinates.lon.previosPos.value[1], (previosPosLen - 1) * 8);
	myNavigator->coordinates.lon.previosPos.value[previosPosLen - 1] = myNavigator->coordinates.lon.decodedPos.value;
#endif

#ifdef includeAlt
	memcpy(myNavigator->coordinates.alt.previosPos.value, &myNavigator->coordinates.alt.previosPos.value[1], (previosPosLen - 1) * 8);
	myNavigator->coordinates.alt.previosPos.value[previosPosLen - 1] = myNavigator->coordinates.alt.decodedPos.value;
#endif

	return;
}