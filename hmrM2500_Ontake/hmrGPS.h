#ifndef HMR_GPS_INC
#define HMR_GPS_INC 3
#
/*===hmrGPS===
GPS関連モジュール
hmrGPS_v0_03/131108 hmIto
	タスクにのってGPSが実際に切り替えられまくる問題を修正
hmrGPS_v0_02/131019 iwahori
	タスクシステム用にtask_setup_talkとtask_interruptを作成
hmrGPS_v0_01/130105 iwahori
	work関数をsetup_lisenとsetup_talkに分離
hmrGPS_v0_00/121208 hmIto
	基本関数作成
*/
#include"hmLib_v3_04/cstring.h"
#include"hmrType.h"
#ifdef __cplusplus
using hmLib::cstring;
#endif
hmLib_boolian gps_listen(cstring Str);
hmLib_boolian gps_talk(cstring* pStr);
void gps_setup_listen(void);
void gps_setup_talk(void);
void gps_task_setup_talk(void);
sint16 gps_task_data(sint16 interval);
sint16 gps_task_inform(sint16 interval);
#
#endif
