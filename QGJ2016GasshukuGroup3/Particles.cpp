#include"Particles.h"

Particle::Particle(int _x, int _y){
	x = _x;
	y = _y;
	particleHandle = LoadGraph("Graphic/ink.png");
	time = 0;
	live_f = true;
	v = GetRand(10);
	theta = GetRand(360) * PI / 180;
	dx = v * cos(theta);
	dy = v * sin(theta);
	size = 1.0*GetRand(100) / 100;
	alpha = GetRand(255);
	vtime = 60 + GetRand(60);
	finish = false;
	end = false;
}

void Particle::Draw(Particle *p){
	if (!p->end){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, p->alpha);
		DrawRotaGraph(p->x, p->y, p->size, 0, p->particleHandle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void Particle::Update(Particle *p){
	++p->time;
	if (p->time > p->vtime){
		p->v = 3;
		p->finish = true;
	}
	if (p->time > 2 * p->vtime){
		p->end = true;
	}
	if (!p->end){
		if ((p->vtime % p->time == 0 && !p->finish)){
			p->x += p->dx;
			p->y += p->dy;
		}
		else if (p->finish) {
			p->end = true;
		}
	}
}

void Particle::UpdateParticles(){
	for (auto it = particles.begin(); it != particles.end(); ) {
		if ((*it)->end) {
			it = particles.erase(it);
		}
		else {
			(*it)->Update(*it);
			++it;
		}
	}
}

void Particle::DrawParticles(){
	for (auto it = particles.begin(); it != particles.end(); ++it) {
		(*it)->Draw(*it);
	}
	if (particles.size() > 0) {
		DrawFormatString(700, 700, Black, "%d", particles.size());
	}
}
void Particle::Factory(Particle *p) {
	for (auto it = particles.begin(); it != particles.end(); ++it) {
		if ((*it)->end) {
			*it = p;
			return;
		}
	}
	particles.push_back(p);
}

void Particle::Clear(){
	particles.clear();
}
