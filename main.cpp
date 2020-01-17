//########## �w�b�_�[�t�@�C���ǂݍ��� ##########
#include "DxLib.h"
#include<stdio.h>
#include <dos.h>
#include<math.h>

//########## �}�N����` ##########
#define GAME_WIDTH	800		//��ʂ̉��̑傫��
#define GAME_HEIGHT	640		//��ʂ̏c�̑傫��
#define GAME_COLOR	32		//��ʂ̃J���[�r�b�g

#define GAME_WINDOW_NAME	"Devil Device RTA"		//�E�B���h�E�̃^�C�g��
#define GAME_WINDOW_MODECHANGE	TRUE			//TRUE�F�E�B���h�E���[�h / FALSE�F�t���X�N���[��

#define SET_WINDOW_ST_MODE_DEFAULT			0	//�f�t�H���g
#define SET_WINDOW_ST_MODE_TITLE_NONE		1	//�^�C�g���o�[�Ȃ�
#define SET_WINDOW_ST_MODE_TITLE_FLAME_NONE	2	//�^�C�g���o�[�ƃt���[���Ȃ�
#define SET_WINDOW_ST_MODE_FLAME_NONE		3	//�t���[���Ȃ�

#define FPS_SPEED    60

#define BACKIMAGE_1 "BACKIMAGE\\haikei4_kai.png"
#define BACKFGROUND_TITLE "BACKIMAGE\\title2.png"

#define MAP_1   "MAPIMAGE\\MapData2.png"

#define MAP_CSV_1    "MAPIMAGE\\stage_120_20_kai.csv"

#define PLAYER_IMAGE "CHARA\\boukensya.png"

#define MAP_BUN_TATE_CNT    20
#define MAP_BUN_YOKO_CNT    20

#define MAP_YOKO  120
#define MAP_TATE  20

#define MAP_OneBlock_TATE_SIZE   32
#define MAP_OneBlock_YOKO_SIZE   32

#define CHARA_BUN_YOKO_CNT 3
#define CHARA_BUN_TATE_CNT 4

#define GAME_ENEMY_BUN_YOKO_CNT 12
#define GAME_ENEMY_BUN_TATE_CNT 8

#define CHARA_TATE_SIZE     32
#define CHARA_YOKO_SIZE     32

#define CHARA_MOTION_NUM    12

#define CHARA_KIND_YOKO    3
#define CHARA_KIND_TATE    4  

/* �}�b�v�ɂ��ړ��̐����� */
#define MAP_NOTDOWN_KIND    5
#define MAP_OKDOWN_KIND     1
#define MAP_NOTUP_KIND      5
#define MAP_NOTLEFT_KIND    5
#define MAP_NOTRIGHT_KIND   5
#define OFF_HIT 1
#define ON_HIT  1

#define GAME_MAP_GOAL_KIND      1  //�S�[���̎��

#define GAME_CHARA_SPEED      2//�L�����̃X�s�[�h

#define GRAVITY    9.8067 //�d�͉����x
#define CHARA_WEIGHT    60//�L�����̏d��

#define GAME_PLAYER_ZANKI_MAX  3

//########## �񋓌^ ##########
enum GAME_SCENE {
	GAME_SCENE_TITLE,	//�^�C�g�����
	GAME_SCENE_SOUSA,
	GAME_SCENE_ZANKI,
	GAME_SCENE_PLAY,	//�v���C���
	GAME_SCENE_END		//�G���h���
};

enum MAP_IMAGE {
	M_N = -1,
	M_B = 19,
	M_G_T = 6,
	M_G_N = 38,
	M_AM = 95,
	M_END = 399
};

enum CHARA_IMAGE {
	CHARA_KIND_1 = 0,
	CHARA_KIND_END = 11
};

enum GAME_END_KIND {
	END_KIND_CONTINUE,
	END_KIND_OVER,
	END_KIND_CLEAR
};

//############�\����##############
typedef struct STRUCT_GAZOU {
	int Handle;
	char FilePath[128];
	int X;
	int Y;
	int Width;
	int Height;
	int c_Width;
	int c_Height;
}GAZOU;

GAZOU BACKGROUND;
GAZOU BACK_TITLE;

typedef struct STRUCT_MAP
{
	char FilePath[128];
	int Handle[MAP_BUN_TATE_CNT*MAP_BUN_YOKO_CNT];
	int X[MAP_BUN_TATE_CNT*MAP_BUN_YOKO_CNT];
	int Y[MAP_BUN_YOKO_CNT*MAP_BUN_TATE_CNT];
	int Width[MAP_BUN_YOKO_CNT*MAP_BUN_TATE_CNT];
	int Height[MAP_BUN_YOKO_CNT*MAP_BUN_TATE_CNT];
	int C_Width[MAP_BUN_YOKO_CNT*MAP_BUN_TATE_CNT];
	int C_Height[MAP_BUN_YOKO_CNT*MAP_BUN_TATE_CNT];

	BOOL charaStopFlag;

	BOOL GoalFlag;

	int EndKind;

	BOOL GoalEffect1;
}MAP;


typedef struct STRUCT_CHARACTOR
{
	char FilePath[128];
	int Handle[GAME_ENEMY_BUN_YOKO_CNT*GAME_ENEMY_BUN_TATE_CNT];
	int X[GAME_ENEMY_BUN_YOKO_CNT*GAME_ENEMY_BUN_TATE_CNT];
	int Y[GAME_ENEMY_BUN_YOKO_CNT*GAME_ENEMY_BUN_YOKO_CNT];
	int Width[GAME_ENEMY_BUN_YOKO_CNT*GAME_ENEMY_BUN_TATE_CNT];
	int Height[GAME_ENEMY_BUN_YOKO_CNT*GAME_ENEMY_BUN_TATE_CNT];
	int C_Width[GAME_ENEMY_BUN_YOKO_CNT*GAME_ENEMY_BUN_TATE_CNT];
	int C_Height[GAME_ENEMY_BUN_YOKO_CNT*GAME_ENEMY_BUN_TATE_CNT];
}CHARA;

typedef struct STRUCT_PLAYER
{
	int Handle[CHARA_MOTION_NUM];
	int X;
	int Y;
	int Width;
	int Height;
	int C_Width;
	int C_Height;
	int NowHandleNum;
	int NowHandleCnt;
	int NowHandleCntMAX;

	int Speed;
	int MoveDist;

	int Zanki;

	BOOL IsJumpNow;

	int JumpStartTime;
	int JumpNowTime;
	int JumpPrevY;
	int JumpValue;
	int Mass;
	int Weight;

	int JumpMaxTop;
	BOOL IsJumpTop;

	BOOL CanMoveLeft;
	BOOL CanMoveRight;
	
	int atariX;
	int atariY;
	int atariWidth;
	int atariHeight;

	RECT atariRect;

}PLAYER;

//########## �O���[�o���ϐ� ##########

//typedef STRUCT_GAZOU GAZOU;
//typedef STRUCT_CHARACTOR CHARA;
//typedef STRUCT_MAP MAP;
//typedef STRUCT_PLAYER PLAYER;

//�E�B���h�E�֌W
WNDPROC WndProc;						//�E�B���h�E�v���V�[�W���̃A�h���X
BOOL IsWM_CREATE = FALSE;				//WM_CREATE������ɓ��삵�������f����

//FPS�֘A
int StartTimeFps;						//����J�n����
int CountFps;							//�J�E���^
float CalcFps;							//�v�Z����
int SampleNumFps = FPS_SPEED;		//���ς����T���v����

