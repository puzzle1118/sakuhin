//########## ヘッダーファイル読み込み ##########
#include "DxLib.h"
#include<stdio.h>
#include <dos.h>
#include<math.h>

//########## マクロ定義 ##########
#define GAME_WIDTH	800		//画面の横の大きさ
#define GAME_HEIGHT	640		//画面の縦の大きさ
#define GAME_COLOR	32		//画面のカラービット

#define GAME_WINDOW_NAME	"Devil Device RTA"		//ウィンドウのタイトル
#define GAME_WINDOW_MODECHANGE	TRUE			//TRUE：ウィンドウモード / FALSE：フルスクリーン

#define SET_WINDOW_ST_MODE_DEFAULT			0	//デフォルト
#define SET_WINDOW_ST_MODE_TITLE_NONE		1	//タイトルバーなし
#define SET_WINDOW_ST_MODE_TITLE_FLAME_NONE	2	//タイトルバーとフレームなし
#define SET_WINDOW_ST_MODE_FLAME_NONE		3	//フレームなし

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

/* マップによる移動の制限数 */
#define MAP_NOTDOWN_KIND    5
#define MAP_OKDOWN_KIND     1
#define MAP_NOTUP_KIND      5
#define MAP_NOTLEFT_KIND    5
#define MAP_NOTRIGHT_KIND   5
#define OFF_HIT 1
#define ON_HIT  1

#define GAME_MAP_GOAL_KIND      1  //ゴールの種類

#define GAME_CHARA_SPEED      2//キャラのスピード

#define GRAVITY    9.8067 //重力加速度
#define CHARA_WEIGHT    60//キャラの重さ

#define GAME_PLAYER_ZANKI_MAX  3

