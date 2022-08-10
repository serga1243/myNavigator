#pragma once

#include "myNavigator.h"

struct MyNavigator;
struct Coordinate;


void changeMsg(struct MyNavigator*);

void overwriteStr(struct Coordinate*, uint8_t[], uint16_t);

uint16_t getNumLen(uint8_t[]);
