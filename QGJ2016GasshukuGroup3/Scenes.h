#pragma once
#include "Particles.h"
#include "Object.h"

enum STATE {
	EXIT = -1,
	TITLE = 0,
	GAME,
	BOSS,
	RESULT,
	GAMEOVER,
};

class Boss : public Rect{
	//Handleån
	const int W = 20, H = 15;
	int body,arm, bgm;
	int ax, ay;
	int hp,maxhp;
	int pattern;
	vector<vector<Rect>> tile;
public:
	Boss();
	void Init();
	void Update();
	void Draw();
	int GetHP();
};

STATE title();
STATE game();
STATE boss();
STATE result();
STATE gameover();
