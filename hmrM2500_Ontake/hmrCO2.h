#ifndef HMR_CO2_INC
#define HMR_CO2_INC 2
#
/*===hmrCO2===
CO2モジュール制御用
hmrCO2_v0_02/131019 iwahori
	タスクシステム用にtask_setup_talkとtask_interruptを作成
hmrCO2_v0_01/130105 iwahori
	work関数をsetup_lisenとsetup_talkに分離
hmrCO2_v0_00/121208 hmIto
	基本関数作成
*/
#include"hmLib_v3_04/cstring.h"
#include"hmrType.h"
#ifdef __cplusplus
using hmLib::cstring;
#endif
hmLib_boolian co2_listen(cstring Str);
hmLib_boolian co2_talk(cstring* pStr);
void co2_setup_listen(void);
void co2_setup_talk(void);
void co2_task_setup_talk(void);
sint16 co2_task_data(sint16 interval);
sint16 co2_task_inform(sint16 interval);
#
#endif