//�L�[�{�[�h�֘A
char AllKeyState[256];			//���ׂẴL�[�̏�Ԃ�����
int GameSceneNow = (int)GAME_SCENE_TITLE;

GAZOU BackImage[4];
int ScrollCntYoko = 0;

int ScrollDistPlusYoko = 1;

MAP MapImage;
int ScrollDistKeyPlusYoko = 2;

int G_Cnt = 0;

int charaCnt = 0;

int mapCnt = 0;

int i = 0;
int tate = 0;
int yoko = 0;
int sum = 0;
int rCnt = 0;
int NotD_Cnt = 0;
int cnt = 0;
int mot = 0;

int num = 0;

int message;

CHARA CharaImage;
PLAYER Myplayer;

FILE *fp_map_csv;

int MapData[MAP_TATE][MAP_YOKO];//�}�b�v�̃f�[�^

int MapData_Init[MAP_TATE][MAP_YOKO];//�}�b�v�̃f�[�^�������p

int ZankiDrawCount = 0;
int ZankiDrawCountMax = 60;

int UntilZankiDrawCount = 0;
int UntilZankiDrawCountMax = 60;

int PlayerImageNum[CHARA_MOTION_NUM] = {
	0,  1,  2,
	3,  4,  5,
	6,  7,  8,
	9, 10, 11
};

int MapNotDownKind[MAP_NOTDOWN_KIND] = { 0,67,107,125,126 };
int MapNotUpKind[MAP_NOTUP_KIND] = { 0,67,107,125,126 };

int MapOKDownKind[MAP_OKDOWN_KIND] = { -1 };

int MapGoalKind[GAME_MAP_GOAL_KIND] = { 96 };

int MapNotLeftKind[MAP_NOTLEFT_KIND] = { 0,67,107,125,126 };
int MapNotRightKind[MAP_NOTRIGHT_KIND] = { 0,67,107,125,126};

int OffHit[OFF_HIT] = { 126 };
int OnHit[ON_HIT] = { 125 };

RECT rectMap_DownNG[MAP_TATE][MAP_YOKO];
RECT rectMap_DownNG_First[MAP_TATE][MAP_YOKO];

RECT rectMap_DownOK[MAP_TATE][MAP_YOKO];
RECT rectMap_DownOK_First[MAP_TATE][MAP_YOKO];

RECT rectMap_LeftNG[MAP_TATE][MAP_YOKO];
RECT rectMap_LeftNG_First[MAP_TATE][MAP_YOKO];

RECT rectMap_RightNG[MAP_TATE][MAP_YOKO];
RECT rectMap_RightNG_First[MAP_TATE][MAP_YOKO];

RECT rectMap_UpNG[MAP_TATE][MAP_YOKO];
RECT rectMap_UpNG_First[MAP_TATE][MAP_YOKO];


//########## �v���g�^�C�v�錾 ##########
LRESULT CALLBACK MY_WNDPROC(HWND, UINT, WPARAM, LPARAM);	//����E�B���h�E�v���V�[�W��

VOID MY_ALL_KEYDOWN_UPDATE(VOID);	//�L�[�̓��͏�Ԃ��X�V����֐�

VOID MY_FPS_UPDATE(VOID);			//FPS�l���v���A�X�V����֐�
VOID MY_FPS_DRAW(VOID);				//FPS�l��`�悷��֐�

VOID MY_GAME_TITLE(VOID);			//�^�C�g����ʂ̊֐�
VOID MY_GAME_SOUSA(VOID);           //���������ʂ̊֐�
VOID MY_GAME_PLAY(VOID);			//�v���C��ʂ̊֐�
VOID MY_GAME_END(VOID);				//�G���h��ʂ̊֐�

BOOL MY_GAZOU_LOAD(GAZOU *, int, int, const char *);


BOOL MY_PLAY_INIT(VOID);
VOID MY_PLAY_BACKIMAGE_DRAW(VOID);
VOID MY_PLAY_SCROLL(VOID);

BOOL MY_MAP_READ_CSV_NUM(FILE *, const char *);

BOOL MY_CHARA_LOAD_BUNKATSU(CHARA *, int, int, int, int, int, const char *);
BOOL MY_INIT_PLAYER(PLAYER *, CHARA, int *, int, int, int);

VOID MY_FPS_WAIT(VOID);				//FPS�l���v�����A�҂֐�
VOID MY_PLAY_PLAYER_DRAW(VOID);
VOID MY_PLAYER_OPERATION(VOID);

VOID MY_PLAY_MAP_DRAW(VOID);//�}�b�v��`�悷��֐�

VOID MY_PLAY_PLAYER_JUMP_PREV(VOID);//�W�����v�̑O�������s���֐�

BOOL MY_PLAY_CHECK_GAME_END(VOID);//�Q�[���G���h�����肷��֐�

VOID MY_DRAW_STRING_CENTER_CENTER(char[][128], int, char *, int);//��ʒ����ɕ�����`�悷��֐�
VOID MY_DRAW_SET_DEFAULT_FONT(BOOL);//�������f�t�H���g�t�H���g�ɐݒ肷��֐�

VOID MY_BG_IMAGE_INIT(VOID);

/* �����蔻��̊֐� */
VOID MY_SET_PLAYER_ATARI(PLAYER *);
BOOL MY_CHECK_RECT_ATARI(RECT, RECT);
BOOL MY_CHECK_RECT_ATARI_CHARA_MAP(RECT, RECT[MAP_TATE][MAP_YOKO]);
BOOL MY_CHECK_RECT_ATARI_CHARA_MAP_ATARIBASHO(RECT, RECT map[MAP_TATE][MAP_YOKO], int *, int *);

VOID MAP_WANA(VOID);

VOID MY_TITLE_BACKGROUND_DRAW(VOID);

BOOL BACKGROUND_LOAD(GAZOU *, int, int, const char *);

VOID MY_GAME_ZANKI(VOID);

VOID MY_FPS_DRAW(VOID);

