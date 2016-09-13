#pragma once
#include "DxLib.h"

const int BASISX = 0;
const int BASISY = 0;

static bool DEBUGMODE = false;
const int WinWidth = DEBUGMODE ? 880 : 680;
const int WinHeight = DEBUGMODE ? 780 : 480;

const double PI = 3.14159265359;

const unsigned int Red = GetColor(0,0,255);
const unsigned int Green = GetColor(0, 200, 0);
const unsigned int Blue = GetColor(255, 0, 0);
const unsigned int White = GetColor(255, 255, 255);
const unsigned int Black = GetColor(0, 0, 0);
const unsigned int Yellow = GetColor(0, 255, 255);
const unsigned int Purple = GetColor(138, 8, 134);
const unsigned int Pink = GetColor(129, 68,255);