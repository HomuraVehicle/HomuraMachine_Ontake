#ifndef HMR_SPRITEINTERFACE_INC
#define HMR_SPRITEINTERFACE_INC 106
#
/*
===hmrSpriteInteface===
SpriteJpegCmrの処理に必要な関数群をまとめたもの
hmrSpriteInterface:v1_05/130706 iwahori
	強制リセット後、通信タイムアウトエラーが出なくなるバグを修正.強制リセット後の初期化忘れ
hmrSpriteInterface:v1_02/130406 iwahori
	強制リセットのバグを修正，カメラからの受信ができないバグを修正，最小サイズの写真取得/連続取得に成功
hmrSpriteInterface:v1_02/130310 iwahori
	オートパワーリセット機能を追加，そのほか微修正
hmrSpriteInterface:v1_02/130302 iwahori
	規約の微修正に伴い、公開関数を修正、パワーリセット、エラー処理、タイムアウト機能を実装
	のこり：オートパワーリセット
	終端化関数で全部ゼロにしたりしているからパワーリセットで終端化関数をそのままつかわない方がいい
	_spriteitf_fnPowerResetでの初期化も要確認
	_spriteitf_timer_enable、_spriteitf_timer_interrupt_enableのdefineが同じ、その辺も要確認
hmrSpriteInterface:v1_01/130223 iwahori
	公開関数を作成
	のこり：エラー処理を公開関数などに実装、パワーリセット、オートリセット
	_spriteitf_informTakePic、_spriteitf_informReadPicDatはいつ登録する？
	StopTakePicするとデータ取得ができなくなるか要確認
hmrSpriteInterface:v1_00/130211 hmIto
	hmrSpriteFunctions_v1_05を継承
	関数群を整理
hmrSpriteFunctions:v1_05/121101 hmIto
	256byte以上のデータを一度に取得できるように対応
		dsize型、boolian型、imagetype型、boaudrate型など、意味に応じた型をtypedef
		関数ポインタの型名も変更
		使っていなかった関数ポインタも同時に整理
hmrSpriteFunctions:v1_04/121031 hmIto
	Intr_ini関数内でconst変数をいじっていたバグを修正
hmrSpriteFunctions:v1_03/121028 hmIto
	Stream名をsprite_StreamからStream_CMRに変更
	initialize関数はpower関数を制御しないように変更
		ただし、power_reset関連の関数群は利用しているので注意
	割り込み関数のdisable化をfinalizeで実行するよう変更
		enable化は、必要となった時点でsprite関数内で随時実行される
hmrSpriteFunctions:v1_02/121020 iwahori
	hmrSensorがsprite_getRequestImageSize()なる関数を必要としているのでとりあえず追加
hmrSpriteFunctions:v1_01/121014 iwahori
	sprite_ON()削除
	p_LED_YELLOWをsprite_setLEDに変更
hmrSpriteFunctions:v1_00/121013 hmIto
	hmrSpriteIntrを元に、Sprite関係の関数群のみ分離開始
	とりあえず完成
*/	
#ifndef HMR_DEVICECONFIG_INC
#	include "hmrDeviceConfig.h"
#endif
#ifndef HMCLIB_BYTEBASETYPE_INC
#	include "hmLib_v3_04/bytebase_type.h"
#endif
//【設定】通信途絶判断時間
#define spriteitf_RecvTimeOut 300	//ms(500まで)

//サイズ型の定義
typedef uint16 spriteitf_dsize_t;

//各ステータスの定義
typedef uint8 spriteitf_status_t;
#define spriteitf_Status_Idle				0x00
#define spriteitf_Status_SetImageSize		0x10
#define spriteitf_Status_ActiveImageSize	0x11
#define spriteitf_Status_TakePic			0x12
#define spriteitf_Status_ReadPicInfo		0x13
#define spriteitf_Status_ReadPicData		0x20
#define spriteitf_Status_EndTakePic			0x30
#define spriteitf_Status_CommandReset		0x40
#define spriteitf_Status_PowerReset			0xF0

