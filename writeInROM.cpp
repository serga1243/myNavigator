#include "writeInROM.h"

void writeInROM(struct MyNavigator* myNavigator)
{

#ifdef WriteCoordsInFlash

	// write in FLASH

	// Проверяем, возможно ли записать еще данные во флеш
	if (myNavigator->writeInFlash.adress < WriteInFlashEndAdress)
	{

		// Широта :
		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (unsigned long long int)myNavigator->coordinates.lat.decodedPos.value);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (unsigned long long int)myNavigator->coordinates.lat.filteredPos.allFilteredValues[0]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (unsigned long long int)myNavigator->coordinates.lat.filteredPos.allFilteredValues[1]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (unsigned long long int)myNavigator->coordinates.lat.filteredPos.allFilteredValues[2]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (unsigned long long int)myNavigator->coordinates.lat.filteredPos.allFilteredValues[3]);
		myNavigator->writeInFlash.adress += 8;

		// Долгота :
		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (unsigned long long int)myNavigator->coordinates.lon.decodedPos.value);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (unsigned long long int)myNavigator->coordinates.lon.filteredPos.allFilteredValues[0]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (unsigned long long int)myNavigator->coordinates.lon.filteredPos.allFilteredValues[1]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (unsigned long long int)myNavigator->coordinates.lon.filteredPos.allFilteredValues[2]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (unsigned long long int)myNavigator->coordinates.lon.filteredPos.allFilteredValues[3]);
		myNavigator->writeInFlash.adress += 8;

		// Высота :
		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (unsigned long long int)myNavigator->coordinates.alt.decodedPos.value);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (unsigned long long int)myNavigator->coordinates.alt.filteredPos.allFilteredValues[0]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (unsigned long long int)myNavigator->coordinates.alt.filteredPos.allFilteredValues[1]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (unsigned long long int)myNavigator->coordinates.alt.filteredPos.allFilteredValues[2]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (unsigned long long int)myNavigator->coordinates.alt.filteredPos.allFilteredValues[3]);
		myNavigator->writeInFlash.adress += 8;

		// Время с прошлого сообщения и время выполнения функции myNavigator :
		(*myNavigator->writeInFlash.flashFunc)(0x00000001U, myNavigator->writeInFlash.adress, (unsigned short)(myNavigator->msgData.dT * 1000));
		myNavigator->writeInFlash.adress += 2;

		(*myNavigator->writeInFlash.flashFunc)(0x00000001U, myNavigator->writeInFlash.adress, myNavigator->myNavigatorExeTime);
		myNavigator->writeInFlash.adress += 2;

	}

#endif

	return;
}