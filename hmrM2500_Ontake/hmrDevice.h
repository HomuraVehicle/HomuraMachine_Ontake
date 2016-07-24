#ifndef HMR_DEVICE_INC
#define HMR_DEVICE_INC 102
#
/*
===hmrDevice===
hmrDevice_XXX.h��include���āA
�u�f�o�C�X��ˑ������A�^�p���@���ŕύX����A�L�͂ɉe�����o��\��������v
�悤�Ȑ錾�A�ϐ��A�֐�����񋟂���B
��
	* Device�񋟂�Pin���錾�ɖ�����^���Ă����錾
	* Device�񋟂̊֐��𕡐��܂Ƃ߂�Ȃǂ��ă��b�s���O
	* Device�񋟂̊֐��𗘗p�����A����ʓI�ȗ��p�`�Ԃ̊֐���
*/
/*
=== hmrDevice ===
v1_02/131102 hmIto
	�S��d�ʐM�ɑΉ����Ă��邩�������Ahmr_isFullDuplex��ǉ�
v1_01/131026 amby
	���̒��ŁA�J�������^�C�}�[1���g���āE�E�E�Ƃ����悤�ȃf�o�C�X�ˑ��������ׂĒ�`�ł���悤�ɂ������߂Ă����r��
	���荞�݊֐��ɂ��Ď�ɑΉ���
v1_00/131019 hmIto
	DeviceConfig�������
=== hmrDeviceConfig ===
v1_09/130112 iwahori
	���[�^�[����̃s��define�ǉ��AhmrDevice_kk06��include����Ă����̂�kk07�ɕύX
	�J�����̃w�b�h���C�g�̃s��define�ǉ�
v1_08/121101 hmIto
	restart_wdt�p�~�imain-loop�ɏW��j
v1_07/121028 hmIto
	sprite_Stream��Stream_CMR�ɕύX
	co2/h2s��power�֐����}�N���ɒu��
	�֐�adc_BatteryID_to_ch��battery_ID_to_adc�ɕύX
	servo��i2c��power�֐��}�N����ǉ�
	uart1_initialize,uart1_getModuleID��ǉ�
		ModuleID�ɂĐ؊������䂪�\
		�ꊇ���āA�d���Ȃǂ��Ǘ�
v1_06/121020 iwahori
	c�t�@�C���̂ق���adc_BatteryID_to_adch��h�t�@�C���ɍ��킹��adc_BatteryID_to_ch�ɕύX
	c�t�@�C���̂ق���adc_BatteryID_to_ch�̈�������ID�������̂�BattelyID�ɕύX
	h2s_powerSensor()�ǉ��s��������Ȃ̂Ŗ�����
	rf_power,manipu_power,subPic_power c�t�@�C���̑啶���͂��܂���������ɕύX
v1_05/121020 hmIto
	ADC��define�ɂƂ肠�����̃`�����l���ԍ���錾
		�������A���ۂɂ����̃Z���T�[���g���ꍇ�́AADC���w�肷��K�v����
v1_04/121014 iwahori
	Stream�̖��̕ύX
	void spi_selectDrive(boolian onoff){HMR_PIN_GPS_DRIVE_SELECT_set(onoff)
	HMR_PIN_GPS_DRIVE_SELECT��HMR_PIN_DRIVE_SPI_SELECT�ɕύX
	AD�s����define��ǉ�
v1_03/121013 hmIto
	ADC_Battery#�̖��̂�ύX
	power�֐���ǉ�
v1_02/121005 hmIto
	�����ƁA�o�[�W�����ԍ��グ��
	�������n�֐��Q��hmrDevice_kk06��Device_initialize�Ɉڍs
v1_01/120922 iwahori
	�ҏW�J�n
	AD_ini()�������CAD�|�[�g�̐ݒ��hmrDevice_kk**.c ��Device_initialize()�Ɏ����ς�
v1_00/120921 hmIto
	�Ƃ肠������{�C���[�W���������o���B
*/
#ifndef HMCLIB_BYTEBASETYPE_OMC
#	include"hmLib_v3_04/bytebase_type.h"
#endif
//#ifndef HMR_DEVICR_KK06_INC
#ifndef HMR_DEVICR_KK08_INC
#	include "hmrDevice_kk08.h"
#endif