//########## 列挙型 ##########
enum GAME_SCENE {
	GAME_SCENE_TITLE,	//タイトル画面
	GAME_SCENE_SOUSA,
	GAME_SCENE_ZANKI,
	GAME_SCENE_PLAY,	//プレイ画面
	GAME_SCENE_END		//エンド画面
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

//############構造体##############
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

//########## グローバル変数 ##########

//typedef STRUCT_GAZOU GAZOU;
//typedef STRUCT_CHARACTOR CHARA;
//typedef STRUCT_MAP MAP;
//typedef STRUCT_PLAYER PLAYER;

//ウィンドウ関係
WNDPROC WndProc;						//ウィンドウプロシージャのアドレス
BOOL IsWM_CREATE = FALSE;				//WM_CREATEが正常に動作したか判断する

//FPS関連
int StartTimeFps;						//測定開始時刻
int CountFps;							//カウンタ
float CalcFps;							//計算結果
int SampleNumFps = FPS_SPEED;		//平均を取るサンプル数

//キーボード関連
char AllKeyState[256];			//すべてのキーの状態が入る
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

int MapData[MAP_TATE][MAP_YOKO];//マップのデータ

int MapData_Init[MAP_TATE][MAP_YOKO];//マップのデータ初期化用

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


//########## プロトタイプ宣言 ##########
LRESULT CALLBACK MY_WNDPROC(HWND, UINT, WPARAM, LPARAM);	//自作ウィンドウプロシージャ

VOID MY_ALL_KEYDOWN_UPDATE(VOID);	//キーの入力状態を更新する関数

VOID MY_FPS_UPDATE(VOID);			//FPS値を計測、更新する関数
VOID MY_FPS_DRAW(VOID);				//FPS値を描画する関数

VOID MY_GAME_TITLE(VOID);			//タイトル画面の関数
VOID MY_GAME_SOUSA(VOID);           //操作説明画面の関数
VOID MY_GAME_PLAY(VOID);			//プレイ画面の関数
VOID MY_GAME_END(VOID);				//エンド画面の関数

BOOL MY_GAZOU_LOAD(GAZOU *, int, int, const char *);


BOOL MY_PLAY_INIT(VOID);
VOID MY_PLAY_BACKIMAGE_DRAW(VOID);
VOID MY_PLAY_SCROLL(VOID);

BOOL MY_MAP_READ_CSV_NUM(FILE *, const char *);

BOOL MY_CHARA_LOAD_BUNKATSU(CHARA *, int, int, int, int, int, const char *);
BOOL MY_INIT_PLAYER(PLAYER *, CHARA, int *, int, int, int);

VOID MY_FPS_WAIT(VOID);				//FPS値を計測し、待つ関数
VOID MY_PLAY_PLAYER_DRAW(VOID);
VOID MY_PLAYER_OPERATION(VOID);

VOID MY_PLAY_MAP_DRAW(VOID);//マップを描画する関数

VOID MY_PLAY_PLAYER_JUMP_PREV(VOID);//ジャンプの前処理を行う関数

BOOL MY_PLAY_CHECK_GAME_END(VOID);//ゲームエンドか判定する関数

VOID MY_DRAW_STRING_CENTER_CENTER(char[][128], int, char *, int);//画面中央に文字を描画する関数
VOID MY_DRAW_SET_DEFAULT_FONT(BOOL);//文字をデフォルトフォントに設定する関数

VOID MY_BG_IMAGE_INIT(VOID);

/* 当たり判定の関数 */
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
//########## プログラムで最初に実行される関数 ##########
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(GAME_WINDOW_MODECHANGE);					//ウィンドウモードに設定
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);			//指定の数値で画面を表示する
	SetWindowStyleMode(SET_WINDOW_ST_MODE_TITLE_NONE);			//タイトルバーなし
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));					//タイトルの文字

	//フック→WM_CLOSEなどのメッセージを引っ掛けて取得する
	SetHookWinProc(MY_WNDPROC);	//ウィンドウプロシージャの設定

	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1) { return -1; }

	//ＤＸライブラリ初期化処理

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

	//無限ループ
	while (TRUE)
	{

		if (ProcessMessage() != 0) { break; }	//メッセージ処理の結果がエラーのとき、強制終了

		if (ClearDrawScreen() != 0) { break; }	//画面を消去できなかったとき、強制終了

		MY_ALL_KEYDOWN_UPDATE();				//キーの状態を取得

		//シーン選択
		switch (GameSceneNow)
		{
		case (int)GAME_SCENE_TITLE:	//タイトル画面の処理ここから

			MY_GAME_TITLE();	//タイトル画面の処理

			break;	//タイトル画面の処理ここまで


		case (int)GAME_SCENE_SOUSA:
			MY_GAME_SOUSA();

			break;

		case (int)GAME_SCENE_ZANKI://残機表示画面の処理ここから

			MY_GAME_ZANKI();//残機表示画面の処理

			break;

		case (int)GAME_SCENE_PLAY:	//プレイ画面の処理ここから

			MY_GAME_PLAY();		//プレイ画面の処理

			break;	//プレイ画面の処理ここまで

		case (int)GAME_SCENE_END:	//エンド画面の処理ここから

			MY_GAME_END();		//エンド画面の処理

			break;	//エンド画面の処理ここまで

		default:

			break;
		}
	
		MY_FPS_UPDATE();						//FPSの処理[更新]
		//DrawGraph(BackImage.X, BackImage.Y, BackImage.Handle, TRUE);
		// 背景の画像を描画

		MY_FPS_DRAW();		//FPSの処理[描画]

		
		 ScreenFlip();		
		 //モニタのリフレッシュレートの速さで裏画面を再描画
		  
		MY_FPS_WAIT();		//FPSの処理[待つ]
	}
	for (mapCnt = 0; mapCnt <MAP_YOKO*MAP_TATE; mapCnt++)
	{
		DeleteGraph(MapImage.Handle[mapCnt]);
	}

	for (charaCnt = 0; charaCnt < CHARA_BUN_YOKO_CNT*CHARA_BUN_TATE_CNT; charaCnt++)
	{
		DeleteGraph(CharaImage.Handle[charaCnt]);
	}
	

	DxLib_End();		//ＤＸライブラリ使用の終了処理

	return 0;
}