BOOL MAP_INIT(MAP *, int,int,int,int,int,const char *);
//########## �v���O�����ōŏ��Ɏ��s�����֐� ##########
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(GAME_WINDOW_MODECHANGE);					//�E�B���h�E���[�h�ɐݒ�
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);			//�w��̐��l�ŉ�ʂ�\������
	SetWindowStyleMode(SET_WINDOW_ST_MODE_TITLE_NONE);			//�^�C�g���o�[�Ȃ�
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));					//�^�C�g���̕���

	//�t�b�N��WM_CLOSE�Ȃǂ̃��b�Z�[�W�������|���Ď擾����
	SetHookWinProc(MY_WNDPROC);	//�E�B���h�E�v���V�[�W���̐ݒ�

	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1) { return -1; }

	//�c�w���C�u��������������

	SetDrawScreen(DX_SCREEN_BACK);	
	
	if (MY_GAZOU_LOAD(&BackImage[0], 0, 0, BACKIMAGE_1) == FALSE) { MessageBox(NULL, BACKIMAGE_1, "NotFound", MB_OK); return -1; }

	//if (!BACKGROUND_LOAD(&BACKGROUND, 0, 0, BACKIMAGE_1))
	//{
	//	return -1;
	//}

	if (!BACKGROUND_LOAD(&BACK_TITLE, 0, 0, BACKFGROUND_TITLE))
	{
		return -1;
	}
	if (MY_MAP_READ_CSV_NUM(fp_map_csv, MAP_CSV_1) == FALSE) { MessageBox(NULL, MAP_CSV_1, "NotFound", MB_OK); return -1; }

	if (MAP_INIT(&MapImage, 200, 20, 20, 32, 32, MAP_1) == FALSE) { return -1; }

	if (MY_CHARA_LOAD_BUNKATSU(&CharaImage, CHARA_BUN_YOKO_CNT*CHARA_BUN_TATE_CNT, CHARA_BUN_YOKO_CNT, CHARA_BUN_TATE_CNT, CHARA_YOKO_SIZE, CHARA_TATE_SIZE, PLAYER_IMAGE) == FALSE) { MessageBox(NULL, PLAYER_IMAGE, "NotFound", MB_OK); return -1; }
	
	if (MY_GAZOU_LOAD(&BackImage[0], 0, 0, BACKIMAGE_1) == FALSE) { return-1; }

	//�������[�v
	while (TRUE)
	{

		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W�����̌��ʂ��G���[�̂Ƃ��A�����I��

		if (ClearDrawScreen() != 0) { break; }	//��ʂ������ł��Ȃ������Ƃ��A�����I��

		MY_ALL_KEYDOWN_UPDATE();				//�L�[�̏�Ԃ��擾

		//�V�[���I��
		switch (GameSceneNow)
		{
		case (int)GAME_SCENE_TITLE:	//�^�C�g����ʂ̏�����������

			MY_GAME_TITLE();	//�^�C�g����ʂ̏���

			break;	//�^�C�g����ʂ̏��������܂�


		case (int)GAME_SCENE_SOUSA:
			MY_GAME_SOUSA();

			break;

		case (int)GAME_SCENE_ZANKI://�c�@�\����ʂ̏�����������

			MY_GAME_ZANKI();//�c�@�\����ʂ̏���

			break;

		case (int)GAME_SCENE_PLAY:	//�v���C��ʂ̏�����������

			MY_GAME_PLAY();		//�v���C��ʂ̏���

			break;	//�v���C��ʂ̏��������܂�

		case (int)GAME_SCENE_END:	//�G���h��ʂ̏�����������

			MY_GAME_END();		//�G���h��ʂ̏���

			break;	//�G���h��ʂ̏��������܂�

		default:

			break;
		}
	
		MY_FPS_UPDATE();						//FPS�̏���[�X�V]
		//DrawGraph(BackImage.X, BackImage.Y, BackImage.Handle, TRUE);
		// �w�i�̉摜��`��

		MY_FPS_DRAW();		//FPS�̏���[�`��]

		
		 ScreenFlip();		
		 //���j�^�̃��t���b�V�����[�g�̑����ŗ���ʂ��ĕ`��
		  
		MY_FPS_WAIT();		//FPS�̏���[�҂�]
	}
	for (mapCnt = 0; mapCnt <MAP_YOKO*MAP_TATE; mapCnt++)
	{
		DeleteGraph(MapImage.Handle[mapCnt]);
	}

	for (charaCnt = 0; charaCnt < CHARA_BUN_YOKO_CNT*CHARA_BUN_TATE_CNT; charaCnt++)
	{
		DeleteGraph(CharaImage.Handle[charaCnt]);
	}
	

	DxLib_End();		//�c�w���C�u�����g�p�̏I������

	return 0;
}

//########## FPS�l���v���A�X�V����֐� ##########
VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0) //1�t���[���ڂȂ玞�����L��
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == SampleNumFps) //60�t���[���ڂȂ畽�ς��v�Z
	{
		int now = GetNowCount();
		CalcFps = 1000.f / ((now - StartTimeFps) / (float)SampleNumFps);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}

//########## FPS�l���v�����A�҂֐� ##########
VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;					//������������
	int waitTime = CountFps * 1000 / FPS_SPEED - resultTime;	//�҂ׂ�����

	if (waitTime > 0)		//�w���FPS�l���������ꍇ
	{
		Sleep(waitTime);	//�҂�
	}
	return;
}

VOID MY_FPS_DRAW(VOID)
{
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255), "FPS:%.1f", CalcFps);

	return;
}

//########## �L�[�̓��͏�Ԃ��X�V����֐� ##########
VOID MY_ALL_KEYDOWN_UPDATE(VOID)
{
	char TempKey[256];

	GetHitKeyStateAll(TempKey);   //���ׂẴL�[���͏�Ԃ��擾����

	for (i = 0; i < 256; i++)
	{
		if (TempKey[i] != 0)
		{
			AllKeyState[i]++;    //������Ă���
		}
		else
		{
			AllKeyState[i] = 0;  //������Ă��Ȃ�
		}
	}
	return;
}

//########## �^�C�g����ʂ̊֐� ##########
VOID MY_GAME_TITLE(VOID)
{
	MY_TITLE_BACKGROUND_DRAW();

	if (AllKeyState[KEY_INPUT_RETURN] != 0)
	{
		if (MY_PLAY_INIT() == TRUE)		//�v���C��ʏ�����
		{

			for (tate = 0; tate < MAP_TATE; tate++)
			{
				for (yoko = 0; yoko < MAP_YOKO; yoko++)
				{
					MapData[tate][yoko] = MapData_Init[tate][yoko];
				}
			}

		}
	}
	if (AllKeyState[KEY_INPUT_RETURN] != 0)	//�G���^�[�L�[��������Ă�����
	{
		GameSceneNow = (int)GAME_SCENE_SOUSA;	//�V�[���𑀍������ʂɂ���
	}
	//DrawString(310, 270, "�f�l�c", GetColor(255, 255, 255), SetFontSize(60));
	DrawString(190, 450, "�G���^�[�L�[�������Ă�������", GetColor(255, 255, 0), SetFontSize(30));

	return;
}

//##########����������##########
VOID MY_GAME_SOUSA(VOID)
{
	if (AllKeyState[KEY_INPUT_SPACE] != 0)	//�G���^�[�L�[��������Ă�����
	{
		GameSceneNow = (int)GAME_SCENE_PLAY;	//�V�[�����v���C��ʂɂ���
	}

	DrawString(190, 330, "�X�y�[�X�L�[�������Ă�������", GetColor(255, 255, 255), SetFontSize(30));
}
//############�c�@�\����ʂ̊֐�############
VOID MY_GAME_ZANKI(VOID)
{
	//�U�O�t���[�����҂�
	if (ZankiDrawCount < ZankiDrawCountMax)
	{
		ZankiDrawCount++;
	}
	else
	{
		GameSceneNow = (int)GAME_SCENE_PLAY;//�V�[�����c�@�\����ʂɂ���
	}

	MY_PLAY_BACKIMAGE_DRAW();//�w�i��`��

	//�����ɂ��鐔�l(0�����߂Ȃ��A255�����߂���)
	double ToukaPercent = (ZankiDrawCountMax - ZankiDrawCount) / (double)ZankiDrawCountMax;//���߁������߂�

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, ToukaPercent * 255);//���߂�����

	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 200, 200), TRUE);//�^���Âȉ�ʂɂ���

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//���߂���߂�

	char StrGameZanki[1][128] = { "\0" };
	char StrFontZanki[128] = { "MS �S�V�b�N" };//�啶�����p[MS],���p�u�󔒁v�A�S�p�u�S�V�b�N�v

	sprintf(&StrGameZanki[0][0], "�v���C���[�~%02d", Myplayer.Zanki);//�v���C���[�c�@�̕\���𐮌`

	MY_DRAW_STRING_CENTER_CENTER(&StrGameZanki[0], 1, StrFontZanki, 64);

	DrawString(0, 0, "�c�@�\�����", GetColor(255, 255, 255));

	return;
}


