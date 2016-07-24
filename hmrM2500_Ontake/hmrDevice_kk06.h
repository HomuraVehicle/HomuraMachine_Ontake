#ifndef HMR_DEVICE_KK06_INC
#define HMR_DEVICE_KK06_INC 108
#
/*===hmrDevice_kk06===
kk06��ŗL��pin�z�u���̐錾��ݒ�p�֐����
*/
/*===�X�V����===
hmrDevice_kk06_v1_00/120921 iwahori
	memo:SPI select pin �͐��_���H�@kk05�ł͕��_���C��H���ɂ͋L�ڂȂ�
hmrDevice_kk06_v1_01/121005 hmIto
	hmrDeviceConfig�ɂ������f�o�C�X�ˑ��̏����������n�֐��Q���ڍs
hmrDevice_kk06_v1_02/121013 hmIto
	���O��Ԃ�Device����device�ɕύX
	���荞�݊֐��̐錾��ǉ�
	spi�ʐM��set�֐��𕉘_���ɕύX
hmrDevice_kk06_v1_03/121014 iwahori
	HMR_PIN_SubPIC_PW_set(val) (HMR_PIN_subPIC_PW=val)
	��ύXsub��s��啶���ɕύX
	HMR_PIN_GPS_SPI_SELECT PORTGbis.RG9 �X�y���~�XGbis�ύX
	HMR_PIN_GPS_DRIVE_SELECT PORTCbis.RC4 HMR_PIN_DRIVE_SPI_SELECT PORTCbits.RC4�C�}�N�������l
hmrDevice_kk06_v1_04/121020 iwahori
	initialze�ɃN���b�N�ݒ�ǉ�
hmrDevice_kk06_v1_05/121027 hmIto
	initialize�֐��𐮗�
	uart1/2_pinselect�֐���ǉ�
	uart1_send/recv_lock/unlock��uart1_fput/fget_lock/unlock�ɕύX
	�D�揇�ʌ���֐��ȊO�̊��荞�ݐ���֐���amPic30Extend�Ɉړ�
hmrDevice_kk06_v1_06/121028 hmIto
	device_initialize/finalize�֐����Ȃ����Amodule�P�ʂł̏��������\�ɕύX
		device:�f�o�C�X�̒��j�����̏�����/�I�[�� �v���O�����J�n����A�I�����O�Ɏ��s
		clock:��̃N���b�N�X�s�[�h�ł̏��������
		uart1/2,spi2,i2c,adc:�ʐM�n�̏������֐�
hmrDevice_kk06_v1_07/121030 iwahori
	configuration bit��_FPOR()��ύXALTI2C2_ON
	�iASDA2/ASCK2 pins are selected as the I/O pins for I2C2�j��ǉ�
	�֐��̖��O��Ԃ�hmr�Ɉړ�,hmr_adc_initialize������
hmrDevice_kk06_v1_08/121101 hmIto
	RG3��input��p�������̂ŁARG2��ServoPower�o�̓s����ύX
*/

#ifndef HMR_24EP512GU810_INC
#	//�f�o�C�X���ȉ��̌^���g���Ă��邱�Ƃ𑼃t�@�C���ɒm�点�邽�߂�define
#	define HMR_24EP512GU810_INC
#	include <p24EP512GU810.h>
#endif
#ifndef AMPIC30_INC
#	include"amPic30_v7.h"
#endif
#ifndef AMPIC30EXTEND_INC
#	include"amPic30Extend.h"
#endif

//=================================interrupt==============================
//���荞�ݏ������֐�
void hmr_interrupt_initalize(void);
//���荞�ݏI�[���֐�
void hmr_interrupt_finalize(void);
//main���[�v�ւ̊��荞�݈ꎞ��~
void hmr_main_lock_interrupt(void);
//main���[�v�ւ̊��荞�ݕ��A
void hmr_main_unlock_interrupt(void);
//uart1_recv�ւ̊��荞�݈ꎞ��~
void hmr_uart1_fget_lock_interrupt(void);
//uart1_recv�ւ̊��荞�ݕ��A
void hmr_uart1_fget_unlock_interrupt(void);
//uart1_send�ւ̊��荞�݈ꎞ��~
void hmr_uart1_fput_lock_interrupt(void);
//uart1_send�ւ̊��荞�ݕ��A
void hmr_uart1_fput_unlock_interrupt(void);
//timer1�ւ̊��荞�݈ꎞ��~
void hmr_timer1_lock_interrupt(void);
//timer1�ւ̊��荞�ݕ��A
void hmr_timer1_unlock_interrupt(void);
//timer5�ւ̊��荞�݈ꎞ��~
void hmr_timer4_lock_interrupt(void);
//timer5�ւ̊��荞�ݕ��A
void hmr_timer4_unlock_interrupt(void);
//=============================Stream===============
#define HMR_STREAM_CH1 1
#define HMR_STREAM_CH2 2
//=================================LED===============
#define HMR_PIN_RedLED PORTAbits.RA4	//OUT
#define HMR_PIN_RedLED_set(val) (HMR_PIN_RedLED=val)

#define HMR_PIN_YellowLED PORTAbits.RA5	//OUT
#define HMR_PIN_YellowLED_set(val) (HMR_PIN_YellowLED=val)

//============================DIP SWITCH=============
#define HMR_PIN_DIP1 PORTGbits.RG0		//IN
#define HMR_PIN_DIP2 PORTGbits.RG1		//IN
#define HMR_PIN_DIP3 PORTFbits.RF1		//IN
#define HMR_PIN_DIP4 PORTFbits.RF0		//IN

