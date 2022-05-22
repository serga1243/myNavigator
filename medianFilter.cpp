#include "medianFilter.h"

static double buff;
static unsigned short bufferCntr = 0;
static unsigned short bufferCntrs = 0;
static int i;
static unsigned short j;

void medianFilter(struct MedianFilterCoordinate* medianFilterCoordinate, struct Coordinate* coordinate, unsigned char* logInfo)
{
	*logInfo = 4;

#if medianFilteringBufferLength == 3

	coordinate->filteredPos.value = (coordinate->previosPos.value[1] < coordinate->previosPos.value[2])
		? ((coordinate->previosPos.value[2] < coordinate->decodedPos.value) ? coordinate->previosPos.value[2]
		: ((coordinate->decodedPos.value < coordinate->previosPos.value[1]) ? coordinate->previosPos.value[2] : coordinate->decodedPos.value))
		: ((coordinate->previosPos.value[1] < coordinate->decodedPos.value) ? coordinate->previosPos.value[1]
		: ((coordinate->decodedPos.value < coordinate->previosPos.value[2]) ? coordinate->previosPos.value[2] : coordinate->decodedPos.value));

#else

	coordinate->filteredPos.value = 0;
	for (i = 0; i < medianFilteringBufferLength - 2; i++)
	{
		*logInfo = 3;
		coordinate->filteredPos.value +=
			findMedianNFast(&medianFilterCoordinate->buffer[medianFilterCoordinate->bufferCntrs[i]],
			coordinate->decodedPos.value,
			&medianFilterCoordinate->filterCntrs[i],
			&medianFilterCoordinate->bufferLens[i],
			logInfo)
			* medianFilterCoordinate->normFunc[i];
		*logInfo = 2;
	}
	*logInfo = 1;
	coordinate->filteredPos.value /= medianFilterCoordinate->normFuncSum;

#endif

	return;
}


double findMedianNFast(double buffer[], double newVal, unsigned int* cntr, unsigned int* len, unsigned char* logInfo)
{
	buff = 0;
	buffer[*cntr] = newVal;
	if ((*cntr < *len - 1) && (buffer[*cntr] > buffer[*cntr + 1])) {
		for (unsigned int i = *cntr; i < *len - 1; i++) {
			if (buffer[i] > buffer[i + 1]) {
				buff = buffer[i];
				buffer[i] = buffer[i + 1];
				buffer[i + 1] = buff;
			}
		}
	}
	else {
		if ((*cntr > 0) && (buffer[*cntr - 1] > buffer[*cntr])) {
			for (int i = *cntr; i > 0; i--) {
				if (buffer[i] < buffer[i - 1]) {
					buff = buffer[i];
					buffer[i] = buffer[i - 1];
					buffer[i - 1] = buff;
				}
			}
		}
	}
	if (++ * cntr >= *len) *cntr = 0;

	return buffer[*len / 2];
}


void medianFilterInit(struct MedianFilterCoordinate* medianFilterCoordinate, const double startCoordinate)
{
	bufferCntr = 0;
	bufferCntrs = 0;
	medianFilterCoordinate->normFuncSum = 0;
	for (i = 0; i < medianFilteringBufferLength; i++)
	{
		for (j = 0; j < medianFilteringBufferLength; j++)
		{
			medianFilterCoordinate->buffer[bufferCntr++] = startCoordinate;
		}
		if (i >= medianFilteringBufferLength - 2)
		{
			continue;
		}
		medianFilterCoordinate->normFunc[i] = pow(2, (double)i);
		medianFilterCoordinate->normFuncSum += medianFilterCoordinate->normFunc[i];
		medianFilterCoordinate->filterCntrs[i] = 0;
		medianFilterCoordinate->bufferCntrs[i] = bufferCntrs;
		medianFilterCoordinate->bufferLens[i] = (int)medianFilteringBufferLength - i;
		bufferCntrs += medianFilterCoordinate->bufferLens[i];
		

	}

	return;
}