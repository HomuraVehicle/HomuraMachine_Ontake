#ifndef HMR_SHT75_INC
#define HMR_SHT75_INC 1
#
/*

===hmrSHT75===
SHT75（温度、湿度同時測定用モジュール）の制御用関数
v0_01/131026 hmIto
	とりあえず作成
*/
#include"hmLib_v3_04/cstring.h"
#include"hmrType.h"
#ifdef __cplusplus
using hmLib::cstring;
#endif
//通常モードのデータ受信用関数
void sht75_setup_listen(void);
hmLib_boolian sht75_listen(cstring Str);
//通常モードのデータ送信用関数
void sht75_setup_talk(void);
hmLib_boolian sht75_talk(cstring* pStr);
//タスクモードのデータ送信用関数
void sht75_task_setup_talk(void);
sint16 sht75_task_data(sint16 interval);
sint16 sht75_task_inform(sint16 interval);
#
#endif
