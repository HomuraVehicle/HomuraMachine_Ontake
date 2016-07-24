#ifndef HMR_INFRARED_INC
#define HMR_INFRARED_INC 2
#
/*===hmrInfraRed===
�o�b�e���[�Ǘ��p�R�}���h
hmrInfraRed_v0_02/131019 iwahori
	�^�X�N�V�X�e���p��task_setup_talk��task_interrupt���쐬
hmrInfThermo_v0_01/130105 iwahori
	work�֐���setup_lisen��setup_talk�ɕ���
hmrInfThermo_v0_00/121208 hmIto
	��{�֐��쐬
*/
#include"hmLib_v3_04/cstring.h"
#include"hmrType.h"
#ifdef __cplusplus
using hmLib::cstring;
#endif
//batttery�Ď��p�֐��Q
hmLib_boolian infrared_listen(cstring Str);
hmLib_boolian infrared_talk(cstring* pStr);
void infrared_setup_listen(void);
void infrared_setup_talk(void);
void infrared_task_setup_talk(void);
sint16 infrared_task_data(sint16 interval);
sint16 infrared_task_inform(sint16 interval);
#
#endif
