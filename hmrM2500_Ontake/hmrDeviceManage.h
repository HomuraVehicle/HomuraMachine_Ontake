#ifndef HMR_DEVICEMANAGE_INC
#define HMR_DEVICEMANAGE_INC 103
#
#ifndef HMR_TYPE_INC
#	include"hmrType.h"
#endif
#ifndef HMR_TASK_INC
#	include"hmrTask.h"
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
v1_03/131026 amby
	割り込み許可関数軍を追加、このクラスで管理する
v1_02/131019 hmIto
	clockmodeをclockに変更
	isSleep, isRoamingを追加
v1_01/131013 hmIto
	インターフェースを大幅に変更＆整理
		モードからクロック速度指定を除外
*/
typedef void(*devmng_vFp_v)(void);

//=== 基本定義 ===
#define devmng_RF_BaudRate 9600
#define devmng_RFDebug_BaudRate 38400
#define devmng_MP_BaudRate 38400

//=== 基本操作関数 ===
//デバイス初期化関数
void devmng_initialize(void);
//デバイス終端化関数
void devmng_finalize(void);
//デバイス強制終了
void devmng_kill(void);
//現在の通信先が全二重通信対応かを返す
boolian devmng_isFullDuplex(void);

//=== モード制御機能 ===
typedef enum{NormalMode,SleepMode,RoamingMode}devmng_mode;
//デバイスモードを変更する
void devmng_mode_set(devmng_mode Mode_);
//デバイスモードを取得する
volatile devmng_mode devmng_mode_get(void);

//=== クロック制御機能 ===
typedef enum{LowClock,NormalClock,HighClock}devmng_clock;
//クロックを変更する
void devmng_clock_set(devmng_clock Clock_);
//クロックを取得する
volatile devmng_clock devmng_clock_get(void);

//=== タスク制御 ===
//タスクを新規登録できる確認する
boolian devmng_task_can_start();
//タスクを登録（二重登録は回避）
boolian devmng_task_start(sint16 Interval,task_function TaskFp,task_inform TaskInformStopFp);
//タスクを登録（二重登録できてしまうので、危険）
boolian devmng_task_quick_start(sint16 Interval,task_function TaskFp,task_inform TaskInformStopFp);
//タスクを設定変更
boolian devmng_task_restart(task_function TaskFp,sint16 Interval);
//タスクを削除
boolian devmng_task_stop(task_function TaskFp);

//=== スリープモード制御 ===
//スリープモードのパラメータを取得する
void devmng_sleep_getInterval(sint16* secNonRem,sint16* secRem);
//スリープモードのパラメータを設定する
void devmng_sleep_setInterval(sint16 secNonRem, sint16 secRem);
//入眠時＆起床時に呼び出される通知関数を登録できるか
boolian devmng_sleep_can_registSleepInformFunction();
//入眠時＆起床時に呼び出される通知関数を登録する
boolian devmng_sleep_registSleepInformFunction(devmng_vFp_v InformSleep,devmng_vFp_v InformWakeUp);
//レムスリープに入る/出る際の通知関数を登録できるか
boolian devmng_sleep_can_registRemInformFunction();
//レムスリープに入る/出る際の通知関数を登録する
boolian devmng_sleep_registRemInformFunction(devmng_vFp_v InformRem,devmng_vFp_v InformNonRem);
//スリープ状態かどうかを取得する
volatile boolian devmng_sleep_isSleep(void);
//レムスリープ状態かどうかを取得する
volatile boolian devmng_sleep_isRem(void);

//=== ローミングモード制御 ===
//ローミングモードのパラメータを取得する
void devmng_roaming_getInterval(sint16* secInterval);
//ローミングモードのパラメータを設定する
void devmng_roaming_setInterval(sint16 secInterval);
//ローミング状態かどうかを取得する
volatile boolian devmng_roaming_isRoaming(void);

//=== 割り込み関数 ===
//タスク、スリープ、およびローミング実行用関数
void devmng_interrupt_function(sint16 Interval_);
//割り込みのmainループへの委譲処理関数
boolian devmng_interrupt_yield(void);

//=== 割り込み制御用関数
// 割り込み許可変えてよいかどうかのチェック関数　冗長過ぎたかも・・
boolian devmng_interrupt_can_enable_streamVMC_fget_interrupt(void);
boolian devmng_interrupt_can_disable_streamVMC_fget_interrupt(void);
boolian devmng_interrupt_can_enable_streamVMC_fput_interrupt(void);
boolian devmng_interrupt_can_disable_streamVMC_fput_interrupt(void);

boolian devmng_interrupt_can_enable_streamCMR_fget_interrupt(void);
boolian devmng_interrupt_can_disable_streamCMR_fget_interrupt(void);
boolian devmng_interrupt_can_enable_streamCMR_fput_interrupt(void);
boolian devmng_interrupt_can_disable_streamCMR_fput_interrupt(void);

boolian devmng_interrupt_can_enable_timerCMR_interrupt(void);
boolian devmng_interrupt_can_disable_timerCMR_interrupt(void);
boolian devmng_interrupt_can_enable_timerDevmng_interrupt(void);
boolian devmng_interrupt_can_disable_timerDevmng_interrupt(void);

// main stream 送受信割り込み許可関数
boolian devmng_interrupt_enable_streamVMC_fget_interrupt(void);
boolian devmng_interrupt_disable_streamVMC_fget_interrupt(void);
boolian devmng_interrupt_enable_streamVMC_fput_interrupt(void);
boolian devmng_interrupt_disable_streamVMC_fput_interrupt(void);

// camera stream 送受信割り込み許可関数
boolian devmng_interrupt_enable_streamCMR_fget_interrupt(void);
boolian devmng_interrupt_disable_streamCMR_fget_interrupt(void);
boolian devmng_interrupt_enable_streamCMR_fput_interrupt(void);
boolian devmng_interrupt_disable_streamCMR_fput_interrupt(void);

// timer CMR　送受信割り込み許可関数
boolian devmng_interrupt_enable_timerCMR_interrupt(void);
boolian devmng_interrupt_disable_timerCMR_interrupt(void);
// timer Device Manager 送受信割り込み許可関数
boolian devmng_interrupt_enable_timerDevmng_interrupt(void);
boolian devmng_interrupt_disable_timerDevmng_interrupt(void);

#
#endif
