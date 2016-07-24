#ifndef HMR_DEVICE_KK09R2_INC
#define HMR_DEVICE_KK09R2_INC 102
#
/*===hmrDevice_kk09R2===
kk09Revision2基板固有のpin配置等の宣言や設定用関数を提供

カメラは4台つなげる．2ビットでカメラを選ぶ
S1 S0 CAMERA ch
0	0	#0
0	1	#1
1	0	#2	コネクタまだなし(2013/12/15)
1	1	#3	コネクタまだなし(2013/12/15)
電源OFFにするにはつないでいないchにする(たとえば，S1=S0=1)

SPIは2つ使う：
SPI#1は24bit ADC用
SPI#2はGPS-PICとSDカード用

POWER SWはたくさんある（すべてHighでON）
ボード電源はこれまで通りジャンパーで
GPSモジュールのSWはGPS-PICで行う．
その他はMain PICでコントロール
	3.3VとBattery電源（7V）のデバイスは個々のPOWER SWで制御
	5V系はまず，5V DC/DCをONにしてから使うこと
		5V系のモジュール達：
			AD(BatteryなどPWピンがないもの含むすべて),USB電源,24bit-ADC,外部I2C,モーターの電圧シフトIC,RF#1(FOMA用)

===更新履歴===
===hmrDevice_kk09===

===hmrDevice_kk08===
v1_02/131123 iwahori
	PICのSDカード用ライブラリのために必要なdefineを追加
v1_01/131102 hmIto
	BaudRateをuartの初期化関数の引数に追加
v1_00/130907 hmIto
	hmrDevice_kk07_v1_03をベースにkk08対応版を作成
*/
//#define HMR_TEST
#ifndef HMR_24EP512GU810_INC
#	//デバイスが以下の型を使っていることを他ファイルに知らせるためのdefine
#	define HMR_24EP512GU810_INC
#	ifdef HMR_TEST
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
#include"hmrType.h"
#define itob(val) (val?1:0)

//=================================interrupt==============================
//割り込み初期化関数
void hmr_interrupt_initalize(void);
//割り込み終端化関数
void hmr_interrupt_finalize(void);

//割り込みロック・アンロック関数
typedef unsigned char mutex;
mutex hmr_interrupt_lock(void);
void hmr_interrupt_unlock(mutex);

// 割り込み禁止・再開用関数
void hmr_interrupt_enable(void);
void hmr_interrupt_disable(void);
//=============================Stream===============
#define HMR_STREAM_CH1 1
#define HMR_STREAM_CH2 2
//=========================== デバイスカーネル =========================
#define HMR_PIN_DEVICE_PW	PORTEbits.RE3
#define HMR_PIN_DEVICE_PW_set(val) HMR_PIN_DEVICE_PW=itob(val);__asm__("nop")
#define xHMR_power(onoff) HMR_PIN_DEVICE_PW_set(onoff)
void HMR_core_initialize(void);

//=========================== デバイスモジュール =============================
//いずれもクロック変更後は初期化が必要
void HMR_uart1_initialize_withRF0_with_flowCtr(uint16 BaudRate);
void HMR_uart1_initialize_withRF0_without_flowCtr(uint16 BaudRate);
void HMR_uart1_initialize_withRF1_with_flowCtr(uint16 BaudRate);
void HMR_uart1_initialize_withRF1_without_flowCtr(uint16 BaudRate);

void HMR_uart1_finalize(void);

void HMR_uart2_initialize(uint16 BaudRate);
void HMR_uart2_finalize(void);

void HMR_spi1_initialize(void);
void HMR_spi2_initialize(void);
void HMR_spi1_finalize(void);
void HMR_spi2_finalize(void);

void HMR_i2c_initialize(void);
void HMR_i2c_finalize(void);

void HMR_adc_initialize(void);
void HMR_adc_finalize(void);

//=================================LED===============
#define HMR_PIN_RedLED			 _LATB13//LATBbits.RB13	//OUT debug
#define HMR_PIN_RedLED_set(val) HMR_PIN_RedLED=itob(val==0);__asm__("nop")

