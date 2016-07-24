#ifndef HMR_DATA_INC
#define HMR_DATA_INC 102
#
/*===hmrData===
hmrのCom/Messageなどで使われる通信用データ型の宣言

hmrData:v1_02/130928 hmIto
	inform_destruct機能をodata_destruct内で実行するように変更
hmrData:v1_01/130216 hmIto
	EDS領域へのアクセスに対応
		DSRPAGをodataに追加
		construct関数を変更
hmrData:v1_00/130105 hmIto
	hmrComからデータ型関連の宣言の未分離
*/
// idata のコンストラクタ、セッティング関数を作る
//  単にラッピングするだけ。
//　ががが
#ifndef HMLIB_CSTRING_INC
#	include"hmLib_v3_04/cstring.h"
#endif
#ifndef HMR_VMC_INC
#	include"hmrVMC1.h"
#endif
#ifdef __cplusplus
using hmLib::cstring;
#endif
//データ型の定義
typedef vmc1_dsize_t data_dsize_t;
typedef vmc1_did_t data_did_t;
typedef data_dsize_t(*data_dsizeFp_v)(void);
typedef void(*data_vFp_v)(void);
//受信のデータ型
typedef struct{
	data_did_t ID;
	cstring Str;
}idata;
//送信のデータ型
typedef struct{
	data_did_t ID;
	cstring Str;
	int DSRPAG;			//EDS領域で確保した場合、確保領域のPageFileを記録しておく
	data_dsizeFp_v FpSendable;
	data_vFp_v FpInformDestruct;
}odata;
//関数群
void idata_format(idata* Dat);
hmLib_boolian idata_is_construct(idata*Dat);
void idata_construct(idata* Dat, data_did_t ID, cstring* mStr);
void idata_destruct(idata* Dat);
void idata_move(idata* From, idata* To);

void odata_format(odata* Dat);
hmLib_boolian odata_is_construct(odata*Dat);
void odata_construct(odata* Dat, data_did_t ID, cstring* mStr, int DSRPAG,
					 data_dsizeFp_v FpSendable, data_vFp_v FpInformDestruct);
void odata_destruct(odata* Dat);
void odata_move(odata* From, odata* To);

#
#endif
