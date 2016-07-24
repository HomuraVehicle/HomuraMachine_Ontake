#ifndef HMR_MESSAGE_INC
#define HMR_MESSAGE_INC 204
#
/*
===hmrMessage===
IDを読み出し、IDを除いた部分をlisten関数に渡す
talk関数で送信すべきデータを受け取り，IDを付加して返す

hmrMessage:v2_04/130212 hmIto
	msg_listen()
		idataのdestructをし忘れていた問題を修正。
		idataの引数を実態コピーからmoveポインタに変更。
	msg_talk()
		trueがFpTalkから戻ってきた際に、万が一Strが確保されている場合destructするよう修正
hmrMessage:v2_03/130202 iwahori
	msg_setup_talk()：forループの上限値をHMR_MESSAGE_BUFSIZEから_msg_BufSizeに修正
	msg_setup_listen()：forループの上限値をHMR_MESSAGE_BUFSIZEから_msg_BufSizeに修正
hmrMessage:v2_02/130127 iwahori
	msg_setup_talk()内で_msg_Buf[i].FpSetupListen()となっていた部分を
	_msg_Buf[i].FpSetupTalk()に修正
hmrMessage:v2_01/130105 hmIto
	hmLib_v3_03に対応
	SetupListen/SetupTalk関数を追加
		ListenとTalk実行前に一度だけ呼ばれる
		送受信フラグの設定用関数
hmrMessage:v2_00/121201 hmIto
	hmrCom_v2_00に対応
	メッセージ処理関数を、Talk関数とListen関数に分割
hmrMessage:v1_05/121128 hmIto
	hmCLibとhmLibの統合に伴い、hmLib_v3_03に対応するよう変更		
hmrMessage:v1_04/121103 hmIto
	NonDeleteFlag機能に対応
		msg_set_nondelete_mode();
hmrMessage:v1_03/121013 hmIto
	名前空間Messageをmsgに変更
hmrMessage:v1_02/121012 hmIto
	MessageType構造体がSendable関数も管理する形に変更
hmrMessage:v1_01/121009 hmIto
	インターフェイスを大きく変更
		msg_rangeは廃止
		work関数の引数をポインタに変更
hmrMessage:v1_00/121005 hmIto
	ひとまず、完成
*/
#ifndef HMCLIB_BYTEBASETYPE_INC
#	include"hmLib_v3_04/bytebase_type.h"
#endif
#ifndef HMCLIB_CSTRING_INC
#	include"hmLib_v3_04/cstring.h"
#endif
#ifndef HMR_COM_INC
#	include"hmrCom.h"
#endif
#ifdef __cplusplus
using hmLib::cstring;
#endif
typedef com_did_t msg_did_t;
typedef com_dsize_t msg_dsize_t;
typedef com_dsizeFp_v msg_dsizeFp_v;
typedef com_vFp_v msg_vFp_v;
//==============設定用=================//
#define HMR_MESSAGE_BUFSIZE 32
//=====================================//

typedef hmLib_boolian(*msg_bFp_s)(cstring);				//Listen用関数
typedef hmLib_boolian(*msg_bFp_ps)(cstring*);	//Talk用関数
//message_t構造体
typedef struct{
	msg_did_t	ID;				//メッセージ識別用ID
	msg_vFp_v	FpSetupListen;	//メッセージ受信準備用関数
	msg_bFp_s	FpListen;		//メッセージ受信用関数 戻り値はメッセージ処理の可否
	msg_vFp_v	FpSetupTalk;	//メッセージ送信準備用関数
	msg_bFp_ps	FpTalk;			//メッセージ送信用関数 戻り値はメッセージの有無
}message_t;
//初期化処理
void msg_initialize(void);
//終端化処理
void msg_finalize(void);
//DatTypeを登録
hmLib_boolian msg_regist(msg_did_t ID_
						 ,msg_vFp_v FpSetupListen_
						 ,msg_bFp_s FpListen_
						 ,msg_vFp_v FpSetupTalk_
						 ,msg_bFp_ps FpTalk_);
//受信準備関数
void msg_setup_listen(void);
//受信関数 返り値は正常にデータが処理されなかった場合にtrue
hmLib_boolian msg_listen(idata* mdata_);
//送信準備関数
void msg_setup_talk(void);
//送信関数 返り値はデータが存在しない場合にtrue
hmLib_boolian msg_talk(odata* pdata_);
//送信可能サイズ関数の使用を宣言する
void msg_talkconfig_useSendable(msg_dsizeFp_v Fp_);
//送信完了通知関数を使用する
void msg_talkconfig_useInformDestruct(msg_vFp_v Fp_);
//EDSメモリ領域に確保した場合のアクセス用に、DSRPageFileを登録する
void msg_talkconfig_useDSRPageFile(int DSRPAG_);
#
#endif
