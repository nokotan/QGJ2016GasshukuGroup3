#include "DxLib.h"
#include "Collider.h"
#include "MapEditor.h"
#include <cmath>

struct Player {
	int x, y, width, height, dx, dy, fly,deathcount1,deathcount2;
	int FloorDeltaX;

	// 方向を表します。
	enum Direction {
		// 左向き
		Direction_Left,
		// 右向き
		Direction_Right
	} FaceDirection;

	bool OnCollideFromSide(int& tileid, int, int);
	bool OnCollideFromBottom(int& tileid, int, int);
	bool OnCollideFromTop(int& tileid, int, int);
};

Player player;
static int ballcount = 0;
static int bcount = 0;

bool Player::OnCollideFromSide(int& tileid, int, int) {
	x = 0;	

	if (tileid == 1 || tileid == 5) {
		return true;
	}

	// 返り値を false にすると、あたり判定を無視しない。
	return false;
}

bool Player::OnCollideFromBottom(int& tileid, int, int) {
	fly = 0;//0のとき飛べる

	if (tileid == 1 || tileid == 5) {
		return true;
	}

	if (tileid == 2) {
		deathcount2++;
	}

	return false;
}

bool Player::OnCollideFromTop(int& tileid, int i, int j) {

	if (tileid == 1) {
		// ブロックを実体化
		tileid = 0;
		int *tileobjptr = &tileid;
		*(tileobjptr + 1) = 2;
		*(tileobjptr - 1) = 2;
		*(tileobjptr - 15) = 2;
		*(tileobjptr + 15) = 2;
	} else if (tileid == 2) {
		// 死亡
		deathcount2++;
	} else if (tileid == 5) {
		return true;
	}

	return false;
}

struct Tile {
	int x, y, dx, dy, width, height;
	bool flag,flag2;
};
const int TILE_MAX = 10;
Tile ball[TILE_MAX];
Tile bridge[TILE_MAX];
Tile drill[TILE_MAX];

int stagenum = 1;

//初期化する関数
void Initialization(int map,MapViewer &mv) {
	player.x = 0;
	player.y = 200;
	player.width = 32;
	player.height = 64;
	player.dx = 0;
	player.dy = 0;

	player.fly = 0;
	ballcount = 0;
	mv.SetData(map);
}

void Stage(int& x,int& y,Tile* ti,MapViewer &mv) {
	static int jimen = LoadGraph("Graphic/Jimen.png");
	static int hasi = LoadGraph("Graphic/Hasi.png");
	static int yokotoge = LoadGraph("Graphic/Yokotoge.png");

}

