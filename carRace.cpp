#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �萔
	const int WIDTH = 720, HEIGHT = 640; // �E�B���h�E�̕��ƍ����̃s�N�Z����

	SetWindowText("�J�[���[�X"); // �E�B���h�E�̃^�C�g��
	SetGraphMode(WIDTH, HEIGHT, 32); // �E�B���h�E�̑傫���ƃJ���[�r�b�g���̎w��
	ChangeWindowMode(TRUE); // �E�B���h�E���[�h�ŋN��
	if (DxLib_Init() == -1) return -1; // ���C�u�����̏������@�G���[���N������I��
	SetBackgroundColor(0, 0, 0); // �w�i�F�̎w��
	SetDrawScreen(DX_SCREEN_BACK); // �`��ʂ𗠉�ʂɂ���

	int bgY = 0; // ���H���X�N���[�������邽�߂̕ϐ�
	int imgBG = LoadGraph("image/bg.png"); // �w�i�̉摜

	while (1) // ���C�����[�v
	{
		ClearDrawScreen(); // ��ʂ��N���A����

		// �w�i�̃X�N���[������
		bgY = bgY + 10;
		if (bgY >= HEIGHT) bgY = bgY - HEIGHT;
		DrawGraph(0, bgY - HEIGHT, imgBG, FALSE);
		DrawGraph(0, bgY, imgBG, FALSE);

		ScreenFlip(); // ����ʂ̓��e��\��ʂɔ��f������
		WaitTimer(16); // ��莞�ԑ҂�
		if (ProcessMessage() == -1) break; // Windows ��������󂯎��G���[���N������I��
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESC �L�[�������ꂽ��I��
	}

	DxLib_End(); // DX���C�u�����g�p�̏I������
	return 0; // �\�t�g�̏I��
}