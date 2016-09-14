#pragma once

#include "BaseUnit.h"

class Lift {
	int Flames;
	CMap MyMap;

public:
	// �����ʒu
	__declspec(property(get = GetX, put = SetX))
	int X;
	// �����ʒu
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

	// �����p�^�[��
	enum MovePattern {
		Side,
		UpAndDown,
		Rotation
	} MyPattern;

	Lift();

	// �X�V
	void Update();

	// ���Z�b�g
	void Reset();

	// �����蔻��I�u�W�F�N�g
	CMap& GetCollider() {
		return MyMap;
	}
};