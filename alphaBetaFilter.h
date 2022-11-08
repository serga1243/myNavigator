#pragma once

#include "myNavigator.h"

struct AlphaBetaFilterCoordinate;
struct Coordinate;

void alphaBetaFilter(struct MyNavigator*, struct AlphaBetaFilterCoordinate*, struct Coordinate*);

void alphaBetaFilterInit(struct AlphaBetaFilterCoordinate*, const double, const double, const double);
