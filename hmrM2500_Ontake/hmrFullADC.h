#ifndef HMR_FULLADC_INC
#define HMR_FULLADC_INC 2
#
/*===hmrBattery===
�o�b�e���[�Ǘ��p�R�}���h
hmrBattery_v0_02/131019 iwahori
�^�X�N�V�X�e���p��task_setup_talk��task_interrupt���쐬
hmrBattery_v0_01/130105 iwahori
work�֐���setup_lisen��setup_talk�ɕ���
hmrBattery_v0_00/121208 hmIto
��{�֐��쐬
*/
#include"hmLib_v3_04/cstring.h"
#include"hmrType.h"
#ifdef __cplusplus
using hmLib::cstring;
#endif
//batttery�Ď��p�֐��Q
hmLib_boolian fulladc_listen(cstring Str);
hmLib_boolian fulladc_talk(cstring* pStr);
void fulladc_setup_listen(void);
void fulladc_setup_talk(void);
void fulladc_task_setup_talk(void);
sint16 fulladc_task_data(sint16 interval);
sint16 fulladc_task_inform(sint16 interval);
#
#endif