//Error
typedef uint8 spriteitf_error_t;
//下位ビットマスク
#define spriteitf_error_LowBitMask									0xF0
//写真基礎データ受信処理中のエラー
#define spriteitf_error_TakePic										0x10
//写真基礎データ受信処理時、途中で変なステータスになった
#define spriteitf_error_TakePic_invalidStatus_Idle					0x11
#define spriteitf_error_TakePic_invalidStatus_ReadPicData			0x12
#define spriteitf_error_TakePic_invalidStatus_EndTakePic			0x13
#define spriteitf_error_TakePic_invalidStatus_CommandReset			0x14
#define spriteitf_error_TakePic_invalidStatus_PowerReset			0x15
//写真基礎データ受信処理時、カメラへのコマンド送信を拒否された
#define spriteitf_error_TakePic_cannotsend_SetImageSize				0x16
#define spriteitf_error_TakePic_cannotsend_ActiveImageSize			0x17
#define spriteitf_error_TakePic_cannotsend_TakePic					0x18
#define spriteitf_error_TakePic_cannotsend_ReadPicInfo				0x19

//写真データ読み込み処理中のエラー
#define spriteitf_error_ReadPicData									0x20
//写真データ読み込み処理時、途中で変なステータスになった
#define spriteitf_error_ReadPicData_invalidStatus_Idle				0x21
#define spriteitf_error_ReadPicData_invalidStatus_SetImageSize		0x22
#define spriteitf_error_ReadPicData_invalidStatus_ActiveImageSize	0x23
#define spriteitf_error_ReadPicData_invalidStatus_TakePic			0x24
#define spriteitf_error_ReadPicData_invalidStatus_ReadPicInfo		0x25
#define spriteitf_error_ReadPicData_invalidStatus_EndTakePic		0x26
#define spriteitf_error_ReadPicData_invalidStatus_CommandReset		0x27
#define spriteitf_error_ReadPicData_invalidStatus_PowerReset		0x28
//写真データ読み込み時処理時、カメラへのコマンド送信を拒否された
#define spriteitf_error_ReadPicData_cannotsend_ReadPicData			0x29

//撮影終了処理中のエラー
#define spriteitf_error_EndTakePic									0x30
//撮影終了処理時、途中で変なステータスになった
#define spriteitf_error_EndTakePic_invalidStatus_Idle				0x31
#define spriteitf_error_EndTakePic_invalidStatus_SetImageSize		0x32
#define spriteitf_error_EndTakePic_invalidStatus_ActiveImageSize	0x33
#define spriteitf_error_EndTakePic_invalidStatus_TakePic			0x34
#define spriteitf_error_EndTakePic_invalidStatus_ReadPicInfo		0x35
#define spriteitf_error_EndTakePic_invalidStatus_ReadPicData		0x36
#define spriteitf_error_EndTakePic_invalidStatus_CommandReset		0x37
#define spriteitf_error_EndTakePic_invalidStatus_PowerReset			0x38
//撮影終了処理時、カメラへのコマンド送信を拒否された
#define spriteitf_error_EndTakePic_cannotsend_EndPicData			0x39

//Command Reset中でのエラー
#define spriteitf_error_CommandReset								0x40
//コマンドリセット処理時、途中で変なステータスになった
#define spriteitf_error_CommandReset_invalidStatus_Idle				0x41
#define spriteitf_error_CommandReset_invalidStatus_SetImageSize		0x42
#define spriteitf_error_CommandReset_invalidStatus_ActiveImageSize	0x43
#define spriteitf_error_CommandReset_invalidStatus_TakePic			0x44
#define spriteitf_error_CommandReset_invalidStatus_ReadPicInfo		0x45
#define spriteitf_error_CommandReset_invalidStatus_ReadPicData		0x46
#define spriteitf_error_CommandReset_invalidStatus_EndTakePic		0x47
#define spriteitf_error_CommandReset_invalidStatus_PowerReset		0x48
//コマンドリセット処理時、カメラへのコマンド送信を拒否された
#define spriteitf_error_CommandReset_cannotsend_CommandReset		0x49

//カメラとの通信タイムアウト
#define spriteitf_error_TimeOut										0x50
#define spriteitf_error_TimeOut_SetImageSize						0x51
#define spriteitf_error_TimeOut_TakePic								0x52
#define spriteitf_error_TimeOut_GetDataSize							0x53
#define spriteitf_error_TimeOut_GetData								0x54
#define spriteitf_error_TimeOut_StopTakePic							0x55
#define spriteitf_error_TimeOut_CommandReset						0x56
#define spriteitf_error_TimeOut_unUsedCommand						0x57


