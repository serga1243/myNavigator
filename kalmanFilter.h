#pragma once

#include "myNavigator.h"

void kalmanFilter(struct KalmanFilterCoordinate*, struct Coordinate*, unsigned char*);

void kalmanFilterInit(struct KalmanFilterCoordinate*, const double);