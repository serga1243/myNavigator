#pragma once

#include "myNavigator.h"

struct MyNavigator;
struct Coordinate;


void changeMsg(struct MyNavigator*);

void overwriteStr(struct Coordinate*, char[], unsigned short);

unsigned short getNumLen(char[]);