//===============================ADC=================
#define HMR_ADC_CH0 20		//AD
#define HMR_ADC_CH1 21		//AD
#define HMR_ADC_CH1_SIGNAL PORTEbits.RE9
#define HMR_ADC_CH1_SIGNAL_set(onoff) (HMR_ADC_CH1_SIGNAL=onoff)
#define HMR_ADC_CH2 5		//AD
#define HMR_ADC_CH2_SIGNAL LATBbits.RB5
#define HMR_ADC_CH2_SIGNAL_set(onoff) (HMR_ADC_CH2_SIGNAL=onoff)
#define HMR_ADC_CH3 4		//AD
#define HMR_ADC_CH3_SIGNAL LATBbits.RB4
#define HMR_ADC_CH3_SIGNAL_set(onoff) (HMR_ADC_CH3_SIGNAL=onoff)
#define HMR_ADC_CH4 3		//AD 
#define HMR_ADC_CH4_SIGNAL LATBbits.RB3
#define HMR_ADC_CH4_SIGNAL_set(onoff) (HMR_ADC_CH4_SIGNAL=onoff)
#define HMR_ADC_CH5 2		//AD
#define HMR_ADC_CH5_SIGNAL LATBbits.RB2
#define HMR_ADC_CH5_SIGNAL_set(onoff) (HMR_ADC_CH5_SIGNAL=onoff)
#define HMR_ADC_CH6 1		//AD
#define HMR_ADC_CH7 0		//signal level changeable by VR50k	AD
#define HMR_ADC_CH8 6		//Read half voltage					AD

#define HMR_ADC_BatteryLC 26	//for battery LeftCenter motor	AD
#define HMR_ADC_BatteryLF 25	//for battery LeftFront motor	AD
#define HMR_ADC_BatteryLR 24	//for battery LeftRrear motor	AD
#define HMR_ADC_BatteryRC 18	//for battery RightCenter motor	AD
#define HMR_ADC_BatteryRF 16	//for battery RightFront motor	AD
#define HMR_ADC_BatteryRR 17	//for battery RightRrear motor	AD
#define HMR_ADC_BatteryMain 22	//for battery Main board		AD

#define HMR_PIN_ADCPW_CH0to6 PORTAbits.RA0	//PowerSwitch for ADC #00 to #06	OUT
#define HMR_PIN_ADCPW_CH0to6_set(val) (HMR_PIN_ADCPW_CH0to6=val)

#define HMR_PIN_ADCPW_CH7 PORTBbits.RB12	//PowerSwitch for ADC #07
#define HMR_PIN_ADCPW_CH7_set(val) (HMR_PIN_ADCPW_CH7=val)

#define HMR_PIN_ADCPW_CH8 PORTBbits.RB14	//PowerSwitch for ADC #08
#define HMR_PIN_ADCPW_CH8_set(val) (HMR_PIN_ADCPW_CH8=val)

//==============================CAMERA======================
#define HMR_PIN_CAM_PW PORTBbits.RB13		//PowerSwitch for Camera	OUT
#define HMR_PIN_CAM_PW_set(val) (HMR_PIN_CAM_PW=val)

//===============================CO2========================
#define HMR_PIN_PUMP_PW PORTDbits.RD15		//PowerSwitch for pump		OUT
#define HMR_PIN_PUMP_PW_set(val) (HMR_PIN_PUMP_PW=val)

//===============================I2C=========================
#define HMR_PIN_I2C_PW PORTFbits.RF8		//PowerSwitch for External I2C devides		OUT
#define HMR_PIN_I2C_PW_set(val) (HMR_PIN_I2C_PW=val)

//===============================RF modeule===================
#define HMR_PIN_RF_PW PORTBbits.RB7			//PowerSwitch for RF module
#define HMR_PIN_RF_PW_set(val) (HMR_PIN_RF_PW=val)

//==============================Servo=========================
#define HMR_PIN_SERVO_PW PORTDbits.RD9		//PowerSwitch for Servo
#define HMR_PIN_SERVO_PW_set(val) (HMR_PIN_SERVO_PW=val)

//============================GPS PIC, Drive PIC===========
#define HMR_PIN_SubPIC_PW PORTAbits.RA9		//PowerSwitch for GPS PIC,Drive PIC
#define HMR_PIN_SubPIC_PW_set(val) (HMR_PIN_SubPIC_PW=val)

#define HMR_PIN_GPS_SPI_SELECT PORTGbits.RG9	//SPI(to GPS PIC) slave select pin
#define HMR_PIN_GPS_SPI_SELECT_set(val) (HMR_PIN_GPS_SPI_SELECT=(!val))

#define HMR_PIN_DRIVE_SPI_SELECT PORTCbits.RC4	//SPI(DRIVE PIC) slave select pin
#define HMR_PIN_DRIVE_SPI_SELECT_set(val) (HMR_PIN_DRIVE_SPI_SELECT=(!val))

//===========================�f�o�C�X�J�[�l��=========================
void hmr_device_initialize(void);
void hmr_device_finalize(void);

//===============================�N���b�N===============================
void hmr_clock_initialize_withNormal(void);
void hmr_clock_initialize_withSlow(void);
void hmr_clock_finalize(void);

//===============================�ʐM�ݒ�n=============================
//��������N���b�N�ύX��͏��������K�v
void hmr_uart1_initialize_withRF(void);
void hmr_uart1_initialize_withMP(void);
void hmr_uart1_finalize(void);
void hmr_uart2_initialize(void);
void hmr_uart2_finalize(void);
void hmr_spi2_initialize(void);
void hmr_spi2_finalize(void);
void hmr_i2c_initialize(void);
void hmr_i2c_finalize(void);
//==============================AD�ϊ�==================================
void hmr_adc_initialize(void);
void hmr_adc_finalize(void);
#
#endif
