#pragma once

#include "myNavigator.h"
#include "medianFilter.h"

struct MedianFilterCoordinate;
struct Coordinate;

void medianFilter(struct MedianFilterCoordinate*, struct Coordinate*);

double findMedianNFast(double[], double, int*, int*);

void medianFilterInit(struct MedianFilterCoordinate*, const double);