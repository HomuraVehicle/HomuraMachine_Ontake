#ifndef HMR_CO2_INC
#define HMR_CO2_INC 2
#
/*===hmrCO2===
CO2���W���[������p
hmrCO2_v0_02/131019 iwahori
	�^�X�N�V�X�e���p��task_setup_talk��task_interrupt���쐬
hmrCO2_v0_01/130105 iwahori
	work�֐���setup_lisen��setup_talk�ɕ���
hmrCO2_v0_00/121208 hmIto
	��{�֐��쐬
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
