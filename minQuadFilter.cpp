#include "minQuadFilter.h"

static unsigned short i;

void minQuadFilter(struct MinQuadFilterCoordinate* minQuadFilterCoordinate, struct Coordinate* coordinate)
{
	minQuadFilterCoordinate->sumY = 0;
	minQuadFilterCoordinate->sumXY = 0;
	for (i = 0; i < previosPosLen - 1; i++)
	{
		minQuadFilterCoordinate->sumY += coordinate->previosPos.value[i + 1];
		minQuadFilterCoordinate->sumXY += (double)i * (coordinate->previosPos.value[i + 1]);
	}
	minQuadFilterCoordinate->sumY += coordinate->decodedPos.value;
	minQuadFilterCoordinate->sumXY += (previosPosLen - 1) * coordinate->decodedPos.value;
	minQuadFilterCoordinate->a = previosPosLen * minQuadFilterCoordinate->sumXY;
	minQuadFilterCoordinate->a -= minQuadFilterCoordinate->sumX * minQuadFilterCoordinate->sumY;
	minQuadFilterCoordinate->a /= previosPosLen * minQuadFilterCoordinate->sumX2 - minQuadFilterCoordinate->sumX * minQuadFilterCoordinate->sumX;
	minQuadFilterCoordinate->b = (minQuadFilterCoordinate->sumY - minQuadFilterCoordinate->a * minQuadFilterCoordinate->sumX) / previosPosLen;
	minQuadFilterCoordinate->a *= previosPosLen;
	coordinate->filteredPos.value = minQuadFilterCoordinate->a + minQuadFilterCoordinate->b;

	return;
}

void minQuadFilterInit(struct MinQuadFilterCoordinate* minQuadFilterCoordinate)
{
	minQuadFilterCoordinate->sumX = 0;
	minQuadFilterCoordinate->sumX2 = 0;
	for (i = 0; i < previosPosLen; i++)
	{
		minQuadFilterCoordinate->sumX += (double)i;
		minQuadFilterCoordinate->sumX2 += (double)i * (double)i;
	}
	minQuadFilterCoordinate->sumY = 0;
	minQuadFilterCoordinate->sumXY = 0;
	minQuadFilterCoordinate->a = 0;
	minQuadFilterCoordinate->b = 0;

	return;
}