//Picture指定用
typedef uint8 spriteitf_imagesize_t;
#define spriteitf_PictureSize_160_120 0x22//          0  
#define spriteitf_PictureSize_320_240 0x11//imagesize 1
#define spriteitf_PictureSize_640_480 0x00//          2 

//Baudrate指定用
typedef uint16 spriteitf_baudrate_t;
#define spriteitf_BaudRate_9600 0xAEC8
#define spriteitf_BaudRate_19200 0x56E4
#define spriteitf_BaudRate_38400 0x2AF2
#define spriteitf_BaudRate_57600 0x1C4C
#define spriteitf_BaudRate_115200 0x0DA6

//コマンド構造体
typedef struct{
	unsigned char* Ptr;			//送信するコマンド
	spriteitf_dsize_t Size;	//送信するコマンドのサイズ
	spriteitf_dsize_t InSize;	//受信するコマンドのサイズ
}spriteitf_command;

//割り込みコマンド関数ポインタ
typedef void (*spriteitf_vFp_v) (void);
//typedef void (*spriteitf_vFp_uc_uc)(unsigned char,unsigned char);
typedef boolian (*spriteitf_bFp_uc_dsize)(unsigned char,spriteitf_dsize_t);
typedef void (*spriteitf_vFp_ucp)(unsigned char*);
//typedef void (*spriteitf_vFp_ui)(unsigned int);
//typedef unsigned char*(*spriteitf_ucpFp_uc)(unsigned char);
//typedef void (*spriteitf_vFp_uc)(unsigned char);

//初期化関数
void spriteitf_initialize(void);
//終端化関数
void spriteitf_finalize(void);

//***Status 状態遷移変数定義***//
//===Status::public:
//Statusチェック関数
spriteitf_status_t spriteitf_getStatus(void);
//状態が待機状態中か？
boolian spriteitf_isIdle(void);

//***Data カメラデータ***//
//===Data::public:
//写真撮影開始要求 エラー時に戻り値1
boolian spriteitf_beginTakePic(spriteitf_imagesize_t ImageSize_,spriteitf_vFp_v fnInformTakePic);
//写真撮影終了要求 エラー時に戻り値1
boolian spriteitf_endTakePic(spriteitf_vFp_v fnInformEndTakePic);
//写真データ読み込み要求 エラー時に戻り値1 ,BufPtr:データ入れてほしい場所,size_ptr_:書き込み完了したデータ数入れる場所,begin:写真データの読み出し開始位置（8の倍数）,size:beginから読み出すデータ数
boolian spriteitf_beginReadPicDat(unsigned char* BufPtr_,spriteitf_dsize_t begin_,spriteitf_dsize_t* size_ptr_,spriteitf_dsize_t size_,spriteitf_vFp_v fnInformReadPicData);
//Request中のImageSizeを取得する関数
spriteitf_imagesize_t spriteitf_getRequestImageSize(void);
//写真全データサイズを要求可能か？
boolian spriteitf_can_getPicInfo(void);
//写真全データサイズ要求
spriteitf_dsize_t spriteitf_getAllPicDataSize(void);
//写真データ用バッファにデータはすべて溜まっているか？
boolian spriteitf_can_getPicData();
//写真データ用バッファサイズ要求
spriteitf_dsize_t spriteitf_sizePicData();
//写真基礎データ受信完了フラグをクリア
void spriteitf_clear_TakePicFlag(void);
//データ受信完了フラグをクリア
void spriteitf_clear_ReadPicDatFlag(void);
//写真撮っていいか？
boolian spriteitf_can_TakePic(void);
//写真読み込み済みデータ数取得
uint16 spriteitf_getReadDataNum(void);

//***Reset リセット処理***//
//===Reset::public:
//リセット要求関数　エラー時に戻り値1
boolian spriteitf_CommandReset(void);
//パワーリセット要求
boolian spriteitf_PowerReset(void);
//オートリセットモードの有無
boolian spriteitf_getAutoResetMode(void);
//オートリセットモードにする
void spriteitf_setAutoResetMode(boolian val);

//***Error エラー処理***//
//===Error::public:
//Error状態か確認
boolian spriteitf_isError(void);
//ErrorCode取得関数
spriteitf_error_t spriteitf_getErrorCode(void);
//ErrorCodeクリア
void spriteitf_clearErrorCode(void);
#
#endif
