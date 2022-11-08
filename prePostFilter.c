#include "prePostFilter.h"

static double calcOffset;

inline void prePostFilter(struct MyNavigator* myNavigator, bool preFiltering)
{
	if (preFiltering)
	{
		calcOffset = 0.9 *
				pow(myNavigator->coordinates.lat.decodedPos.value - myNavigator->coordinates.lat.previosPos.value[PREVIOS_POS_LEN - 1], 2.0) +
				pow(myNavigator->coordinates.lon.decodedPos.value - myNavigator->coordinates.lon.previosPos.value[PREVIOS_POS_LEN - 1], 2.0) +
				pow(myNavigator->coordinates.alt.decodedPos.value - myNavigator->coordinates.alt.previosPos.value[PREVIOS_POS_LEN - 1], 2.0);

		if ((calcOffset > pow(myNavigator->msgData.dT * myNavigator->coordinates.vel.decodedPos.value, 2.0))
			||
			(calcOffset > pow(myNavigator->msgData.dT * OBJECT_MAX_SPEED, 2.0)))
		{
			myNavigator->coordinates.lat.decodedPos.value = myNavigator->coordinates.lat.previosPos.value[PREVIOS_POS_LEN - 1];
			myNavigator->coordinates.lon.decodedPos.value = myNavigator->coordinates.lon.previosPos.value[PREVIOS_POS_LEN - 1];
			myNavigator->coordinates.alt.decodedPos.value = myNavigator->coordinates.alt.previosPos.value[PREVIOS_POS_LEN - 1];
		}
	}
	else
	{
		calcOffset = 0.9 *
				pow(myNavigator->coordinates.lat.filteredPos.value - myNavigator->coordinates.lat.previosPos.value[PREVIOS_POS_LEN - 1], 2.0) +
				pow(myNavigator->coordinates.lon.filteredPos.value - myNavigator->coordinates.lon.previosPos.value[PREVIOS_POS_LEN - 1], 2.0) +
				pow(myNavigator->coordinates.alt.filteredPos.value - myNavigator->coordinates.alt.previosPos.value[PREVIOS_POS_LEN - 1], 2.0);


		if ((calcOffset > pow(myNavigator->msgData.dT * myNavigator->coordinates.vel.decodedPos.value, 2.0))
			||
			(calcOffset > pow(myNavigator->msgData.dT * OBJECT_MAX_SPEED, 2.0)))
		{
			myNavigator->coordinates.lat.filteredPos.value = myNavigator->coordinates.lat.decodedPos.value;
			myNavigator->coordinates.lon.filteredPos.value = myNavigator->coordinates.lon.decodedPos.value;
			myNavigator->coordinates.alt.filteredPos.value = myNavigator->coordinates.alt.decodedPos.value;
		}
	}

	return;
}
