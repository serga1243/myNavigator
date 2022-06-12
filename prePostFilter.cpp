#include "prePostFilter.h"

void prePostFilter(struct MyNavigator* myNavigator, bool preFiltering)
{
#ifdef TransformCoords

	if (preFiltering)
	{
		if (pow(myNavigator->coordinates.lat.decodedPos.value - myNavigator->coordinates.lat.previosPos.value[previosPosLen - 1], 2.0) +
			pow(myNavigator->coordinates.lon.decodedPos.value - myNavigator->coordinates.lon.previosPos.value[previosPosLen - 1], 2.0) +
			pow(myNavigator->coordinates.alt.decodedPos.value - myNavigator->coordinates.alt.previosPos.value[previosPosLen - 1], 2.0) >
			pow(myNavigator->msgData.dT * objectMaxSpeed, 2.0))
		{
			myNavigator->coordinates.lat.decodedPos.value = myNavigator->coordinates.lat.previosPos.value[previosPosLen - 1];
			myNavigator->coordinates.lon.decodedPos.value = myNavigator->coordinates.lon.previosPos.value[previosPosLen - 1];
			myNavigator->coordinates.alt.decodedPos.value = myNavigator->coordinates.alt.previosPos.value[previosPosLen - 1];
		}
	}
	else
	{
		if (pow(myNavigator->coordinates.lat.filteredPos.value - myNavigator->coordinates.lat.previosPos.value[previosPosLen - 1], 2.0) +
			pow(myNavigator->coordinates.lon.filteredPos.value - myNavigator->coordinates.lon.previosPos.value[previosPosLen - 1], 2.0) +
			pow(myNavigator->coordinates.alt.filteredPos.value - myNavigator->coordinates.alt.previosPos.value[previosPosLen - 1], 2.0) >
			pow(myNavigator->msgData.dT * objectMaxSpeed, 2.0))
		{
			myNavigator->coordinates.lat.filteredPos.value = myNavigator->coordinates.lat.decodedPos.value;
			myNavigator->coordinates.lon.filteredPos.value = myNavigator->coordinates.lon.decodedPos.value;
			myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;
		}
	}

#endif

	return;
}