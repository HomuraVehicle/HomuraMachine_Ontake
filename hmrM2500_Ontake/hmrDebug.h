#ifndef HMR_DEBUG_INC
#define HMR_DEBUG_INC 100
#
/*===hmrDebug===
Debug�֌W�̊֐��Q
*/
#include"hmrType.h"
//���[�^�[�o�̓s���ŁA0-15�̐������o�͂���
//	3-0bit�ڂ����ꂼ�ꏇ��LA,LB,RA,RB
void debug_num4Motor(uint8 num_0_15_);
void debug_num4Motor2(uint8 num_0_15_);
//0-1�o�͂�CO2PumpPw�s�����g���ďo��
void debug_isCO2PumpPw(boolian val_);
//0-1�o�͂�SpritePw�s�����g���ďo��
//void debug_isSpritePw(boolian val_);
//0-1�o�͂�MPPw�s�����g���ďo��
void debug_isMPPw(boolian val_);
#
#endif
