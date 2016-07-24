#ifndef HMR_DEVICE_KK07_INC
#define HMR_DEVICE_KK07_INC 103
#
/*===hmrDevice_kk07===
kk07��ŗL��pin�z�u���̐錾��ݒ�p�֐����
*/
/*===�X�V����===
hmrDevice_kk07_v1_03/130727 iwahori
	AD�|�[�g�ݒ��ύX�D
	�f�[�^�V�[�g�ɍڂ��Ă��Ȃ��̂�AD�|�[�g�ݒ�ł���������̂Œ��ӂ��K�v�DPIC�̃w�b�_�t�@�C��������̂����S
hmrDevice_kk07_v1_02/130330 iwahori
	�����|�[�g��A�����đ��삷��Ƒ��삪���s����PIC�̎d�l�Ɉ��������郊�X�N�������邽�߂�
	�s���̃Z�b�g�֐��}�N����__asm("nop")��ǉ�
	�i���Ƃ��΁CA�|�[�g��1�r�b�g�ڂ𑀍삵�āC����ɓ���A�|�[�g��3�r�b�g�ڂƂ��̑��������ƁC�����̑��삪���s����j
hmrDevice_kk07_v1_01/130223 hmIto
	hmr_uart1_initialize�֐��Q�ɂ����āCCTS�̋@�\��L����
		�ꎞ���̂��̂��߁CRF�̎�M�s����MP�pCTS�ɓ]�p
hmrDevice_kk07_v1_00/121208 hmIto
	hmrDevice_kk06_v1_08���x�[�X��kk07�Ή��ł��쐬
*/

#ifndef HMR_24EP512GU810_INC
#	//�f�o�C�X���ȉ��̌^���g���Ă��邱�Ƃ𑼃t�@�C���ɒm�点�邽�߂�define
#	define HMR_24EP512GU810_INC
#	ifdef __cplusplus
#		include "p24EP512GU810_defines.h"
#	else
#		include <p24EP512GU810.h>
#	endif
#endif
#ifndef AMPIC30_INC
#	include"amPic30_v7.h"
#endif
#ifndef AMPIC30EXTEND_INC
#	include"amPic30Extend.h"
#endif

#define itob(val) (val?1:0)
//=================================interrupt==============================
//���荞�ݏ������֐�
void hmr_interrupt_initalize(void);
//���荞�ݏI�[���֐�
void hmr_interrupt_finalize(void);
//main���[�v�ւ̊��荞�݈ꎞ��~
void hmr_main_lock_interrupt(void);
//main���[�v�ւ̑S�Ă̊��荞�݈ꎞ��~
void hmr_main_strong_lock_interrupt(void);
//main���[�v�ւ̊��荞�ݕ��A
void hmr_main_unlock_interrupt(void);
//uart1_recv�ւ̊��荞�݈ꎞ��~
void hmr_uart1_fget_lock_interrupt(void);
//uart1_recv�ւ̊��荞�ݕ��A
void hmr_uart1_fget_unlock_interrupt(void);
//uart1_send�ւ̊��荞�݈ꎞ��~
void hmr_uart1_fput_lock_interrupt(void);
//uart1_send�ւ̑S�Ă̊��荞�݈ꎞ��~
void hmr_uart1_fput_strong_lock_interrupt(void);
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
#define HMR_PIN_debugLED        PORTAbits.RA4	//
#define HMR_PIN_debugLED_set(val) HMR_PIN_debugLED=itob(val==0);__asm("nop")

#define HMR_PIN_RedLED			PORTAbits.RA5	//OUT	//RA4
//#define HMR_PIN_RedLED_set(val) (HMR_PIN_RedLED=itob(val==0);__asm("nop"))
#define HMR_PIN_RedLED_set(val) HMR_PIN_RedLED=itob(val==0);__asm("nop")

#define HMR_PIN_YellowLED		PORTAbits.RA5	//OUT
//#define HMR_PIN_YellowLED_set(val) (HMR_PIN_YellowLED=itob(val==0);__asm("nop"))
#define HMR_PIN_YellowLED_set(val) HMR_PIN_YellowLED=itob(val==0);__asm("nop")

//============================DIP SWITCH=============
#define HMR_PIN_DIP1			PORTGbits.RG0		//IN
#define HMR_PIN_DIP2			PORTGbits.RG1		//IN
#define HMR_PIN_DIP3			PORTFbits.RF1		//IN
#define HMR_PIN_DIP4			PORTFbits.RF0		//IN

//===============================ADC=================
#define HMR_ADC_CH0				20		//AD
#define HMR_ADC_CH1				21		//AD
#define HMR_ADC_CH2				5		//AD
#define HMR_ADC_CH3				4		//AD
#define HMR_ADC_CH4				3		//AD 
#define HMR_ADC_CH5				2		//AD
#define HMR_ADC_CH6				1		//AD
#define HMR_ADC_CH7				0		//signal level changeable by VR50k	AD
#define HMR_ADC_CH8				8		//Read half voltage					AD

//#define HMR_ADC_BatteryLC		26	//for battery LeftCenter motor	AD
//#define HMR_ADC_BatteryLF		25	//for battery LeftFront motor	AD
#define HMR_ADC_BatteryLR		23	//for battery LeftRrear motor	AD
//#define HMR_ADC_BatteryRC		18	//for battery RightCenter motor	AD
#define HMR_ADC_BatteryRF		31	//for battery RightFront motor	AD
//#define HMR_ADC_BatteryRR		17	//for battery RightRrear motor	AD
#define HMR_ADC_BatteryMain		25	//for battery Main board		AD


