#pragma once

#include<string>
#include "Materials.h"

using namespace std;

#define RTOD(RAD) ((RAD)*(180/PI))
#define DTOR(DEG) ((DEG)*(PI/180))

class Pos{
public:
	double x, y;
	Pos(){};
	Pos(double _x, double _y) :x(_x), y(_y){};
	bool operator==(const Pos &pos){
		return x == pos.x && y == pos.y;
	}
};

using Vector = Pos;

class Object{
protected:
	int handle, num;
	int time;
	string path;
	bool fail,flag;
public:
	double x, y, r;
	int width, height;
	Object(){};
	~Object() {};
	Object(double, double, char*);
	virtual void Init(){};
	virtual void Update();
	virtual void Draw();
	bool InMouseClick();
	bool InMousePointer();
	Pos GetPosition();
	void PosSet(Pos);
	bool operator!=(Object*);
};

class Circle :public Object{
public:
	double r;
	Circle(){ r = 0.0; }
	Circle(int _x, int _y, double _r) :r(_r){
		x = _x;
		y = _y;
	}
};

class Rect :public Object{
	int kind;
public:
	Pos p[4];
	Rect(){};
	Rect(Pos p0, Pos p1, Pos p2, Pos p3){
		p[0] = p0;//  p0------p1
		p[1] = p1;//  |		  |
		p[2] = p2;//  |		  |
		p[3] = p3;//  p2------p3
	};
	void Draw();
	void SetHandle(int);
};


Vector CreateVector(const Pos&, const Pos&);
double VectorLength2(const Vector&);
double InnerProduct(const Vector&, const Vector&);
double OuterProduct(const Vector&, const Vector&);
//円と矩形
bool CircleCollision(const Circle&, const Circle&);
bool RectCollision1(const Rect&, const Circle&);
bool RectCollision2(const Rect&, const Circle&);
bool RectCollision3(const Rect&, const Circle&);
bool RectCollision(const Rect&, const Circle&);
bool RectCollision(const Rect&, const Circle&);
//矩形と矩形
bool RectCollision(const Rect&, const Rect&);
//http://1st.geocities.jp/shift486909/program/collision.html　より