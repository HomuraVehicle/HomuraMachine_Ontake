#ifndef HMR_CO2_C_INC
#define HMR_CO2_C_INC 2
#
#define HMLIB_NOLIB
#ifndef HMR_CO2_INC
#	include"hmrCO2.h"
#endif
#ifndef HMR_UTILITY_INC
#	include "hmrUtility.h"
#endif
#ifndef HMR_DEVICE_INC
#	include"hmrDevice.h"
#endif
struct{
	unsigned Data:1;
	unsigned SendData:1;
	unsigned SendData_i:1;
	unsigned PowerPump:1;
	unsigned PowerPump_i:1;
	unsigned PowerSensor:1;
	unsigned PowerSensor_i:1;
}_co2_Mode={0,0,1,0,1,0,1};
hmLib_boolian co2_listen(cstring Str){
	switch(cstring_getc(&Str,0)){
	case 0x10://sensor on
		_co2_Mode.SendData=hmLib_true;
		_co2_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	case 0x11://sensor on
		_co2_Mode.SendData=hmLib_false;
		_co2_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	case 0x20://sensor on
		_co2_Mode.PowerSensor=hmLib_true;
		_co2_Mode.PowerSensor_i=hmLib_true;
		co2_powerSensor(1);
		return hmLib_false;
	case 0x21://sensor on
		_co2_Mode.PowerSensor=hmLib_false;
		_co2_Mode.PowerSensor_i=hmLib_true;
		co2_powerSensor(0);
		return hmLib_false;
	case 0x30://pump on
		_co2_Mode.PowerPump=hmLib_true;
		_co2_Mode.PowerPump_i=hmLib_true;
		co2_powerPump(1);
		return hmLib_false;
	case 0x31://pump off
		_co2_Mode.PowerPump=hmLib_false;
		_co2_Mode.PowerPump_i=hmLib_true;
		co2_powerPump(0);
		return hmLib_false;
	default:
		return hmLib_true;
	}
}
hmLib_boolian co2_talk(cstring* pStr){
	uint16 sumadc;
	if(_co2_Mode.SendData_i){
		cstring_construct_dynamic_safe(pStr,1);
		if(_co2_Mode.SendData)cstring_putc(pStr,0,0x10);
		else cstring_putc(pStr,0,0x11);
		_co2_Mode.SendData_i=hmLib_false;
		return hmLib_false;
	}else if(_co2_Mode.PowerSensor_i){
		cstring_construct_dynamic_safe(pStr,1);
		if(_co2_Mode.PowerSensor)cstring_putc(pStr,0,0x20);
		else cstring_putc(pStr,0,0x21);
		_co2_Mode.PowerSensor_i=hmLib_false;
		return hmLib_false;
	}else if(_co2_Mode.PowerPump_i){
		cstring_construct_dynamic_safe(pStr,1);
		if(_co2_Mode.PowerPump)cstring_putc(pStr,0,0x30);
		else cstring_putc(pStr,0,0x31);
		_co2_Mode.PowerPump_i=hmLib_false;
		return hmLib_false;
	}else if(_co2_Mode.Data){
		cstring_construct_dynamic_safe(pStr,3);
		cstring_putc(pStr,0,0x00);
		// CO2ƒf[ƒ^Žæ“¾
		sumadc = adc_get_average(Vref_GND,ADC_CO2);
		cstring_putc(pStr,1,(unsigned char)(sumadc&0x00FF));
		cstring_putc(pStr,2,(unsigned char)((sumadc>>8)&0x00FF));
		_co2_Mode.Data=hmLib_false;
		return hmLib_false;
	}
	return hmLib_true;
}
void co2_setup_listen(void){return;}
void co2_setup_talk(void){
	_co2_Mode.Data=_co2_Mode.SendData;
}
void co2_task_setup_talk(void){return;}
sint16 co2_task_data(sint16 interval){
	_co2_Mode.Data=_co2_Mode.SendData;
	return interval;
}
sint16 co2_task_inform(sint16 interval){
	_co2_Mode.SendData_i=hmLib_true;
	_co2_Mode.PowerPump_i=hmLib_true;
	_co2_Mode.PowerSensor_i=hmLib_true;
	return interval;
}

#
#endif
