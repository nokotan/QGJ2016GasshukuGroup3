#include "DxLib.h"
#include "Collider.h"
#include "MapEditor.h"
#include "Scenes.h"
#include "Lift.h"
#include <complex>
#include <cmath>
#include<algorithm>

using namespace std;

int Sound1, Sound2, Sound3;
// 決定音
int KetteiSound;


bool titleflag = false;
int titleHandle;
int FontHandle;
STATE title() {
	if (!titleflag) {
		titleHandle = LoadGraph("Graphic/タイトル画面.png");

		//音楽のための変数と読み込み
		Sound1 = LoadSoundMem("音楽/合宿QGJ_タイトル.ogg");
//		ChangeVolumeSoundMem(216, Sound1);
		Sound2 = LoadSoundMem("音楽/合宿QGJ_メイン.ogg");
	//	ChangeVolumeSoundMem(216, Sound2);
		Sound3 = LoadSoundMem("音楽/合宿QGJ_リザルト.ogg");
		//ChangeVolumeSoundMem(216, Sound3);
		KetteiSound = LoadSoundMem("音楽/合宿QGJ_SE_決定音.ogg");

		PlaySoundMem(Sound1, DX_PLAYTYPE_LOOP);

		// 作成したデータの識別番号を変数 FontHandle に保存する
		FontHandle = CreateFontToHandle(NULL, 40, 3, DX_FONTTYPE_ANTIALIASING);
		titleflag = true;
	}
	else {
		// キーの入力待ち
		if (getKeyPress(KEY_INPUT_SPACE,PRESS_ONCE)) {
			// 作成したフォントデータを削除する
			DeleteFontToHandle(FontHandle);
			StopSoundMem(Sound1);
			PlaySoundMem(KetteiSound, DX_PLAYTYPE_BACK);
			return GAME;
		}

		//タイトル描画
		

		// 読みこんだグラフィックを画面左上に描画
		DrawGraph(0, 0, titleHandle, TRUE);
		DrawStringToHandle(200, 400, "PRESS SPACE !!", GetColor(0, 255, 255), FontHandle);
		//ScreenFlip();//描画の反映
	}

	return TITLE;
}

struct Player {
	int x, y, width, height, dx, dy, fly, deathcount1, deathcount2;
	int FloorDeltaX;

	// 方向を表します。
	enum Direction {
		// 左向き
		Direction_Left = 1,
		// 右向き
		Direction_Right = 2,
	} FaceDirection;
	int AnimationFlame;
	int CollidedDirection;

	bool OnCollideFromSide(int& tileid, int, int);
	bool OnCollideFromBottom(int& tileid, int, int);
	bool OnCollideFromTop(int& tileid, int, int);
};

Player player;

bool Player::OnCollideFromSide(int& tileid, int, int) {
	x = 0;

	if (tileid == 3) {
		return true;
	}

	for (int id : { 5, 6, 7, 8 }) {
		if (tileid == id) {
			// 死亡
			deathcount2++;
		}
	}

	// 返り値を false にすると、あたり判定を無視しない。
	return false;
}

bool Player::OnCollideFromBottom(int& tileid, int, int) {
	if (tileid == 3) {
		return true;
	}

	fly = 0;//0のとき飛べる

	for (int id : { 5, 6, 7, 8 }) {
		if (tileid == id) {
			// 死亡
			deathcount2++;
		}
	}

	return false;
}

bool Player::OnCollideFromTop(int& tileid, int i, int j) {

	if (tileid == 4) {
		// ブロックを実体化
		tileid = 0;
		//int *tileobjptr = &tileid;
		//*(tileobjptr + 1) = 2;
		//*(tileobjptr - 1) = 2;
		//*(tileobjptr - 15) = 2;
		//*(tileobjptr + 15) = 2;
	} else if (tileid == 3) {
		return true;
	}

	for (int id : { 5, 6, 7, 8 }) {
		if (tileid == id) {
			// 死亡
			deathcount2++;
		}
	}

	return false;
}

struct Tile {
	int x, y, dx, dy, width, height;
	bool flag, flag2;
	int dir;
};
const int TILE_MAX = 20;
Tile ball[TILE_MAX];
Tile bridge[TILE_MAX];
Tile drill[TILE_MAX];
Tile invis[TILE_MAX];
Lift Lifts[TILE_MAX];

int stagenum = 1;

static int ballcount = 0;
static int bcount = 0;
static int drillcount = 0;
static int inviscount = 0;
static int LiftCount = 0;

