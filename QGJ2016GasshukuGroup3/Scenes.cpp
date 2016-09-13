#include "DxLib.h"
#include "Collider.h"
#include "MapEditor.h"
#include "Scenes.h"
#include <cmath>

bool titleflag = false;
int titleHandle;
int FontHandle;
STATE title() {
	if (!titleflag) {
		titleHandle = LoadGraph("Graphic/�^�C�g�����.png");

		// �쐬�����f�[�^�̎��ʔԍ���ϐ� FontHandle �ɕۑ�����
		FontHandle = CreateFontToHandle(NULL, 40, 3, DX_FONTTYPE_ANTIALIASING);
		titleflag = true;
	}
	else {
		// �L�[�̓��͑҂�
		if (getKeyPress(KEY_INPUT_SPACE,PRESS_ONCE)) {
			// �쐬�����t�H���g�f�[�^���폜����
			DeleteFontToHandle(FontHandle);
			return GAME;
		}

		//�^�C�g���`��
		

		// �ǂ݂��񂾃O���t�B�b�N����ʍ���ɕ`��
		DrawGraph(0, 0, titleHandle, TRUE);
		DrawStringToHandle(200, 400, "PRESS SPACE !!", GetColor(0, 255, 255), FontHandle);
		//ScreenFlip();//�`��̔��f
	}

	return TITLE;
}

struct Player {
	int x, y, width, height, dx, dy, fly, deathcount1, deathcount2;
	int FloorDeltaX;

	// ������\���܂��B
	enum Direction {
		// ������
		Direction_Left = 1,
		// �E����
		Direction_Right = 0
	} FaceDirection;

	bool OnCollideFromSide(int& tileid, int, int);
	bool OnCollideFromBottom(int& tileid, int, int);
	bool OnCollideFromTop(int& tileid, int, int);
};

Player player;
static int ballcount = 0;
static int bcount = 0;
static int drillcount = 0;

bool Player::OnCollideFromSide(int& tileid, int, int) {
	x = 0;

	if (tileid == 1 || tileid == 5) {
		return true;
	}

	// �Ԃ�l�� false �ɂ���ƁA�����蔻��𖳎����Ȃ��B
	return false;
}

bool Player::OnCollideFromBottom(int& tileid, int, int) {
	fly = 0;//0�̂Ƃ���ׂ�

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
		// �u���b�N�����̉�
		tileid = 0;
		int *tileobjptr = &tileid;
		*(tileobjptr + 1) = 2;
		*(tileobjptr - 1) = 2;
		*(tileobjptr - 15) = 2;
		*(tileobjptr + 15) = 2;
	}
	else if (tileid == 2) {
		// ���S
		deathcount2++;
	}
	else if (tileid == 5) {
		return true;
	}

	return false;
}

struct Tile {
	int x, y, dx, dy, width, height;
	bool flag, flag2;
};
const int TILE_MAX = 10;
Tile ball[TILE_MAX];
Tile bridge[TILE_MAX];
Tile drill[TILE_MAX];

int stagenum = 1;

