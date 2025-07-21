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

	// �R���A�C�e���p�̕ϐ�
	int fuel = 0;
	int fuelX = WIDTH / 2;
	int fuelY = 0;
	int imgFuel = LoadGraph("image/fuel.png");

	// �Q�[���i�s�Ɋւ���ϐ�
	enum { TITLE, PLAY, OVER };
	int scene = TITLE;
	int timer = 0;

	// �T�E���h�̓ǂݍ��݂Ɖ��ʐݒ�
	int bgm = LoadSoundMem("sound/bgm.mp3");
	int jin = LoadSoundMem("sound/gameover.mp3");
	int seFuel = LoadSoundMem("sound/fuel.mp3");
	int seCrash = LoadSoundMem("sound/crash.mp3");
	ChangeVolumeSoundMem(128, bgm);
	ChangeVolumeSoundMem(128, jin);

	while (1) // ���C�����[�v
	{
		ClearDrawScreen(); // ��ʂ��N���A����

		// �w�i�̃X�N���[������
		if (scene == PLAY) bgY = (bgY + 10) % HEIGHT; // �v���C���ɂ����X�N���[��
		DrawGraph(0, bgY - HEIGHT, imgBG, FALSE);
		DrawGraph(0, bgY, imgBG, FALSE);

		// �v���C���[�̎Ԃ𓮂�������
		if (scene == PLAY) // �v���C���ɂ���������
		{
			GetMousePoint(&playerX, &playerY);
			if (playerX < 260) playerX = 260;
			if (playerX > 460) playerX = 460;
			if (playerY < 40) playerY = 40;
			if (playerY > 600) playerY = 600;
		}
		drawCar(playerX, playerY, playerType);

		// �R���s���[�^�[�̎Ԃ𓮂�������
		for (int i = 0; i < COM_MAX; i++)
		{
			if (scene == PLAY) // �v���C���̎Ԃ̏���
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
					PlaySoundMem(seCrash, DX_PLAYTYPE_BACK); // ���ʉ�
					fuel -= 10;
				}
				// �ǂ����������𔻒�
				if (computerY[i] > playerY && computerFlag[i] == 0)
				{
					computerFlag[i] = 1;
					score += 100;
					if (score > highScore) highScore = score;
				}
			}
			else  // �^�C�g����ʂƃQ�[���I�[�o�[��ʂł̎Ԃ̓���
			{
				computerY[i] = computerY[i] - 1 - i;
				if (computerY[i] < -100) computerY[i] = HEIGHT + 100;
			}
			drawCar(computerX[i], computerY[i], computerType[i]);
		}

		// �R���A�C�e���̏���
		if (scene == PLAY) // �Q�[���������o��
		{
			fuelY += 4;
			if (fuelY > HEIGHT) fuelY = -100;
			if (abs(fuelX - playerX) < CAR_W[playerType] / 2 + 12 && abs(fuelY - playerY) < CAR_H[playerType] / 2 + 12)
			{
				fuelX = rand() % 180 + 270;
				fuelY = -500;
				fuel += 200;
				PlaySoundMem(seFuel, DX_PLAYTYPE_BACK); // ���ʉ�
			}
			DrawGraph(fuelX - 12, fuelY - 12, imgFuel, TRUE);
		}

		timer++; // �^�C�}�[���J�E���g
		switch (scene) // �^�C�g���A�Q�[���v���C�A�Q�[���I�[�o�[�̕���
		{
		case TITLE: // �^�C�g����ʂ̏���
			drawText(160, 160, 0xffffff, "Car Race", 0, 100);
			if (timer % 60 < 30) drawText(210, 400, 0x00ff00, "Click to start.", 0, 40);
			if (GetMouseInput() & MOUSE_INPUT_LEFT)
			{
				playerX = WIDTH / 2;
				playerY = HEIGHT / 2;
				for (int i = 0; i < COM_MAX; i++)
				{
					computerY[i] = HEIGHT + 100;
					computerFlag[i] = 0;
				}
				fuelX = WIDTH / 2;
				fuelY = -100;
				score = 0;
				fuel = 1000;
				scene = PLAY;
				PlaySoundMem(bgm, DX_PLAYTYPE_LOOP); // BGM�����[�v�Đ�
			}
			break;

		case PLAY: // �Q�[�����v���C���鏈��
			fuel -= 1;
			if (fuel < 0)
			{
				fuel = 0;
				scene = OVER;
				timer = 0;
				StopSoundMem(bgm); // BGM���~
				PlaySoundMem(jin, DX_PLAYTYPE_BACK); // �W���O�����o��
			}
			break;

		case OVER: // �Q�[���I�[�o�[�̏���
			drawText(180, 240, 0xff0000, "GAME OVER", 0, 80);
			if (timer > 60 * 5) scene = TITLE;
			break;
		}
		
		// �X�R�A�Ȃǂ̕\��
		drawText(10, 10, 0x00ffff, "SCORE %d", score, 30);
		drawText(WIDTH - 200, 10, 0xffff00, "HI-SC %d", highScore, 30);
		int col = 0x00ff00; // �R���̒l��\������F
		if (fuel < 400) col = 0xffc000;
		if (fuel < 200) col = 0xff0000;
		drawText(10, HEIGHT - 40, col, "FUEL %d", fuel, 30);
		
		ScreenFlip(); // ����ʂ̓��e��\��ʂɔ��f������
		WaitTimer(16); // ��莞�ԑ҂�
		if (ProcessMessage() == -1) break; // Windows ��������󂯎��G���[���N������I��
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESC �L�[�������ꂽ��I��
	}

	DxLib_End(); // DX���C�u�����g�p�̏I������
	return 0; // �\�t�g�̏I��
}