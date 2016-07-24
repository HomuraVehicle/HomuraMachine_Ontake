#ifndef HMR_THERMO_INC
#define HMR_THERMO_INC 2
#
/*===hmrThermo===
�o�b�e���[�Ǘ��p�R�}���h
hmrThermo_v0_02/131019 iwahori
	�^�X�N�V�X�e���p��task_setup_talk��task_interrupt���쐬
hmrThermo_v0_01/130112 iwahori
	work�֐���setup_listen��setup_talk�ɕ���
hmrThermo_v0_00/121208 hmIto
	��{�֐��쐬
*/
#include"hmLib_v3_04/cstring.h"
#include"hmrType.h"
#ifdef __cplusplus
using hmLib::cstring;
#endif
//batttery�Ď��p�֐��Q
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
