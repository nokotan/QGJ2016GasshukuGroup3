#include "DxLib.h"

int title() {

	int title = LoadGraph("�^�C�g�����.png");
	int FontHandle;
	// �쐬�����f�[�^�̎��ʔԍ���ϐ� FontHandle �ɕۑ�����
	FontHandle = CreateFontToHandle(NULL, 40, 3, DX_FONTTYPE_ANTIALIASING);


	while (ProcessMessage() != -1 && ClearDrawScreen() != -1) {
		ClearDrawScreen();//��ʂ�����(��������Ȃ��ƕ`�悪��ςȂ��Ƃ�)

		// �L�[�̓��͑҂�
		if (CheckHitKeyAll() == 0) {
			// �쐬�����t�H���g�f�[�^���폜����
			DeleteFontToHandle(FontHandle);
			return 0;
		}
	
	//�^�C�g���`��
		DrawStringToHandle(100, 200, "PRESS START !!", GetColor(255, 255, 255), FontHandle);

	// �ǂ݂��񂾃O���t�B�b�N����ʍ���ɕ`��
		DrawGraph(0, 0, title, TRUE);

		ScreenFlip();//�`��̔��f
	}
	return -1;
} 