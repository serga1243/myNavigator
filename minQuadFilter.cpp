#include "minQuadFilter.h"


void minQuadFilter(struct MinQuadFilterCoordinate* minQuadFilterCoordinate, struct Coordinate* coordinate, unsigned char* logInfo)
{
	*logInfo = 3;
	minQuadFilterCoordinate->sumY = 0;
	minQuadFilterCoordinate->sumXY = 0;
	for (int i = 0; i < PreviosPosLen - 1; i++)
	{
		minQuadFilterCoordinate->sumY += coordinate->previosPos.value[i + 1];
		minQuadFilterCoordinate->sumXY += (double)i * (coordinate->previosPos.value[i + 1]);
	}
	*logInfo = 2;
	minQuadFilterCoordinate->sumY += coordinate->decodedPos.value;
	minQuadFilterCoordinate->sumXY += (PreviosPosLen - 1) * coordinate->decodedPos.value;
	minQuadFilterCoordinate->a = PreviosPosLen * minQuadFilterCoordinate->sumXY;
	minQuadFilterCoordinate->a -= minQuadFilterCoordinate->sumX * minQuadFilterCoordinate->sumY;
	minQuadFilterCoordinate->a /= PreviosPosLen * minQuadFilterCoordinate->sumX2 - minQuadFilterCoordinate->sumX * minQuadFilterCoordinate->sumX;
	*logInfo = 1;
	minQuadFilterCoordinate->b = (minQuadFilterCoordinate->sumY - minQuadFilterCoordinate->a * minQuadFilterCoordinate->sumX) / PreviosPosLen;
	minQuadFilterCoordinate->a *= PreviosPosLen;
	coordinate->filteredPos.value = minQuadFilterCoordinate->a + minQuadFilterCoordinate->b;

	*logInfo = 0;

	return;
}

void minQuadFilterInit(struct MinQuadFilterCoordinate* minQuadFilterCoordinate)
{
	for (int i = 0; i < PreviosPosLen; i++)
	{
		minQuadFilterCoordinate->sumX += i;
		minQuadFilterCoordinate->sumX2 += i * i;
	}
	minQuadFilterCoordinate->sumY = 0;
	minQuadFilterCoordinate->sumXY = 0;
	minQuadFilterCoordinate->a = 0;
	minQuadFilterCoordinate->b = 0;

	return;
}