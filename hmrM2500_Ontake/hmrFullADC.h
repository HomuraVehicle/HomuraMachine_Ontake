#ifndef HMR_FULLADC_INC
#define HMR_FULLADC_INC 2
#
/*===hmrBattery===
バッテリー管理用コマンド
hmrBattery_v0_02/131019 iwahori
タスクシステム用にtask_setup_talkとtask_interruptを作成
hmrBattery_v0_01/130105 iwahori
work関数をsetup_lisenとsetup_talkに分離
hmrBattery_v0_00/121208 hmIto
基本関数作成
*/
#include"hmLib_v3_04/cstring.h"
#include"hmrType.h"
#ifdef __cplusplus
using hmLib::cstring;
#endif
//batttery監視用関数群
hmLib_boolian fulladc_listen(cstring Str);
hmLib_boolian fulladc_talk(cstring* pStr);
void fulladc_setup_listen(void);
void fulladc_setup_talk(void);
void fulladc_task_setup_talk(void);
sint16 fulladc_task_data(sint16 interval);
sint16 fulladc_task_inform(sint16 interval);
#
#endif
