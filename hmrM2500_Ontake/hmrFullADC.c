#ifndef HMR_FULLADC_C_INC
#define HMR_FULLADC_C_INC 2
#
#define HMLIB_NOLIB
#ifndef HMR_FULLADC_INC
#	include"hmrFullADC.h"
#endif
#ifndef HMR_UTILITY_INC
#	include "hmrUtility.h"
#endif
#ifndef HMR_DEVICE_INC
#	include"hmrDevice.h"
#endif
struct{
	unsigned Data : 1;				//�f�[�^�𑗐M���ɗ�����
	unsigned SendData : 1;			//�f�[�^���M���[�h
	unsigned SendData_i : 1;		//�f�[�^���M���[�h��
}_fulladc_Mode = {0,0,1};
hmLib_boolian fulladc_listen(cstring Str){
	//�f�[�^�T�C�Y�m�F
	if(cstring_size(&Str) == 0)return hmLib_true;

	//1byte�ڂŃ��[�h�𕪂���
	switch(cstring_getc(&Str, 0)){
	case 0x10:
		_fulladc_Mode.SendData = hmLib_true;
		_fulladc_Mode.SendData_i = hmLib_true;
		return hmLib_false;
	case 0x11:
		_fulladc_Mode.SendData = hmLib_false;
		_fulladc_Mode.SendData_i = hmLib_true;
		return hmLib_false;
	default:
		return hmLib_true;
	}
}
hmLib_boolian fulladc_talk(cstring* pStr){
	uint32 sumadc;

	if(_fulladc_Mode.SendData_i){
		cstring_construct_dynamic_safe(pStr, 1);
		if(_fulladc_Mode.SendData)cstring_putc(pStr, 0, 0x10);
		else cstring_putc(pStr, 0, 0x11);

		//���M�㎩���I�Ƀt���O�𗎂Ƃ�
		_fulladc_Mode.SendData_i = hmLib_false;
		return hmLib_false;
	} else if(_fulladc_Mode.Data){
		cstring_construct_dynamic_safe(pStr, 1 + 2 * 8);
		cstring_putc(pStr, 0, 0x00);

		//�f�[�^�擾
		sumadc = adc_get_average(Vref_GND, HMR_ADC_CH0);//ID��AN�ɕϊ����Ă����̃f�[�^��ǂݓǂ�
		cstring_putc(pStr, 1, (hmLib_uint8)(sumadc & 0x00FF));		//Low
		cstring_putc(pStr, 2, (hmLib_uint8)((sumadc >> 8) & 0x00FF));	//High

		sumadc = adc_get_average(Vref_GND, HMR_ADC_CH1);//ID��AN�ɕϊ����Ă����̃f�[�^��ǂݓǂ�
		cstring_putc(pStr, 3, (hmLib_uint8)(sumadc & 0x00FF));		//Low
		cstring_putc(pStr, 4, (hmLib_uint8)((sumadc >> 8) & 0x00FF));	//High

		sumadc = adc_get_average(Vref_GND, HMR_ADC_CH2);//ID��AN�ɕϊ����Ă����̃f�[�^��ǂݓǂ�
		cstring_putc(pStr, 5, (hmLib_uint8)(sumadc & 0x00FF));		//Low
		cstring_putc(pStr, 6, (hmLib_uint8)((sumadc >> 8) & 0x00FF));	//High

		sumadc = adc_get_average(Vref_GND, HMR_ADC_CH3);//ID��AN�ɕϊ����Ă����̃f�[�^��ǂݓǂ�
		cstring_putc(pStr, 7, (hmLib_uint8)(sumadc & 0x00FF));		//Low
		cstring_putc(pStr, 8, (hmLib_uint8)((sumadc >> 8) & 0x00FF));	//High
																		
		sumadc = adc_get_average(Vref_GND, HMR_ADC_CH4);//ID��AN�ɕϊ����Ă����̃f�[�^��ǂݓǂ�
		cstring_putc(pStr,  9, (hmLib_uint8)(sumadc & 0x00FF));		//Low
		cstring_putc(pStr, 10, (hmLib_uint8)((sumadc >> 8) & 0x00FF));	//High

		sumadc = adc_get_average(Vref_GND, HMR_ADC_CH5);//ID��AN�ɕϊ����Ă����̃f�[�^��ǂݓǂ�
		cstring_putc(pStr, 11, (hmLib_uint8)(sumadc & 0x00FF));		//Low
		cstring_putc(pStr, 12, (hmLib_uint8)((sumadc >> 8) & 0x00FF));	//High

		sumadc = adc_get_average(Vref_GND, HMR_ADC_CH6);//ID��AN�ɕϊ����Ă����̃f�[�^��ǂݓǂ�
		cstring_putc(pStr, 13, (hmLib_uint8)(sumadc & 0x00FF));		//Low
		cstring_putc(pStr, 14, (hmLib_uint8)((sumadc >> 8) & 0x00FF));	//High

		sumadc = adc_get_average(Vref_GND, HMR_ADC_CH7);//ID��AN�ɕϊ����Ă����̃f�[�^��ǂݓǂ�
		cstring_putc(pStr, 15, (hmLib_uint8)(sumadc & 0x00FF));		//Low
		cstring_putc(pStr, 16, (hmLib_uint8)((sumadc >> 8) & 0x00FF));	//High


		//���M�㎩���I�Ƀt���O�𗎂Ƃ�
		_fulladc_Mode.Data = hmLib_false;
		return hmLib_false;
	}

	return hmLib_true;
}
void fulladc_setup_listen(void){ return; }
void fulladc_setup_talk(void){
	_fulladc_Mode.Data = _fulladc_Mode.SendData;
}
void fulladc_task_setup_talk(void){ return; }
sint16 fulladc_task_data(sint16 interval){
	_fulladc_Mode.Data = _fulladc_Mode.SendData;
	return interval;
}
sint16 fulladc_task_inform(sint16 interval){
	_fulladc_Mode.SendData_i = hmLib_true;
	return interval;
}
#
#endif
