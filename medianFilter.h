#pragma once

#include "myNavigator.h"
#include "medianFilter.h"

struct MedianFilterCoordinate;
struct Coordinate;

void medianFilter(struct MedianFilterCoordinate*, struct Coordinate*);

double findMedianNFast(double[], double, unsigned int*, unsigned int*);

void medianFilterInit(struct MedianFilterCoordinate*, const double);