#include "writeInROM.h"

void writeInROM(struct Coordinates* coordinates)
{

#ifdef WriteCoordsInFlash

	// write in FLASH
	&coordinates->lat.filteredPos.value;
	coordinates->lat.filteredPos.allFilteredValues;
	

	&coordinates->lon.filteredPos.value;
	coordinates->lon.filteredPos.allFilteredValues;

	&coordinates->alt.filteredPos.value;
	coordinates->alt.filteredPos.allFilteredValues;

#endif

	return;
}