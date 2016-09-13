#pragma once
#include "Dxlib.h"
#include"Materials.h"
#include<vector>
#include<memory>

using namespace std;

class Particle{
	int x, y, alpha, time, vtime, live_f;
	int particleHandle;
	double v, theta,angle, size, dx, dy;
	bool finish,end;
	vector<Particle*> particles;
public:
	Particle(){};
	Particle(int, int);
	static void Draw(Particle*);
	static void Update(Particle*);
	void UpdateParticles();
	void DrawParticles();
	void Factory(Particle*);
	void Clear();
};