#define HMR_PIN_YellowLED		 _LATB12//LATBbits.RB12	//OUT debug
#define HMR_PIN_YellowLED_set(val) HMR_PIN_YellowLED=itob(val==0);__asm__("nop")

#define HMR_PIN_DebugLED		//PORTBbits.RB13	//OUT debug
#define HMR_PIN_DebugLED_set(val) 0//HMR_PIN_DebugLED=itob(val==0);__asm__("nop")

#define HMR_PIN_Debug2LED		//PORTBbits.RB12	//OUT debug
#define HMR_PIN_Debug2LED_set(val) 0//HMR_PIN_Debug2LED=itob(val==0);__asm__("nop")

//============================DIP SWITCH=============
#define HMR_PIN_DIP1			PORTGbits.RG0		//IN
#define HMR_PIN_DIP2			PORTGbits.RG1		//IN
#define HMR_PIN_DIP3			PORTFbits.RF1		//IN
#define HMR_PIN_DIP4			PORTFbits.RF0		//IN

//============================5V DC/DC=============
#define HMR_PIN_5VDCDC_PW		PORTCbits.RC3
#define HMR_PIN_5VDCDC_PW_set(val)	HMR_PIN_5VDCDC_PW=itob(val!=0);__asm__("nop")

//===============================ADC=================
#define HMR_ADC_CO2				20		//AD Read half voltage(for CO2)
#define HMR_ADC_CH1				5		//AD
#define HMR_ADC_CH2				4		//AD
#define HMR_ADC_CH3				3		//AD
#define HMR_ADC_CH4				2		//AD attenated signal(for )
#define HMR_ADC_CH5				23		//AD
#define HMR_ADC_CH6				22		//AD

#define HMR_ADC_BatteryLR		28	//for battery LeftRrear motor	AD
#define HMR_ADC_BatteryRF		31	//for battery RightFront motor	AD
#define HMR_ADC_BatteryMain		17	//for battery Main board		AD

#define HMR_PIN_ADCPW		PORTEbits.RE9	//PowerSwitch for ADC #01 to #04
#define HMR_PIN_ADCPW_CO2		PORTAbits.RA0	//PowerSwitch for ADC #00(for CO2)

#define HMR_PIN_ADCPW_set(val)			HMR_PIN_ADCPW=itob(val!=0);__asm__("nop")
#define HMR_PIN_ADCPW_CO2_set(val)		HMR_PIN_ADCPW_CO2=itob(val!=0);__asm__("nop")

//==============================CAMERA======================
/*カメラは4台つなげる．2ビットでカメラを選ぶ
S1 S0 CAMERA ch
0	0	#0
0	1	#1
1	0	#2	コネクタまだなし(2013/12/15)
1	1	#3	コネクタまだなし(2013/12/15)
電源OFFにするにはS0,S1をinput設定にするかつないでいないchにする(たとえば，S1=S0=1)
*/
//#define HMR_PIN_CAM_PW			PORTAbits.RA1		//PowerSwitch for Camera	OUT
//#define HMR_PIN_CAM_PW_set(val)			HMR_PIN_CAM_PW=itob(val!=0);__asm__("nop")
#define HMR_PIN_CAM_SELECT0_TRIS			TRISFbits.TRISF3
#define HMR_PIN_CAM_SELECT0_TRIS_set(val)	HMR_PIN_CAM_SELECT0_TRIS=itob(val!=0);__asm__("nop")

#define HMR_PIN_CAM_SELECT1_TRIS			TRISFbits.TRISF8
#define HMR_PIN_CAM_SELECT1_TRIS_set(val)	HMR_PIN_CAM_SELECT1_TRIS=itob(val!=0);__asm__("nop")

void HMR_PIN_CAM_PW_set(unsigned char val);				//PowerSwitch for Camera

#define HMR_PIN_CAM_SELECT0		PORTFbits.RF3		//カメラ選択用ビットの0bit目
#define HMR_PIN_CAM_SELECT0_set(val)	HMR_PIN_CAM_SELECT0=itob(val!=0);__asm__("nop")

