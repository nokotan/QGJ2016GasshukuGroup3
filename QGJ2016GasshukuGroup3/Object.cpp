#include "Object.h"
#include"Dxlib.h"
#include<utility>
#include "Materials.h"
#include "Input.h"

Object::Object(double _x, double _y, char* _path) :x(_x), y(_y), path(_path){
	handle = LoadGraph(_path);
	GetGraphSize(handle, &width, &height);
	flag = false;
};

void Object::Update(){
	if (InMouseClick()){
		flag = !flag;
	}
}

void Object::Draw(){
	DrawGraph(x, y, handle, TRUE);
}

bool Object::InMouseClick(){
	return x <= getMouseX() && (getMouseX() <= x + width) && y <= getMouseY() && (getMouseY() <= y + height) && getMouseLeftPress(PRESS_ONCE);
}

bool Object::InMousePointer(){
	return x <= getMouseX() && (getMouseX() <= x + width) && y <= getMouseY() && (getMouseY() <= y + height);
}

Pos Object::GetPosition() {
	return Pos(x, y);
}

void Object::PosSet(Pos p) {
	x = p.x;
	y = p.y;
}

bool Object::operator!=(Object* op) {
	return this != op;
}

void Rect::Update() {
	++time;
	if (pattern) {
		if (time >= 600) {
			time = 0;
			if (pattern == 1) {
				pattern = 2;
			}
			else {
				pattern = 1;
			}
		}
	}
}

void Rect::Draw() {
	DrawGraph(x, y, handle, TRUE);
}

void Rect::SetPattern(int _p,int _t) {
	pattern = _p;
	time = _t;
}

void Rect::SetHandle(int _h) {
	handle = _h;
}

//�~�Ɖ~�̐ڐG����
bool CircleCollision(const Circle &m1, const Circle &m2){
	double dx, dy, r;
	dx = m2.x - m1.x;//���������̋���
	dy = m2.y - m1.y;//���������̋���
	r = m2.r + m1.r;//���a�̘a

	//�O�����̒藝
	return ((dx*dx) + (dy*dy)<(r*r));//�������Ă�����true
}

//�x�N�g�������֐�
Vector CreateVector(const Pos& p, const Pos& q){
	return move(Vector(q.x - p.x, q.y - p.y));//p->q�x�N�g��
}

//�x�N�g���̓���
double InnerProduct(const Vector& a, const Vector& b){
	return (a.x*b.x + a.y*b.y);//a�Eb
}

//�x�N�g���̊O��
double OuterProduct(const Vector& a, const Vector& b){
	return (a.x*b.y - a.y*b.x);//a�~b
}

//�x�N�g���̒�����2��
double VectorLength2(const Vector& v){
	return InnerProduct(v, v);//v�Ev=|v|^2
}

//�l���̏�ɑ��݂��Ă��邩(�~�Ƌ�`)
bool RectCollision1(const Rect& r, const Circle& m){
	Circle t;

	for (int i = 0; i<4; i++){
		t.x = r.p[i].x;	t.y = r.p[i].y;
		if (CircleCollision(t, m)){
			return true;
		}
	}

	return false;
}

bool RectCollision2(const Rect& r, const Circle& m){
	Vector pq, pm, qm;
	double d2, cross;
	const int n[][4] = { { 0, 1, 3, 2 }, { 1, 3, 2, 0 } };

	for (int i = 0; i<4; i++){
		pq = CreateVector(r.p[n[0][i]], r.p[n[1][i]]);
		pm = CreateVector(r.p[n[0][i]], Pos(m.x,m.y));
		qm = CreateVector(r.p[n[1][i]], Pos(m.x,m.y));

		cross = OuterProduct(pm, pq);
		d2 = cross*cross / VectorLength2(pq);//������2��

		if (d2<m.r*m.r){//������\��������
			if (InnerProduct(pm, pq)*InnerProduct(qm, pq)<0){//�����ɂ���
				return true;
			}
		}
	}
	return false;
}

bool RectCollision3(const Rect& r, const Circle& m){
	Vector pp, pm;
	Pos p(m.x,m.y);
	double inner, outer, theta[2];

	for (int i = 0; i < 2; i++){
		pp = CreateVector(r.p[i * 3], r.p[1 + i]);//0.1:3.2
		pm = CreateVector(r.p[i * 3], p);

		inner = InnerProduct(pp, pm);
		outer = OuterProduct(pp, pm);

		theta[i] = RTOD(atan2(outer, inner));
	}

	if (0 < theta[0] && theta[0] < 90 &&
		0 < theta[1] && theta[1] < 90){
		return true;
	}

	return false;
}

//���[�U�[(�����`)�Ɖ~�̐ڐG����
bool RectCollision(const Rect& r, const Circle& m) {
	if (RectCollision1(r, m)) {
		return true;
	}
	if (RectCollision2(r, m)) {
		return true;
	}
	if (RectCollision3(r, m)) {
		return true;
	}
	return false;
}
bool RectCollision(const Circle& m, const Rect& r) {
	return RectCollision(r,m);
}

bool RectCollision(const Rect& a, const Rect& b) {
	if ((a.p[0].x < b.p[3].x)
		&& (a.p[3].x > b.p[0].x)
		&& (a.p[0].y < b.p[3].y)
		&& (a.p[3].y > b.p[0].y)) {
		return true;
	}
	return false;
}