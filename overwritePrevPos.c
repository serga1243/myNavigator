#include "overwritePrevPos.h"

//static uint16_t i;

inline void overwritePrevPos(struct MyNavigator* myNavigator)
{
	register double *arrPtr;

#ifdef INCLUDE_LAT
	/*for (i = 0; i < PREVIOS_POS_LEN - 1; i++)
	{
		myNavigator->coordinates.lat.previosPos.value[i] = myNavigator->coordinates.lat.previosPos.value[i + 1];
	}
	myNavigator->coordinates.lat.previosPos.value[PREVIOS_POS_LEN - 1] = myNavigator->coordinates.lat.decodedPos.value;
	*/
	for (arrPtr = myNavigator->coordinates.lat.previosPos.value; arrPtr < arrPtr + sizeof(double) * (PREVIOS_POS_LEN - 1); arrPtr++)
	{
		*arrPtr = *(arrPtr + 1);
	}
	*arrPtr = myNavigator->coordinates.lat.decodedPos.value;
#endif

#ifdef INCLUDE_LON
	/*for (i = 0; i < PREVIOS_POS_LEN - 1; i++)
	{
		myNavigator->coordinates.lon.previosPos.value[i] = myNavigator->coordinates.lon.previosPos.value[i + 1];
	}
	myNavigator->coordinates.lon.previosPos.value[PREVIOS_POS_LEN - 1] = myNavigator->coordinates.lon.decodedPos.value;
	*/
	for (arrPtr = myNavigator->coordinates.lon.previosPos.value; arrPtr < arrPtr + sizeof(double) * (PREVIOS_POS_LEN - 1); arrPtr++)
	{
		*arrPtr = *(arrPtr + 1);
	}
	*arrPtr = myNavigator->coordinates.lon.decodedPos.value;
#endif

#ifdef INCLUDE_ALT
	/*for (i = 0; i < PREVIOS_POS_LEN - 1; i++)
	{
		myNavigator->coordinates.alt.previosPos.value[i] = myNavigator->coordinates.alt.previosPos.value[i + 1];
	}
	myNavigator->coordinates.alt.previosPos.value[PREVIOS_POS_LEN - 1] = myNavigator->coordinates.alt.decodedPos.value;
	*/
	for (arrPtr = myNavigator->coordinates.alt.previosPos.value; arrPtr < arrPtr + sizeof(double) * (PREVIOS_POS_LEN - 1); arrPtr++)
	{
		*arrPtr = *(arrPtr + 1);
	}
	*arrPtr = myNavigator->coordinates.alt.decodedPos.value;
#endif

	return;
}