//1は敵２は自機、あたり判定
bool Checkhitchery(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) {
	if (x1 < (x2 + width2)) {
		if ((x1 + width1) > x2) {
			if (y1 < (y2 + height2) ) {
				if ((y1 + height1) >y2) {
					return true;
				}
				return false;
			}
			return false;
		}
		return false;
	}
	return false;
}

void moveBall(Tile* ball) {
	for (int i = 0; i < ballcount; ++i) {
		if ((player.x + player.width) >= ball[i].x) {
			ball[i].dy = 30;
			ball[i].flag = true;
		}
		ball[i].y += ball[i].dy;
	}
	for (int i = 0; i < ballcount; ++i) {
		if (Checkhitchery(ball[i].x, ball[i].y, ball[i].width, ball[i].height, player.x, player.y, player.width, player.height)) {
			player.deathcount2++;
		}
	}
}
const int dx[] = { 0,1,0,-1 };
const int dy[] = { 1,0,-1,0 };
const int MapTilesWidth = 20;
const int MapTilesHeight = 15;
const int drillsuf = 9;
int MapTiles[MapTilesWidth][MapTilesHeight];

bool IsDrillHit(Player p, Tile t) {
	int dir = t.dir;
	if (dir == 0 || dir == 2) {
		if (t.x -1<= p.x + p.width ) {
			return true;
		}
	}
	else if ((dir == 1)&& p.y <= t.y + t.height && t.x - p.x >= 0 && t.x - p.x < 32 * 3) {
		return true;
	}
	else if (dir == 3 && p.y <= t.y + t.height && p.x - t.x >= 0) {
		return true;
	}
	return false;
}

void drillAttack(Tile* drill ) {
	for (int i = 0; i < drillcount; ++i) {
			int dir = drill[i].dir;
		if ( drill[i].flag &&  IsDrillHit(player,drill[i])) {
			MapTiles[drill[i].x / 32][drill[i].y / 32] = -1;
			drill[i].dx = -10 * dx[dir];
			drill[i].dy = -10 * dy[dir];
			drill[i].flag = false;
		}
	}
	for (int i = 0; i < drillcount; ++i) {
		drill[i].x += drill[i].dx;
		drill[i].y += drill[i].dy;
	}
	for (int i = 0; i < drillcount; ++i) {
		if (Checkhitchery(drill[i].x, drill[i].y, drill[i].width, drill[i].height, player.x, player.y, player.width, player.height)) {
			player.deathcount2++;
		}
	}
}

void invisManifestation(Tile* t) {
	for (int i = 0; i < inviscount; ++i) {
		if (t[i].x <= player.x && player.x + player.width <= t[i].x + t[i].width) {
			if (player.y - t[i].y >= 0 && player.y - t[i].y <= 68) {
				t[i].flag = true;
			}
		}
	}
}

void moveBridge(Tile *b) {
	for (int i = 0; i < bcount; ++i) {
		if (b[i].flag2 && (player.x + player.width / 2) >= b[i].x && abs(player.y - b[i].y) < 100 && b[i].flag) {
			b[i].dy = 50;
			b[i].flag = false;
		}
	}
}

bool gameflag = false;
int BackImageHandle, jimen,toge[4], hasi, ballHandle;
int JumpSound, KilledSound;
int timer;
int PlayerImageHandles[3];
CMap MyMap;
MapViewer mv;
vector<vector<int>> tmp(MapTilesHeight, vector<int>(MapTilesWidth, -1));
Particle particle;


//初期化する関数
void Initialization(int map, MapViewer &mv) {
	player.x = 0;
	player.y = 200;
	player.width = 32;
	player.height = 64;
	player.dx = 0;
	player.dy = 0;
	//player.deathcount1 = 0;

	player.fly = 0;
	ballcount = 0;
	mv.SetData(map);
}