//����������֐�
void Initialization(int map, MapViewer &mv) {
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


//1�͓G�Q�͎��@�A�����蔻��
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

void drillAttack(Tile* drill) {
	for (int i = 0; i < drillcount; ++i) {
		if (Checkhitchery(drill[i].x, drill[i].y, drill[i].width, drill[i].height, player.x, player.y, player.width, player.height)) {
			player.deathcount2++;
		}
	}
}

void moveBridge(Tile *b) {
	for (int i = 0; i < bcount; ++i) {
		if ((player.x + player.width / 2) >= b[i].x && abs(player.y - b[i].y) < 100 && b[i].flag) {
			b[i].dy = 50;
			b[i].flag = false;
		}
	}
}
bool gameflag = false;
int Sound1, Sound2, Sound3;
int BackImageHandle, jimen, yokotoge, hasi, ballHandle;
int timer;
int PlayerImageHandles[3];
CMap MyMap;
MapViewer mv;
const int MapTilesWidth = 20;
const int MapTilesHeight = 15;
int MapTiles[MapTilesWidth][MapTilesHeight];
vector<vector<int>> tmp(MapTilesHeight, vector<int>(MapTilesWidth, -1));

STATE game() {
	if (!gameflag) {
		// �^�C���}�b�v�Ƃ��Ďg���Q�����z��

		// �^�C���}�b�v�� -1 (�����Ȃ�) �Ŗ��߂�
		for (int i = 0; i < MapTilesWidth; i++) {
			for (int j = 0; j < MapTilesHeight; j++) {
				MapTiles[i][j] = -1;

				// ��ԉ��͏��Ƃ��� 0 �����Ă���
				if (j == MapTilesHeight - 1) { //&& (i < 5 || i > 7)) {
					MapTiles[i][j] = 0;
				}
			}
		}

		//�^�C�}�[
		timer = 0;

		//���y�̂��߂̕ϐ��Ɠǂݍ���
		Sound1 = LoadSoundMem("���y/���hQGJ_�^�C�g��.ogg");
		Sound2 = LoadSoundMem("���y/���hQGJ_���C��.ogg");
		Sound3 = LoadSoundMem("���y/���hQGJ_���U���g.ogg");

		// �w�i�̓ǂݍ���
		BackImageHandle = LoadGraph("Graphic/�w�i.jpg");
		// �v���C���[�̉摜�̓ǂݍ���
		LoadDivGraph("Graphic/Character.png", 3, 3, 1, 32, 64, PlayerImageHandles);

		jimen = LoadGraph("Graphic/Jimen.png");
		hasi = LoadGraph("Graphic/Hasi.png");
		yokotoge = LoadGraph("Graphic/Yokotoge.png");
		ballHandle = LoadGraph("Graphic/ball.png");
		MyMap.Create(30, 30);
		MyMap.Fill(-1);

		for (int i : { 10, 11, 12, 13, 14, 15 }) {
			MyMap[i][12] = 0;
		}

		MyMap.X = -10 * 32;
		MyMap.Y = 0;
		MyMap.DeltaX = 2; MyMap.DeltaY = 0;

		// player ��������
		mv = MapViewer(1);
		Initialization(stagenum, mv);

		mv.SetTileKind(tmp);
		for (int i = 0; i < MapTilesHeight; ++i) {
			for (int j = 0; j < MapTilesWidth; ++j) {
				MapTiles[j][i] = tmp[i][j];
			}
		}
		//Ball�̃Z�b�g
		//Bridge�̃Z�b�g�i��������j
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


		drillcount = 2;
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
		STATE nextstate = TITLE;
		gameflag = true;
	}
	else{
		// ���C�����[�v
		mv.Update();


		//���y�̍Đ�
		if (CheckSoundMem(Sound2) == 0) {
			PlaySoundMem(Sound2, DX_PLAYTYPE_LOOP, TRUE);
		}

		// ���͂ɉ����āA�v���C���[�̃X�s�[�h��ς���
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

		// �d�͉����x��������B
		player.dy += 1;
		if (player.dy > 10) {
			player.dy = 10;
		}

		//��������������
		if (player.y > 480) {
			player.x = 0;
			player.y = 382;
			player.fly = 0;
			player.deathcount2++;
		}
		//Tile�𓮂���
		//if () {//map�ɂ���ĕύX������

		moveBall(ball);
		moveBridge(bridge);

		//}

		if (MyMap.X < -10 * 32) {
			MyMap.DeltaX = 2;
		}
		else if (MyMap.X > -6 * 32) {
			MyMap.DeltaX = -2;
		}

		MyMap.X += MyMap.DeltaX;
		MyMap.Y += MyMap.DeltaY;

		// �����蔻����s���B
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
		}
		else {
			if (player.x < NewX) {
				player.x = NewX;
			}
		}

		if (DefDeltaY - MyMap.DeltaY > 0) {
			if (player.y > NewY) {
				player.y = NewY;
			}
		}
		else {
			if (player.y < NewY) {
				player.y = NewY;
			}
		}


		for (int i = 0; i < TILE_MAX; ++i) {
			drill[i].x += drill[i].dx;
			drill[i].y += drill[i].dy;
		}

		//���񂾂�deathcount�𑝂₵�d�|�������ɖ߂�Bplayer�͒��Ԃɔ��(���S����)
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
		}


		// �w�i�̕`��
		DrawGraph(0, 0, BackImageHandle, FALSE);
		for (int i = 0; i < MapTilesHeight; ++i) {
			for (int j = 0; j < MapTilesWidth; ++j) {
				if (MapTiles[j][i] == 0) {
					DrawGraph(j * 32, i * 32, jimen, TRUE);
				}
			}
		}
		//�����Ă��鋅
		for (int i = 0; i < ballcount; ++i) {
			if (ball[i].flag)
				DrawGraph(ball[i].x, ball[i].y, ballHandle, TRUE);
		}

		//�����鋴
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

		for (int i = 0; i < MyMap.Cols(); i++) {
			for (int j = 0; j < MyMap.Rows(); j++) {
				if (MyMap[i][j] != -1 && MyMap[i][j] != 1) {
					DrawGraph(MyMap.X + i * 32, MyMap.Y + j * 32, jimen, TRUE);
					// DrawBox(MyMap.X + i * 32, MyMap.Y + j * 32, MyMap.X + i * 32 + 32, MyMap.Y + j * 32 + 32, GetColor(0, 216, 0), TRUE);
				}
			}
		}

		if (player.FaceDirection == Player::Direction::Direction_Left) {
			DrawTurnGraph(player.x, player.y, PlayerImageHandles[0], TRUE);
		}
		else {
			DrawGraph(player.x, player.y, PlayerImageHandles[0], TRUE);
		}


		// ���F�̒l���擾
		unsigned Cr;
		Cr = GetColor(255, 255, 255);

		DrawFormatString(500, 0, Cr, "Death Count %d", player.deathcount1);
		DrawFormatString(500, 20, Cr, "Stage %d", stagenum);



		switch (stagenum) {
		case 2:
			//���ɓ����Ă���Ƃ�
			for (int i = 0; i < 2; ++i) {
				if (abs(player.x - drill[i].x) < 32 * 2 && (drill[i].y - player.y) < 32 * 2) {
					drill[i].dx = -10;
				}
				//�������Ƃ��`��
				DrawGraph(drill[i].x, drill[i].y, yokotoge, true);
			}
			break;
		}
		if (player.x >= 608 && stagenum < 3) {
			//�}�b�v�ړ�
			player.x = 0;
			++stagenum;
			Initialization(stagenum, mv);
			mv.SetTileKind(tmp);
			for (int i = 0; i < MapTilesHeight; ++i) {
				for (int j = 0; j < MapTilesWidth; ++j) {
					MapTiles[j][i] = tmp[i][j];
				}
			}
			ballcount = 0, bcount = 0;
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
		}
		++timer;
		//�O���o������Q�[���I�[�o�[
		if (timer == 180 * 60) {
			titleflag = false;
			gameflag = false;
			if (CheckSoundMem(Sound2) == 1) {
				StopSoundMem(Sound2);
			}
			return GAMEOVER;
		}
		mv.Draw();
	}
	return GAME;
}