//1は敵２は自機、あたり判定
bool Checkhitchery(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) {
	if (x1 <= (x2 + width2)) {
		if ((x1 + width1) >= x2) {
			if (y1 <= (y2 + height2)) {
				if ((y1 + height1) >= y2) {
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

void moveBridge(Tile *b) {
	for (int i = 0; i < bcount; ++i) {
		if ((player.x + player.width / 2) >= b[i].x && player.y - b[i].y < 5 ) {
			b[i].dy = 50;
			b[i].flag = false;
		}
		b[i].y += b[i].dy;
	}
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1) {
		return 0;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	// タイルマップとして使う２次元配列
	const int MapTilesWidth = 20;
	const int MapTilesHeight = 15;
	int MapTiles[MapTilesWidth][MapTilesHeight];
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

	//音楽のための変数と読み込み
	int Sound1, Sound2, Sound3;
	Sound1 = LoadSoundMem("合宿QGJ_タイトル.ogg");
	Sound2 = LoadSoundMem("合宿QGJ_メイン.ogg");
	Sound3 = LoadSoundMem("合宿QGJ_リザルト.ogg");

	// 背景の読み込み
	int BackImageHandle = LoadGraph("Graphic/背景.jpg");
	// プレイヤーの画像の読み込み
	int PlayerImageHandles[3];
	LoadDivGraph("Graphic/Character.png", 3, 3, 1, 32, 64, PlayerImageHandles);

	int jimen = LoadGraph("Graphic/Jimen.png");
	int hasi = LoadGraph("Graphic/Hasi.png");
	int yokotoge = LoadGraph("Graphic/Yokotoge.png");
	int ballHandle = LoadGraph("Graphic/ball.png");
	CMap MyMap { 30, 30 };
	MyMap.Fill(-1);

	for (int i : { 10, 11, 12, 13, 14, 15 }) {
		MyMap[i][12] = 0;
	}

	MyMap.X = -10 * 32;
	MyMap.Y = 0;
	MyMap.DeltaX = 2; MyMap.DeltaY = 0;

	// player を初期化
	MapViewer mv(1);
	Initialization(stagenum,mv);
	vector<vector<int>> tmp(MapTilesHeight, vector<int>(MapTilesWidth,-1));
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
			if (MapTiles[j][i] == 5) {
				ball[ballcount] = Tile{ j * 32, i * 32, 0, 00, 32, 32,false };
				++ballcount;
			}
			if (MapTiles[j][i] == 3) {
				bridge[bcount] = Tile{ j * 32, i * 32, 0, 00, 32, 32,true,true };
				++bcount;
			}
		}
	}



	for (int i = 0; i < TILE_MAX; ++i) {
		drill[i].height = 32;
		drill[i].width = 32;
	}
	drill[0].x = 32 * 3;
	drill[0].y = 32 * 12;
	drill[0].dx = 0;
	drill[0].dy = 0;
	drill[1].x = 32 * 3;
	drill[1].y = 32 * 13;
	drill[1].dx = 0;
	drill[1].dy = 0;

	// メインループ
	while (ProcessMessage() != -1 && ClearDrawScreen() != -1 && gpUpdateKey() == 0) {
		mv.Update();
		//音楽の再生
		if (CheckSoundMem(Sound2) == 0) {
			PlaySoundMem(Sound2, DX_PLAYTYPE_LOOP, TRUE);
		}

		// 入力に応じて、プレイヤーのスピードを変える
		if (CheckHitKey(KEY_INPUT_LEFT)) {
			player.FaceDirection = Player::Direction::Direction_Left;
			player.dx = -2 + player.FloorDeltaX;
		}
		else if (CheckHitKey(KEY_INPUT_RIGHT)) {
			player.FaceDirection = Player::Direction::Direction_Right;
			player.dx = 2 + player.FloorDeltaX;
		}
		else {
			player.dx = player.FloorDeltaX;
		}

		if (CheckHitKey(KEY_INPUT_SPACE) && player.fly == 0 && player.dy == 0) {
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
		
		//}

		if (MyMap.X < -10 * 32) {
			MyMap.DeltaX = 2;
		} else if (MyMap.X > -6 * 32) {
			MyMap.DeltaX = -2;
		}

		MyMap.X += MyMap.DeltaX;
		MyMap.Y += MyMap.DeltaY;

		// あたり判定を行う。
		player.FloorDeltaX = 0;
		int DefX = player.x, DefY = player.y;
		int DefDeltaX = player.dx, DefDeltaY = player.dy;
		CollisionCheck(player, MapTiles, 32, -1);	
		int NewX = player.x, NewY = player.y;

		player.x = DefX; player.y = DefY;
		CollisionCheck(player, MyMap, -1);
			
		if (DefDeltaX - MyMap.DeltaX > 0) {
			if (player.x > NewX) {
				player.x = NewX;
			}
		} else {
			if (player.x < NewX) {
				player.x = NewX;
			}
		}

		if (DefDeltaY - MyMap.DeltaY > 0) {
			if (player.y > NewY) {
				player.y = NewY;
			}
		} else {
			if (player.y < NewY) {
				player.y = NewY;
			}
		}

		
		for (int i = 0; i < TILE_MAX; ++i) {
			drill[i].x += drill[i].dx;
			drill[i].y += drill[i].dy;
		}
		
		//死んだらdeathcountを増やし仕掛けが元に戻る。playerは中間に飛ぶ(死亡処理)
		if (player.deathcount1 < player.deathcount2) {
			player.deathcount1 = player.deathcount2;
			Initialization(stagenum, mv);
			mv.SetTileKind(tmp);
			for (int i = 0; i < MapTilesHeight; ++i) {
				for (int j = 0; j < MapTilesWidth; ++j) {
					MapTiles[j][i] = tmp[i][j];
				}
			}
			ballcount = 0;
			bcount = 0;
			for (int i = 0; i < MapTilesHeight; ++i) {
				for (int j = 0; j < MapTilesWidth; ++j) {
					if (MapTiles[j][i] == 5) {
						ball[ballcount] = Tile{ j * 32, i * 32, 0, 00, 32, 32,false };
						++ballcount;
					}
					if (MapTiles[j][i] == 3) {
						bridge[bcount] = Tile{ j * 32, i * 32, 0, 00, 32, 32,true,true };
						++bcount;
				}
			}
		}
		
		// プレイヤー描画
		/*for (int i = 0; i < MapTilesWidth; i++) {
			for (int j = 0; j < MapTilesHeight; j++) {
				if (MapTiles[i][j] == 0) {
					DrawGraph(i * 32, j * 32, jimen, TRUE);
				}
				else if (MapTiles[i][j] == 4) {
					DrawGraph(i * 32, j * 32, hasi, TRUE);
				}
				}
			}*/
		

		// 背景の描画
		DrawGraph(0, 0, BackImageHandle, FALSE);


		//落ちてくる球
		for (int i = 0; i < ballcount; ++i) {
			if(ball[i].flag)
			DrawGraph(ball[i].x, ball[i].y, ballHandle, TRUE);
		}
		//落ちる橋
		for (int i = 0; i < bcount; ++i) {
			if (bridge[i].flag) {
				DrawGraph(bridge[i].x, bridge[i].y, hasi, TRUE);
			}
			else if(bridge[i].flag2){
				int X = bridge[i].x / 32, Y = bridge[i].y / 32;
				MapTiles[X][Y] = -1;
				bridge[i].flag2 = false;
			}
		}

		for (int i = 0; i < MyMap.Cols(); i++) {
			for (int j = 0; j < MyMap.Rows(); j++) {
				if (MyMap[i][j] != -1 && MyMap[i][j] != 1) {
					DrawBox(MyMap.X + i * 32, MyMap.Y + j * 32, MyMap.X + i * 32 + 32, MyMap.Y + j * 32 + 32, GetColor(0, 216, 0), TRUE);
				}
			}
		}

		if (player.FaceDirection == Player::Direction::Direction_Left) {
			DrawTurnGraph(player.x, player.y, PlayerImageHandles[0], TRUE);
		} else {
			DrawGraph(player.x, player.y, PlayerImageHandles[0], TRUE);
		}


		// 白色の値を取得
		unsigned Cr;
		Cr = GetColor(255, 255, 255);

		DrawFormatString(500, 0,Cr, "Death Count %d",player.deathcount1);
		DrawFormatString(500, 20,Cr, "Stage %d",stagenum);

		Stage(player.x, player.y, drill,mv);


		switch (stagenum) {
		case 1:
			for (int i : {0, 1, 2, 6, 7, 8, 11, 12, 15, 16, 17, 18, 19}) {
			if (jimen != -1) {
			//地面の描画
			DrawGraph(32 * i, 32 * 14, jimen, false);
			}
			}
			for (int i : {9, 10, 13, 14}) {
			if (hasi != -1) {
			//橋の描画
			DrawGraph(32 * i, 32 * 14, hasi, true);
			}
			}
			break;
		case 2:
			for (int i = 0; i < 20; ++i) {
				if (jimen != -1) {
					//地面の描画
					DrawGraph(32 * i, 32 * 14, jimen, true);
				}
			}
			//横に動いてくるとげ
			for (int i = 0; i < 2; ++i) {
				if (abs(player.x - drill[i].x) < 32 * 2 && (drill[i].y - player.y) < 32 * 2) {
					drill [i].dx = -10;
				}
				//横向きとげ描画
				DrawGraph(drill[i].x, drill [i].y, yokotoge, true);
			}
			break;
		}
		if (player.x >= 608 && stagenum < 2) {
			//マップ移動
			player.x = 0;
			++stagenum;
			Initialization(stagenum, mv);
		}
		mv.Draw();
		
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}