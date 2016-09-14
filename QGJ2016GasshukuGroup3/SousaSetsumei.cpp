#include "Scenes.h"
#include "Input.h"

static int SetsumeiImage[2] = { -1, -1 };
static bool SetsumeiInitialized = false;
static int FontHandle = -1;
static int Phase = 0;
static int Flames = 0;

STATE setsumei() {
	if (!SetsumeiInitialized) {
		Phase = 0;
		Flames = 0;

		if (SetsumeiImage[0] == -1) {
			SetsumeiImage[0] = LoadGraph("Graphic/ストーリー.png");
			SetsumeiImage[1] = LoadGraph("Graphic/操作説明.png");

			FontHandle = CreateFontToHandle("ＭＳ 明朝", 32, 3, DX_FONTTYPE_ANTIALIASING);
		}

		SetsumeiInitialized = true;
	}
	
	Flames++;

	if (Phase == 0) {
		if (getKeyPress(KEY_INPUT_SPACE, PRESS_ONCE)) {
			Phase = 1;
		}

		DrawGraph(0, 0, SetsumeiImage[0], FALSE);
	} else {
		if (getKeyPress(KEY_INPUT_SPACE, PRESS_ONCE)) {
			SetsumeiInitialized = false;
			return GAME;
		}

		DrawGraph(0, 0, SetsumeiImage[1], FALSE);
	}

	int Brightness = Flames % 256 <= 128 ? Flames % 256 : 128 - (Flames - 128) % 256;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, Brightness);
	DrawStringToHandle(300 - 3, 425 - 3, "[Space] で次に進む...", GetColor(0, 0, 156), FontHandle);
	DrawStringToHandle(300, 425, "[Space] で次に進む...", GetColor(96, 96, 255), FontHandle);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	return SETSUMEI;
}