#include "DxLib.h"

int title() {

	int title = LoadGraph("�^�C�g�����.png");

	while (ProcessMessage() != -1 && ClearDrawScreen() != -1) {
		ClearDrawScreen();//��ʂ�����(��������Ȃ��ƕ`�悪��ςȂ��Ƃ�)

		// �L�[�̓��͑҂�
		if (CheckHitKeyAll() == 0) {
			return 0;
		}
	
	//�^�C�g���`��
	// �ǂ݂��񂾃O���t�B�b�N����ʍ���ɕ`��
		DrawGraph(0, 0, title, TRUE);

		ScreenFlip();//�`��̔��f
	}
	return -1;
} 