//########## FPS値を計測、更新する関数 ##########
VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0) //1フレーム目なら時刻を記憶
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == SampleNumFps) //60フレーム目なら平均を計算
	{
		int now = GetNowCount();
		CalcFps = 1000.f / ((now - StartTimeFps) / (float)SampleNumFps);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}

//########## FPS値を計測し、待つ関数 ##########
VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;					//かかった時間
	int waitTime = CountFps * 1000 / FPS_SPEED - resultTime;	//待つべき時間

	if (waitTime > 0)		//指定のFPS値よりも早い場合
	{
		Sleep(waitTime);	//待つ
	}
	return;
}

VOID MY_FPS_DRAW(VOID)
{
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255), "FPS:%.1f", CalcFps);

	return;
}

//########## キーの入力状態を更新する関数 ##########
VOID MY_ALL_KEYDOWN_UPDATE(VOID)
{
	char TempKey[256];

	GetHitKeyStateAll(TempKey);   //ずべてのキー入力状態を取得する

	for (i = 0; i < 256; i++)
	{
		if (TempKey[i] != 0)
		{
			AllKeyState[i]++;    //押されている
		}
		else
		{
			AllKeyState[i] = 0;  //押されていない
		}
	}
	return;
}

//########## タイトル画面の関数 ##########
VOID MY_GAME_TITLE(VOID)
{
	MY_TITLE_BACKGROUND_DRAW();

	if (AllKeyState[KEY_INPUT_RETURN] != 0)
	{
		if (MY_PLAY_INIT() == TRUE)		//プレイ画面初期化
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
	if (AllKeyState[KEY_INPUT_RETURN] != 0)	//エンターキーが押されていた時
	{
		GameSceneNow = (int)GAME_SCENE_SOUSA;	//シーンを操作説明画面にする
	}
	//DrawString(310, 270, "ＧＭＤ", GetColor(255, 255, 255), SetFontSize(60));
	DrawString(190, 450, "エンターキーを押してください", GetColor(255, 255, 0), SetFontSize(30));

	return;
}

//##########操作説明画面##########
VOID MY_GAME_SOUSA(VOID)
{
	if (AllKeyState[KEY_INPUT_SPACE] != 0)	//エンターキーが押されていた時
	{
		GameSceneNow = (int)GAME_SCENE_PLAY;	//シーンをプレイ画面にする
	}

	DrawString(190, 330, "スペースキーを押してください", GetColor(255, 255, 255), SetFontSize(30));
}
//############残機表示画面の関数############
VOID MY_GAME_ZANKI(VOID)
{
	//６０フレーム分待つ
	if (ZankiDrawCount < ZankiDrawCountMax)
	{
		ZankiDrawCount++;
	}
	else
	{
		GameSceneNow = (int)GAME_SCENE_PLAY;//シーンを残機表示画面にする
	}

	MY_PLAY_BACKIMAGE_DRAW();//背景を描画

	//透明にする数値(0が透過なし、255が透過する)
	double ToukaPercent = (ZankiDrawCountMax - ZankiDrawCount) / (double)ZankiDrawCountMax;//透過％を求める

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, ToukaPercent * 255);//透過させる

	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 200, 200), TRUE);//真っ暗な画面にする

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//透過をやめる

	char StrGameZanki[1][128] = { "\0" };
	char StrFontZanki[128] = { "MS ゴシック" };//大文字半角[MS],半角「空白」、全角「ゴシック」

	sprintf(&StrGameZanki[0][0], "プレイヤー×%02d", Myplayer.Zanki);//プレイヤー残機の表示を整形

	MY_DRAW_STRING_CENTER_CENTER(&StrGameZanki[0], 1, StrFontZanki, 64);

	DrawString(0, 0, "残機表示画面", GetColor(255, 255, 255));

	return;
}


