#include "writeInROM.h"

void writeInROM(struct MyNavigator* myNavigator)
{

#ifdef WriteCoordsInFlash

	// write in FLASH

	// Проверяем, возможно ли записать еще данные во флеш
	if (myNavigator->writeInFlash.adress < WriteInFlashEndAdress)
	{
		// Сигнализируем, что будем записывать данные во флеш память :
		(*myNavigator->funcsPointrs.LEDaction)(6);

		// Широта :
		(*myNavigator->funcsPointrs.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lat.decodedPos.value * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lat.filteredPos.allFilteredValues[0] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lat.filteredPos.allFilteredValues[1] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lat.filteredPos.allFilteredValues[2] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lat.filteredPos.allFilteredValues[3] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		// Долгота :
		(*myNavigator->funcsPointrs.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lon.decodedPos.value * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lon.filteredPos.allFilteredValues[0] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lon.filteredPos.allFilteredValues[1] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lon.filteredPos.allFilteredValues[2] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lon.filteredPos.allFilteredValues[3] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		// Высота :
		(*myNavigator->funcsPointrs.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.alt.decodedPos.value * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.alt.filteredPos.allFilteredValues[0] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.alt.filteredPos.allFilteredValues[1] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.alt.filteredPos.allFilteredValues[2] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.alt.filteredPos.allFilteredValues[3] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		// Время с прошлого сообщения и время выполнения функции myNavigator :
		(*myNavigator->funcsPointrs.flashFunc)(0x00000001U, myNavigator->writeInFlash.adress, (uint16_t)(myNavigator->msgData.dT * 1.0E3));
		myNavigator->writeInFlash.adress += 2;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000001U, myNavigator->writeInFlash.adress, (uint16_t)myNavigator->myNavigatorExeTime);
		myNavigator->writeInFlash.adress += 2;

		// Сигнализируем, что данные во флеш память записаны :
		(*myNavigator->funcsPointrs.LEDaction)(6);

	}
	else
	{
		if (myNavigator->writeInFlash.isUnlockedFlash)
		{
			(*myNavigator->funcsPointrs.flashLockFunc)();
			myNavigator->writeInFlash.isUnlockedFlash = false;
		}
	}

#endif

	return;
}