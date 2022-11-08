#pragma once

#include "myNavigator.h"
#include "medianFilter.h"

struct MedianFilterCoordinate;
struct Coordinate;

void medianFilter(struct MedianFilterCoordinate*, struct Coordinate*);

double findMedianNFast(double[], double, int32_t*, int32_t*);

void medianFilterInit(struct MedianFilterCoordinate*, const double);