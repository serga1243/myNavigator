#pragma once

#include "myNavigator.h"

struct KalmanFilterCoordinate;
struct Coordinate;

void kalmanFilter(struct KalmanFilterCoordinate*, struct Coordinate*, unsigned char*);

void kalmanFilterInit(struct KalmanFilterCoordinate*, const double, const double);