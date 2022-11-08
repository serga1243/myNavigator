#include "writeInROM.h"
#include "myIncs.h"

inline void writeInROM(struct MyNavigator* myNavigator)
{

#ifdef WRITE_COORDS_IN_FLASH

	// write in FLASH

	// ���������, �������� �� �������� ��� ������ �� ����
	if (myNavigator->writeInFlash.adress < WRITE_IN_FLASH_END_ADRESS)
	{
		// �������������, ��� ����� ���������� ������ �� ���� ������ (���������� ���������� ����������) :
		(*myNavigator->funcsPointrs.LEDaction)(11);

		// ������ :
		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lat.decodedPos.value * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lat.filteredPos.allFilteredValues[0] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lat.filteredPos.allFilteredValues[1] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lat.filteredPos.allFilteredValues[2] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lat.filteredPos.allFilteredValues[3] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		// ������� :
		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lon.decodedPos.value * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lon.filteredPos.allFilteredValues[0] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lon.filteredPos.allFilteredValues[1] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lon.filteredPos.allFilteredValues[2] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.lon.filteredPos.allFilteredValues[3] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		// ������ :
		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.alt.decodedPos.value * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.alt.filteredPos.allFilteredValues[0] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.alt.filteredPos.allFilteredValues[1] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.alt.filteredPos.allFilteredValues[2] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint64_t)(myNavigator->coordinates.alt.filteredPos.allFilteredValues[3] * 1.0E6));
		myNavigator->writeInFlash.adress += 8;

		// ����� � �������� ��������� � ����� ���������� ������� myNavigator :
		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint16_t)(myNavigator->msgData.dT * 1.0E6));
		myNavigator->writeInFlash.adress += 2;

		(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint16_t)myNavigator->myNavigatorExeTime);
		//(*myNavigator->funcsPointrs.flashFunc)(FLASH_TYPEPROGRAM_DOUBLEWORD, myNavigator->writeInFlash.adress, (uint16_t)0xFFFF);
		myNavigator->writeInFlash.adress += 2;

		// �������������, ��� ������ �� ���� ������ �������� (����������� ���������� ����������) :
		(*myNavigator->funcsPointrs.LEDaction)(12);

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
