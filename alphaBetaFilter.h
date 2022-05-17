#pragma once

#include "myNavigator.h"

struct KalmanFilterCoordinate;
struct Coordinate;

void alphaBetaFilter(struct AlphaBetaFilterCoordinate*, struct Coordinate*, unsigned char*);

void alphaBetaFilterInit(struct AlphaBetaFilterCoordinate*, const double, const double, const double);