//########## �v���C��ʂ̊֐� ##########
VOID MY_GAME_PLAY(VOID)
{

	if (AllKeyState[KEY_INPUT_SPACE] != 0)
	{
		//GameSceneNow = (int)GAME_SCENE_END;
	}

	if (AllKeyState[KEY_INPUT_LEFT] != 0)
	{

		if (Myplayer.CanMoveLeft == TRUE)//�v���C���[�����ɓ�����Ƃ�
		{
			if (MapImage.charaStopFlag == TRUE)//�v���C���[�𓮂����Ȃ��Ȃ��
			{
				if (ScrollCntYoko > 0)//�}�C�i�X�̃X�N���[�������Ȃ�
				{
					ScrollCntYoko -= Myplayer.Speed;//��ʂ����ɃX�N���[��
				}
			}
		}

	}
	if (AllKeyState[KEY_INPUT_RIGHT] != 0)
	{
		if (Myplayer.CanMoveRight == TRUE)//�v���C���[���E�ɓ�����Ƃ�
		{
			if (MapImage.charaStopFlag == TRUE)//�v���C���[�𓮂����Ȃ��Ȃ��
			{
				if (ScrollCntYoko < MAP_OneBlock_YOKO_SIZE * MAP_YOKO)//��ʊO�փX�N���[�������Ȃ�
				{
					ScrollCntYoko += Myplayer.Speed;//��ʂ��E�ɃX�N���[��
				}
			}
		}
	}

	if (MY_PLAY_CHECK_GAME_END() == TRUE)
	{
		//StopSoundMem(MUSIC_MY_BGM_1.Handle);
		GameSceneNow = (int)GAME_SCENE_END;

	}
	else
	{
		MY_PLAYER_OPERATION();	//�v���C���[�̑���
	}

	//MY_PLAYER_OPERATION();

	MY_PLAY_BACKIMAGE_DRAW();	//�w�i��`��

	MY_PLAY_MAP_DRAW();  //�}�b�v��`��

	MY_PLAY_PLAYER_DRAW();

	DrawString(0, 0, "�G���^�[�L�[�������ƈړ�", GetColor(255, 255, 255));
	//DrawFormatString(20, 20, GetColor(255, 255, 255), "%d", playerX);

	return;
}

//###########��ʒ����ɕ�����`�悷��֐�############
VOID MY_DRAW_STRING_CENTER_CENTER(char str[][128], int row, char *fontname, int size)
{
	ChangeFont(fontname, DX_CHARSET_DEFAULT);//�f�t�H���g�̃t�H���g��ύX����
	SetFontSize(size);                       //�f�t�H���g�̃t�H���g�T�C�Y��ς���
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);//�t�H���g�����ꂢ�ɂ݂���

	int StrHeightSum = 0;
	for (sum = 0; sum < row; sum++)//�s�S�̂̍������W�v
	{
		StrHeightSum += GetFontSize();//�f�t�H���g�̃t�H���g�̍������擾
	}

	for (rCnt = 0; rCnt < row; rCnt++)//�t�H���g��`��
	{
		int StrWidth = GetDrawFormatStringWidth(&str[rCnt][0]);//�f�t�H���g�̃t�H���g�̉������擾
		int StrHeight = GetFontSize();

		DrawFormatString(
			GAME_WIDTH / 2 - StrWidth / 2,
			GAME_HEIGHT / 2 - StrHeightSum + StrHeight * rCnt,//��ʂ̒��S�@-�@�s�S�̂̕����̍����@+�@�P�s�̍���
			GetColor(255, 255, 255), &str[rCnt][0]);//�f�t�H���g�̃t�H���g�ŕ`��
	}

	MY_DRAW_SET_DEFAULT_FONT(TRUE);//�f�t�H���g�t�H���g�ɐݒ肷��
}



//########## �Q�[���G���h�����肷��֐� ##########
BOOL MY_PLAY_CHECK_GAME_END(VOID)
{
	//�v���C���[�̈ʒu�́A�v���C���[�̒��S
	int PlayerToMapNumY = (Myplayer.Y + Myplayer.C_Height) / MAP_OneBlock_TATE_SIZE;
	int PlayerToMapNumX = (Myplayer.MoveDist + Myplayer.C_Width) /MAP_OneBlock_TATE_SIZE;


	//�v���C���[��ʊO�ɗ������Ƃ�
	if (Myplayer.Y > GAME_HEIGHT)
	{
		
			//ChangeVolumeSoundMem(255 * 80 / 100, MUSIC_MY_ZANKIMI.Handle);
			//PlaySoundMem(MUSIC_MY_ZANKIMI.Handle, DX_PLAYTYPE_BACK);
			//MapImage.EndKind = (int)END_KIND_CONTINUE;
	
	
			MapImage.EndKind = (int)END_KIND_OVER;
		


		//MapImage.GameEndKind = (int)END_KIND_OVER;
		return TRUE;

	}

	if (PlayerToMapNumX<0 || PlayerToMapNumX>MAP_YOKO) { return FALSE; }
	if (PlayerToMapNumY<0 || PlayerToMapNumY>MAP_TATE) { return FALSE; }

	for (G_Cnt = 0; G_Cnt < GAME_MAP_GOAL_KIND; G_Cnt++)
	{
		//�v���C���[���S�[���̂��̂ƐڐG�����Ƃ�
		//if (MapData[PlayerToMapNumY][PlayerToMapNumX] == MapData[ken_X][ken_Y])
		//{
		//	//if (Myplayer.GetRedCoinNum == 5)
		//	//{
		//	//ChangeVolumeSoundMem(255 * 80 / 100, MUSIC_MY_CLEAR.Handle);
		//	//PlaySoundMem(MUSIC_MY_CLEAR.Handle, DX_PLAYTYPE_BACK);
		//	MapImage.EndKind = (int)END_KIND_CLEAR;
		//	return TRUE;
		//	break;
		//	//}
		//}
	}

	return FALSE;//�Q�[�����s

}


//########## �G���h��ʂ̊֐� ##########
VOID MY_GAME_END(VOID)
{
	if (AllKeyState[KEY_INPUT_BACK] != 0)	//�o�b�N�X�y�[�X�[�L�[��������Ă�����
	{
		GameSceneNow = (int)GAME_SCENE_TITLE;	//�V�[�����^�C�g����ʂɂ���
	}
	DrawString(210, 300, "�f�`�l�d�@�n�u�d�q", GetColor(255, 0, 0) ,SetFontSize(40));

	DrawString(150, 350, "�^�C�g���ɖ߂�ɂ̓o�b�N�X�y�[�X�L�[�������Ă�������", GetColor(255, 255, 255), SetFontSize(18));

	DrawString(210, 368, "�I���������ꍇ�͉E�N���b�N�����Ă�������", GetColor(255, 255, 255), SetFontSize(18));

	return;
}

//�������f�t�H���g�t�H���g�ɐݒ肷��֐�
VOID MY_DRAW_SET_DEFAULT_FONT(BOOL anti)
{
	ChangeFont("MS �S�V�b�N", DX_CHARSET_DEFAULT);
	SetFontSize(16);

	if (anti)
	{
		ChangeFontType(DX_FONTTYPE_ANTIALIASING);
	}
	return;
}