STATE game() {
	if (!gameflag) {
		// タイルマップとして使う２次元配列

		// タイルマップを -1 (何もない) で埋める
		for (int i = 0; i < MapTilesWidth; i++) {
			for (int j = 0; j < MapTilesHeight; j++) {
				MapTiles[i][j] = -1;

				// 一番下は床として 0 を入れておく
				if (j == MapTilesHeight - 1) { //&& (i < 5 || i > 7)) {
					MapTiles[i][j] = 0;
				}
			}
		}

		//タイマー
		timer = 0;
		//ステージの初期化
		stagenum = 1;


		// 背景の読み込み
		BackImageHandle = LoadGraph("Graphic/背景.jpg");
		// プレイヤーの画像の読み込み
		LoadDivGraph("Graphic/Character.png", 3, 3, 1, 32, 64, PlayerImageHandles);

		jimen = LoadGraph("Graphic/Jimen.png");
		hasi = LoadGraph("Graphic/Hasi.png");
		for (int i = 0; i < 4; ++i) {
			toge[i] = LoadGraph((string("Graphic/toge") + to_string(i)+ ".png").c_str());
		}
		ballHandle = LoadGraph("Graphic/ball.png");
		JumpSound = LoadSoundMem("音楽/合宿QGJ_SE_ジャンプ.ogg");
		KilledSound = LoadSoundMem("音楽/合宿QGJ_SE_死亡.ogg");

		for (auto& item : Lifts) {
			item.Reset();
		}

		Lifts[0].MyPattern = Lift::Side;
		Lifts[0].X = 0;
		Lifts[0].Y = 32 * 12;
		Lifts[1].MyPattern = Lift::Side;
		Lifts[1].X = 32 * 6;
		Lifts[1].Y = 32 * 6;
		Lifts[2].MyPattern = Lift::Rotation;
		Lifts[2].X = 32 * 10;
		Lifts[2].Y = 32 * 4;
		Lifts[3].MyPattern = Lift::UpAndDown;
		Lifts[3].X = 32;
		Lifts[3].Y = 32 * 6;
		LiftCount = 4;

		// player を初期化
		mv = MapViewer(1);
		Initialization(stagenum, mv);

		mv.SetTileKind(tmp);
		for (int i = 0; i < MapTilesHeight; ++i) {
			for (int j = 0; j < MapTilesWidth; ++j) {
				MapTiles[j][i] = tmp[i][j];
			}
		}
		//Ballのセット
		//Bridgeのセット（落ちる方）
		for (int i = 0; i < MapTilesHeight; ++i) {
			for (int j = 0; j < MapTilesWidth; ++j) {
				if (MapTiles[j][i] == 1) {
					bridge[bcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,true };
					++bcount;
				}
				else if (MapTiles[j][i] == 2) {
					bridge[bcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,false };
					++bcount;
				}
				else if (MapTiles[j][i] == 3) {
					ball[ballcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,false };
					++ballcount;
				}
				else if (MapTiles[j][i] == 4) {
					invis[inviscount] = Tile{ j * 32,i * 32,0,0,32,36,false };
					++inviscount;
				}
				else if (drillsuf - 4 <= MapTiles[j][i] && MapTiles[j][i] < drillsuf) {
					drill[drillcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,false,true, MapTiles[j][i] - (drillsuf - 4) };
					++drillcount;
				}
				else if (MapTiles[j][i] >= drillsuf) {//動くトゲ
					drill[drillcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,true, MapTiles[j][i] - drillsuf };
					++drillcount;
				}
			}
		}
		gameflag = true;
		return BOSS;
	}
	else{
		// 強制終了コマンド
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			StopSoundMem(Sound2);
			PlaySoundMem(Sound1, DX_PLAYTYPE_LOOP);
			gameflag = false;
			return TITLE;
		}

		// メインループ
		mv.Update();
		particle.UpdateParticles();


		//音楽の再生
		if (CheckSoundMem(Sound2) == 0) {
			PlaySoundMem(Sound2, DX_PLAYTYPE_LOOP, TRUE);
		}

		// 入力に応じて、プレイヤーのスピードを変える
		if (CheckHitKey(KEY_INPUT_LEFT)) {
			player.FaceDirection = Player::Direction::Direction_Left;
			player.AnimationFlame++;
			player.dx = -2 + player.FloorDeltaX;
		}
		else if (CheckHitKey(KEY_INPUT_RIGHT)) {
			player.FaceDirection = Player::Direction::Direction_Right;
			player.AnimationFlame++;
			player.dx = 2 + player.FloorDeltaX;
		}
		else {
			player.AnimationFlame = 0;
			player.dx = player.FloorDeltaX;
		}

		if (CheckHitKey(KEY_INPUT_SPACE) && player.fly == 0) { // && player.dy == 0) {
			PlaySoundMem(JumpSound, DX_PLAYTYPE_BACK);
			player.dy = -20;
			player.fly = 1;
		}

		// 重力加速度を加える。
		player.dy += 1;
		if (player.dy > 10) {
			player.dy = 10;
		}

		//落下死を加える
		if (player.y > 480) {
			player.x = 0;
			player.y = 382;
			player.fly = 0;
			player.deathcount2++;
		}
		//Tileを動かす
		//if () {//mapによって変更をする

		moveBall(ball);
		moveBridge(bridge);
		drillAttack(drill);
		invisManifestation(invis);

		//}

		for (int i = 0; i < LiftCount; i++) {
			Lifts[i].Update();
		}

		// あたり判定を行う。
		player.FloorDeltaX = 0;
		player.CollidedDirection = Direction::None;

		int DefX = player.x, DefY = player.y;
		int DefDeltaX = player.dx, DefDeltaY = player.dy;
		CollisionCheck(player, MapTiles, 32, -1);
		int NewX = player.x, NewY = player.y;
		int TempCollideDirection = Direction::None;
		TempCollideDirection = player.CollidedDirection;

		for (int i = 0; i < LiftCount; i++) {
			player.CollidedDirection = Direction::None;
			player.x = DefX; player.y = DefY;
			CollisionCheck(player, Lifts[i].GetCollider(), -1);

			if (DefDeltaX - Lifts[i].GetCollider().DeltaX > 0) {
				if (static_cast<bool>(player.CollidedDirection & Direction::Right) && player.x <= NewX) {
					NewX = player.x;
				}
			} else if (DefDeltaX - Lifts[i].GetCollider().DeltaX < 0) {
				if (static_cast<bool>(player.CollidedDirection & Direction::Left) && player.x >= NewX) {
					NewX = player.x;
				}
			}

			if (DefDeltaY - Lifts[i].GetCollider().DeltaY > 0) {
				if (static_cast<bool>(player.CollidedDirection & Direction::Down) && player.y <= NewY) {
					NewY = player.y;
				}
			}
			else if (DefDeltaY - Lifts[i].GetCollider().DeltaY < 0) {
				if (static_cast<bool>(player.CollidedDirection & Direction::Up) && player.y >= NewY) {
					NewY = player.y;
				}
			}

			TempCollideDirection |= player.CollidedDirection;
		}

		player.x = NewX;
		player.y = NewY;

		// 挟まり判定
		if ((TempCollideDirection & Direction::LeftAndRight) == Direction::LeftAndRight || (TempCollideDirection & Direction::UpAndDown) == Direction::UpAndDown) {
			player.deathcount2++;
		}

		clsDx();
		printfDx("Player\nCollideDirection : %d\nx : %d\ndx : %d\ndy : %d", player.CollidedDirection, player.x, player.dx, player.dy);

		//死んだらdeathcountを増やし仕掛けが元に戻る。playerは中間に飛ぶ(死亡処理)
		if (player.deathcount1 < player.deathcount2) {
			PlaySoundMem(KilledSound, DX_PLAYTYPE_BACK);

			player.deathcount1 = player.deathcount2;
			for (int i = 0; i < 30; ++i) {
				auto p = (new Particle(player.x,player.y));
				particle.Factory(p);
			}
			Initialization(stagenum, mv);
			mv.SetTileKind(tmp);
			for (int i = 0; i < MapTilesHeight; ++i) {
				for (int j = 0; j < MapTilesWidth; ++j) {
					MapTiles[j][i] = tmp[i][j];
				}
			}
			ballcount = 0;
			bcount = 0;
			drillcount = 0;
			inviscount = 0;
			for (int i = 0; i < MapTilesHeight; ++i) {
				for (int j = 0; j < MapTilesWidth; ++j) {
					if (MapTiles[j][i] == 1) {
						bridge[bcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,true };
						++bcount;
					}
					else if (MapTiles[j][i] == 2) {
						bridge[bcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,false };
						++bcount;
					}
					else if (MapTiles[j][i] == 3) {
						ball[ballcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,false };
						++ballcount;
					}
					else if (MapTiles[j][i] == 4) {
						invis[inviscount] = Tile{ j * 32,i * 32,0,0,32,36,false };
						++inviscount;
					}
					else if (drillsuf - 4 <= MapTiles[j][i] && MapTiles[j][i] < drillsuf) {
						drill[drillcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,false,true, MapTiles[j][i] - (drillsuf - 4) };
						++drillcount;
					}
					else if (MapTiles[j][i] >= drillsuf) {//動くトゲ
						drill[drillcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,true, MapTiles[j][i] - drillsuf };
						++drillcount;
					}
				}
			}
		}


		// 背景の描画
		DrawGraph(0, 0, BackImageHandle, FALSE);
		for (int i = 0; i < MapTilesHeight; ++i) {
			for (int j = 0; j < MapTilesWidth; ++j) {
				if (MapTiles[j][i] == 0) {
					DrawGraph(j * 32, i * 32, jimen, TRUE);
				}
			}
		}
		//落ちてくる球
		for (int i = 0; i < ballcount; ++i) {
			if (ball[i].flag)
				DrawGraph(ball[i].x, ball[i].y, ballHandle, TRUE);
		}

		//落ちる橋
		for (int i = 0; i < bcount; ++i) {
			if (bridge[i].flag) {
				DrawGraph(bridge[i].x, bridge[i].y, hasi, TRUE);
			}
			else if (bridge[i].flag2) {
				int X = bridge[i].x / 32, Y = bridge[i].y / 32;
				MapTiles[X][Y] = -1;
				bridge[i].flag2 = false;
			}
		}
		for (int i = 0; i < drillcount; ++i) {
			DrawGraph(drill[i].x, drill[i].y, toge[drill[i].dir], TRUE);
		}
		for (int i = 0; i < inviscount; ++i) {
			if(invis[i].flag)
				DrawGraph(invis[i].x, invis[i].y, jimen, TRUE);
		}

		for (int index = 0; index < LiftCount; index++) {
			for (int i = 0; i < Lifts[index].GetCollider().Cols(); i++) {
				for (int j = 0; j < Lifts[index].GetCollider().Rows(); j++) {
					if (Lifts[index].GetCollider()[i][j] != -1 && Lifts[index].GetCollider()[i][j] != 1) {
						DrawGraph(Lifts[index].GetCollider().X + i * 32, Lifts[index].GetCollider().Y + j * 32, jimen, TRUE);
						// DrawBox(MyMap.X + i * 32, MyMap.Y + j * 32, MyMap.X + i * 32 + 32, MyMap.Y + j * 32 + 32, GetColor(0, 216, 0), TRUE);
					}
				}
			}
		}

		if (player.FaceDirection == Player::Direction::Direction_Left) {
			DrawTurnGraph(player.x, player.y, PlayerImageHandles[((player.AnimationFlame / 5) + 1) % 3], TRUE);
		}
		else {
			DrawGraph(player.x, player.y, PlayerImageHandles[((player.AnimationFlame / 5) + 1) % 3], TRUE);
		}


		// 黒色の値を取得
		unsigned Cr;
		Cr = GetColor(0, 0, 0);

		//死亡回数、ステージ、残り時間の表示
		DrawFormatString(500, 0, Cr, "Death Count %d", player.deathcount1);
		DrawFormatString(500, 20, Cr, "Stage %d", stagenum);
		DrawFormatString(500, 40, Cr, "time %dmin %02dsec", (180 - timer/60)/60, 60 - (timer / 60) % 60 == 60 ? 0 : 60 - (timer / 60) % 60);

		if (player.x >= 608 && stagenum < 5) {
			//マップ移動
			player.x = 0;
			++stagenum;
			Initialization(stagenum, mv);
			mv.SetTileKind(tmp);
			for (int i = 0; i < MapTilesHeight; ++i) {
				for (int j = 0; j < MapTilesWidth; ++j) {
					MapTiles[j][i] = tmp[i][j];
				}
			}
			ballcount = 0;
			bcount = 0;
			drillcount = 0;
			for (int i = 0; i < MapTilesHeight; ++i) {
				for (int j = 0; j < MapTilesWidth; ++j) {
					if (MapTiles[j][i] == 1) {
						bridge[bcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,true };
						++bcount;
					}
					else if (MapTiles[j][i] == 2) {
						bridge[bcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,false };
						++bcount;
					}
					else if (MapTiles[j][i] == 3) {
						ball[ballcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,false };
						++ballcount;
					}
					else if (MapTiles[j][i] == 4) {
						invis[inviscount] = Tile{ j * 32,i * 32,0,0,32,32,false };
						++inviscount;
					}
					else if (drillsuf - 4 <= MapTiles[j][i] && MapTiles[j][i] < drillsuf) {
						drill[drillcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,false,true, MapTiles[j][i] - (drillsuf - 4) };
						++drillcount;
					}
					else if (MapTiles[j][i] >= drillsuf) {//動くトゲ
						drill[drillcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,true, MapTiles[j][i] - drillsuf };
						++drillcount;
					}
				}
			}
		}
		++timer;
		//三分経ったらゲームオーバー
		if (timer >= 180 * 60) {
			titleflag = false;
			gameflag = false;
			if (CheckSoundMem(Sound2) == 1) {
				StopSoundMem(Sound2);
			}
			return GAMEOVER;
		}
		mv.Draw();
		particle.DrawParticles();
	}
	return GAME;
}

