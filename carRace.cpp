#include "DxLib.h"
#include <stdlib.h>

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

// �e��t�����������\������֐�
void drawText(int x, int y, int col, const char* txt, int val, int siz)
{
	SetFontSize(siz);
	DrawFormatString(x + 2, y + 2, 0x000000, txt, val);
	DrawFormatString(x, y, col, txt, val);
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

	//�@�v���C���[�̎ԗp�̕ϐ�
	int playerX = WIDTH / 2;
	int playerY = HEIGHT / 2;
	int playerType = RED;

	// �R���s���[�^�[���������ԗp�̔z��
	const int COM_MAX = 8;
	int computerX[COM_MAX], computerY[COM_MAX], computerType[COM_MAX], computerFlag[COM_MAX];
	for (int i = 0; i < COM_MAX; i++) // �����l�̑��
	{
		computerX[i] = rand() % 180 + 270;
		computerY[i] = -100;
		computerType[i] = YELLOW + rand() % 3;
		computerFlag[i] = 0;
	}

	// �X�R�A�ƃn�C�X�R�A��������ϐ�
	int score = 0;
	int highScore = 5000;

	while (1) // ���C�����[�v
	{
		ClearDrawScreen(); // ��ʂ��N���A����

		// �w�i�̃X�N���[������
		bgY = bgY + 10;
		if (bgY >= HEIGHT) bgY = bgY - HEIGHT;
		DrawGraph(0, bgY - HEIGHT, imgBG, FALSE);
		DrawGraph(0, bgY, imgBG, FALSE);

		// �v���C���[�̎Ԃ𓮂�������
		GetMousePoint(&playerX, &playerY);
		if (playerX < 260) playerX = 260;
		if (playerX > 460) playerX = 460;
		if (playerY < 40) playerY = 40;
		if (playerY > 600) playerY = 600;
		drawCar(playerX, playerY, playerType);

		// �R���s���[�^�[�̎Ԃ𓮂�������
		for (int i = 0; i < COM_MAX; i++)
		{
			computerY[i] = computerY[i] + 1 + i;
			// ��ʂ̉�����O�ɏo�����𔻒�
			if (computerY[i] > HEIGHT + 100)
			{
				computerX[i] = rand() % 180 + 270;
				computerY[i] = -100;
				computerType[i] = YELLOW + rand() % 3;
				computerFlag[i] = 0;
			}
			// �q�b�g�`�F�b�N
			int dx = abs(computerX[i] - playerX); // X�������̃s�N�Z����
			int dy = abs(computerY[i] - playerY); // Y�������̃s�N�Z����
			int wid = CAR_W[playerType] / 2 + CAR_W[computerType[i]] / 2 - 4;
			int hei = CAR_H[playerType] / 2 + CAR_H[computerType[i]] / 2 - 4;
			if (dx < wid && dy < hei) // �ڐG���Ă��邩
			{
				int col = GetColor(rand() % 256, rand() % 256, rand() % 256); // �d�˂�F
				SetDrawBlendMode(DX_BLENDMODE_ADD, 255); // �F�����Z����ݒ�
				DrawBox(playerX - CAR_W[playerType] / 2, playerY - CAR_H[playerType] / 2, playerX + CAR_W[playerType] / 2, playerY + CAR_H[playerType] / 2, col, TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // �ʏ�̕`��ɖ߂�
			}

			// �ǂ����������𔻒�
			if (computerY[i] > playerY && computerFlag[i] == 0)
			{
				computerFlag[i] = 1;
				score += 100;
				if (score > highScore) highScore = score;
			}
			drawCar(computerX[i], computerY[i], computerType[i]);
		}

		// �X�R�A�Ȃǂ̕\��
		drawText(10, 10, 0x00ffff, "SCORE %d", score, 30);
		drawText(WIDTH - 200, 10, 0xffff00, "HI-SC %d", highScore, 30);
		drawText(10, HEIGHT - 40, 0x00ff00, "FUEL %d", 333, 30);
		
		ScreenFlip(); // ����ʂ̓��e��\��ʂɔ��f������
		WaitTimer(16); // ��莞�ԑ҂�
		if (ProcessMessage() == -1) break; // Windows ��������󂯎��G���[���N������I��
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESC �L�[�������ꂽ��I��
	}

	DxLib_End(); // DX���C�u�����g�p�̏I������
	return 0; // �\�t�g�̏I��
}