//########## �E�B���h�E�v���V�[�W���֐� ##########
LRESULT CALLBACK MY_WNDPROC(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	int closeMsgRet = 0;

	switch (msg)
	{
	case WM_CREATE:	//�E�B���h�E�̐�����������


		IsWM_CREATE = TRUE;	//WM_CREATE����I��
		return 0;

	case WM_CLOSE:		//����{�^�����������Ƃ�

		message=MessageBox(hwnd, TEXT("�Q�[�����I�����܂�"), TEXT("�I�����b�Z�[�W"), MB_OK/*YESNO*/);

	/*	if (message == IDYES)
		{
			return 0;
		}
		else
		{
			DefWindowProc(hwnd, msg, wp, lp);
		}*/

		break;

	case WM_RBUTTONDOWN:	//�}�E�X�̉E�{�^�����������Ƃ�

		closeMsgRet = MessageBox(hwnd, TEXT("�Q�[�����I�����܂���"), TEXT("�I�����b�Z�[�W"), MB_YESNO);

		if (closeMsgRet == IDYES)
		{
			SendMessage(hwnd, WM_CLOSE, 0, 0);		//WM_CLOSE���b�Z�[�W���L���[�ɒǉ�
		}
		else if (closeMsgRet == IDNO)
		{
			//break;
		}

		break;

	case WM_LBUTTONDOWN:	//�}�E�X�̍��{�^�����������Ƃ�

		//WM_NCLBUTTONDOWN(�^�C�g���o�[�Ń}�E�X�̍��{�^����������)���b�Z�[�W�������ɔ��s
		//PostMessage(hwnd, WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, lp);
		break;

	case WM_DESTROY:	//�E�B���h�E���j�����ꂽ(�Ȃ��Ȃ���)�Ƃ�

			PostQuitMessage(0);		//���b�Z�[�W�L���[�� WM_QUIT �𑗂�

			return 0;
	}

	//�f�t�H���g�̃E�B���h�E�v���V�[�W���֐����Ăяo��
	return DefWindowProc(hwnd, msg, wp, lp);
}

//�������̊֐�
BOOL MY_PLAY_INIT(VOID)
{
	if (MY_INIT_PLAYER(&Myplayer, CharaImage, &PlayerImageNum[0], MAP_OneBlock_YOKO_SIZE * 0, MAP_OneBlock_TATE_SIZE * 0, GAME_CHARA_SPEED) == FALSE) { MessageBox(NULL, "CHARA_INIT", "NotFound", MB_OK); return FALSE; }

	//MapImage.GoalFlg = FALSE;//�S�[����Ԃ�������

	//MapImage.GameEndKind = (int)END_KIND_CONTINUE;

	//MapImage.GoalEffect1 = FALSE;

	//�v���C���[�̏�����


	MY_DRAW_SET_DEFAULT_FONT(TRUE);	//�f�t�H���g�t�H���g�ɐݒ肷��

	MY_BG_IMAGE_INIT();//�w�i�̈ʒu��������

	ScrollCntYoko = 0;    //�X�N���[���ʂ�������

	ZankiDrawCount = 0;//�c�@�\���J�E���^������
	UntilZankiDrawCount = 0;//�c�@�\���܂ł̃J�E���^������

	return TRUE;

}

VOID MY_BG_IMAGE_INIT(VOID)
{
	//�w�i�̈ʒu
	BackImage[0].X = 0;

	return;
}

//�v���C��ʂ̔w�i��`�悷��֐�
VOID MY_PLAY_BACKIMAGE_DRAW(VOID)
{
	DrawGraph(BACKGROUND.X, BACKGROUND.Y, BACKGROUND.Handle, TRUE);
}

//�^�C�g����ʂ̔w�i��`�悷��֐�
VOID MY_TITLE_BACKGROUND_DRAW(VOID)
{
	DrawGraph(BACK_TITLE.X, BACK_TITLE.Y, BACK_TITLE.Handle, TRUE);
}

//�摜��ǂݍ��ސݒ������֐�
BOOL MY_GAZOU_LOAD(GAZOU *g, int x, int y, const char *path)
{
	wsprintf(g->FilePath, path);
	g->Handle = LoadGraph(g->FilePath);

	if (g->Handle == -1) { return FALSE; }

	GetGraphSize(g->Handle, &g->Width, &g->Height);
	g->X = x;
	g->Y = y;
	g->c_Width = g->Width / 2;
	g->c_Height = g->Height / 2;

	return TRUE;
}

//�}�b�v�𕪊����ēǂݍ��ސݒ������֐�
BOOL MAP_INIT(MAP *m, int bun_num, int bun_x_num, int bun_y_num, int bun_width, int bun_height, const char *path)
{
	int ret = 0;
	wsprintf(m->FilePath, path);

	ret = LoadDivGraph(m->FilePath, bun_num, bun_x_num, bun_y_num, bun_width, bun_height, m->Handle);

	if (ret == -1)
	{
		return FALSE;
	}

	for (int count = 0; count < MAP_BUN_TATE_CNT*MAP_BUN_YOKO_CNT; count++)
	{
		GetGraphSize(
			m->Handle[cnt],
			&m->Width[cnt],
			&m->Height[cnt]
		);
		m->C_Width[cnt] = m->Width[cnt] / 2;
		m->C_Height[cnt] = m->Height[cnt] / 2;
	}

	m->charaStopFlag = FALSE;

	return TRUE;
}

//�L�����𕪊����ēǂݍ��ފ֐�
BOOL MY_CHARA_LOAD_BUNKATSU(CHARA *c, int bun_num, int bun_x_num, int bun_y_num, int bun_width, int bun_height, const char *path)
{
	int ret = 0;
	wsprintf(c->FilePath, path);

	ret = LoadDivGraph(c->FilePath, bun_num, bun_x_num, bun_y_num, bun_width, bun_height, c->Handle);

	if (ret == -1) { return FALSE; }

	for (cnt = 0; cnt < MAP_BUN_YOKO_CNT*MAP_BUN_TATE_CNT; cnt++)
	{
		GetGraphSize(
			c->Handle[cnt],
			&c->Width[cnt],
			&c->Height[cnt]
		);

		c->C_Width[cnt] = c->Width[cnt] / 2;
		c->C_Height[cnt] = c->Height[cnt] / 2;
	}
	return TRUE;
}



//�w�i��ǂݍ��ފ֐�
BOOL BACKGROUND_LOAD(GAZOU *g, int x, int y, const char *path)
{
	wsprintf(g->FilePath, path);
	g->Handle = LoadGraph(g->FilePath);
	if (g->Handle == -1)
	{
		return FALSE;
	}
	GetGraphSize(g->Handle, &g->Width, &g->Height);
	g->X = x;
	g->Y = y;

	return TRUE;
}