bool bossflag = false;

Boss::Boss() {
	Init();
}

void drillAttack2(Tile* drill ) {
	for (int i = 0; i < drillcount; ++i) {
			int dir = drill[i].dir;
		if ( drill[i].flag) {
			MapTiles[drill[i].x / 32][drill[i].y / 32] = -1;
			drill[i].dx = -5 * dx[dir];
			drill[i].dy = -5 * dy[dir];
			drill[i].flag = false;
		}
	}
	for (int i = 0; i < drillcount; ++i) {
		drill[i].x += drill[i].dx;
		drill[i].y += drill[i].dy;
	}
	for (int i = 0; i < drillcount; ++i) {
		if (Checkhitchery(drill[i].x, drill[i].y, drill[i].width, drill[i].height, player.x, player.y, player.width, player.height)) {
			player.deathcount2++;
		}
	}
}
void Boss::Init() {
	body = LoadGraph("Graphic/God.png");
	arm = LoadGraph("Graphic/GodArm.png");
	bgm = LoadSoundMem("音楽/合宿QGJ_ボス戦.ogg");
	ax = -40, ay = -100;
	pattern = GetRand(3);
	gameover = false;
	time = 0;
	maxhp = hp = 6;
	player.x = 60, player.y = 100;
	//トゲ
	tile = vector<vector<Rect>>(W, vector<Rect>(H));
	for (int i = 0; i < W; ++i) {
		for (int j = 0; j < H; ++j) {
			MapTiles[i][j] = -1;
		}
	}
	for (int j = 0; j < H; ++j) {
		int i = 0;
		tile[i][j].PosSet(Pos(i*32,j*32));
		tile[0][j].SetHandle(toge[3]);
		tile[0][j].kind = 3;
		tile[0][j].pattern = 0;
		MapTiles[0][j] = 8;
	}
	for (int j = 0; j < H; ++j) {
		int i = W - 1;
		tile[i][j].PosSet(Pos(i*32,j*32));
		tile[i][j].SetHandle(toge[1]);
		tile[i][j].kind = 5;
		tile[i][j].pattern = 0;
		MapTiles[i][j] = 6;
	}
	for (int i = 0; i < W; ++i) {
		int j = 0;
		tile[i][j].PosSet(Pos(i*32,j*32));
		tile[i][j].SetHandle(toge[2]);
		tile[i][j].kind = 5;
		tile[i][j].pattern = 0;
		MapTiles[i][j] = 7;
	}
	for (int i = 0; i < W; ++i) {
		int j = H - 1;
		tile[i][j].PosSet(Pos(i*32,j*32));
		tile[i][j].SetHandle(toge[0]);
		tile[i][j].kind = 5;
		tile[i][j].pattern = 0;
		MapTiles[i][j] = 5;
	}
	//足場
	for (int i : {1, 3,5, 13,15,17}) {
		int j = 10;
		tile[i][j].PosSet(Pos(i*32,j*32));
		tile[i][j].SetHandle(jimen);
		tile[i][j].SetPattern(1, 0);
		tile[i][j].kind = 0;
		MapTiles[i][j] = 0;
	}
	for (int i : {7,9,11}) {
		int j = 11;
		tile[i][j].PosSet(Pos(i*32,j*32));
		tile[i][j].SetHandle(jimen);
		tile[i][j].SetPattern(1, 0);
		tile[i][j].kind = 0;
		MapTiles[i][j] = 0;
	}
	for (int i : {2,4,14,16}) {
		int j = 10;
		tile[i][j].PosSet(Pos(i*32,j*32));
		tile[i][j].SetHandle(jimen);
		tile[i][j].SetPattern(2, 0);
		tile[i][j].kind = 0;
		MapTiles[i][j] = 0;
	}
	for (int i : {6,8,10,12}) {
		int j = 11;
		tile[i][j].PosSet(Pos(i*32,j*32));
		tile[i][j].SetHandle(jimen);
		tile[i][j].SetPattern(2, 0);
		tile[i][j].kind = 0;
		MapTiles[i][j] = 0;
	}
	for (int i : {3, 15}) {
		int j = 7;
		tile[i][j].PosSet(Pos(i*32,j*32));
		tile[i][j].SetHandle(jimen);
		tile[i][j].SetPattern(0, 0);
		tile[i][j].kind = 0;
		MapTiles[i][j] = 0;
	}
	int x = 9, y = 8;
	tile[x][y].PosSet(Pos(x * 32, y * 32));
	tile[x][y].SetHandle(jimen);
	tile[x][y].SetPattern(0, 0);
	tile[x][y].kind = 0;
	MapTiles[x][y] = 0;
}

