#ifndef HMR_LOGGERMESSAGE_C_INC
#define HMR_LOGGERMESSAGE_C_INC 100
#
#include"hmrLoggerMessage_b.h"
#include"hmrBufLogger.h"
#include"hmrUtility.h"

//ロガー駆動用登録関数群
typedef struct {
	unsigned char ID;
	vFp_v setup_talk;
	logmsg_bFp_ps talk;
	bFp_v is_talking;
}logmessage_t;
typedef struct {
	unsigned InformLogSize:1;
	unsigned InformRequestData:1;
	unsigned InformSendData:1;
}logmsg_flags;
logmsg_flags x_logmsg_Flags;
uint32 x_logmsg_RequestSize;
uint32 x_logmsg_SendSize;
//関数群登録用バッファ
logmessage_t x_logmsg_Buffer[logmsg_BUFSIZE];
//関数群登録用バッファのサイズ
unsigned char x_logmsg_BufferSize;
//Loggerモードに入ったかのフラグ
boolian x_logmsg_IsLogging;
//Logger保存済みメッセージ位置
unsigned char x_logmsg_BufferCnt;

//初期化処理
void logmsg_initialize(void) {
	x_logmsg_BufferSize=0;
	x_logmsg_IsLogging=0;
	x_logmsg_BufferCnt=0;
}
//終端化処理
void logmsg_finalize(void) {
}
//logger用message関数を登録
hmLib_boolian logmsg_regist(logmsg_did_t ID_
	, vFp_v FpSetupTalk_
	, logmsg_bFp_ps FpTalk_
	, bFp_v FpIsTalking_) {
	//登録上限に達していたらはじく
	if(x_logmsg_BufferSize>=logmsg_BUFSIZE)return hmLib_true;

	//関数を登録
	x_logmsg_Buffer[x_logmsg_BufferSize].ID=ID_;
	x_logmsg_Buffer[x_logmsg_BufferSize].setup_talk=FpSetupTalk_;
	x_logmsg_Buffer[x_logmsg_BufferSize].talk=FpTalk_;
	x_logmsg_Buffer[x_logmsg_BufferSize].is_talking=FpIsTalking_;

	//サイズを増やしておく
	++x_logmsg_BufferSize;

	return hmLib_false;
}
//ログ動作スタート
void logmsg_logging(void) {
	unsigned char Cnt;
	//setup_talkを全員に実行させる
	for(Cnt=0; Cnt<x_logmsg_BufferSize; ++Cnt) {
		//関数が登録されていることを確かめてから、実行
		if(x_logmsg_Buffer[Cnt].setup_talk)x_logmsg_Buffer[Cnt].setup_talk();
	}

	//ログ動作の初期化
	x_logmsg_BufferCnt=0;
	x_logmsg_IsLogging=hmLib_true;
}
//ログ動作継続中か？
boolian logmsg_is_logging(void) {
	return x_logmsg_IsLogging;
}
//logmsgモジュールのワーク関数
void logmsg_work(void) {
	odata Data;

	odata_format(&Data);

	//ログモードでなければ無視する
	if(!x_logmsg_IsLogging)return;

	//書き込めないときも無視する
	if(buflogger_can_write_bytes(4096)==0)return;

	//登録データの末尾に到達するまで続ける
	while (x_logmsg_BufferCnt<x_logmsg_BufferSize) {
		//talkの取得に成功すれば、脱出
		if (x_logmsg_Buffer[x_logmsg_BufferCnt].talk(&(Data.Str)) == 0) {
			break;
		}

		//念のため破棄しておく
		if (cstring_is_construct(&(Data.Str)))cstring_destruct_safe(&(Data.Str));

		//まだ話し中にもかかわらず読み込み失敗の場合は、次回にかける
		if (x_logmsg_Buffer[x_logmsg_BufferCnt].is_talking())return;

		//次のデータへ
		++x_logmsg_BufferCnt;
	}

	//末尾まで到達した場合はここでログ動作を終える
	if (x_logmsg_BufferCnt == x_logmsg_BufferSize) {
		x_logmsg_IsLogging = hmLib_false;
		return;
	}

/*
	//データを読み込みに成功するまで続ける
	while(x_logmsg_Buffer[x_logmsg_BufferCnt].talk(&(Data.Str))) {
		//念のため破棄しておく
		if(cstring_is_construct(&(Data.Str)))cstring_destruct_safe(&(Data.Str));
		
		//まだ話し中にもかかわらず読み込み失敗の場合は、次回にかける
		if(x_logmsg_Buffer[x_logmsg_BufferCnt].is_talking())return;

		//次のデータが存在しない場合は、ここでログ動作を終える
		if(++x_logmsg_BufferCnt == x_logmsg_BufferSize) {
			x_logmsg_IsLogging=hmLib_false;
			return;
		}
	}
	*/

	//IDを控える
	Data.ID=x_logmsg_Buffer[x_logmsg_BufferCnt].ID;

	//データをロガーにぶち込む
	buflogger_move_write(&Data);
}
//受信関数 返り値は正常にデータが処理されなかった場合にtrue
hmLib_boolian logmsg_listen(cstring Str) {
	if(cstring_size(&Str)==0)return hmLib_true;

	switch(cstring_getc(&Str, 0)) {
	case 0x00:
		x_logmsg_Flags.InformLogSize=1;
		return hmLib_false;
	case 0x01:
		if(cstring_size(&Str)!=3)return hmLib_true;

		//既に送信中の場合は外す
		if(x_logmsg_RequestSize>0)return hmLib_false;

		//サイズを取得
		x_logmsg_SendSize=0;
		x_logmsg_RequestSize=(uint32)(cstring_getc(&Str, 1))+((uint32)(cstring_getc(&Str, 2))<<8);
		x_logmsg_Flags.InformRequestData=1;
		return hmLib_false;
	default:
		return hmLib_true;
	}
}
//送信関数 返り値はデータが存在しない場合にtrue
hmLib_boolian logmsg_talk(cstring* pStr) {
	uint32 u32;
	odata Data;
	if(x_logmsg_Flags.InformLogSize) {
		cstring_construct_dynamic_safe(pStr, 5);
		cstring_putc(pStr, 0, 0x00);
		u32=buflogger_size();
		cstring_putc(pStr, 1, (uint8)(u32));
		u32>>=8;
		cstring_putc(pStr, 2, (uint8)(u32));
		u32>>=8;
		cstring_putc(pStr, 3, (uint8)(u32));
		u32>>=8;
		cstring_putc(pStr, 4, (uint8)(u32));
		x_logmsg_Flags.InformLogSize=0;
		return hmLib_false;
	} else if(x_logmsg_Flags.InformRequestData) {
		cstring_construct_dynamic_safe(pStr, 3);
		cstring_putc(pStr, 0, 0x00);
		u32=x_logmsg_RequestSize;
		cstring_putc(pStr, 1, (uint8)(u32));
		u32>>=8;
		cstring_putc(pStr, 2, (uint8)(u32));
		x_logmsg_Flags.InformRequestData=0;
		return hmLib_false;
	}
	
	if(x_logmsg_SendSize<x_logmsg_RequestSize) {
		if(!buflogger_can_read()) {
			x_logmsg_Flags.InformSendData=1;
			x_logmsg_SendSize=0;
			x_logmsg_RequestSize=0;
		} else {
			buflogger_move_read(&Data);
			msg_talkconfig_setID(Data.ID);
			cstring_move(&(Data.Str), pStr);
			x_logmsg_SendSize+=cstring_size(pStr)+3;
			if(x_logmsg_SendSize>=x_logmsg_RequestSize) {
				x_logmsg_Flags.InformSendData=1;
				x_logmsg_SendSize=0;
				x_logmsg_RequestSize=0;
			}
			return hmLib_false;
		}
	}
	if(x_logmsg_Flags.InformSendData) {
		u32=buflogger_size();
		cstring_construct_dynamic_safe(pStr, 5);
		cstring_putc(pStr, 0, 0x20);
		cstring_putc(pStr, 1, (uint8)(u32));
		u32>>=8;
		cstring_putc(pStr, 2, (uint8)(u32));
		u32>>=8;
		cstring_putc(pStr, 3, (uint8)(u32));
		u32>>=8;
		cstring_putc(pStr, 4, (uint8)(u32));
		x_logmsg_Flags.InformSendData=0;
		return hmLib_false;
	}
	return hmLib_true;
}
#
#endif