//=================UART===============================
#define Stream_VMC HMR_STREAM_CH1
#define Stream_CMR HMR_STREAM_CH2

//================ ���荞�ݐ���n ====================
// Main stream �p����֐�
#define streamVMC_enable_fget_interrupt()		uart1_enable_fget_interrupt()
#define streamVMC_disable_fget_interrupt()		uart1_disable_fget_interrupt()
#define streamVMC_set_fget_interrupt_flag()		uart1_set_fget_interrupt_flag()
#define streamVMC_clear_fget_interrupt_flag()	uart1_clear_fget_interrupt_flag()
#define streamVMC_fget_interrupt				uart1_fget_interrupt

#define streamVMC_enable_fput_interrupt()		uart1_enable_fput_interrupt()
#define streamVMC_disable_fput_interrupt()		uart1_disable_fput_interrupt()
#define streamVMC_set_fput_interrupt_flag()		uart1_set_fput_interrupt_flag()
#define streamVMC_clear_fput_interrupt_flag()	uart1_clear_fput_interrupt_flag()
#define streamVMC_fput_interrupt				uart1_fput_interrupt
#define streamVMC_is_enable_fput_interrupt()	uart1_is_enable_fput_interrupt()

// CMR stream �p����֐�
#define streamCMR_enable_fget_interrupt()		uart2_enable_fget_interrupt()
#define streamCMR_disable_fget_interrupt()		uart2_disable_fget_interrupt()
#define streamCMR_set_fget_interrupt_flag()		uart2_set_fget_interrupt_flag()
#define streamCMR_clear_fget_interrupt_flag()	uart2_clear_fget_interrupt_flag()
#define streamCMR_fget_interrupt				uart2_fget_interrupt

#define streamCMR_enable_fput_interrupt()		uart2_enable_fput_interrupt()
#define streamCMR_disable_fput_interrupt()		uart2_disable_fput_interrupt()
#define streamCMR_set_fput_interrupt_flag()		uart2_set_fput_interrupt_flag()
#define streamCMR_clear_fput_interrupt_flag()	uart2_clear_fput_interrupt_flag()
#define streamCMR_fput_interrupt				uart2_fput_interrupt

// CMR timer �p����֐�
#define timerCMR_reset()					timer1_reset()
#define timerCMR_interrupt					timer1_interrupt
#define timerCMR_disable_interrupt()		timer1_disable_interrupt()
#define timerCMR_enable_interrupt()			timer1_enable_interrupt()
#define timerCMR_clear_interrupt_flag()		timer1_clear_interrupt_flag()
#define timerCMR_set_interrupt_flag()		timer1_set_interrupt_flag()
#define timerCMR_initialize(Time)			timer1_initialize(Time)

// DeviceManager Timer �p����֐�
#define timerDevmng_reset()					timer4_5_reset()
#define timerDevmng_interrupt				timer4_5_interrupt
#define timerDevmng_disable_interrupt()		timer4_5_disable_interrupt()
#define timerDevmng_enable_interrupt()		timer4_5_enable_interrupt()
#define timerDevmng_clear_interrupt_flag()	timer4_5_clear_interrupt_flag()
#define timerDevmng_set_interrupt_flag()	timer4_5_set_interrupt_flag()
#define timerDevmng_initialize(Time)		timer4_5_initialize(Time)

//================Delay�n�֐�=========================
//#define delay_ms_main(Time) delay_ms(Time)
#define delay_ms_sub(Time) delay_ms2(Time) 

