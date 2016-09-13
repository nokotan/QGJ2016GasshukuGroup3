#pragma once
#include "DxLib.h"

#define PRESS		0
#define PRESS_NOT	1
#define PRESS_ONCE	2

const int joypads[3] = { DX_INPUT_PAD2, DX_INPUT_PAD3, DX_INPUT_PAD4 };
const int masks[32] = { PAD_INPUT_DOWN, PAD_INPUT_LEFT, PAD_INPUT_RIGHT, PAD_INPUT_UP,
PAD_INPUT_1, PAD_INPUT_2, PAD_INPUT_3, PAD_INPUT_4, PAD_INPUT_5, PAD_INPUT_6, PAD_INPUT_7, PAD_INPUT_8, PAD_INPUT_9, PAD_INPUT_10,
PAD_INPUT_11, PAD_INPUT_12, PAD_INPUT_13, PAD_INPUT_14, PAD_INPUT_15, PAD_INPUT_16, PAD_INPUT_17, PAD_INPUT_18, PAD_INPUT_19, PAD_INPUT_20,
PAD_INPUT_21, PAD_INPUT_22, PAD_INPUT_23, PAD_INPUT_24, PAD_INPUT_25, PAD_INPUT_26, PAD_INPUT_27, PAD_INPUT_28 };
const int KEY_INPUT_CONTROL[2] = { KEY_INPUT_LCONTROL, KEY_INPUT_RCONTROL };
const int KEY_INPUT_SHIFT[2] = { KEY_INPUT_LSHIFT, KEY_INPUT_RSHIFT };
const int KEY_INPUT_ALT[2] = { KEY_INPUT_LALT, KEY_INPUT_RALT };

//キーボード
int gpUpdateKey();
int getKey(const int KeyCode);
bool getKeyInterval(const int KeyCode, int interval);
bool getKeySwitched(const int);
bool getKeyPress(const int KeyCode, int par);
bool getKeyPress2(const int KeyCode[], int par, int num);
bool getKeyPressDoubleOnce(const int KeyCode1, const int KeyCode2);
bool getKeyPressDoubleOnce1_2(const int KeyCode1, const int KeyCode2[], int num);
bool getKeyPressDoubleOnce2_2(const int KeyCode1[], const int KeyCode2[], int num1, int num2);

//マウス
int gpUpdateMouse();
int getMouseX();
int getMouseY();
int getMouseLeftPressTime();
int getMouseRightPressTime();
int getMouseMiddlePressTime();
int getMouseWheelVol();
int getMouseWheelVolPM();
bool getMouseLeftPress(int par);
bool getMouseRightPress(int par);
bool getMouseMiddlePress(int par);
//左上の座標、右下の座標指定
bool checkMouseSquare(double x1, double y1, double x2, double y2);
//左上の座標、縦横指定
bool checkMouseSquare2(double x, double y, double w, double h);
bool checkMouseCircle(double x, double y, double r);

//ゲームパッド
int gpUpdatePad();
int getPadPressTime(int PadNum, int Mask);
bool getPadPress(int PadNum, int Mask, int par);