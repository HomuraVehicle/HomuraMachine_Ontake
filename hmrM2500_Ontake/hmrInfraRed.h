#ifndef HMR_INFRARED_INC
#define HMR_INFRARED_INC 2
#
/*===hmrInfraRed===
バッテリー管理用コマンド
hmrInfraRed_v0_02/131019 iwahori
	タスクシステム用にtask_setup_talkとtask_interruptを作成
hmrInfThermo_v0_01/130105 iwahori
	work関数をsetup_lisenとsetup_talkに分離
hmrInfThermo_v0_00/121208 hmIto
	基本関数作成
*/
#include"hmLib_v3_04/cstring.h"
#include"hmrType.h"
#ifdef __cplusplus
using hmLib::cstring;
#endif
//batttery監視用関数群
hmLib_boolian infrared_listen(cstring Str);
hmLib_boolian infrared_talk(cstring* pStr);
void infrared_setup_listen(void);
void infrared_setup_talk(void);
void infrared_task_setup_talk(void);
sint16 infrared_task_data(sint16 interval);
sint16 infrared_task_inform(sint16 interval);
#
#endif