//�v���C���[������������֐�
BOOL MY_INIT_PLAYER(PLAYER *p, CHARA c, int *num, int x, int y, int speed)
{
	for (mot = 0; mot < CHARA_MOTION_NUM; mot++)
	{
		p->Handle[mot] = c.Handle[*(num + mot)];
	}

	p->X = x;
	p->Y = y;
	p->Width = c.Width[*(num + 0)];
	p->Height = c.Height[*(num + 0)];
	p->C_Width = c.C_Width[*(num + 0)];
	p->C_Height = c.C_Height[*(num + 0)];

	p->Width = MAP_OneBlock_YOKO_SIZE;
	p->Height = MAP_OneBlock_TATE_SIZE;
	p->C_Width = MAP_OneBlock_YOKO_SIZE / 2;
	p->C_Height = MAP_OneBlock_TATE_SIZE / 2;

	p->NowHandleNum = 6;
	p->NowHandleCnt = 0;
	p->NowHandleCntMAX = 4;

	p->Speed = speed;

	p->MoveDist = x;

	p->IsJumpNow = FALSE;
	p->JumpStartTime = 0;
	p->JumpNowTime = 0;
	p->JumpPrevY = 0;
	p->JumpValue = 0;
	p->Mass = CHARA_WEIGHT;
	p->Weight = p->Mass*GRAVITY;

	p->CanMoveLeft = TRUE;
	p->CanMoveRight = TRUE;

	p->atariX = 4;
	p->atariY = 0;
	p->atariWidth = 25;
	p->atariHeight = 32;

	//MY_SET_PLAYER_ATARI(p);

	return TRUE;
}

//�v���C���[�𑀍삷��֐�
VOID MY_PLAYER_OPERATION(VOID)
{
	BOOL IsKeyDown = FALSE;

	BOOL CanMoveLeft = TRUE;//���ɍs���邩
	BOOL CanMoveRight = TRUE;//�E�ɍs���邩

	//�v���C���[�̈ʒu���}�b�v�z��̂ǂ��ɂ��邩�ϊ�
	int PlayerToMapNumY;
	//�v���C���[�́y�ړ������ʒu�z���}�b�v�z��̂ǂ��ɂ��邩�ϊ�	���v���C���[�̈ʒu�́A�X�N���[�����A�ς��Ȃ�����
	//�L�����̈ʒu���A�P�}�b�v�̔����̈ʒu�ɂ���
	int PlayerToMapNumX;

	if (AllKeyState[KEY_INPUT_LEFT] != 0)
	{
		IsKeyDown = TRUE;
		if (Myplayer.NowHandleCnt < Myplayer.NowHandleCntMAX)
		{
			Myplayer.NowHandleCnt++;

		}
		else
		{
			Myplayer.NowHandleCnt = 0;

			if (Myplayer.NowHandleNum >= 3 && Myplayer.NowHandleNum < 5)
			{
				Myplayer.NowHandleNum++;
			}
			else
			{
				Myplayer.NowHandleNum = 3;
			}
		}
		//�������ɁA�܂�������
		Myplayer.CanMoveLeft = TRUE;

		MY_SET_PLAYER_ATARI(&Myplayer);
		Myplayer.atariRect.left -= 4;
		Myplayer.atariRect.right -= 4;

		if (MY_CHECK_RECT_ATARI_CHARA_MAP(Myplayer.atariRect, rectMap_LeftNG) == TRUE)
		{
			Myplayer.CanMoveLeft = FALSE;
		}

		if (Myplayer.CanMoveLeft == TRUE)//���Ɉړ��ł���Ƃ�
		{
			if (MapImage.charaStopFlag == FALSE)
			{
				if (Myplayer.X > 0)
				{
					Myplayer.X -= Myplayer.Speed;//�v���C���[�����Ɉړ�
				}
			}
			if (Myplayer.MoveDist > 0)
			{
				Myplayer.MoveDist -= Myplayer.Speed;//�������������v�Z
			}
		}
	}

	if (AllKeyState[KEY_INPUT_RIGHT] != 0)
	{

		IsKeyDown = TRUE;

		if (Myplayer.NowHandleCnt < Myplayer.NowHandleCntMAX)
		{
			Myplayer.NowHandleCnt++;
		}
		else
		{
			Myplayer.NowHandleCnt = 0;

			if (Myplayer.NowHandleNum >= 6 && Myplayer.NowHandleNum < 8)
			{
				Myplayer.NowHandleNum++;
			}
			else
			{
				Myplayer.NowHandleNum = 6;
			}
		}
		Myplayer.CanMoveRight = TRUE;

		MY_SET_PLAYER_ATARI(&Myplayer);
		Myplayer.atariRect.left += 4;
		Myplayer.atariRect.right += 4;

		if (MY_CHECK_RECT_ATARI_CHARA_MAP(Myplayer.atariRect, rectMap_RightNG) == TRUE)
		{
			Myplayer.CanMoveRight = FALSE;
		}

		if (Myplayer.CanMoveRight == TRUE)//�E�Ɉړ��ł���Ƃ�
		{
			if (MapImage.charaStopFlag == FALSE)
			{
				if (Myplayer.X + Myplayer.Width < GAME_WIDTH)
				{
					Myplayer.X += Myplayer.Speed;//�v���C���[���E�Ɉړ�
				}
			}
			if (Myplayer.MoveDist < MAP_OneBlock_YOKO_SIZE * MAP_YOKO)
			{
				Myplayer.MoveDist += Myplayer.Speed;//�������������v�Z
			}
		}
	}

	//�}�b�v�̍����ɂ���Ƃ��́A�v���C���[�𓮂���
	if (Myplayer.MoveDist > 0 &&
		Myplayer.MoveDist <= MAP_OneBlock_YOKO_SIZE * 10)
	{
		MapImage.charaStopFlag = FALSE;
	}

	//�}�b�v�^�񒆂�ւ�ɂ���Ƃ��́A�}�b�v�𓮂���
	if (Myplayer.MoveDist > MAP_OneBlock_YOKO_SIZE * 10 &&
		Myplayer.MoveDist <= MAP_OneBlock_YOKO_SIZE * 119)
	{
		MapImage.charaStopFlag = TRUE;
	}

	//�}�b�v�̉E���ɂ���Ƃ��́A�v���C���[�𓮂���
	if (Myplayer.MoveDist > MAP_OneBlock_YOKO_SIZE * 119 &&
		Myplayer.MoveDist <= MAP_OneBlock_YOKO_SIZE * MAP_YOKO)
	{
		MapImage.charaStopFlag = FALSE;
	}

	//�L�[��������Ă��Ȃ��Ƃ�
	if (IsKeyDown == FALSE)
	{
		if (Myplayer.NowHandleNum >= 6 && Myplayer.NowHandleNum <= 8)
		{
			Myplayer.NowHandleNum = 6;
		}
		else if (Myplayer.NowHandleNum >= 3 && Myplayer.NowHandleNum <= 5)
		{
			Myplayer.NowHandleNum = 3;
		}
		Myplayer.NowHandleCnt = Myplayer.NowHandleCntMAX - 1;	//�����摜��ς�����悤�ɂ���
	}

	//�Q�l�͂�����
	//https://dixq.net/g/30.html

	if (AllKeyState[KEY_INPUT_UP] != 0)//����L�[��������Ă�����
	{
		if (Myplayer.IsJumpNow == FALSE)//�W�����v���Ă��Ȃ��Ƃ�
		{
			MY_PLAY_PLAYER_JUMP_PREV();

			//PlaySoundMem(MUSIC_MY_SE_JUMP.Handle, DX_PLAYTYPE_BACK);//�W�����v�������ʉ���炷	

		}
	}

	if (Myplayer.IsJumpNow == TRUE)//�W�����v���Ă���Ƃ�
	{
		Myplayer.JumpNowTime = GetNowCount();//���݂̎������擾

		//����L�[�������āA�W�����v�������Ƃ�����̌o�ߎ��Ԃ��擾
		double KeikaTime = (Myplayer.JumpNowTime - Myplayer.JumpStartTime) / 1000.0;

		//�W�����v
		//�����������^���̌������g�p����{�����x�����̂Ƃ��́A�������^�����邱�Ƃ𗘗p���āE�E�E
		//�Q�l�T�C�g�Fhttp://www.wakariyasui.sakura.ne.jp/p/mech/henni/toukasokudo.html#matome
		double ConstAcceleMoveCalc =
			(
				//(
				sqrt(2.0*Myplayer.Weight*Myplayer.JumpValue)
				*KeikaTime
				- 0.5  // 1/2
				*Myplayer.Weight*pow(KeikaTime, 2)
				//)

				);
		Myplayer.Y = Myplayer.JumpPrevY - ConstAcceleMoveCalc;

	}

	if (Myplayer.IsJumpNow == TRUE)//�W�����v��
	{
		if (Myplayer.Y <= Myplayer.JumpMaxTop + 1)//�}�C�i�X�̐��l�̂Ƃ����l�����A0�̕����A+1
		{
			Myplayer.IsJumpTop = TRUE;//�W�����v�Œ��_�ɓ��B����
		}

		if (Myplayer.IsJumpTop == FALSE)
		{
			MY_SET_PLAYER_ATARI(&Myplayer);
			Myplayer.atariRect.top -= 4;
			Myplayer.atariRect.bottom -= 4;

			if (MY_CHECK_RECT_ATARI_CHARA_MAP(Myplayer.atariRect, rectMap_UpNG) == TRUE)
			{
				while (MY_CHECK_RECT_ATARI_CHARA_MAP(Myplayer.atariRect, rectMap_UpNG) == TRUE)
				{
					MY_SET_PLAYER_ATARI(&Myplayer);
					Myplayer.Y++;
				}

				Myplayer.IsJumpNow = FALSE;
				Myplayer.IsJumpTop = FALSE;
			}
		}


	}
	//if (Myplayer.IsJumpNow == TRUE && Myplayer.IsJumpTop == TRUE)//���_�ɓ��B������A���Ƃ͗�����̂�
	//{
	MY_SET_PLAYER_ATARI(&Myplayer);
	if (MY_CHECK_RECT_ATARI_CHARA_MAP(Myplayer.atariRect, rectMap_DownNG) == TRUE)
	{
		if (Myplayer.Y >= 0 && Myplayer.Y <= GAME_HEIGHT)
		{
			int PullUpPlayer = Myplayer.Y / MAP_OneBlock_TATE_SIZE;
			while (Myplayer.Y > PullUpPlayer*MAP_OneBlock_TATE_SIZE)
			{
				Myplayer.Y--;
			}
		}

		Myplayer.IsJumpNow = FALSE;
		Myplayer.IsJumpTop = FALSE;
	}

	if (Myplayer.IsJumpNow == FALSE)
	{

		MY_SET_PLAYER_ATARI(&Myplayer);
		Myplayer.atariRect.top += 4;
		Myplayer.atariRect.bottom += 4;

		if (MY_CHECK_RECT_ATARI_CHARA_MAP(Myplayer.atariRect, rectMap_DownNG) == FALSE)
		{
			MY_PLAY_PLAYER_JUMP_PREV();
			Myplayer.JumpValue = 0;
			Myplayer.IsJumpNow = TRUE;
			Myplayer.IsJumpTop = TRUE;
		}

	}

	//�}�b�v�̓����蔻����X�N���[��������
	for (tate = 0; tate < MAP_TATE; tate++)
	{
		for (yoko = 0; yoko < MAP_YOKO; yoko++)
		{
			rectMap_DownNG[tate][yoko].left = rectMap_DownNG_First[tate][yoko].left - ScrollCntYoko;
			rectMap_DownNG[tate][yoko].right = rectMap_DownNG_First[tate][yoko].right - ScrollCntYoko;

			rectMap_DownOK[tate][yoko].left = rectMap_DownOK_First[tate][yoko].left - ScrollCntYoko;
			rectMap_DownOK[tate][yoko].right = rectMap_DownOK_First[tate][yoko].right - ScrollCntYoko;

			rectMap_LeftNG[tate][yoko].left = rectMap_LeftNG_First[tate][yoko].left - ScrollCntYoko;
			rectMap_LeftNG[tate][yoko].right = rectMap_LeftNG_First[tate][yoko].right - ScrollCntYoko;

			rectMap_RightNG[tate][yoko].left = rectMap_RightNG_First[tate][yoko].left - ScrollCntYoko;
			rectMap_RightNG[tate][yoko].right = rectMap_RightNG_First[tate][yoko].right - ScrollCntYoko;

			rectMap_UpNG[tate][yoko].left = rectMap_UpNG_First[tate][yoko].left - ScrollCntYoko;
			rectMap_UpNG[tate][yoko].right = rectMap_UpNG_First[tate][yoko].right - ScrollCntYoko;

		}
	}

	return;
	//}
}