bool resultflag = false;
int resultHandle;
int FontHandle2;

STATE result() {
	if (!resultflag) {
		resultHandle = LoadGraph("Graphic/���U���g���.png");
		FontHandle = CreateFontToHandle(NULL, 40, 3, DX_FONTTYPE_ANTIALIASING);
		FontHandle2 = CreateFontToHandle(NULL, 30, 3, DX_FONTTYPE_ANTIALIASING);
		resultflag = true;
	}
	else {
		DrawGraph(0, 0, resultHandle, false);
		//DrawFormatString(50, 240, GetColor(0, 0, 0), "���S�� %d\n", player.deathcount2);
		DrawFormatStringToHandle(50, 300, GetColor(0, 255, 0), FontHandle, "���S�� %3d��\n", player.deathcount2);
		DrawFormatStringToHandle(50, 350, GetColor(0, 255, 0), FontHandle, "�N���A���� %3.0f�b\n", (double)timer / 60);
		DrawStringToHandle(100, 400, "PRESS SPACE", GetColor(0, 0, 255), FontHandle2);
		if (getKeyPress(KEY_INPUT_SPACE, PRESS_ONCE)) {
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
	}
	else {
		DrawGraph(0, 0, gameoverHandle, true);
		if (getKeyPress(KEY_INPUT_SPACE, PRESS_ONCE)) {
			titleflag = false;
			gameflag = false;
			gameoverflag = false;
			return TITLE;
		}
	}
	return GAMEOVER;
}