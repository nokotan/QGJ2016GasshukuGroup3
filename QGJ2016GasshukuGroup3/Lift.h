#pragma once

#include "BaseUnit.h"

class Lift {
	int Flames;
	CMap MyMap;

public:
	// 初期位置
	__declspec(property(get = GetX, put = SetX))
	int X;
	// 初期位置
	__declspec(property(get = GetY, put = SetY))
	int Y;

	void SetX(int x) {
		MyMap.X = x - 20 * MapTile::MapSize;
	}

	int GetX() {
		return MyMap.X;
	}

	void SetY(int y) {
		MyMap.Y = y - 25 * MapTile::MapSize;
	}

	int GetY() {
		return MyMap.Y;
	}

	// 動くパターン
	enum MovePattern {
		Side,
		UpAndDown,
		Rotation
	} MyPattern;

	Lift();

	// 更新
	void Update();

	// リセット
	void Reset();

	// あたり判定オブジェクト
	CMap& GetCollider() {
		return MyMap;
	}
};