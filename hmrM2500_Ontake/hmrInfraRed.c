#ifndef HMR_INFRARED_C_INC
#define HMR_INFRARED_C_INC 2
#
#define HMLIB_NOLIB
#ifndef HMR_INFRARED_INC
#	include"hmrInfraRed.h"
#endif
#ifndef HMR_DEVICE_INC
#	include"hmrDevice.h"
#endif
#ifndef HMR_UTILITY_INC
#	include "hmrUtility.h"
#endif
struct{
	unsigned Data:1;				//データを送信時に落ちる
	unsigned SendData:1;			//データ送信モード
	unsigned SendData_i:1;			//データ送信モード受理
}_infrared_Mode={0,0,1};
hmLib_boolian infrared_listen(cstring Str){
	//データサイズ確認
	if(cstring_size(&Str)<=1)return hmLib_true;

	//1byte目でモードを分ける
	switch(cstring_getc(&Str,0)){
	case 0x10:
		_infrared_Mode.SendData=hmLib_true;
		_infrared_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	case 0x11:
		_infrared_Mode.SendData=hmLib_false;
		_infrared_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	default:
		return hmLib_true;
	}
}
hmLib_boolian infrared_talk(cstring* pStr){
	hmLib_uint16 sumadc;

	if(_infrared_Mode.SendData_i){
		cstring_construct_dynamic_safe(pStr,1);
		if(_infrared_Mode.SendData)cstring_putc(pStr,0,0x10);
		else cstring_putc(pStr,0,0x11);

		//送信後自動的にフラグを落とす
		_infrared_Mode.SendData_i=hmLib_false;
		return hmLib_false;
	}else if(_infrared_Mode.Data){
		cstring_construct_dynamic_safe(pStr,5);
		cstring_putc(pStr,0,0x00);

		// 外部温度取得
		sumadc = adc_get_average(Vref_GND,ADC_InfThermo_thermo);
		cstring_putc(pStr,1,(unsigned char)(sumadc%256));
		cstring_putc(pStr,2,(unsigned char)((sumadc>>8)%256));

		//内部温度取得
		sumadc = adc_get_average(Vref_GND,ADC_InfThermo);
		cstring_putc(pStr,3,(unsigned char)(sumadc%256));
		cstring_putc(pStr,4,(unsigned char)((sumadc>>8)%256));

		//送信後自動的にフラグを落とす
		_infrared_Mode.Data=hmLib_false;
		return hmLib_false;
	}

	return hmLib_true;
}
void infrared_setup_listen(void){return;}
void infrared_setup_talk(void){
	_infrared_Mode.Data=_infrared_Mode.SendData;
}
void infrared_task_setup_talk(void){return;}
sint16 infrared_task_data(sint16 interval){
	_infrared_Mode.Data=_infrared_Mode.SendData;
	return interval;
}
sint16 infrared_task_inform(sint16 interval){
	_infrared_Mode.SendData_i=hmLib_true;
	return interval;
}

#
#endif