void Boss::Update() {
	++time;
	for (int i = 0; i < W; ++i) {
		for (int j = 0; j < H; ++j) {
			tile[i][j].Update();
			if (tile[i][j].pattern == 2) {
				MapTiles[i][j] = -1;
			}
			if (tile[i][j].pattern == 1) {
				MapTiles[i][j] = 0;
			}
			if (MapTiles[i][j] == 5) {
				if (Checkhitchery(player.x, player.y, 32, 32, i * 32, j * 32, 32, 32)) {
					++player.deathcount1;
					break;
				}
			}
		}
	}
	drillAttack2(drill);
	// 入力に応じて、プレイヤーのスピードを変える
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		player.FaceDirection = Player::Direction::Direction_Left;
		player.AnimationFlame++;
		player.dx = -2 + player.FloorDeltaX;
	}
	else if (CheckHitKey(KEY_INPUT_RIGHT)) {
		player.FaceDirection = Player::Direction::Direction_Right;
		player.AnimationFlame++;
		player.dx = 2 + player.FloorDeltaX;
	}
	else {
		player.AnimationFlame = 0;
		player.dx = player.FloorDeltaX;
	}

	if (CheckHitKey(KEY_INPUT_SPACE) && player.fly == 0) {
		player.dy = -20;
		player.fly = 1;
	}

	// 重力加速度を加える。
	player.dy += 1;
	if (player.dy > 10) {
		player.dy = 10;
	}
	CollisionCheck(player, MapTiles, 32, -1);
	if (player.y > 375 || player.x < 32 || player.x > 596 || player.y < 32) {
		for (int i = 0; i < 50; ++i) {
			auto pt = new Particle(player.x, player.y);
			particle.Factory(pt);
		}
		player.deathcount1 = player.deathcount2;
		maxhp = hp = 10;
		player.x = 60, player.y = 100;
		player.fly = 0;
		player.deathcount2++;
	}
	else if (player.deathcount1 < player.deathcount2) {
		player.deathcount1 = player.deathcount2;
		for (int i = 0; i < 50; ++i) {
			auto pt = new Particle(player.x, player.y);
			particle.Factory(pt);
		}
		maxhp = hp = 10;
		player.x = 60, player.y = 100;
	}
	particle.UpdateParticles();
	if (time >= 270 && !flag) {
		flag = true;
		time2 = 0;
	}
	if (time >= 300) {
		int i, j,k =0,dir;
		switch (pattern)
		{
		case 6:
		case 5:
			drillcount = 4;
			i = (GetRand(1)) ? 0: W-1 ;
			dir = (i == 0) ? 3: 1;
			for (j = 0; j < H; ++j) {
				if (MapTiles[i][j] >= drillsuf - 4 && GetRand(1)) {//動くトゲ
					drill[k] = Tile{ i * 32, j * 32, 0, 0, 32, 32,true,true, dir };
					++k;
					if (k == drillcount) {
						break;
					}
				}
			}
			time = -100;
			break;
		case 4:
		case 3:
			drillcount = 4;
			j = (GetRand(1)) ? 0 : H - 1;
			dir = (j == 0) ? 2 : 0;
			for (i = 0; i < W; ++i) {
				if (MapTiles[i][j] >= drillsuf - 4 && GetRand(1)) {//動くトゲ
					drill[k] = Tile{ i * 32, j * 32, 0, 0, 32, 32,true,true, dir };
					++k;
					if (k == drillcount) {
						break;
					}
				}
			}
			time = -100;
			break;
		case 2:
		case 1:
			drillcount = 5;
			for (i = 0; i < W; ++i) {
				for (j = 0; j < H; ++j) {
					if (MapTiles[i][j] >= 5 && GetRand(1)) {//動くトゲ
						drill[k] = Tile{ i * 32, j * 32, 0, 0, 32, 32,true,true, MapTiles[i][j] - 5 };
						++k;
						if (k == drillcount) {
							i = W;
							break;
						}
					}
				}
			}
			time = 0;
			break;
		default:
			break;
		}
	}
}

