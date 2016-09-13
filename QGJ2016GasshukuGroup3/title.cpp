#include "DxLib.h"

int title() {

	int title = LoadGraph("タイトル画面.png");

	while (ProcessMessage() != -1 && ClearDrawScreen() != -1) {
		ClearDrawScreen();//画面を消す(これをしないと描画が大変なことに)

		// キーの入力待ち
		if (CheckHitKeyAll() == 0) {
			return 0;
		}
	
	//タイトル描画
	// 読みこんだグラフィックを画面左上に描画
		DrawGraph(0, 0, title, TRUE);

		ScreenFlip();//描画の反映
	}
	return -1;
} 