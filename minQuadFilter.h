#pragma once

#include "myNavigator.h"

struct MinQuadFilterCoordinate;
struct Coordinate;


void minQuadFilter(struct MinQuadFilterCoordinate*, struct Coordinate*);

void minQuadFilterInit(struct MinQuadFilterCoordinate*);