void Boss::Draw() {
	if (!flag) {
		ax = -40, ay = -100;
	}
	else {
		++time2;
		complex<double> ci(-40, -100);
		if (time2 < 30) {
			ci = polar(abs(ci), arg(ci) + DTOR(45.0));
			ax = real(ci), ay = imag(ci);
		}
		else {
			ci = polar(abs(ci), arg(ci) + DTOR(-45.0));
			ax = real(ci) + 20, ay = imag(ci);
		}
		if (time2 > 60) {
			flag = false;
		}
	}
	DrawGraph(ax, ay, arm, TRUE);
	DrawGraph(0, 0, body, TRUE);
	DrawFormatString(500, 40, blue, "%d", player.deathcount2);
	if (player.FaceDirection == Player::Direction::Direction_Left) {
		DrawTurnGraph(player.x, player.y, PlayerImageHandles[((player.AnimationFlame / 5) + 1) % 3], TRUE);
	}
	else {
		DrawGraph(player.x, player.y, PlayerImageHandles[((player.AnimationFlame / 5) + 1) % 3], TRUE);
	}
	for (int i = 0; i < drillcount; ++i) {
		DrawGraph(drill[i].x, drill[i].y, toge[drill[i].dir], TRUE);
	}
	particle.DrawParticles();
	for (int i = 0; i < W; ++i) {
		for (int j = 0; j < H; ++j) {
			if (tile[i][j].pattern <= 1) {
				tile[i][j].Draw();
			}
		}
	}
}

