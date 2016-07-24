#ifndef HMR_THERMO_C_INC
#define HMR_THERMO_C_INC 2
#
#define HMLIB_NOLIB
#ifndef HMR_THERMO_INC
#	include"hmrThermo.h"
#endif
#ifndef HMR_DEVICE_INC
#	include"hmrDevice.h"
#endif
#ifndef HMR_UTILITY_INC
#	include "hmrUtility.h"
#endif
#include"hmrDebug.h"
struct{
	unsigned Data:1;				//�f�[�^�𑗐M���ɗ�����
	unsigned SendData:1;			//�f�[�^���M���[�h
	unsigned SendData_i:1;			//�f�[�^���M���[�h��
}_thermo_Mode={0,0,1};
hmLib_boolian thermo_listen(cstring Str){

	//�f�[�^�T�C�Y�m�F
	if(cstring_size(&Str)==0)return hmLib_true;

	//1byte�ڂŃ��[�h�𕪂���
	switch(cstring_getc(&Str,0)){
	case 0x10:
		_thermo_Mode.SendData=hmLib_true;
		_thermo_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	case 0x11:
		_thermo_Mode.SendData=hmLib_false;
		_thermo_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	default:
		return hmLib_true;
	}
}
hmLib_boolian thermo_talk(cstring* pStr){
	hmLib_uint16 sumadc;

	if(_thermo_Mode.SendData_i){
		cstring_construct_dynamic_safe(pStr,1);
		if(_thermo_Mode.SendData)cstring_putc(pStr,0,0x10);
		else cstring_putc(pStr,0,0x11);

		//���M�㎩���I�Ƀt���O�𗎂Ƃ�
		_thermo_Mode.SendData_i=hmLib_false;
		return hmLib_false;
	}else if(_thermo_Mode.Data){
		cstring_construct_dynamic_safe(pStr,3);
		cstring_putc(pStr,0,0x00);

		//���x�f�[�^�擾
		sumadc = adc_get_average(Vref_GND,ADC_Thermo);
		cstring_putc(pStr,1,(unsigned char)(sumadc%256));
		cstring_putc(pStr,2,(unsigned char)((sumadc>>8)%256));

		//���M�㎩���I�Ƀt���O�𗎂Ƃ�
		_thermo_Mode.Data=hmLib_false;
		return hmLib_false;
	}

	return hmLib_true;
}
void thermo_setup_listen(void){return;}
void thermo_setup_talk(void){
	_thermo_Mode.Data=_thermo_Mode.SendData;
}
void thermo_task_setup_talk(void){return;}
sint16 thermo_task_data(sint16 interval){
	_thermo_Mode.Data=_thermo_Mode.SendData;
	return interval;
}
sint16 thermo_task_inform(sint16 interval){
	_thermo_Mode.SendData_i=hmLib_true;
	return interval;
}
#
#endif
