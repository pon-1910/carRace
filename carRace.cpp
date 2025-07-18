#include "DxLib.h"

// �Ԃ̉摜���Ǘ�����萔�Ɣz��
enum { RED, YELLOW, BLUE, TRUCK};
const int CAR_MAX = 4;
int imagCar[CAR_MAX];
const int CAR_W[CAR_MAX] = { 32, 26, 26, 40 };
const int CAR_H[CAR_MAX] = { 48, 48, 48, 100 };

// �Ԃ�\������֐�
void drawCar(int x, int y, int type) 
{
	DrawGraph(x - CAR_W[type] / 2, y - CAR_H[type] / 2, imagCar[type], TRUE);
}

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

	// �Ԃ̉摜��z��ɓǂݍ���
	imagCar[RED] = LoadGraph("image/car_red.png");
	imagCar[YELLOW] = LoadGraph("image/car_yellow.png");
	imagCar[BLUE] = LoadGraph("image/car_blue.png");
	imagCar[TRUCK] = LoadGraph("image/truck.png");

	while (1) // ���C�����[�v
	{
		ClearDrawScreen(); // ��ʂ��N���A����

		// �w�i�̃X�N���[������
		bgY = bgY + 10;
		if (bgY >= HEIGHT) bgY = bgY - HEIGHT;
		DrawGraph(0, bgY - HEIGHT, imgBG, FALSE);
		DrawGraph(0, bgY, imgBG, FALSE);

		// �ԗ��̕\���@������ߒ�
		drawCar(300, 360, RED); // ��
		drawCar(340, 360, YELLOW); // ���F
		drawCar(380, 360, BLUE); // ��
		drawCar(420, 360, TRUCK); // �g���b�N

		ScreenFlip(); // ����ʂ̓��e��\��ʂɔ��f������
		WaitTimer(16); // ��莞�ԑ҂�
		if (ProcessMessage() == -1) break; // Windows ��������󂯎��G���[���N������I��
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESC �L�[�������ꂽ��I��
	}

	DxLib_End(); // DX���C�u�����g�p�̏I������
	return 0; // �\�t�g�̏I��
}