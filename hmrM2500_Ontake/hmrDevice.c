#ifndef HMR_DEVICE_C_INC
#define HMR_DEVICE_C_INC 102
#
#define HMLIB_NOLIB
#ifndef HMR_DEVICE_INC
#	include "hmrDevice.h"
#endif
//==================I2C===============================
void i2c_power(boolian onoff){
	 HMR_PIN_I2C_ACCEL0_PW_set(onoff);
	 HMR_PIN_I2C_ACCEL1_PW_set(onoff);
	 HMR_PIN_I2C_EXTERNA_PW_set(onoff);
}

//==================ADC===============================
//ADC��100�񕽋ώ擾�p�֐�
uint32 adc_get_average(uint8 mode, uint8 ch){
	sint8 j = 0;
	uint32 sumadc;

	setup_adc1_Vref(mode);//MODE�ݒ�

	set_adc_channel(ch);//ADCCh�I�� 

	sumadc = 0;
	for(j=0;j<100;++j){
		sumadc += (uint16)(read_adc());
	}
	sumadc = sumadc/100;
	return sumadc;
}

//===============Battery==============================
uint8 battery_id_to_adc(uint8 BatteryID){
	switch(BatteryID){
	case 0:
		return HMR_ADC_BatteryMain;
	case 1:
		return HMR_ADC_BatteryLR;
	case 2:
		return HMR_ADC_BatteryRF;
	default:
		return 0xff;//�g���ĂȂ�PIN nennnotame
	}
}

//==============Select UART1==================================
//���݃A�N�e�B�u��ID
unsigned char _uart1_ModuleID=UART1_MODULEID_NULL;
//ModuleID�̃��W���[���ŏ���������
void hmr_uart1_initialize(unsigned char ModuleID_,uint16 BaudRate_){
	if(ModuleID_==UART1_MODULEID_MP){
		_uart1_ModuleID=UART1_MODULEID_MP;
		hmr_uart1_initialize_withMP(BaudRate_);
	}else if(ModuleID_==UART1_MODULEID_RF){
		_uart1_ModuleID=UART1_MODULEID_RF;
		hmr_uart1_initialize_withRF(BaudRate_);
	}
}
//ModuleID�ɓd����ύX����
void hmr_uart1_power_switch(boolian onoff,unsigned char ModuleID_){
	if(onoff && ModuleID_==UART1_MODULEID_MP){
		mp_power(1);
		rf_power(0);
	}else if(onoff && ModuleID_==UART1_MODULEID_RF){
		mp_power(0);
		rf_power(1);
	}else{
		mp_power(0);
		rf_power(0);
	}
}
//���݂�ModuleID���擾����
unsigned char hmr_uart1_getModuleID(void){return _uart1_ModuleID;}

//==============Clock==========================
//���݃A�N�e�B�u��ID
unsigned char _clock_ModuleID=CLOCK_NULL;
void hmr_clock_initialize(unsigned char Mode_){
	switch(Mode_){
	case CLOCK_SLOW:
		hmr_clock_initialize_withSlow();
		_clock_ModuleID=Mode_;
		return;
	case CLOCK_NORMAL:
		hmr_clock_initialize_withNormal();
		_clock_ModuleID=Mode_;
		return;
	case CLOCK_FAST:
		hmr_clock_initialize_withFast();
		_clock_ModuleID=Mode_;
		return;
	}
}
//���݂�ModuleID���擾����
unsigned char hmr_clock_getModuleID(void){return _clock_ModuleID;}
#
#endif
