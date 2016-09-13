#include"MapEditor.h"

using namespace std;

MapFragment::MapFragment(){};

MapFragment::MapFragment(int _x, int _y, MAPSTATE _attribute, string _filePath) :x(_x), y(_y), attribute(_attribute), filePath(_filePath){
	if (filePath != "") {
		//drawH = LoadGraph(filePath.c_str());
		GetGraphSize(drawH, &width, &height);
	}
};

void MapFragment::Update(){

}

void MapFragment::Draw(){
}

void MapFragment::DrawKind(){
	DrawFormatString(x,y, StringColor, " %d", attribute);
}

bool MapFragment::LoadFail(){
	return (-1 == drawH);
}

bool MapFragment::InMouseClick(){
	return x <= getMouseX() && (getMouseX() <= x + width) && y <= getMouseY() && (getMouseY() <= y + height  ) && getMouseLeftPress(PRESS_ONCE);
}

MAPSTATE MapFragment::GetAttribute(){
	return attribute;
}

Pos MapFragment::GetPos(){
	return Pos(x, y);
}

MapViewer::MapViewer(){};
MapViewer::MapViewer(int n){
	const string PATH = "Data/Map/SaveData";
	filePath = PATH + itos(n) + ".csv";
	SetData(n);
};

void MapViewer::Update(){
	atbflag = false;
	for (auto it = fragments.begin(); it != fragments.end();++it){
		it->Update();
		if (it->InMouseClick()){
			select = it - fragments.begin();
			attribute = it->GetAttribute();
			selectPos = it->GetPos();
			atbflag = true;
		}
	}
	if (!atbflag){
		attribute = -1;
		select = -1;
	}
}

void MapViewer::Draw(){
	DrawExtendGraph(BASISX, BASISY + 40, BASISX + mapW, BASISY + 40 + mapH, handle, TRUE);
	if (getKeySwitched(KEY_INPUT_V)) {
		for (auto n : fragments) {
			n.DrawKind();
		}
	}
	if (getKeySwitched(KEY_INPUT_I)){
		DrawInfo();
	}
}

void MapViewer::DrawInfo(){
	DrawFormatString(0, 400, StringColor, "MapSize(W*H) = %d * %d, Imagenum = %d", mwidth, mheight,imagenum);
	DrawFormatString(0,420,StringColor,"FragmentSize = %d ", fragments.size());
	DrawFormatString(0, 380, StringColor, "filePath = %d ", filePath.c_str());
}

void MapViewer::SetData(int number){
	mwidth = 0;
	mheight = 0;
	string str;
	stringstream ss;
	ifstream ifs;
	const string PATH = "Data/Map/SaveData";
	filePath = PATH + itos(number) + ".csv";
	ifs.open(filePath);
	fail = ifs.fail();
	getline(ifs.seekg(0, ios_base::cur), str, ',');
	ds.resize(2);
	Split(ds, str, ":");
	ss.str(ds[0]);
	ss >> width;
	ss.str(empty_string);
	ss.clear();
	ss.str(ds[1]);
	ss >> height;
	ss.str(empty_string);
	ss.clear();
	getline(ifs.seekg(0, ios_base::cur), str, ',');
	Split(ds, str, ":");
	ss.str(ds[0]);
	ss >>mwidth;
	ss.str(empty_string);
	ss.clear();
	ss.str(ds[1]);

	ss >> mheight;
	ss.str(empty_string);
	ss.clear();
	getline(ifs.seekg(0, ios_base::cur), str, ',');
	ss.str(str);
	ss >> imagenum;
	ss.str(empty_string);
	ss.clear();
	getline(ifs.seekg(0, ios_base::cur), str);
	filepaths.resize(imagenum);
	for (int i = 0; i < imagenum; ++i){//ファイル名の読み込み
		getline(ifs.seekg(0, ios_base::cur), str, ',');
		str = "Picture/MapTip/" + str;
		ss.str(str);
		ss >> filepaths[i];
		ss.str(empty_string);
		ss.clear();
	}
	fragments.resize(mwidth*mheight);
	int k = 0;
	for (int i = 0, x = 0, y = 0; i < mheight*mwidth; ++i){
		int j = i % mwidth;
		if (i != 0 && j == 0) {
			++k;
			getline(ifs.seekg(0, ios_base::cur), str);
		}
		getline(ifs.seekg(0, ios_base::cur), str, ',');
		Split(ds, str, ":");
		ss.str(ds[0]);
		ss >> x;
		ss.str(empty_string);
		ss.clear();
		ss.str(ds[1]);
		ss >> y;
		if (y >= 0) {
			fragments[pos(j, k)] = MapFragment(j*width + BASISX, k*height + BASISY+Y, (MAPSTATE)x, filepaths[y]);
		}
		else {
			fragments[pos(j, k)] = MapFragment(j*width + BASISX, k*height + BASISY+Y, NONE, "");
		}
		ss.str(empty_string);
		ss.clear();
	}
}

bool MapViewer::Fail(){
	return fail;
}

int MapViewer::GetSelect(){
	return select;
}

int MapViewer::GetSelectAttribute(){
	return attribute;
}


void MapViewer::SetTileKind(vector<vector<int>> &_v) {
	for (int i = 0; i < _v.size(); ++i) {
		for (int j = 0; j < _v[i].size(); ++j) {
			_v[i][j] = fragments[pos(j,i)].GetAttribute();
		}
	}
	v = _v;
}

Pos MapViewer::GetSelectPos(){
	return selectPos;
}


Pos MapViewer::FindStartPos(){
	return startPos;
}

Pos MapViewer::FindEndPos(){
	return endPos;
}

void Split(vector<string> &ds,const string& str, const string& delimiter) {
	// delimiter(2 文字以上も可) を空白に置換
	if (str == "") return;
	string item(str);
	int a;
	for (unsigned pos = item.find(delimiter); pos != string::npos; pos = item.find(delimiter, pos)) {
		item.replace(pos, delimiter.size(), " ");
	}
	// 分解
	stringstream buf(item);

	// 読み取り
	ds.clear();
	while (buf >> item) {
		ds.push_back(item);
	}
}

string itos(int num){
	stringstream ss;
	ss << num;
	return ss.str();
}

double GetMax(vector<double> dt){
	double max = 0.0;
	for (auto n : dt){
		if (n > max){
			max = n;
		}
	}
	return max;
}

int GetMaxDDT(vector<vector<int>> ddt){
	int max = 0.0;
	for (auto n : ddt){
		for (auto n2 : n){
			if (n2 > max){
				max = n2;
			}
		}
	}
	return max;
}