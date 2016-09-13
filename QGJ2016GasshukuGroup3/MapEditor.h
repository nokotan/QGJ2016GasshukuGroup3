#pragma once

#include "DxLib.h"
#include"Object.h"
#include"Input.h"
#include<string>
#include<vector>
#include<sstream>
#include<fstream>
#include<utility>

#define pos(i,j) ((i) + mwidth*(j))
#define saferange(x,y,w,h) (0 <= x && x < w && 0 <= y && y < h)

using namespace std;

//定数式置き場所
static const string empty_string;
const unsigned int red = GetColor(0, 0, 255);
const unsigned int green = GetColor(0, 200, 0);
const unsigned int blue = GetColor(255, 0, 0);
const unsigned int white = GetColor(255, 255, 255);
const unsigned int black = GetColor(0, 0, 0);
const unsigned int StringColor = red;
const int X = 0, Y = 40;//マップの原点

//グローバル変数	

//以下関数
void Split(vector<string> &, const string&, const string&);
string itos(int);
double GetMax(vector<double>);
int GetMaxDDT(vector<vector<int>>);
enum MAPSTATE{
	NONE = -1,
	ROAD = 0,
	PUT,
	NOPUT,
	START,
	GOAL,
	MAPSTATESIZE,
};


//以下クラス
class MapFragment{
private:
	string filePath;
	int drawH, x, y, width, height;
	int attribute;
public:
	MapFragment();
	MapFragment(int,int,int,string);
	void Update();
	void Draw();
	void DrawKind();
	bool LoadFail();
	bool InMouseClick();
	int GetAttribute();
	Pos GetPos();
};

class MapViewer{
private:
	string filePath;
	int handle,width, height, mwidth, mheight, imagenum, select, attribute,mapW,mapH;
	bool fail,atbflag;
	Pos startPos, endPos,startSuf,endSuf,selectPos;
	vector<int> drawHs;
	vector<vector<int>> v;
	vector< string > ds,filepaths;
	vector<MapFragment> fragments;
public:
	MapViewer();
	MapViewer(int);
	void Update();
	void Draw();
	void DrawInfo();
	void SetData(int);
	bool Fail();
	int GetSelect();
	int GetSelectAttribute();
	void SetTileKind(vector<vector<int>>&);
	Pos GetSelectPos();
	Pos FindStartPos();
	Pos FindEndPos();
};
