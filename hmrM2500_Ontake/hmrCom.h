#ifndef HMR_COM_INC
#define HMR_COM_INC 205
#
/*
===hmrCom===
送受信バッファについて取り仕切る
	@コマンドがパケットの区切り
		パケットを受信し終わると、自動的に受信バッファに@コマンドデータがpushされる
		送信時にパケット区切りには@コマンドデータを送信バッファにpushしておくとパケットが閉じられる

v2_05/131102 hmIto
	全二重/半二重に応じて、常にデータを送る/受信済みの数だけデータを送る、を変更
v2_04/131026 hmIto
	com用WatchDogTimer機能を提供
		com_ini_recvPac/sendPacでrestartされる。
		com_wdt_sec以上の時間送受信がなく、かつrestartも呼ばれないと、killする。
		機能を有効にするには、com_wdt_interrupt_functionをtaskに登録する必要がある。
v2_03/130928 hmIto
	iBuf/oBuf_move_pop/pushにおいて、empty/fullであってもつかえてしまう問題を修正
	odata::informDestruct機能の処理をodata::destructに委譲
v2_02/130803 hmIto
	inform_destruct機能を利用している際に，まれにinformされない問題を修正
		エラー発生時にfinSendPacが呼ばれるが，その際にdestructはされるがinformは実行されていなかった．
		同様の問題が，FailPacが存在する条件下におけるmove_pushにおいても存在していた．
v2_00/121201 hmIto
	odata/iQue_dataがcstringを使うよう変更
	odata型を変更
		NonDeleteFlagを廃止
			cstringのdynamic_destructとstatic_destructが統合されたため
		FpSendableを追加
			送信可能文字数を取得する関数
		FpInformDestructを追加
			cstringのdestruct時に元のmoduleに対し通知する関数
			特にstaticバッファの再利用の可否の確認に利用
v1_09/121128 hmIto
	hmCLibとhmLibの統合に伴い、hmLib_v3_03に対応するよう変更	
v1_08/121101 hmIto
	HMR_COM_PACTRMNIDを@コマンドの代わりに使用
	odataからFnSendableを廃止
	odataにNonDeleteFlagを追加
v1_07/121028 hmIto
	boolian com_out_empty(void)追加
v1_06/121021 hmIto
	送受信バッファ操作用関数名を変更(Inをinに、Outをoutに）
	0byteコマンドを送信時に異常終了扱いされる致命的バグを修正
		IDがsizeに含まれていた名残で、Size=0をエラーデータとしていた問題
	hmrVMC1_v2_00に対応
		vmc1_create,vmc1_initialize関数の引数追加に対応
		合わせて、com_createVMC1にも自身のVMCID指定用引数増加
v1_05/121020 iwahori
	cファイルのcom_IO関数群の名称のhファイルとの相違を変更．(inをInに，outをOutに)
v1_04/121012 hmIto
	com_iQue/oQueをhmrCom.c内で隠蔽(__を付加)
	インターフェイスをBegin/EndからPtr/Sizeに戻す
	名前空間をComからcomに変更
v1_03/121008 hmIto
	関数形を抜本的に改定
		VMC1関数群をラッピングしていたcom_RF関数群は、廃止
		VMC1との通信に特化するcom_VMC関数群は、ヘッダファイルでは隠ぺい
		利用者はvmc1_create/release関数を使って、VMCを利用
		Bufの送受信制御は、com_In/com_Out関数群に集約
			com_In/Outでは、インターフェイスをID/Begin/Endに統一
			受信中のデータの送信可能サイズを返すsendProgressを廃止
			送信段階で送信データを訪ねるpush_later関数を追加
	lock/unlock関数廃止
		Mutexが使えないため、バッファのロックは各スレッドの優先順位で制御。
		そのため、ロックするスレッドによって処理が異なる。
		以後は、各スレッドが独自に自身の優先度を上げるようなlock/unlock処理を行う。
	121009Test_hmrcom_hmrVMC1.cにて、動作確認済み
v1_02/121005 hmIto
	VMC1の関数群をラッピングするのではなく、VMC1を生成する関数を提供する形に変更
		通信を実際に行う関数は、VMC1を利用する
v1_01/120922 hmIto
	ひとまず動作が安定していることを確認（120922Test_hmrcom_hmrVMC1.cにて動作確認済み）
v1_00/120921 hmIto
	バージョン管理開始

*/
#ifndef HMR_DATA_INC
#	include"hmrData.h"
#endif
#ifndef HMR_VMC_INC
#	include"hmrVMC1.h"
#endif
#include"hmLib_v3_04/cqueue.h"
#ifdef __cplusplus
using hmLib::cstring;
#endif
#define HMR_COM_PACTRMNID 0xF0
//データ型の定義
typedef vmc1_dsize_t com_dsize_t;
typedef vmc1_did_t com_did_t;
typedef com_dsize_t(*com_dsizeFp_v)(void);
typedef void(*com_vFp_v)(void);

//**************Com関数群*************//
//初期化関数
void com_initialize(void);
//終端化関数
void com_finalize(void);
//受信モードか確認
boolian com_isRecvMode(void);
//送信モードか確認
boolian com_isSendMode(void);
//VMC1を作成する
VMC1* com_createVMC1(void);
//VMC1を破棄する
void com_releaseVMC1(VMC1* pVMC1);

//****************com_IO関数群******************//
//受信可能であるか確認
boolian com_in_empty(void);
//受信バッファに空きがあるか確認
boolian com_in_full(void);
//データを受信
idata com_in_front(void);
//データを受信を終了
void com_in_move_pop(idata* pData_);
//送信可能であるかを確認
boolian com_out_full(void);
//送信バッファが空かどうかの確認
boolian com_out_empty(void);
//データを送信(受信済データ用)
void com_out_move_push(odata* mData_);

//**************ComWDT関連************//
#define com_wdt_sec 180
//com監視用WatchDogTimerのリセット
void com_wdt_restart();
//com監視用WatchDogTimerを停止
void com_wdt_enable();
//com監視用WatchDogTimerを停止
void com_wdt_disable();
//com監視用WatchDogTimerのタスク関数
sint16 com_wdt_task_function(sint16 Sec);

//******************com_ioBuf内部関数群*****************//
//iBufのサイズを取得する
cqueue_size_t _com_iBuf_size(void);
cqueue_size_t _com_oBuf_size(void);
uint8 _com_getFailPacNum(void);
uint8 _com_getHoldPacNum(void);
boolian com_isWaitSendPacket(void);
boolian _com_oBuf_empty(void);
boolian _com_oBuf_isPacEnd(void);

//*****************debug****************//
boolian com_vmc_can_send(void);
#
#endif
