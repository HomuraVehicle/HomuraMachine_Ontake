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
	unsigned Data:1;				//データを送信時に落ちる
	unsigned SendData:1;			//データ送信モード
	unsigned SendData_i:1;			//データ送信モード受理
}_battery_Mode={0,0,1};
hmLib_boolian battery_listen(cstring Str){
	//データサイズ確認
	if(cstring_size(&Str)==0)return hmLib_true;

	//1byte目でモードを分ける
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

		//送信後自動的にフラグを落とす
		_battery_Mode.SendData_i=hmLib_false;
		return hmLib_false;
	}else if(_battery_Mode.Data){
		cstring_construct_dynamic_safe(pStr,1+2*3);
		cstring_putc(pStr,0,0x00);

		//データ取得
		sumadc = adc_get_average(Vref_GND,battery_id_to_adc(0));//IDをANに変換してそこのデータを読み読み
		cstring_putc(pStr,1,(hmLib_uint8)(sumadc&0x00FF));		//Low
		cstring_putc(pStr,2,(hmLib_uint8)((sumadc>>8)&0x00FF));	//High

		sumadc = adc_get_average(Vref_GND,battery_id_to_adc(1));//IDをANに変換してそこのデータを読み読み
		cstring_putc(pStr,3,(hmLib_uint8)(sumadc&0x00FF));		//Low
		cstring_putc(pStr,4,(hmLib_uint8)((sumadc>>8)&0x00FF));	//High

		sumadc = adc_get_average(Vref_GND,battery_id_to_adc(2));//IDをANに変換してそこのデータを読み読み
		cstring_putc(pStr,5,(hmLib_uint8)(sumadc&0x00FF));		//Low
		cstring_putc(pStr,6,(hmLib_uint8)((sumadc>>8)&0x00FF));	//High

		//送信後自動的にフラグを落とす
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
