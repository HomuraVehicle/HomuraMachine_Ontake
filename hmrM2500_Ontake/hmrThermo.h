#ifndef HMR_THERMO_INC
#define HMR_THERMO_INC 2
#
/*===hmrThermo===
バッテリー管理用コマンド
hmrThermo_v0_02/131019 iwahori
	タスクシステム用にtask_setup_talkとtask_interruptを作成
hmrThermo_v0_01/130112 iwahori
	work関数をsetup_listenとsetup_talkに分離
hmrThermo_v0_00/121208 hmIto
	基本関数作成
*/
#include"hmLib_v3_04/cstring.h"
#include"hmrType.h"
#ifdef __cplusplus
using hmLib::cstring;
#endif
//batttery監視用関数群
hmLib_boolian thermo_listen(cstring Str);
hmLib_boolian thermo_talk(cstring* pStr);
void thermo_setup_listen(void);
void thermo_setup_talk(void);
void thermo_task_setup_listen(void);
void thermo_task_setup_talk(void);
void thermo_task_setup_talk(void);
sint16 thermo_task_data(sint16 interval);
sint16 thermo_task_inform(sint16 interval);
#
#endif
