#include "writeInROM.h"

void writeInROM(struct MyNavigator* myNavigator)
{

#ifdef WriteCoordsInFlash

	// write in FLASH

	// ���������, �������� �� �������� ��� ������ �� ����
	if (myNavigator->writeInFlash.adress < WriteInFlashEndAdress)
	{
		// �������������, ��� ����� ���������� ������ �� ���� ������ (���������� ���������� ����������) :
		(*myNavigator->funcsPointrs.LEDaction)(8);

		// ������ :
		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lat.decodedPos.value * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lat.filteredPos.allFilteredValues[0] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lat.filteredPos.allFilteredValues[1] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lat.filteredPos.allFilteredValues[2] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lat.filteredPos.allFilteredValues[3] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		// ������� :
		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lon.decodedPos.value * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lon.filteredPos.allFilteredValues[0] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lon.filteredPos.allFilteredValues[1] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lon.filteredPos.allFilteredValues[2] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lon.filteredPos.allFilteredValues[3] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		// ������ :
		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.alt.decodedPos.value * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.alt.filteredPos.allFilteredValues[0] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.alt.filteredPos.allFilteredValues[1] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.alt.filteredPos.allFilteredValues[2] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.alt.filteredPos.allFilteredValues[3] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		// ����� � �������� ��������� � ����� ���������� ������� myNavigator :
		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint16_t)(myNavigator->msgData.dT * 1.0E3));
		myNavigator->writeInFlash.adress += 2;

		(*myNavigator->funcsPointrs.flashFunc)(0x00000002U, myNavigator->writeInFlash.adress, (uint16_t)myNavigator->myNavigatorExeTime);
		myNavigator->writeInFlash.adress += 2;

		// �������������, ��� ������ �� ���� ������ �������� (����������� ���������� ����������) :
		(*myNavigator->funcsPointrs.LEDaction)(9);

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