#include "writeInROM.h"

void writeInROM(struct MyNavigator* myNavigator)
{

#ifdef WriteCoordsInFlash

	// write in FLASH

	// ���������, �������� �� �������� ��� ������ �� ����
	if (myNavigator->writeInFlash.adress < WriteInFlashEndAdress)
	{

		// ������ :
		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)myNavigator->coordinates.lat.decodedPos.value);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)myNavigator->coordinates.lat.filteredPos.allFilteredValues[0]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)myNavigator->coordinates.lat.filteredPos.allFilteredValues[1]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)myNavigator->coordinates.lat.filteredPos.allFilteredValues[2]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)myNavigator->coordinates.lat.filteredPos.allFilteredValues[3]);
		myNavigator->writeInFlash.adress += 8;

		// ������� :
		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)myNavigator->coordinates.lon.decodedPos.value);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)myNavigator->coordinates.lon.filteredPos.allFilteredValues[0]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)myNavigator->coordinates.lon.filteredPos.allFilteredValues[1]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)myNavigator->coordinates.lon.filteredPos.allFilteredValues[2]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)myNavigator->coordinates.lon.filteredPos.allFilteredValues[3]);
		myNavigator->writeInFlash.adress += 8;

		// ������ :
		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)myNavigator->coordinates.alt.decodedPos.value);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)myNavigator->coordinates.alt.filteredPos.allFilteredValues[0]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)myNavigator->coordinates.alt.filteredPos.allFilteredValues[1]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)myNavigator->coordinates.alt.filteredPos.allFilteredValues[2]);
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->writeInFlash.flashFunc)(0x00000003U, myNavigator->writeInFlash.adress, (uint64_t)myNavigator->coordinates.alt.filteredPos.allFilteredValues[3]);
		myNavigator->writeInFlash.adress += 8;

		// ����� � �������� ��������� � ����� ���������� ������� myNavigator :
		(*myNavigator->writeInFlash.flashFunc)(0x00000001U, myNavigator->writeInFlash.adress, (uint16_t)(myNavigator->msgData.dT * 1000));
		myNavigator->writeInFlash.adress += 2;

		(*myNavigator->writeInFlash.flashFunc)(0x00000001U, myNavigator->writeInFlash.adress, (uint16_t)myNavigator->myNavigatorExeTime);
		myNavigator->writeInFlash.adress += 2;

	}

#endif

	return;
}