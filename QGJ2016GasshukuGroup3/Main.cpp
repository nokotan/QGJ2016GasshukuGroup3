#include "DxLib.h"
#include "Collider.h"
#include "MapEditor.h"
#include <cmath>

struct Player {
	int x, y, width, height, dx, dy, fly,deathcount1,deathcount2;
	int FloorDeltaX;
	bool OnCollideFromSide(int& tileid, int, int);
	bool OnCollideFromBottom(int& tileid, int, int);
	bool OnCollideFromTop(int& tileid, int, int);
};

Player player;
static int ballcount = 0;

bool Player::OnCollideFromSide(int& tileid, int, int) {
	x = 0;	

	if (tileid == 1) {
		return true;
	}

	// �Ԃ�l�� false �ɂ���ƁA�����蔻��𖳎����Ȃ��B
	return false;
}

bool Player::OnCollideFromBottom(int& tileid, int, int) {
	fly = 0;//0�̂Ƃ���ׂ�

	if (tileid == 1) {
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
	return false;
}

struct Tile {
	int x, y, dx, dy, width, height;
	bool flag;
};
const int TILE_MAX = 10;
Tile ball[TILE_MAX];
Tile drill[TILE_MAX];

int stagenum = 1;

//����������֐�
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
	int jimen = LoadGraph("Graphic/Jimen.png");
	int hasi = LoadGraph("Graphic/Hasi.png");
	int yokotoge = LoadGraph("Graphic/Yokotoge.png");

	switch(stagenum){
	case 1:
		/*for (int i : {0, 1, 2, 6, 7, 8, 11, 12, 15, 16, 17, 18, 19}) {
			if (jimen != -1) {
				//�n�ʂ̕`��
				DrawGraph(32 * i, 32 * 14, jimen, false);
		}
		}
		for (int i : {9, 10, 13, 14}) {
			if (hasi != -1) {
				//���̕`��
				DrawGraph(32 * i, 32 * 14, hasi, true);
		}
		}*/
		break;
	case 2:
		for (int i = 0; i < 20; ++i) {
			if (jimen != -1) {
				//�n�ʂ̕`��
				DrawGraph(32 * i, 32 * 14, jimen, true);
		}
		}
		 //���ɓ����Ă���Ƃ�
		for (int i = 0; i < 2; ++i) {
			if (abs(player.x - ti[i].x) < 32 * 2 && (ti[i].y - player.y) < 32 * 2) {
				ti[i].dx = -10;
			}
			//�������Ƃ��`��
			DrawGraph(ti[i].x, ti[i].y, yokotoge, true);
		}
		break;
	}
	if (x >= 608 && stagenum < 2) {
		//�}�b�v�ړ�
		x = 0;
		++stagenum;
		Initialization(stagenum,mv);
	}
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


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1) {
		return 0;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	// �^�C���}�b�v�Ƃ��Ďg���Q�����z��
	const int MapTilesWidth = 20;
	const int MapTilesHeight = 15;
	int MapTiles[MapTilesWidth][MapTilesHeight];
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

	//Halksel

	//���y�̂��߂̕ϐ��Ɠǂݍ���
	int Sound1, Sound2, Sound3;
	Sound1 = LoadSoundMem("���hQGJ_�^�C�g��.ogg");
	Sound2 = LoadSoundMem("���hQGJ_���C��.ogg");
	Sound3 = LoadSoundMem("���hQGJ_���U���g");
	int jimen = LoadGraph("Graphic/Jimen.png");
	int hasi = LoadGraph("Graphic/Hasi.png");
	int yokotoge = LoadGraph("Graphic/Yokotoge.png");
	CMap MyMap { 30, 30 };
	MyMap.Fill(-1);

	for (int i : { 10, 11, 12, 13, 14, 15 }) {
		MyMap[i][12] = 0;
	}

	MyMap.X = -10 * 32;
	MyMap.Y = 0;
	MyMap.DeltaX = 2; MyMap.DeltaY = 0;

	// player ��������
	MapViewer mv(1);
	Initialization(stagenum,mv);
	vector<vector<int>> tmp(MapTilesHeight, vector<int>(MapTilesWidth,-1));
	mv.SetTileKind(tmp);
	for (int i = 0; i < MapTilesHeight; ++i) {
		for (int j = 0; j < MapTilesWidth; ++j) {
			MapTiles[j][i] = tmp[i][j];
		}
	}
	//Ball�̃Z�b�g
	for (int i = 0; i < MapTilesHeight; ++i) {
		for (int j = 0; j < MapTilesWidth; ++j) {
			if (MapTiles[j][i] == 5) {
				ball[ballcount] = Tile{ j * 32, i * 32, 0, 00, 32, 32,false };
				++ballcount;
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

	// ���C�����[�v
	while (ProcessMessage() != -1 && ClearDrawScreen() != -1 && gpUpdateKey() == 0) {
		mv.Update();
		//���y�̍Đ�
		if (CheckSoundMem(Sound2) == 0) {
			PlaySoundMem(Sound2, DX_PLAYTYPE_LOOP, TRUE);
		}

		// ���͂ɉ����āA�v���C���[�̃X�s�[�h��ς���
		if (CheckHitKey(KEY_INPUT_LEFT)) {
			player.dx = -2 + player.FloorDeltaX;
		}
		else if (CheckHitKey(KEY_INPUT_RIGHT)) {
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

		// mv.Draw();

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
		
		//}

		if (MyMap.X < -10 * 32) {
			MyMap.DeltaX = 2;
		} else if (MyMap.X > -6 * 32) {
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
		
		//���񂾂�deathcount�𑝂₵�d�|�������ɖ߂�Bplayer�͒��Ԃɔ��(���S����)
		if (player.deathcount1 < player.deathcount2) {
			player.deathcount1 = player.deathcount2;
			Initialization(stagenum,mv);
			mv.SetTileKind(tmp);
			for (int i = 0; i < MapTilesHeight; ++i) {
				for (int j = 0; j < MapTilesWidth; ++j) {
					MapTiles[j][i] = tmp[i][j];
				}
			}
			ballcount = 0;
			for (int i = 0; i < MapTilesHeight; ++i) {
				for (int j = 0; j < MapTilesWidth; ++j) {
					if (MapTiles[j][i] == 5) {
						ball[ballcount] = Tile{ j * 32, i * 32, 0, 00, 32, 32,false };
						++ballcount;
					}
				}
			}
		}
		
		// �v���C���[�`��
		for (int i = 0; i < MapTilesWidth; i++) {
			for (int j = 0; j < MapTilesHeight; j++) {
				if (MapTiles[i][j] == 0) {
					DrawGraph(i * 32, j * 32, jimen, TRUE);
				}
				else if (MapTiles[i][j] == 4) {
					DrawGraph(i * 32, j * 32, hasi, TRUE);
				}
			}
		}
		for (int i = 0; i < ballcount; ++i) {
			if(ball[i].flag)
			DrawBox(ball[i].x, ball[i].y, ball[i].x + ball[i].width, ball[i].y + ball[i].height, GetColor(0, 0, 222), TRUE);
		}

		for (int i = 0; i < MyMap.Cols(); i++) {
			for (int j = 0; j < MyMap.Rows(); j++) {
				if (MyMap[i][j] != -1 && MyMap[i][j] != 1) {
					DrawBox(MyMap.X + i * 32, MyMap.Y + j * 32, MyMap.X + i * 32 + 32, MyMap.Y + j * 32 + 32, GetColor(0, 216, 0), TRUE);
				}
			}
		}

		DrawBox(player.x, player.y, player.x + player.width, player.y + player.height, GetColor(255, 255, 255), TRUE);

		// ���F�̒l���擾
		unsigned Cr;
		Cr = GetColor(255, 255, 255);

		DrawFormatString(500, 0,Cr, "Death Count %d",player.deathcount1);

		Stage(player.x, player.y, drill,mv);
		mv.Draw();
		
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}