#define HMR_PIN_ADCPW_CH0to6	PORTAbits.RA0	//PowerSwitch for ADC #00 to #06	OUT
#define HMR_PIN_ADCPW_CH7		PORTBbits.RB12	//PowerSwitch for ADC #07
#define HMR_PIN_ADCPW_CH8		PORTBbits.RB14	//PowerSwitch for ADC #08
#define HMR_PIN_ADCPW_CH0to6_set(val)	HMR_PIN_ADCPW_CH0to6=itob(val!=0);__asm("nop")
#define HMR_PIN_ADCPW_CH7_set(val)		HMR_PIN_ADCPW_CH7=itob(val!=0);__asm("nop")
#define HMR_PIN_ADCPW_CH8_set(val)		HMR_PIN_ADCPW_CH8=itob(val!=0);__asm("nop")

//==============================CAMERA======================
#define HMR_PIN_CAM_PW			PORTBbits.RB13		//PowerSwitch for Camera	OUT
#define HMR_PIN_CAM_PW_set(val)			HMR_PIN_CAM_PW=itob(val!=0);__asm("nop")

//===============================CO2========================
#define HMR_PIN_PUMP_PW			PORTDbits.RD14		//PowerSwitch for pump		OUT
#define HMR_PIN_PUMP_PW_set(val)	HMR_PIN_PUMP_PW=itob(val!=0);__asm("nop")

//===============================I2C=========================
#define HMR_PIN_I2C_EXTERNA_PW			PORTFbits.RF8		//PowerSwitch for External I2C devides		OUT
#define HMR_PIN_I2C_EXTERNA_PW_set(val) HMR_PIN_I2C_EXTERNA_PW=itob(val!=0);__asm("nop")

#define HMR_PIN_I2C_ACCEL0_PW			PORTDbits.RD15		//PowerSwitch for Accele#0, Magnetic#0 I2C devides		OUT
#define HMR_PIN_I2C_ACCEL0_PW_set(val) HMR_PIN_I2C_ACCEL0_PW=itob(val!=0);__asm("nop")

#define HMR_PIN_I2C_ACCEL1_PW			PORTEbits.RE0		//PowerSwitch for Accele#1, Magnetic#1 and Gyro I2C devides		OUT
#define HMR_PIN_I2C_ACCEL1_PW_set(val) HMR_PIN_I2C_ACCEL1_PW=itob(val!=0);__asm("nop")

#define HMR_PIN_GYRO_READYDATA			PORTDbits.RD12


//===============================RF modeule===================
#define HMR_PIN_RF_PW			PORTGbits.RG12		//PowerSwitch for RF module
#define HMR_PIN_RF_PW_set(val) HMR_PIN_RF_PW=itob(val!=0);__asm("nop")

#define HMR_PIN_MP_PW			PORTEbits.RE4		//PowerSwitch for MoblePhone module
#define HMR_PIN_MP_PW_set(val) HMR_PIN_MP_PW=itob(val!=0);__asm("nop")

//==============================Servo,Motor=========================
#define HMR_PIN_SERVO_PW		PORTAbits.RA6		//PowerSwitch for Servo
#define HMR_PIN_SERVO_PW_set(val) HMR_PIN_SERVO_PW=val;__asm("nop")

#define HMR_PIN_MOTORL_A		PORTCbits.RC1
#define HMR_PIN_MOTORL_A_set(val) HMR_PIN_MOTORL_A=itob(val);__asm("nop")

#define HMR_PIN_MOTORL_B		PORTCbits.RC2
#define HMR_PIN_MOTORL_B_set(val) HMR_PIN_MOTORL_B=itob(val);__asm("nop")

#define HMR_PIN_MOTORL_PW		PORTCbits.RC3
#define HMR_PIN_MOTORL_PW_set(val) HMR_PIN_MOTORL_PW=itob(val);__asm("nop")

#define HMR_PIN_MOTORR_A		PORTDbits.RD2
#define HMR_PIN_MOTORR_A_set(val) HMR_PIN_MOTORR_A=itob(val);__asm("nop")

#define HMR_PIN_MOTORR_B		PORTDbits.RD1
#define HMR_PIN_MOTORR_B_set(val) HMR_PIN_MOTORR_B=itob(val);__asm("nop")

#define HMR_PIN_MOTORR_PW		PORTDbits.RD3
#define HMR_PIN_MOTORR_PW_set(val) HMR_PIN_MOTORR_PW=itob(val);__asm("nop")

//============================GPS PIC, Drive PIC===========
#define HMR_PIN_SubPIC_PW		PORTGbits.RG14		//PowerSwitch for GPS PIC,Drive PIC
#define HMR_PIN_SubPIC_PW_set(val) HMR_PIN_SubPIC_PW=val;__asm("nop")

//===========================�w�b�h���C�g=======================
#define HMR_PIN_HEADLIGHT_PW		PORTEbits.RE3	//PowerSwitch for Head Liht
#define HMR_PIN_HEADLIGHT_PW_set(val) HMR_PIN_HEADLIGHT_PW=itob(val!=0);__asm("nop")

//===========================SPI============================
#define HMR_PIN_GPS_SPI_SELECT	PORTGbits.RG9	//SPI(to GPS PIC) slave select pin
#define HMR_PIN_GPS_SPI_SELECT_set(val) HMR_PIN_GPS_SPI_SELECT=(!val);__asm("nop")

#define HMR_PIN_DRIVE_SPI_SELECT PORTCbits.RC4	//SPI(DRIVE PIC) slave select pin
#define HMR_PIN_DRIVE_SPI_SELECT_set(val) HMR_PIN_DRIVE_SPI_SELECT=(!val);__asm("nop")

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