//�W�����v�̑O�������s���֐�
VOID MY_PLAY_PLAYER_JUMP_PREV(VOID)
{
	Myplayer.IsJumpNow = TRUE;
	Myplayer.IsJumpTop = FALSE;

	Myplayer.JumpValue = CHARA_TATE_SIZE * 2;
	Myplayer.JumpMaxTop = Myplayer.Y - Myplayer.JumpValue;

	Myplayer.JumpPrevY = Myplayer.Y;
	Myplayer.JumpStartTime = GetNowCount();

	return;
}

//�v���C���[��\������֐�
VOID MY_PLAY_PLAYER_DRAW(VOID)
{
	DrawExtendGraph(
		Myplayer.X,
		Myplayer.Y,
		Myplayer.X + Myplayer.Width,
		Myplayer.Y + Myplayer.Height,
		Myplayer.Handle[Myplayer.NowHandleNum], TRUE);

	//�W�����v��
	if (Myplayer.IsJumpNow == TRUE)
	{
		DrawString(0, 120, "�W�����v�t���OON", GetColor(255, 255, 255));
	}

	//���_�ɒB�����Ƃ�
	if (Myplayer.IsJumpTop == TRUE)
	{
		DrawString(0, 140, "���_�t���OON", GetColor(255, 255, 255));
	}

	return;
}

//�}�b�v��\������֐�
VOID MY_PLAY_MAP_DRAW(VOID)
{
	for (tate = 0; tate < MAP_TATE; tate++)
	{
		for (yoko = 0; yoko < MAP_YOKO; yoko++)
		{
			if ((yoko + 1)* MAP_OneBlock_YOKO_SIZE - ScrollCntYoko >= 0 &&
				(yoko + 0)* MAP_OneBlock_YOKO_SIZE - ScrollCntYoko <= GAME_WIDTH)
			{
				DrawGraph(
					yoko * MAP_OneBlock_YOKO_SIZE - ScrollCntYoko,
					tate * MAP_OneBlock_TATE_SIZE,
					MapImage.Handle[MapData[tate][yoko]], TRUE);
			}
		}
	}
}

//�Q�[����ʂ̃X�N���[�����s���֐�
VOID MY_PLAY_SCROLL(VOID)
{
	ScrollCntYoko += ScrollDistPlusYoko;
	DrawFormatString(0, 20, GetColor(255, 255, 255), "�X�N���[��������(��):%06d", ScrollCntYoko);

	return;
}

