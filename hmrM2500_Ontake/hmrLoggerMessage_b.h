#ifndef HMR_LOGGERMESSAGE_INC
#define HMR_LOGGERMESSAGE_INC 100
#
/*
===hmrLoggerMessage===
ロガー経由でデータをため、PCからのメッセージで蓄えたデータを放出するモジュール

使い方
まず、message形式のsetup_talk, talk、およびinform_talk_end関数をregist関数で登録する
	setup_talk：一回のログ動作前に一度だけ呼ばれる、ログ動作通知関数　この関数動作時に、データ取得を開始しておくことが望ましい
	talk：ログ動作関数　記録したいデータを渡す　まだ準備できていなければ、trueを返せばいい
	is_talking：今回のtalk動作が現在も継続しているかを示す関数
logmsg_logging関数を実行すると、log動作がスタートする
	setup_talkが一度ずつよばれる
	IsTalkingフラグが立ち上がり、logmsg_work関数内でtalk動作を実行する
logmsg_work関数内で、実際のログ動作が実行される
	登録されたtalk関数が呼ばれ、出てきたメッセージをloggerに記録する
	is_talk_end関数が立ち上がると、次に登録されたtalk関数を読みに行く
logmsg_is_logging関数で、すべてのログ動作が完了しているか確認できる
logmsg_message関数群をmessageに登録すると、自動的にログデータの読み出し等の機能を実行できる
	>> 0x00
		保存されているデータサイズを要求
	<< 0x00 LogSize[4]
		データサイズを4byteで戻す
	<> 0x01 RequestSize[2]
		最低RequestSize分のデータを要求する/受諾する（超えた瞬間終わる、足りなくてももちろん終わる）
	<< 0x02 LogSize[4]
		ログデータの送信完了を通知するとともに、残りのログデータサイズを通知
===hmrLoggerMessage===
hmrLoggerMessage_v1_00
	hmrMessage:v2_04を継承して作成
*/
#include"hmLib_v3_04/bytebase_type.h"
#include"hmLib_v3_04/cstring.h"
#ifndef HMR_DATA_INC
#	include"hmrData.h"
#endif
#ifndef HMR_COM_INC
#	include"hmrCom.h"
#endif
#ifndef HMR_MESSAGE_INC
#	include"hmrMessage.h"
#endif

typedef com_did_t logmsg_did_t;
//==============設定用=================//
#define logmsg_BUFSIZE 8
//=====================================//
typedef hmLib_boolian(*logmsg_bFp_ps)(cstring*);	//Talk用関数
//typedef void(*vFp_v)(void);
//typedef boolian(*bFp_v)(void);

//初期化処理
void logmsg_initialize(void);
//終端化処理
void logmsg_finalize(void);
//logger用message関数を登録
hmLib_boolian logmsg_regist(logmsg_did_t ID_
	, vFp_v FpSetupTalk_
	, logmsg_bFp_ps FpTalk_
	, bFp_v FpIsTalking_);
//ログ動作スタート
void logmsg_logging(void);
//ログ動作継続中か？
boolian logmsg_is_logging(void);
//logmsgモジュールのワーク関数
void logmsg_work(void);
//受信準備関数
void logmsg_setup_listen(void);
//受信関数 返り値は正常にデータが処理されなかった場合にtrue
hmLib_boolian logmsg_listen(cstring Str);
//送信準備関数
void logmsg_setup_talk(void);
//送信関数 返り値はデータが存在しない場合にtrue
hmLib_boolian logmsg_talk(cstring* pStr);
#
#endif