bool Boss::IsOver() {
	return gameover;
}

bool Boss::IsEnd() {
	return time >= 60 * 30 || hp <= 0;
}

Boss enemy;

STATE boss() {
	if (!bossflag) {
		enemy.Init();
		bossflag = true;
		PlaySoundMem(enemy.bgm, DX_PLAYTYPE_LOOP);
	}
	else {
		enemy.Update();
		enemy.Draw();
		if (enemy.IsOver()) {
			return GAMEOVER;
		}
		if (enemy.IsEnd()) {
			return RESULT;
		}
	}
	return BOSS;
}


bool resultflag = false;
int resultHandle;
int FontHandle2;

STATE result() {
	if (!resultflag) {
		resultHandle = LoadGraph("Graphic/リザルト画面.png");
		FontHandle = CreateFontToHandle(NULL, 40, 3, DX_FONTTYPE_ANTIALIASING);
		FontHandle2 = CreateFontToHandle(NULL, 30, 3, DX_FONTTYPE_ANTIALIASING);
		resultflag = true;
	}
	else {
		DrawGraph(0, 0, resultHandle, false);
		DrawFormatStringToHandle(50, 300, GetColor(0, 255, 0), FontHandle, "死亡回数 %3d回\n", player.deathcount2);
		DrawFormatStringToHandle(50, 350, GetColor(0, 255, 0), FontHandle, "クリア時間 %3.1f秒\n", (double)timer / 60);
		DrawStringToHandle(100, 400, "PRESS SPACE", GetColor(0, 0, 255), FontHandle2);
		player.deathcount1 = 0;
		player.deathcount2 = 0;
		if (getKeyPress(KEY_INPUT_SPACE, PRESS_ONCE)) {
			player.deathcount1 = 0;
			titleflag = false;
			gameflag = false;
			resultflag = false;
			return TITLE;
		}
	}
	return RESULT;
}


bool gameoverflag = false;
int gameoverHandle;

STATE gameover() {
	if (!gameoverflag) {
		gameoverHandle = LoadGraph("Graphic/gameover.png");
		gameoverflag = true;
		PlaySoundMem(Sound3, DX_PLAYTYPE_LOOP);
	}
	else {
		DrawGraph(0, 0, gameoverHandle, true);
		player.deathcount1 = 0;
		player.deathcount2 = 0;
		if (getKeyPress(KEY_INPUT_SPACE, PRESS_ONCE)) {
			titleflag = false;
			gameflag = false;
			bossflag = false;
			gameoverflag = false;
			StopSoundMem(Sound3);
			return TITLE;
		}
	}
	return GAMEOVER;
}