#ifndef HMR_DEVICE_KK07_INC
#define HMR_DEVICE_KK07_INC 103
#
/*===hmrDevice_kk07===
kk07基板固有のpin配置等の宣言や設定用関数を提供
*/
/*===更新履歴===
hmrDevice_kk07_v1_03/130727 iwahori
	ADポート設定を変更．
	データシートに載っていないのにADポート設定できるやつがいるので注意が必要．PICのヘッダファイルを見るのが安心
hmrDevice_kk07_v1_02/130330 iwahori
	同じポートを連続して操作すると操作が失敗するPICの仕様に引っかかるリスクをさけるために
	ピンのセット関数マクロに__asm("nop")を追加
	（たとえば，Aポートの1ビット目を操作して，直後に同じAポートの3ビット目とかの操作をすると，両方の操作が失敗する）
hmrDevice_kk07_v1_01/130223 hmIto
	hmr_uart1_initialize関数群において，CTSの機能を有効化
		一時しのぎのため，RFの受信ピンをMP用CTSに転用
hmrDevice_kk07_v1_00/121208 hmIto
	hmrDevice_kk06_v1_08をベースにkk07対応版を作成
*/

#ifndef HMR_24EP512GU810_INC
#	//デバイスが以下の型を使っていることを他ファイルに知らせるためのdefine
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
//割り込み初期化関数
void hmr_interrupt_initalize(void);
//割り込み終端化関数
void hmr_interrupt_finalize(void);
//mainループへの割り込み一時停止
void hmr_main_lock_interrupt(void);
//mainループへの全ての割り込み一時停止
void hmr_main_strong_lock_interrupt(void);
//mainループへの割り込み復帰
void hmr_main_unlock_interrupt(void);
//uart1_recvへの割り込み一時停止
void hmr_uart1_fget_lock_interrupt(void);
//uart1_recvへの割り込み復帰
void hmr_uart1_fget_unlock_interrupt(void);
//uart1_sendへの割り込み一時停止
void hmr_uart1_fput_lock_interrupt(void);
//uart1_sendへの全ての割り込み一時停止
void hmr_uart1_fput_strong_lock_interrupt(void);
//uart1_sendへの割り込み復帰
void hmr_uart1_fput_unlock_interrupt(void);
//timer1への割り込み一時停止
void hmr_timer1_lock_interrupt(void);
//timer1への割り込み復帰
void hmr_timer1_unlock_interrupt(void);
//timer5への割り込み一時停止
void hmr_timer4_lock_interrupt(void);
//timer5への割り込み復帰
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

//===========================ヘッドライト=======================
#define HMR_PIN_HEADLIGHT_PW		PORTEbits.RE3	//PowerSwitch for Head Liht
#define HMR_PIN_HEADLIGHT_PW_set(val) HMR_PIN_HEADLIGHT_PW=itob(val!=0);__asm("nop")

//===========================SPI============================
#define HMR_PIN_GPS_SPI_SELECT	PORTGbits.RG9	//SPI(to GPS PIC) slave select pin
#define HMR_PIN_GPS_SPI_SELECT_set(val) HMR_PIN_GPS_SPI_SELECT=(!val);__asm("nop")

#define HMR_PIN_DRIVE_SPI_SELECT PORTCbits.RC4	//SPI(DRIVE PIC) slave select pin
#define HMR_PIN_DRIVE_SPI_SELECT_set(val) HMR_PIN_DRIVE_SPI_SELECT=(!val);__asm("nop")

//===========================デバイスカーネル=========================
void hmr_device_initialize(void);
void hmr_device_finalize(void);

//===============================クロック===============================
void hmr_clock_initialize_withNormal(void);
void hmr_clock_initialize_withSlow(void);
void hmr_clock_finalize(void);

//===============================通信設定系=============================
//いずれもクロック変更後は初期化が必要
void hmr_uart1_initialize_withRF(void);
void hmr_uart1_initialize_withMP(void);
void hmr_uart1_finalize(void);
void hmr_uart2_initialize(void);
void hmr_uart2_finalize(void);
void hmr_spi2_initialize(void);
void hmr_spi2_finalize(void);
void hmr_i2c_initialize(void);
void hmr_i2c_finalize(void);
//==============================AD変換==================================
void hmr_adc_initialize(void);
void hmr_adc_finalize(void);
#
#endif
