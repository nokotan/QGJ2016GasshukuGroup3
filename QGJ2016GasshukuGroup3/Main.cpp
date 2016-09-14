#include "DxLib.h"
#include "Collider.h"
#include "MapEditor.h"
#include "Scenes.h"
#include <cmath>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
	ChangeWindowMode(TRUE);
	SetMainWindowText("神の名は。 -God name.-");
	DxLib_Init();
	if (CheckHitKey(KEY_INPUT_F2)) {
		ChangeWindowMode(FALSE);
	}
	SetDrawScreen(DX_SCREEN_BACK);
	STATE nextstate = TITLE;
	// メインループ
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
		case BOSS:
			nextstate = boss();
			break;
		case RESULT:
			nextstate = result();
			break;
		case GAMEOVER:
			nextstate = gameover();
			break;
		case SETSUMEI:
			nextstate = setsumei();
			break;
		default:
			break;
		}
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}