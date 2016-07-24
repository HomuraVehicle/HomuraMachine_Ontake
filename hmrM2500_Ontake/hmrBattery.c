#ifndef HMR_BATTERY_C_INC
#define HMR_BATTERY_C_INC 2
#
#define HMLIB_NOLIB
#ifndef HMR_BATTERY_INC
#	include"hmrBattery.h"
#endif
#ifndef HMR_UTILITY_INC
#	include "hmrUtility.h"
#endif
#ifndef HMR_DEVICE_INC
#	include"hmrDevice.h"
#endif
struct{
	unsigned Data:1;				//�f�[�^�𑗐M���ɗ�����
	unsigned SendData:1;			//�f�[�^���M���[�h
	unsigned SendData_i:1;			//�f�[�^���M���[�h��
}_battery_Mode={0,0,1};
hmLib_boolian battery_listen(cstring Str){
	//�f�[�^�T�C�Y�m�F
	if(cstring_size(&Str)==0)return hmLib_true;

	//1byte�ڂŃ��[�h�𕪂���
	switch(cstring_getc(&Str,0)){
	case 0x10:
		_battery_Mode.SendData=hmLib_true;
		_battery_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	case 0x11:
		_battery_Mode.SendData=hmLib_false;
		_battery_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	default:
		return hmLib_true;
	}
}
hmLib_boolian battery_talk(cstring* pStr){
	uint32 sumadc;

	if(_battery_Mode.SendData_i){
		cstring_construct_dynamic_safe(pStr,1);
		if(_battery_Mode.SendData)cstring_putc(pStr,0,0x10);
		else cstring_putc(pStr,0,0x11);

		//���M�㎩���I�Ƀt���O�𗎂Ƃ�
		_battery_Mode.SendData_i=hmLib_false;
		return hmLib_false;
	}else if(_battery_Mode.Data){
		cstring_construct_dynamic_safe(pStr,1+2*3);
		cstring_putc(pStr,0,0x00);

		//�f�[�^�擾
		sumadc = adc_get_average(Vref_GND,battery_id_to_adc(0));//ID��AN�ɕϊ����Ă����̃f�[�^��ǂݓǂ�
		cstring_putc(pStr,1,(hmLib_uint8)(sumadc&0x00FF));		//Low
		cstring_putc(pStr,2,(hmLib_uint8)((sumadc>>8)&0x00FF));	//High

		sumadc = adc_get_average(Vref_GND,battery_id_to_adc(1));//ID��AN�ɕϊ����Ă����̃f�[�^��ǂݓǂ�
		cstring_putc(pStr,3,(hmLib_uint8)(sumadc&0x00FF));		//Low
		cstring_putc(pStr,4,(hmLib_uint8)((sumadc>>8)&0x00FF));	//High

		sumadc = adc_get_average(Vref_GND,battery_id_to_adc(2));//ID��AN�ɕϊ����Ă����̃f�[�^��ǂݓǂ�
		cstring_putc(pStr,5,(hmLib_uint8)(sumadc&0x00FF));		//Low
		cstring_putc(pStr,6,(hmLib_uint8)((sumadc>>8)&0x00FF));	//High

		//���M�㎩���I�Ƀt���O�𗎂Ƃ�
		_battery_Mode.Data=hmLib_false;
		return hmLib_false;
	}

	return hmLib_true;
}
void battery_setup_listen(void){return;}
void battery_setup_talk(void){
	_battery_Mode.Data=_battery_Mode.SendData;
}
void battery_task_setup_talk(void){return;}
sint16 battery_task_data(sint16 interval){
	_battery_Mode.Data=_battery_Mode.SendData;
	return interval;
}
sint16 battery_task_inform(sint16 interval){
	_battery_Mode.SendData_i=hmLib_true;
	return interval;
}
#
#endif
