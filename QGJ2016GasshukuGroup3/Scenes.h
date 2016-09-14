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
	SETSUMEI
};

class Boss : public Rect{
	//Handle�n
	const int W = 20, H = 15;
	int ax, ay;
	int hp,maxhp;
	int pattern;
	bool gameover,flag = false;
	Rect p;
	vector<vector<Rect>> tile;
public:
	int body, arm, bgm, time,time2;
	Boss();
	void Init();
	void Update();
	void Draw();
	bool IsOver();
	bool IsEnd();
};

STATE title();
STATE game();
STATE boss();
STATE result();
STATE gameover();
STATE setsumei();