//================ADC=================================
#define adc_power(onoff) HMR_PIN_ADCPW_CH0to6_set(onoff)
//ADC�`�����l���ݒ�
#define ADC_CO2 HMR_ADC_CH8					//CO2 AD�`�����l��
#define ADC_AxelX 0xFF
#define ADC_AxelY 0xFF
#define ADC_AxelZ 0xFF
#define ADC_Thermo HMR_ADC_CH0
#define ADC_InfThermo_thermo 0xFF
#define ADC_InfThermo 0xFF
#define ADC_Hygro 0xFF
#define ADC_Compass 0xFF
#define ADC_H2S 0xFF
//#define light_power(onoff) HMR_ADC_CH1_SIGNAL_set(onoff)
//ADC��100�񕽋ώ擾�p�֐�
uint32 adc_get_average(uint8 mode, uint8 ch);

//================CO2=================================
#define co2_powerPump(onoff) HMR_PIN_PUMP_PW_set(onoff)
#define co2_powerSensor(onoff) HMR_PIN_ADCPW_CH8_set(onoff)

//================H2S=================================
#define h2s_powerSensor(onoff)

//===============Battery==============================
uint8 battery_id_to_adc(uint8 BatteryID);

//==============Sprite Camera================================
#define sprite_setLED(onoff) HMR_PIN_YellowLED_set(onoff)
#define sprite_power(onoff) HMR_PIN_CAM_PW_set(onoff)
#define sprite_powerLight(onoff) HMR_PIN_HEADLIGHT_PW_set(onoff)

//===============================I2C=========================
void i2c_power(boolian onoff);

//==============================Servo=========================
//#define servo_power(onoff) HMR_PIN_SERVO_PW_set(onoff)

//===========================motor===========================
#define motor_powerLA(onoff) HMR_PIN_MOTORL_A_set(onoff)
#define motor_powerLB(onoff) HMR_PIN_MOTORL_B_set(onoff)
#define motor_LPW(onoff) HMR_PIN_MOTORL_PW_set(onoff)

#define motor_powerRA(onoff) HMR_PIN_MOTORR_A_set(onoff)
#define motor_powerRB(onoff) HMR_PIN_MOTORR_B_set(onoff)
#define motor_RPW(onoff) HMR_PIN_MOTORR_PW_set(onoff)

//===============================RF modeule===================
#define rf_power(onoff) HMR_PIN_RF_PW_set(onoff)

//==========================Mobile Phone module===============
#define mp_power(onoff) HMR_PIN_MP_PW_set(onoff)

//==============GPS PIC, Drive PIC===========================
#define subPIC_power(onoff) HMR_PIN_SubPIC_PW_set(onoff)

//==============SPI==========================================
#define spi_selectGPS(onoff) HMR_PIN_GPS_SPI_SELECT_set(onoff)
#define spi_selectDrive(onoff) HMR_PIN_DRIVE_SPI_SELECT_set(onoff)

//==============UserConfig==================================
#define Config_DisableWatchDogTimer (!(HMR_PIN_DIP1))
#define Config_DefaultMobilePhone (!(HMR_PIN_DIP2))
#define Config_DefaultHighClock (!(HMR_PIN_DIP3))
#define Config_RFDebugMode (!(HMR_PIN_DIP4))
boolian hmr_isFullDuplex(void);

//==============Select UART1==================================
#define UART1_MODULEID_NULL 0x00
#define UART1_MODULEID_MP 0x01
#define UART1_MODULEID_RF 0x02
//ModuleID�̃��W���[���ŏ���������
void hmr_uart1_initialize(unsigned char ModuleID_,uint16 BaudRate_);
//ModuleID�ɓd����ύX����
void hmr_uart1_power_switch(boolian onoff,unsigned char ModuleID_);
//���݂�ModuleID���擾����
unsigned char hmr_uart1_getModuleID(void);

//==============Clock==========================
#define CLOCK_NULL 0x00
#define CLOCK_SLOW 0x01
#define CLOCK_NORMAL 0x02
#define CLOCK_FAST 0x03
//�N���b�N���w�胂�[�h�ŏ���������
void hmr_clock_initialize(unsigned char Mode_);
//���݂�ModuleID���擾����
unsigned char hmr_clock_getModuleID(void);
#
#endif