//�Q�[���}�b�v��CSV��ǂݍ��ފ֐�
BOOL MY_MAP_READ_CSV_NUM(FILE *fp,const char *path)
{
	if ((fp = fopen(path, "r")) == NULL)
	{
		return FALSE;
	}

	int result = 0;
	int LoopCnt = 0;
	while (result != EOF)
	{
		result = fscanf(fp, "%d,", &MapData[LoopCnt / MAP_YOKO][LoopCnt%MAP_YOKO]);

		MapData_Init[LoopCnt / MAP_YOKO][LoopCnt%MAP_YOKO] = MapData[LoopCnt / MAP_YOKO][LoopCnt%MAP_YOKO];//�l���R�s�[

		LoopCnt++;
	}

	fclose(fp);

	int cnt;

	for (tate = 0; tate < MAP_TATE; tate++)
	{
		for (yoko = 0; yoko < MAP_YOKO; yoko++)
		{
			for (cnt = 0; cnt < MAP_NOTDOWN_KIND; cnt++)
			{
				if (MapData[tate][yoko] == MapNotDownKind[cnt])
				{
					rectMap_DownNG[tate][yoko].left = yoko * MAP_OneBlock_YOKO_SIZE + 1;
					rectMap_DownNG[tate][yoko].top = tate * MAP_OneBlock_TATE_SIZE + 1;
					rectMap_DownNG[tate][yoko].right = (yoko + 1)*MAP_OneBlock_YOKO_SIZE - 1;
					rectMap_DownNG[tate][yoko].bottom = (tate + 1)*MAP_OneBlock_TATE_SIZE - 1;

					rectMap_DownNG_First[tate][yoko] = rectMap_DownNG[tate][yoko];
				}
			}

			for (cnt = 0; cnt < MAP_OKDOWN_KIND; cnt++)
			{
				if (MapData[tate][yoko] == MapOKDownKind[cnt])
				{
					rectMap_DownOK[tate][yoko].left = yoko * MAP_OneBlock_YOKO_SIZE + 1;
					rectMap_DownOK[tate][yoko].top = tate * MAP_OneBlock_TATE_SIZE + 1;
					rectMap_DownOK[tate][yoko].right = (yoko + 1)*MAP_OneBlock_YOKO_SIZE - 1;
					rectMap_DownOK[tate][yoko].bottom = (tate + 1)*MAP_OneBlock_TATE_SIZE - 1;

					rectMap_DownOK_First[tate][yoko] = rectMap_DownOK[tate][yoko];
				}
			}

			for (cnt = 0; cnt < MAP_NOTLEFT_KIND; cnt++)
			{
				if (MapData[tate][yoko] == MapNotLeftKind[cnt])
				{
					rectMap_LeftNG[tate][yoko].left = yoko * MAP_OneBlock_YOKO_SIZE + 1;
					rectMap_LeftNG[tate][yoko].top = tate * MAP_OneBlock_TATE_SIZE + 1;
					rectMap_LeftNG[tate][yoko].right = (yoko + 1)*MAP_OneBlock_YOKO_SIZE - 1;
					rectMap_LeftNG[tate][yoko].bottom = (tate + 1)*MAP_OneBlock_TATE_SIZE - 1;

					rectMap_LeftNG_First[tate][yoko] = rectMap_LeftNG[tate][yoko];
				}
			}

			for (cnt = 0; cnt < MAP_NOTRIGHT_KIND; cnt++)
			{
				if (MapData[tate][yoko] == MapNotRightKind[cnt])
				{
					rectMap_RightNG[tate][yoko].left = yoko * MAP_OneBlock_YOKO_SIZE + 1;
					rectMap_RightNG[tate][yoko].top = tate * MAP_OneBlock_TATE_SIZE + 1;
					rectMap_RightNG[tate][yoko].right = (yoko + 1)*MAP_OneBlock_YOKO_SIZE - 1;
					rectMap_RightNG[tate][yoko].bottom = (tate + 1)*MAP_OneBlock_TATE_SIZE - 1;

					rectMap_RightNG_First[tate][yoko] = rectMap_RightNG[tate][yoko];
				}
			}
			for (cnt = 0; cnt < MAP_NOTUP_KIND; cnt++)
			{
				if (MapData[tate][yoko] == MapNotUpKind[cnt])
				{
					rectMap_RightNG[tate][yoko].left = yoko * MAP_OneBlock_YOKO_SIZE + 1;
					rectMap_RightNG[tate][yoko].top = tate * MAP_OneBlock_TATE_SIZE + 1;
					rectMap_RightNG[tate][yoko].right = (yoko + 1)*MAP_OneBlock_YOKO_SIZE - 1;
					rectMap_RightNG[tate][yoko].bottom = (tate + 1)*MAP_OneBlock_TATE_SIZE - 1;

					rectMap_RightNG_First[tate][yoko] = rectMap_RightNG[tate][yoko];
				}
			}
			for (cnt = 0; cnt < MAP_NOTUP_KIND; cnt++)
			{
				if (MapData[tate][yoko] == MapNotUpKind[cnt])
				{
					rectMap_UpNG[tate][yoko].left = yoko * MAP_OneBlock_YOKO_SIZE + 1;
					rectMap_UpNG[tate][yoko].top = tate * MAP_OneBlock_TATE_SIZE + 1;
					rectMap_UpNG[tate][yoko].right = (yoko + 1)*MAP_OneBlock_YOKO_SIZE - 1;
					rectMap_UpNG[tate][yoko].bottom = (tate + 1)*MAP_OneBlock_TATE_SIZE - 1;

					rectMap_UpNG_First[tate][yoko] = rectMap_UpNG[tate][yoko];
				}
			}
		}
	}

	return TRUE;
}
//�v���C���[�̓����蔻��̗̈��ݒ肷��֐�
VOID MY_SET_PLAYER_ATARI(PLAYER *p)
{
	p->atariRect.left = p->X + p->atariX + 1;
	p->atariRect.top = p->Y + p->atariY + 1;
	p->atariRect.right = p->X + p->atariX + p->atariWidth - 1;
	p->atariRect.bottom = p->Y + p->atariY + p->atariHeight - 1;

	return;
}

//##########���@�̂����蔻�������֐�##########
BOOL MY_CHECK_RECT_ATARI(RECT a, RECT b)
{
	if (a.left < b.right &&
		a.top < b.bottom &&
		a.right > b.left &&
		a.bottom > b.top)
	{
		return TRUE;
	}

	return FALSE;
}

//�v���C���[�ƃ}�b�v�̓����蔻�������֐�
BOOL MY_CHECK_RECT_ATARI_CHARA_MAP(RECT chara, RECT map[MAP_TATE][MAP_YOKO])
{
	for (tate = 0; tate < MAP_TATE; tate++)
	{
		for (yoko = 0; yoko < MAP_YOKO; yoko++)
		{
			if (MY_CHECK_RECT_ATARI(chara, map[tate][yoko]) == TRUE)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL MY_CHECK_RECT_ATARI_CHARA_MAP_ATARIBASHO(RECT chara, RECT map[MAP_TATE][MAP_YOKO], int *atariX, int *atariY)
{
	for (tate = 0; tate < MAP_TATE; tate++)
	{
		for (yoko = 0; yoko < MAP_YOKO; yoko)
		{
			if (MY_CHECK_RECT_ATARI(chara, map[tate][yoko]) == TRUE)
			{
				*atariY = tate;
				*atariX = yoko;

				return TRUE;
			}
		}
	}

	return FALSE;
}


