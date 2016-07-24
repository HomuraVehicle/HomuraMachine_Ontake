#ifndef HMR_DEBUG_C_INC
#define HMR_DEBUG_C_INC 100
#
#include"hmrDebug.h"
#include"hmrDevice.h"
//���[�^�[�o�̓s���ŁA0-15�̐������o�͂���
//	3-0bit�ڂ����ꂼ�ꏇ��LA,LB,RA,RB
void debug_num4Motor(uint8 num_0_15_){}
void debug_num4Motor2(uint8 num_0_15_){
	HMR_PIN_MOTORR_B_set(!(num_0_15_&0x01));
	num_0_15_>>=1;
	HMR_PIN_MOTORR_A_set(!(num_0_15_&0x01));
	num_0_15_>>=1;
	HMR_PIN_MOTORL_B_set(!(num_0_15_&0x01));
	num_0_15_>>=1;
	HMR_PIN_MOTORL_A_set(!(num_0_15_&0x01));
}
//0-1�o�͂�CO2PumpPw�s�����g���ďo��
void debug_isCO2PumpPw(boolian val_){
	HMR_PIN_PUMP_PW_set(val_);
}
//0-1�o�͂�SpritePw�s�����g���ďo��
void debug_isSpritePw(boolian val_){
	HMR_PIN_CAM_PW_set(val_);
}
//0-1�o�͂�MPPw�s�����g���ďo��
void debug_isMPPw(boolian val_){
	HMR_PIN_MP_PW_set(val_);
}
#
#endif
