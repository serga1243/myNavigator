#include "minQuadFilter.h"

static uint16_t i;

void minQuadFilter(struct MinQuadFilterCoordinate* minQuadFilterCoordinate, struct Coordinate* coordinate)
{
	minQuadFilterCoordinate->sumY = 0.0;
	minQuadFilterCoordinate->sumXY = 0.0;
	for (i = 0; i < PREVIOS_POS_LEN - 1; i++)
	{
		minQuadFilterCoordinate->sumY += coordinate->previosPos.value[i + 1];
		minQuadFilterCoordinate->sumXY += (double)i * (coordinate->previosPos.value[i + 1]);
	}
	minQuadFilterCoordinate->sumY += coordinate->decodedPos.value;
	minQuadFilterCoordinate->sumXY += (PREVIOS_POS_LEN - 1.0) * coordinate->decodedPos.value;
	minQuadFilterCoordinate->a = PREVIOS_POS_LEN * minQuadFilterCoordinate->sumXY;
	minQuadFilterCoordinate->a -= minQuadFilterCoordinate->sumX * minQuadFilterCoordinate->sumY;
	minQuadFilterCoordinate->a /= PREVIOS_POS_LEN * minQuadFilterCoordinate->sumX2 - minQuadFilterCoordinate->sumX * minQuadFilterCoordinate->sumX;
	minQuadFilterCoordinate->b = (minQuadFilterCoordinate->sumY - minQuadFilterCoordinate->a * minQuadFilterCoordinate->sumX) / PREVIOS_POS_LEN;
	minQuadFilterCoordinate->a *= PREVIOS_POS_LEN;
	coordinate->filteredPos.value = minQuadFilterCoordinate->a + minQuadFilterCoordinate->b;

	return;
}

void minQuadFilterInit(struct MinQuadFilterCoordinate* minQuadFilterCoordinate)
{
	minQuadFilterCoordinate->sumX = 0.0;
	minQuadFilterCoordinate->sumX2 = 0.0;
	for (i = 0; i < PREVIOS_POS_LEN; i++)
	{
		minQuadFilterCoordinate->sumX += (double)i;
		minQuadFilterCoordinate->sumX2 += (double)i * (double)i;
	}
	minQuadFilterCoordinate->sumY = 0.0;
	minQuadFilterCoordinate->sumXY = 0.0;
	minQuadFilterCoordinate->a = 0.0;
	minQuadFilterCoordinate->b = 0.0;

	return;
}