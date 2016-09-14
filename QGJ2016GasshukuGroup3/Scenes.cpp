#include "DxLib.h"
#include "Collider.h"
#include "MapEditor.h"
#include "Scenes.h"
#include "Lift.h"

#include <cmath>

int Sound1, Sound2, Sound3;


bool titleflag = false;
int titleHandle;
int FontHandle;
STATE title() {
	if (!titleflag) {
		titleHandle = LoadGraph("Graphic/�^�C�g�����.png");

		//���y�̂��߂̕ϐ��Ɠǂݍ���
		Sound1 = LoadSoundMem("���y/���hQGJ_�^�C�g��.ogg");
		Sound2 = LoadSoundMem("���y/���hQGJ_���C��.ogg");
		Sound3 = LoadSoundMem("���y/���hQGJ_���U���g.ogg");

		PlaySoundMem(Sound1, DX_PLAYTYPE_LOOP);

		// �쐬�����f�[�^�̎��ʔԍ���ϐ� FontHandle �ɕۑ�����
		FontHandle = CreateFontToHandle(NULL, 40, 3, DX_FONTTYPE_ANTIALIASING);
		titleflag = true;
	}
	else {
		// �L�[�̓��͑҂�
		if (getKeyPress(KEY_INPUT_SPACE,PRESS_ONCE)) {
			// �쐬�����t�H���g�f�[�^���폜����
			DeleteFontToHandle(FontHandle);
			StopSoundMem(Sound1);
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

	if (tileid == 1 || tileid == 5) {
		return true;
	}

	for (int id : { 2, 5, 6, 7, 8 }) {
		if (tileid == id) {
			// ���S
			deathcount2++;
		}
	}

	// �Ԃ�l�� false �ɂ���ƁA�����蔻��𖳎����Ȃ��B
	return false;
}

bool Player::OnCollideFromBottom(int& tileid, int, int) {
	fly = 0;//0�̂Ƃ���ׂ�

	if (tileid == 1 || tileid == 5) {
		return true;
	}

	for (int id : { 2, 5, 6, 7, 8 }) {
		if (tileid == id) {
			// ���S
			deathcount2++;
		}
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
	} else if (tileid == 5) {
		return true;
	}

	for (int id : { 2, 5, 6, 7, 8 }) {
		if (tileid == id) {
			// ���S
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

//1�͓G�Q�͎��@�A�����蔻��
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
	else if ((dir == 1 || dir == 3)&& p.y <= t.y + t.height) {
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
int timer;
int PlayerImageHandles[3];
CMap MyMap;
MapViewer mv;
vector<vector<int>> tmp(MapTilesHeight, vector<int>(MapTilesWidth, -1));
Particle particle;
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



		// �w�i�̓ǂݍ���
		BackImageHandle = LoadGraph("Graphic/�w�i.jpg");
		// �v���C���[�̉摜�̓ǂݍ���
		LoadDivGraph("Graphic/Character.png", 3, 3, 1, 32, 64, PlayerImageHandles);

		jimen = LoadGraph("Graphic/Jimen.png");
		hasi = LoadGraph("Graphic/Hasi.png");
		for (int i = 0; i < 4; ++i) {
			toge[i] = LoadGraph((string("Graphic/toge") + to_string(i)+ ".png").c_str());
		}
		ballHandle = LoadGraph("Graphic/ball.png");

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
		Lifts[3].X = 0;
		Lifts[3].Y = 32 * 6;
		LiftCount = 4;

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
				else if (MapTiles[j][i] >= drillsuf) {//�����g�Q
					drill[drillcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,true, MapTiles[j][i] - drillsuf };
					++drillcount;
				}
			}
		}
		STATE nextstate = TITLE;
		gameflag = true;
	}
	else{
		// ���C�����[�v
		mv.Update();
		particle.UpdateParticles();


		//���y�̍Đ�
		if (CheckSoundMem(Sound2) == 0) {
			PlaySoundMem(Sound2, DX_PLAYTYPE_LOOP, TRUE);
		}

		// ���͂ɉ����āA�v���C���[�̃X�s�[�h��ς���
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
		drillAttack(drill);
		invisManifestation(invis);

		//}

		for (int i = 0; i < LiftCount; i++) {
			Lifts[i].Update();
		}

		// �����蔻����s���B
		player.FloorDeltaX = 0;
		player.CollidedDirection = Direction::None;

		int DefX = player.x, DefY = player.y;
		int DefDeltaX = player.dx, DefDeltaY = player.dy;
		CollisionCheck(player, MapTiles, 32, -1);
		int NewX = player.x, NewY = player.y;

		for (int i = 0; i < LiftCount; i++) {
			player.x = DefX; player.y = DefY;
			CollisionCheck(player, Lifts[i].GetCollider(), -1);

			if (DefDeltaX - Lifts[i].GetCollider().DeltaX > 0) {
				if (player.x <= NewX) {
					NewX = player.x;
				}
			}
			else {
				if (player.x >= NewX) {
					NewX = player.x;
				}
			}

			if (DefDeltaY - Lifts[i].GetCollider().DeltaY > 0) {
				if (player.y <= NewY) {
					NewY = player.y;
				}
			}
			else {
				if (player.y >= NewY) {
					NewY = player.y;
				}
			}
		}

		player.x = NewX;
		player.y = NewY;

		// ���܂蔻��
		if ((player.CollidedDirection & Direction::LeftAndRight) == Direction::LeftAndRight || (player.CollidedDirection & Direction::UpAndDown) == Direction::UpAndDown) {
			player.deathcount2++;
		}

		clsDx();
		printfDx("%d, dx = %d, dy = %d", player.CollidedDirection, player.dx, player.dy);

		//���񂾂�deathcount�𑝂₵�d�|�������ɖ߂�Bplayer�͒��Ԃɔ��(���S����)
		if (player.deathcount1 < player.deathcount2) {
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
					else if (MapTiles[j][i] >= drillsuf) {//�����g�Q
						drill[drillcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,true, MapTiles[j][i] - drillsuf };
						++drillcount;
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
		for (int i = 0; i < drillcount; ++i) {
			DrawGraph(drill[i].x, drill[i].y, toge[drill[i].dir], TRUE);
			DrawFormatString(drill[i].x, drill[i].y, blue, "%d", drill[i].dir);
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


		// ���F�̒l���擾
		unsigned Cr;
		Cr = GetColor(255, 255, 255);

		DrawFormatString(500, 0, Cr, "Death Count %d", player.deathcount1);
		DrawFormatString(500, 20, Cr, "Stage %d", stagenum);
		DrawFormatString(500, 40, Cr, "time %dmin %dsec", (180 - timer/60)/60,60 - (timer/60)%60);

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
					else if (MapTiles[j][i] >= drillsuf) {//�����g�Q
						drill[drillcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,true, MapTiles[j][i] - drillsuf };
						++drillcount;
					}
				}
			}
		}
		++timer;
		//�O���o������Q�[���I�[�o�[
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
		DrawFormatStringToHandle(50, 350, GetColor(0, 255, 0), FontHandle, "�N���A���� %3.1f�b\n", (double)timer / 60);
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