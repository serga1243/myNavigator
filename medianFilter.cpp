#include "medianFilter.h"

static double buff;
static unsigned short bufferCntr = 0;
static unsigned short bufferCntrs = 0;
static int i;
static int ii;
static unsigned short j;

void medianFilter(struct MedianFilterCoordinate* medianFilterCoordinate, struct Coordinate* coordinate)
{

#if medianFilteringBufferLength == 3

	coordinate->filteredPos.value = (coordinate->previosPos.value[1] < coordinate->previosPos.value[2])
		? ((coordinate->previosPos.value[2] < coordinate->decodedPos.value) ? coordinate->previosPos.value[2]
		: ((coordinate->decodedPos.value < coordinate->previosPos.value[1]) ? coordinate->previosPos.value[2] : coordinate->decodedPos.value))
		: ((coordinate->previosPos.value[1] < coordinate->decodedPos.value) ? coordinate->previosPos.value[1]
		: ((coordinate->decodedPos.value < coordinate->previosPos.value[2]) ? coordinate->previosPos.value[2] : coordinate->decodedPos.value));

#else

	coordinate->filteredPos.value = 0.0;
	for (i = 0; i < medianFilteringBufferLength - 2; i++)
	{
		coordinate->filteredPos.value +=
			findMedianNFast(&medianFilterCoordinate->buffer[medianFilterCoordinate->bufferCntrs[i]],
			coordinate->decodedPos.value,
			&medianFilterCoordinate->filterCntrs[i],
			&medianFilterCoordinate->bufferLens[i])
			* medianFilterCoordinate->normFunc[i];
	}
	coordinate->filteredPos.value *= medianFilterCoordinate->normFuncSum;

#endif

	return;
}


double findMedianNFast(double buffer[], double newVal, int* cntr, int* len)
{
	buff = 0.0;
	buffer[*cntr] = newVal;
	if ((*cntr < *len - 1) && (buffer[*cntr] > buffer[*cntr + 1])) {
		for (ii = *cntr; ii < *len - 1; ii++) {
			if (buffer[ii] > buffer[ii + 1]) {
				buff = buffer[ii];
				buffer[ii] = buffer[ii + 1];
				buffer[ii + 1] = buff;
			}
		}
	}
	else {
		if ((*cntr > 0) && (buffer[*cntr - 1] > buffer[*cntr])) {
			for (ii = *cntr; ii > 0; ii--) {
				if (buffer[ii] < buffer[ii - 1]) {
					buff = buffer[ii];
					buffer[ii] = buffer[ii - 1];
					buffer[ii - 1] = buff;
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
	medianFilterCoordinate->normFuncSum = 0.0;
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
		medianFilterCoordinate->normFunc[i] = pow(2.0, (double)i);
		medianFilterCoordinate->normFuncSum += medianFilterCoordinate->normFunc[i];
		medianFilterCoordinate->filterCntrs[i] = 0;
		medianFilterCoordinate->bufferCntrs[i] = bufferCntrs;
		medianFilterCoordinate->bufferLens[i] = (int)medianFilteringBufferLength - i;
		bufferCntrs += medianFilterCoordinate->bufferLens[i];
		

	}
	medianFilterCoordinate->normFuncSum = 1.0 / medianFilterCoordinate->normFuncSum;
	return;
}