#include "Lift.h"
#include <cmath>
#include "DxLib.h"

Lift::Lift() :
	MyMap(60, 45, -1) {
	
	for (int i : { 20, 21, 22, 23, 24, 25 }) {
		MyMap[i][25] = 0;
	}
}

void Lift::Update() {

	switch (MyPattern) {
		case Lift::Side:
			if (Flames == 0) {
				MyMap.DeltaX = 2;
			} else if (Flames == 80) {
				MyMap.DeltaX = -2;
			} else if (Flames == 160 - 1) {
				Flames = -1;
			}
			break;
		case Lift::UpAndDown:
			if (Flames == 0) {
				MyMap.DeltaY = 2;
			} else if (Flames == 80) {
				MyMap.DeltaY = -2;
			} else if (Flames == 160 - 1) {
				Flames = -1;
			}
			break;
		case Lift::Rotation:
			MyMap.DeltaX = static_cast<int>(3 * cos(Flames * 2 * DX_PI / 360));
			MyMap.DeltaY = static_cast<int>(3 * sin(Flames * 2 * DX_PI / 360));
			
			if (Flames == 360 - 1) {
				Flames = -1;
			}
		default:
			break;
	}

	Flames++;

	MyMap.X += MyMap.DeltaX;
	MyMap.Y += MyMap.DeltaY;
}