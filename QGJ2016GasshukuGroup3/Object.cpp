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

void Rect::Draw() {
	DrawGraph(x, y, handle, TRUE);
}

void Rect::SetHandle(int _h) {
	handle = _h;
}

//円と円の接触判定
bool CircleCollision(const Circle &m1, const Circle &m2){
	double dx, dy, r;
	dx = m2.x - m1.x;//水平方向の距離
	dy = m2.y - m1.y;//鉛直方向の距離
	r = m2.r + m1.r;//半径の和

	//三平方の定理
	return ((dx*dx) + (dy*dy)<(r*r));//当たっていたらtrue
}

//ベクトル生成関数
Vector CreateVector(const Pos& p, const Pos& q){
	return move(Vector(q.x - p.x, q.y - p.y));//p->qベクトル
}

//ベクトルの内積
double InnerProduct(const Vector& a, const Vector& b){
	return (a.x*b.x + a.y*b.y);//a・b
}

//ベクトルの外積
double OuterProduct(const Vector& a, const Vector& b){
	return (a.x*b.y - a.y*b.x);//a×b
}

//ベクトルの長さの2乗
double VectorLength2(const Vector& v){
	return InnerProduct(v, v);//v・v=|v|^2
}

//四隅の上に存在しているか(円と矩形)
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
		d2 = cross*cross / VectorLength2(pq);//距離の2乗

		if (d2<m.r*m.r){//当たる可能性がある
			if (InnerProduct(pm, pq)*InnerProduct(qm, pq)<0){//内側にある
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

	if (0 <= theta[0] && theta[0] <= 90 &&
		0 <= theta[1] && theta[1] <= 90){
		return true;
	}

	return false;
}

//レーザー(長方形)と円の接触判定
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