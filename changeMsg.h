#pragma once

#include "myNavigator.h"

struct MyNavigator;
struct Coordinate;


void changeMsg(struct MyNavigator*, unsigned char[]);

void overwriteStr(struct Coordinate*, char[], unsigned short, unsigned char*);

unsigned short getNumLen(char[]);