#define HMR_PIN_CAM_SELECT1		PORTFbits.RF8		//カメラ選択用ビットの1bit目
#define HMR_PIN_CAM_SELECT1_set(val)	HMR_PIN_CAM_SELECT1=itob(val!=0);__asm__("nop")

//===============================CO2========================
#define HMR_PIN_PUMP_PW			PORTAbits.RA1		//PowerSwitch for pump		OUT
#define HMR_PIN_PUMP_PW_set(val)	HMR_PIN_PUMP_PW=itob(val!=0);__asm__("nop")

//===============================microphone========================
#define HMR_PIN_MICROPHONE_PW	PORTAbits.RA14		//PowerSwitch for microphone
#define HMR_PIN_MICROPHONE_PW_set(val) HMR_PIN_MICROPHONE_PW=itob(val!=0);__asm__("nop")

//============================24bit ADC module===========
#define HMR_PIN_24bitADC_PW		PORTAbits.RA15		//PowerSwitch for 24bit ADC module
#define HMR_PIN_24bitADC_PW_set(val)	HMR_PIN_24bitADC_PW=itob(val!=0);__asm__("nop")
//SDカードとつながるピンのdefineいろいろ
#define HMR_PIN_24bitADC_SPI_SELECT	LATBbits.LATB6		//SD-SPI Slave Select Output bit
#define HMR_PIN_24bitADC_SPI_SELECT_set(val)	HMR_PIN_24bitADC_SPI_SELECT=itob(val);__asm__("nop")

//===============================I2C=========================
#define HMR_PIN_I2C_EXTERNA_PW			PORTAbits.RA4		//PowerSwitch for External I2C devides		OUT
#define HMR_PIN_I2C_EXTERNA_PW_set(val) HMR_PIN_I2C_EXTERNA_PW=itob(val!=0);__asm__("nop")

#define HMR_PIN_I2C_EXTERNA_SCL_TRIS	TRISDbits.TRISA2
#define HMR_PIN_I2C_EXTERNA_SCL			PORTDbits.RA2
//#define HMR_PIN_I2C_EXTERNA_SCL_set(val) HMR_PIN_I2C_EXTERNA_SCL=itob(val!=0)

#define HMR_PIN_I2C_EXTERNA_SDA_TRIS	TRISDbits.TRISA3
#define HMR_PIN_I2C_EXTERNA_SDA			PORTDbits.RA3
//#define HMR_PIN_I2C_EXTERNA_SDA_set(val) HMR_PIN_I2C_EXTERNA_SDA=itob(val!=0)

#define HMR_PIN_I2C_ACCEL1_PW			PORTDbits.RD11		//PowerSwitch for Accele#1, Magnetic#1 and Gyro I2C devides		OUT
#define HMR_PIN_I2C_ACCEL1_PW_set(val) HMR_PIN_I2C_ACCEL1_PW=itob(val!=0);__asm__("nop")

#define HMR_PIN_GYRO_READYDATA			PORTDbits.RD0


//===============================RF modeule===================
#define HMR_PIN_RF_PW			PORTAbits.RA9		//PowerSwitch for RF module
#define HMR_PIN_RF_PW_set(val) HMR_PIN_RF_PW=itob(val!=0);__asm__("nop")

#define HMR_PIN_MP_PW			PORTEbits.RE2		//PowerSwitch for MoblePhone module
#define HMR_PIN_MP_PW_set(val) HMR_PIN_MP_PW=itob(val!=0);__asm__("nop")

//==============================Servo,Motor=========================
//モータ用のレベル変換回路　Enable信号を廃止．その代り，電源そのもののON/OFFを行う．
//#define HMR_PIN_SERVO_PW		PORTFbits.RF8		//PowerSwitch for Servo
//#define HMR_PIN_SERVO_PW_set(val) HMR_PIN_SERVO_PW=itob(val!=0);__asm__("nop")

#define HMR_PIN_MOTORL_A		PORTEbits.RE5
#define HMR_PIN_MOTORL_A_set(val) HMR_PIN_MOTORL_A=itob(val);__asm__("nop")

