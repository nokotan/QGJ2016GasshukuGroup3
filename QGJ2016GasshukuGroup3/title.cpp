#include "DxLib.h"

int title() {

	int title = LoadGraph("タイトル画面.png");
	int FontHandle;
	// 作成したデータの識別番号を変数 FontHandle に保存する
	FontHandle = CreateFontToHandle(NULL, 40, 3, DX_FONTTYPE_ANTIALIASING);


	while (ProcessMessage() != -1 && ClearDrawScreen() != -1) {
		ClearDrawScreen();//画面を消す(これをしないと描画が大変なことに)

		// キーの入力待ち
		if (CheckHitKeyAll() == 0) {
			// 作成したフォントデータを削除する
			DeleteFontToHandle(FontHandle);
			return 0;
		}
	
	//タイトル描画
		DrawStringToHandle(100, 200, "PRESS START !!", GetColor(255, 255, 255), FontHandle);

	// 読みこんだグラフィックを画面左上に描画
		DrawGraph(0, 0, title, TRUE);

		ScreenFlip();//描画の反映
	}
	return -1;
} 