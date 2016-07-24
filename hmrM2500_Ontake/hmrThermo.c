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
	unsigned Data:1;				//データを送信時に落ちる
	unsigned SendData:1;			//データ送信モード
	unsigned SendData_i:1;			//データ送信モード受理
}_thermo_Mode={0,0,1};
hmLib_boolian thermo_listen(cstring Str){

	//データサイズ確認
	if(cstring_size(&Str)==0)return hmLib_true;

	//1byte目でモードを分ける
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

		//送信後自動的にフラグを落とす
		_thermo_Mode.SendData_i=hmLib_false;
		return hmLib_false;
	}else if(_thermo_Mode.Data){
		cstring_construct_dynamic_safe(pStr,3);
		cstring_putc(pStr,0,0x00);

		//温度データ取得
		sumadc = adc_get_average(Vref_GND,ADC_Thermo);
		cstring_putc(pStr,1,(unsigned char)(sumadc%256));
		cstring_putc(pStr,2,(unsigned char)((sumadc>>8)%256));

		//送信後自動的にフラグを落とす
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
