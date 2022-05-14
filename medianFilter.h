#pragma once

#include "myNavigator.h"


#include "medianFilter.h"

void medianFilter(struct MedianFilterCoordinate*, struct Coordinate*, unsigned char*);

double findMedianNFast(double[], double, unsigned int*, unsigned int*, unsigned char*);


void medianFilterInit(struct MedianFilterCoordinate*, const double);