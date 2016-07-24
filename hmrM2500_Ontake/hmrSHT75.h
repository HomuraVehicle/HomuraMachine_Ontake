#ifndef HMR_SHT75_INC
#define HMR_SHT75_INC 1
#
/*

===hmrSHT75===
SHT75�i���x�A���x��������p���W���[���j�̐���p�֐�
v0_01/131026 hmIto
	�Ƃ肠�����쐬
*/
#include"hmLib_v3_04/cstring.h"
#include"hmrType.h"
#ifdef __cplusplus
using hmLib::cstring;
#endif
//�ʏ탂�[�h�̃f�[�^��M�p�֐�
void sht75_setup_listen(void);
hmLib_boolian sht75_listen(cstring Str);
//�ʏ탂�[�h�̃f�[�^���M�p�֐�
void sht75_setup_talk(void);
hmLib_boolian sht75_talk(cstring* pStr);
//�^�X�N���[�h�̃f�[�^���M�p�֐�
void sht75_task_setup_talk(void);
sint16 sht75_task_data(sint16 interval);
sint16 sht75_task_inform(sint16 interval);
#
#endif