//########## プレイ画面の関数 ##########
VOID MY_GAME_PLAY(VOID)
{

	if (AllKeyState[KEY_INPUT_SPACE] != 0)
	{
		//GameSceneNow = (int)GAME_SCENE_END;
	}

	if (AllKeyState[KEY_INPUT_LEFT] != 0)
	{

		if (Myplayer.CanMoveLeft == TRUE)//プレイヤーが左に動けるとき
		{
			if (MapImage.charaStopFlag == TRUE)//プレイヤーを動かせないならば
			{
				if (ScrollCntYoko > 0)//マイナスのスクロールをしない
				{
					ScrollCntYoko -= Myplayer.Speed;//画面を左にスクロール
				}
			}
		}

	}
	if (AllKeyState[KEY_INPUT_RIGHT] != 0)
	{
		if (Myplayer.CanMoveRight == TRUE)//プレイヤーが右に動けるとき
		{
			if (MapImage.charaStopFlag == TRUE)//プレイヤーを動かせないならば
			{
				if (ScrollCntYoko < MAP_OneBlock_YOKO_SIZE * MAP_YOKO)//画面外へスクロールをしない
				{
					ScrollCntYoko += Myplayer.Speed;//画面を右にスクロール
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
		MY_PLAYER_OPERATION();	//プレイヤーの操作
	}

	//MY_PLAYER_OPERATION();

	MY_PLAY_BACKIMAGE_DRAW();	//背景を描画

	MY_PLAY_MAP_DRAW();  //マップを描画

	MY_PLAY_PLAYER_DRAW();

	DrawString(0, 0, "エンターキーを押すと移動", GetColor(255, 255, 255));
	//DrawFormatString(20, 20, GetColor(255, 255, 255), "%d", playerX);

	return;
}

//###########画面中央に文字を描画する関数############
VOID MY_DRAW_STRING_CENTER_CENTER(char str[][128], int row, char *fontname, int size)
{
	ChangeFont(fontname, DX_CHARSET_DEFAULT);//デフォルトのフォントを変更する
	SetFontSize(size);                       //デフォルトのフォントサイズを変える
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);//フォントをきれいにみせる

	int StrHeightSum = 0;
	for (sum = 0; sum < row; sum++)//行全体の高さを集計
	{
		StrHeightSum += GetFontSize();//デフォルトのフォントの高さを取得
	}

	for (rCnt = 0; rCnt < row; rCnt++)//フォントを描画
	{
		int StrWidth = GetDrawFormatStringWidth(&str[rCnt][0]);//デフォルトのフォントの横幅を取得
		int StrHeight = GetFontSize();

		DrawFormatString(
			GAME_WIDTH / 2 - StrWidth / 2,
			GAME_HEIGHT / 2 - StrHeightSum + StrHeight * rCnt,//画面の中心　-　行全体の文字の高さ　+　１行の高さ
			GetColor(255, 255, 255), &str[rCnt][0]);//デフォルトのフォントで描画
	}

	MY_DRAW_SET_DEFAULT_FONT(TRUE);//デフォルトフォントに設定する
}



//########## ゲームエンドか判定する関数 ##########
BOOL MY_PLAY_CHECK_GAME_END(VOID)
{
	//プレイヤーの位置は、プレイヤーの中心
	int PlayerToMapNumY = (Myplayer.Y + Myplayer.C_Height) / MAP_OneBlock_TATE_SIZE;
	int PlayerToMapNumX = (Myplayer.MoveDist + Myplayer.C_Width) /MAP_OneBlock_TATE_SIZE;


	//プレイヤー画面外に落ちたとき
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
		//プレイヤーがゴールのものと接触したとき
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

	return FALSE;//ゲーム続行

}


//########## エンド画面の関数 ##########
VOID MY_GAME_END(VOID)
{
	if (AllKeyState[KEY_INPUT_BACK] != 0)	//バックスペースーキーが押されていた時
	{
		GameSceneNow = (int)GAME_SCENE_TITLE;	//シーンをタイトル画面にする
	}
	DrawString(210, 300, "ＧＡＭＥ　ＯＶＥＲ", GetColor(255, 0, 0) ,SetFontSize(40));

	DrawString(150, 350, "タイトルに戻るにはバックスペースキーを押してください", GetColor(255, 255, 255), SetFontSize(18));

	DrawString(210, 368, "終了したい場合は右クリックをしてください", GetColor(255, 255, 255), SetFontSize(18));

	return;
}

//文字をデフォルトフォントに設定する関数
VOID MY_DRAW_SET_DEFAULT_FONT(BOOL anti)
{
	ChangeFont("MS ゴシック", DX_CHARSET_DEFAULT);
	SetFontSize(16);

	if (anti)
	{
		ChangeFontType(DX_FONTTYPE_ANTIALIASING);
	}
	return;
}

//########## ウィンドウプロシージャ関数 ##########
LRESULT CALLBACK MY_WNDPROC(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	int closeMsgRet = 0;

	switch (msg)
	{
	case WM_CREATE:	//ウィンドウの生成＆初期化


		IsWM_CREATE = TRUE;	//WM_CREATE正常終了
		return 0;

	case WM_CLOSE:		//閉じるボタンを押したとき

		message=MessageBox(hwnd, TEXT("ゲームを終了します"), TEXT("終了メッセージ"), MB_OK/*YESNO*/);

	/*	if (message == IDYES)
		{
			return 0;
		}
		else
		{
			DefWindowProc(hwnd, msg, wp, lp);
		}*/

		break;

	case WM_RBUTTONDOWN:	//マウスの右ボタンを押したとき

		closeMsgRet = MessageBox(hwnd, TEXT("ゲームを終了しますか"), TEXT("終了メッセージ"), MB_YESNO);

		if (closeMsgRet == IDYES)
		{
			SendMessage(hwnd, WM_CLOSE, 0, 0);		//WM_CLOSEメッセージをキューに追加
		}
		else if (closeMsgRet == IDNO)
		{
			//break;
		}

		break;

	case WM_LBUTTONDOWN:	//マウスの左ボタンを押したとき

		//WM_NCLBUTTONDOWN(タイトルバーでマウスの左ボタンを押した)メッセージをすぐに発行
		//PostMessage(hwnd, WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, lp);
		break;

	case WM_DESTROY:	//ウィンドウが破棄された(なくなった)とき

			PostQuitMessage(0);		//メッセージキューに WM_QUIT を送る

			return 0;
	}

	//デフォルトのウィンドウプロシージャ関数を呼び出す
	return DefWindowProc(hwnd, msg, wp, lp);
}

//初期化の関数
BOOL MY_PLAY_INIT(VOID)
{
	if (MY_INIT_PLAYER(&Myplayer, CharaImage, &PlayerImageNum[0], MAP_OneBlock_YOKO_SIZE * 0, MAP_OneBlock_TATE_SIZE * 0, GAME_CHARA_SPEED) == FALSE) { MessageBox(NULL, "CHARA_INIT", "NotFound", MB_OK); return FALSE; }

	//MapImage.GoalFlg = FALSE;//ゴール状態を初期化

	//MapImage.GameEndKind = (int)END_KIND_CONTINUE;

	//MapImage.GoalEffect1 = FALSE;

	//プレイヤーの初期化


	MY_DRAW_SET_DEFAULT_FONT(TRUE);	//デフォルトフォントに設定する

	MY_BG_IMAGE_INIT();//背景の位置を初期化

	ScrollCntYoko = 0;    //スクロール量を初期化

	ZankiDrawCount = 0;//残機表示カウンタ初期化
	UntilZankiDrawCount = 0;//残機表示までのカウンタ初期化

	return TRUE;

}

VOID MY_BG_IMAGE_INIT(VOID)
{
	//背景の位置
	BackImage[0].X = 0;

	return;
}

//プレイ画面の背景を描画する関数
VOID MY_PLAY_BACKIMAGE_DRAW(VOID)
{
	DrawGraph(BACKGROUND.X, BACKGROUND.Y, BACKGROUND.Handle, TRUE);
}

//タイトル画面の背景を描画する関数
VOID MY_TITLE_BACKGROUND_DRAW(VOID)
{
	DrawGraph(BACK_TITLE.X, BACK_TITLE.Y, BACK_TITLE.Handle, TRUE);
}

//画像を読み込む設定をする関数
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

//マップを分割して読み込む設定をする関数
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

//キャラを分割して読み込む関数
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



//背景を読み込む関数
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

//プレイヤーを初期化する関数
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

//プレイヤーを操作する関数
VOID MY_PLAYER_OPERATION(VOID)
{
	BOOL IsKeyDown = FALSE;

	BOOL CanMoveLeft = TRUE;//左に行けるか
	BOOL CanMoveRight = TRUE;//右に行けるか

	//プレイヤーの位置がマップ配列のどこにいるか変換
	int PlayerToMapNumY;
	//プレイヤーの【移動した位置】がマップ配列のどこにいるか変換	※プレイヤーの位置は、スクロール時、変わらないため
	//キャラの位置を、１マップの半分の位置にする
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
		//左方向に、まだ動ける
		Myplayer.CanMoveLeft = TRUE;

		MY_SET_PLAYER_ATARI(&Myplayer);
		Myplayer.atariRect.left -= 4;
		Myplayer.atariRect.right -= 4;

		if (MY_CHECK_RECT_ATARI_CHARA_MAP(Myplayer.atariRect, rectMap_LeftNG) == TRUE)
		{
			Myplayer.CanMoveLeft = FALSE;
		}

		if (Myplayer.CanMoveLeft == TRUE)//左に移動できるとき
		{
			if (MapImage.charaStopFlag == FALSE)
			{
				if (Myplayer.X > 0)
				{
					Myplayer.X -= Myplayer.Speed;//プレイヤーを左に移動
				}
			}
			if (Myplayer.MoveDist > 0)
			{
				Myplayer.MoveDist -= Myplayer.Speed;//動いた距離を計算
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

		if (Myplayer.CanMoveRight == TRUE)//右に移動できるとき
		{
			if (MapImage.charaStopFlag == FALSE)
			{
				if (Myplayer.X + Myplayer.Width < GAME_WIDTH)
				{
					Myplayer.X += Myplayer.Speed;//プレイヤーを右に移動
				}
			}
			if (Myplayer.MoveDist < MAP_OneBlock_YOKO_SIZE * MAP_YOKO)
			{
				Myplayer.MoveDist += Myplayer.Speed;//動いた距離を計算
			}
		}
	}

	//マップの左側にいるときは、プレイヤーを動かす
	if (Myplayer.MoveDist > 0 &&
		Myplayer.MoveDist <= MAP_OneBlock_YOKO_SIZE * 10)
	{
		MapImage.charaStopFlag = FALSE;
	}

	//マップ真ん中らへんにいるときは、マップを動かす
	if (Myplayer.MoveDist > MAP_OneBlock_YOKO_SIZE * 10 &&
		Myplayer.MoveDist <= MAP_OneBlock_YOKO_SIZE * 119)
	{
		MapImage.charaStopFlag = TRUE;
	}

	//マップの右側にいるときは、プレイヤーを動かす
	if (Myplayer.MoveDist > MAP_OneBlock_YOKO_SIZE * 119 &&
		Myplayer.MoveDist <= MAP_OneBlock_YOKO_SIZE * MAP_YOKO)
	{
		MapImage.charaStopFlag = FALSE;
	}

	//キーが押されていないとき
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
		Myplayer.NowHandleCnt = Myplayer.NowHandleCntMAX - 1;	//すぐ画像を変えられるようにする
	}

	//参考はこちら
	//https://dixq.net/g/30.html

	if (AllKeyState[KEY_INPUT_UP] != 0)//上矢印キーが押されていた時
	{
		if (Myplayer.IsJumpNow == FALSE)//ジャンプしていないとき
		{
			MY_PLAY_PLAYER_JUMP_PREV();

			//PlaySoundMem(MUSIC_MY_SE_JUMP.Handle, DX_PLAYTYPE_BACK);//ジャンプした効果音を鳴らす	

		}
	}

	if (Myplayer.IsJumpNow == TRUE)//ジャンプしているとき
	{
		Myplayer.JumpNowTime = GetNowCount();//現在の時刻を取得

		//上矢印キーを押して、ジャンプをしたときからの経過時間を取得
		double KeikaTime = (Myplayer.JumpNowTime - Myplayer.JumpStartTime) / 1000.0;

		//ジャンプ
		//等加速直線運動の公式を使用する＋加速度が負のときは、放物線運動することを利用して・・・
		//参考サイト：http://www.wakariyasui.sakura.ne.jp/p/mech/henni/toukasokudo.html#matome
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

	if (Myplayer.IsJumpNow == TRUE)//ジャンプ中
	{
		if (Myplayer.Y <= Myplayer.JumpMaxTop + 1)//マイナスの数値のときを考慮し、0の分を、+1
		{
			Myplayer.IsJumpTop = TRUE;//ジャンプで頂点に到達した
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
	//if (Myplayer.IsJumpNow == TRUE && Myplayer.IsJumpTop == TRUE)//頂点に到達したら、あとは落ちるのみ
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

	//マップの当たり判定もスクロールさせる
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

//ジャンプの前処理を行う関数
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

//プレイヤーを表示する関数
VOID MY_PLAY_PLAYER_DRAW(VOID)
{
	DrawExtendGraph(
		Myplayer.X,
		Myplayer.Y,
		Myplayer.X + Myplayer.Width,
		Myplayer.Y + Myplayer.Height,
		Myplayer.Handle[Myplayer.NowHandleNum], TRUE);

	//ジャンプ中
	if (Myplayer.IsJumpNow == TRUE)
	{
		DrawString(0, 120, "ジャンプフラグON", GetColor(255, 255, 255));
	}

	//頂点に達したとき
	if (Myplayer.IsJumpTop == TRUE)
	{
		DrawString(0, 140, "頂点フラグON", GetColor(255, 255, 255));
	}

	return;
}

//マップを表示する関数
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

//ゲーム画面のスクロールを行う関数
VOID MY_PLAY_SCROLL(VOID)
{
	ScrollCntYoko += ScrollDistPlusYoko;
	DrawFormatString(0, 20, GetColor(255, 255, 255), "スクロールした量(横):%06d", ScrollCntYoko);

	return;
}

//ゲームマップのCSVを読み込む関数
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

		MapData_Init[LoopCnt / MAP_YOKO][LoopCnt%MAP_YOKO] = MapData[LoopCnt / MAP_YOKO][LoopCnt%MAP_YOKO];//値をコピー

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
//プレイヤーの当たり判定の領域を設定する関数
VOID MY_SET_PLAYER_ATARI(PLAYER *p)
{
	p->atariRect.left = p->X + p->atariX + 1;
	p->atariRect.top = p->Y + p->atariY + 1;
	p->atariRect.right = p->X + p->atariX + p->atariWidth - 1;
	p->atariRect.bottom = p->Y + p->atariY + p->atariHeight - 1;

	return;
}

//##########両機のあたり判定をする関数##########
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

//プレイヤーとマップの当たり判定をする関数
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


