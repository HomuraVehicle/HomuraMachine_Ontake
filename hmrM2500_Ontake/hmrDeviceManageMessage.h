#ifndef HMR_DEVICEMANAGEMESSAGE_INC
#define HMR_DEVICEMANAGEMESSAGE_INC 102

/*
===hmrDeviceManage===

v1_02/131019 amby
	メッセージ関数の追加 その他の機能はインターフェースへ…

v1_01/131013 hmIto
	インターフェースを大幅に変更＆整理
		モードからクロック速度指定を除外
*/

#ifndef HMR_TYPE_INC
#	include"hmrType.h"
#endif
#ifndef HMR_DEVICECONFIG_INC
#	include"hmrDevice.h"
#endif
#ifndef HMR_DEVICEMANAGE_INC
#	include"hmrDeviceManage.h"
#endif
#ifndef HMR_TASK_INC
#	include"hmrTask.h"
#endif
#include"hmLib_v3_04/bytebase_type.h"
#include"hmLib_v3_04/cstring.h"

#ifdef __cplusplus
using hmLib::cstring;
#endif

//void devmngmsg_initialize(void);
//void devmngmsg_finalize(void);

hmLib_boolian devmngmsg_listen(cstring Str);
hmLib_boolian devmngmsg_talk(cstring* pStr);
void devmngmsg_setup_listen(void);
void devmngmsg_setup_talk(void);

sint16 devmngmsg_task_inform(sint16 interval);
#
#endif


/*
Device制御用の機能を提供する
	Kill機能
		自分自身とすべてのの電源を切る
	スリープ機能
		定期的に電源をオン/オフを繰り返す
	ローミング機能
		Roamingモードに入ると、RF(RoamingTime)=>MB(RoamingTime)=>RFのように、使用通信機の遷移を繰り返す。
		RoamingTimeは内部変数で決定する（コマンドで設定可能）
	クロックモード変更機能
		設定クロック速度を変更する

===hmrDeviceManage===
v1_01/131013 hmIto
	インターフェースを大幅に変更＆整理
		モードからクロック速度指定を除外
*/

/*
typedef void(*devmng_vFp_v)(void);
//=== 基本操作関数 ===
//デバイス初期化関数
void devmng_initialize(void);
//デバイス終端化関数
void devmng_finalize(void);
//デバイス強制終了
void devmng_kill(void);

//=== モード制御機能 ===
typedef enum{NormalMode,SleepMode,RoamingMode}devmng_mode;
//デバイスモードを変更する
void devmng_mode_set(devmng_mode Mode_);
//デバイスモードを取得する
devmng_mode devmng_mode_get(void);

//=== クロックモード制御機能 ===
typedef enum{LowClockMode,NormalClockMode,HighClockMode}devmng_clockmode;
//クロックモードを変更する
void devmng_clockmode_set(devmng_clockmode ClockMode_);
//クロックモードを取得する
devmng_clockmode devmng_clockmode_get(void);

//=== スリープモード制御 ===
//スリープモードのパラメータを取得する
void devmng_sleep_getInterval(uint16& secNonRem,uint16& secRem);
//スリープモードのパラメータを設定する
void devmng_sleep_setInterval(uint16 secNonRem, uint16 secRem);
//スリープに入る/出る際の通知関数を登録する
void devmng_sleep_registSleepInformFunction(devmng_vFp_v InformSleep,devmng_vFp_v InformWakeUp);
//レムスリープに入る/出る際の通知関数を登録する
void devmng_sleep_registRemInformFunction(devmng_vFp_v InformRem,devmng_vFp_v InformNonRem);

//=== ローミングモード制御 ===
//ローミングモードのパラメータを取得する
void devmng_roaming_getInterval(uint16& secInterval);
//ローミングモードのパラメータを設定する
void devmng_roaming_setInterval(uint16 secInterval);
#
#endif
*/