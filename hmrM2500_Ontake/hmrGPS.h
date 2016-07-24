#ifndef HMR_GPS_INC
#define HMR_GPS_INC 3
#
/*===hmrGPS===
GPS�֘A���W���[��
hmrGPS_v0_03/131108 hmIto
	�^�X�N�ɂ̂���GPS�����ۂɐ؂�ւ����܂�������C��
hmrGPS_v0_02/131019 iwahori
	�^�X�N�V�X�e���p��task_setup_talk��task_interrupt���쐬
hmrGPS_v0_01/130105 iwahori
	work�֐���setup_lisen��setup_talk�ɕ���
hmrGPS_v0_00/121208 hmIto
	��{�֐��쐬
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
