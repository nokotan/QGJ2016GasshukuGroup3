#include "DxLib.h"
#include "Collider.h"
#include "MapEditor.h"
#include "Scenes.h"
#include <cmath>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
	SetMainWindowText("�_�̖���");
	// �^�C���}�b�v�Ƃ��Ďg���Q�����z��;
	DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);
	SetBackgroundColor(255, 255, 255);
	STATE nextstate = TITLE;
	// ���C�����[�v
	while (true) {
		if (ProcessMessage() == -1 || ClearDrawScreen() == -1 || gpUpdateKey() != 0) {
			return -1;
		}
		switch (nextstate)
		{
		case EXIT:
			break;
		case TITLE:
			nextstate = title();
			break;
		case GAME:
			nextstate = game();
			break;
		case RESULT:
			nextstate = result();
			break;
		default:
			break;
		}
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}