#define HMR_PIN_MOTORL_B		PORTGbits.RG15
#define HMR_PIN_MOTORL_B_set(val) HMR_PIN_MOTORL_B=itob(val);__asm__("nop")

#define HMR_PIN_MOTORL_PW		PORTCbits.RE6
#define HMR_PIN_MOTORL_PW_set(val) HMR_PIN_MOTORL_PW=itob(val);__asm__("nop")

#define HMR_PIN_MOTORR_A		PORTDbits.RD5
#define HMR_PIN_MOTORR_A_set(val) HMR_PIN_MOTORR_A=itob(val);__asm__("nop")

#define HMR_PIN_MOTORR_B		PORTDbits.RD4
#define HMR_PIN_MOTORR_B_set(val) HMR_PIN_MOTORR_B=itob(val);__asm__("nop")

#define HMR_PIN_MOTORR_PW		PORTEbits.RE6
#define HMR_PIN_MOTORR_PW_set(val) HMR_PIN_MOTORR_PW=itob(val);__asm__("nop")

//============================GPS PIC, Drive PIC===========
#define HMR_PIN_SubPIC_PW		PORTBbits.RB7		//PowerSwitch for GPS PIC,Drive PIC
#define HMR_PIN_SubPIC_PW_set(val) HMR_PIN_SubPIC_PW=val;__asm__("nop")


//===========================ヘッドライト=======================
#define HMR_PIN_HEADLIGHT_PW		PORTEbits.RE0//PORTEbits.RE3	//PowerSwitch for Head Light
#define HMR_PIN_HEADLIGHT_PW_set(val) __asm__("nop");HMR_PIN_HEADLIGHT_PW=itob(val!=0);__asm__("nop")	//なぜか前にnopがないとつかない

//===========================SPI============================
/*SPIは2つ使う：
SPI#1は24bit ADC用
SPI#2はGPS-PICとSDカード用
*/
#define HMR_PIN_GPS_SPI_SELECT	PORTCbits.RC4	//SPI(to GPS PIC) slave select pin
#define HMR_PIN_GPS_SPI_SELECT_set(val) HMR_PIN_GPS_SPI_SELECT=(!val);__asm__("nop")

//#define HMR_PIN_DRIVE_SPI_SELECT PORTCbits.RC4	//SPI(DRIVE PIC) slave select pin
//#define HMR_PIN_DRIVE_SPI_SELECT_set(val) HMR_PIN_DRIVE_SPI_SELECT=(!val);__asm__("nop")

#define HMR_PIN_24ADC_SPI_SELECT	PORTBbits.RB6
#define HMR_PIN_24ADC_SPI_SELECT_set(val) HMR_PIN_24ADC_SPI_SELECT=(!val);__asm__("nop")

#define HMR_PIN_EXTERNAL_SPI_SELECT	PORTBbits.RB8
#define HMR_PIN_EXTERNAL_SPI_SELECT_set(val) HMR_PIN_EXTERNAL_SPI_SELECT=(!val);__asm__("nop")

#define HMR_PIN_EXTERNAL_SPI_PW		PORTDbits.RD8
#define HMR_PIN_EXTERNAL_SPI_PW_set(val)	HMR_PIN_EXTERNAL_SPI_PW=itob(val!=0);__asm__("nop")

//==============================SDカード================================
#define HMR_PIN_SDcard_PW		PORTDbits.RD13
#define HMR_PIN_SDcard_PW_set(val)	HMR_PIN_SDcard_PW=itob(val!=0);__asm__("nop")

//SDカードとつながるピンのdefineいろいろ
#define HMR_PIN_SD_SPI_SELECT	LATCbits.LATC1		//SD-SPI Slave Select Output bit
#define HMR_PIN_SD_SPI_SELECT_set(val)	HMR_PIN_SD_SPI_SELECT=itob(val);__asm__("nop")

//==============================USB================================]
#define HMR_PIN_USB_PW		PORTDbits.RD7
#define HMR_PIN_USB_PW_set(val)	HMR_PIN_USB_PW=itob(val!=0);__asm__("nop")
/*メモ
USB_D-	RG3
USB_D+	RG2

*/
#
#endif
