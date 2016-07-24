#ifndef HMR_DEVICE_KK06_INC
#define HMR_DEVICE_KK06_INC 108
#
/*===hmrDevice_kk06===
kk06基板固有のpin配置等の宣言や設定用関数を提供
*/
/*===更新履歴===
hmrDevice_kk06_v1_00/120921 iwahori
	memo:SPI select pin は正論理？　kk05では負論理，回路情報には記載なし
hmrDevice_kk06_v1_01/121005 hmIto
	hmrDeviceConfigにあったデバイス依存の初期化処理系関数群を移行
hmrDevice_kk06_v1_02/121013 hmIto
	名前空間をDeviceからdeviceに変更
	割り込み関数の宣言を追加
	spi通信のset関数を負論理に変更
hmrDevice_kk06_v1_03/121014 iwahori
	HMR_PIN_SubPIC_PW_set(val) (HMR_PIN_subPIC_PW=val)
	を変更subのsを大文字に変更
	HMR_PIN_GPS_SPI_SELECT PORTGbis.RG9 スペルミスGbis変更
	HMR_PIN_GPS_DRIVE_SELECT PORTCbis.RC4 HMR_PIN_DRIVE_SPI_SELECT PORTCbits.RC4，マクロも同様
hmrDevice_kk06_v1_04/121020 iwahori
	initialzeにクロック設定追加
hmrDevice_kk06_v1_05/121027 hmIto
	initialize関数を整理
	uart1/2_pinselect関数を追加
	uart1_send/recv_lock/unlockをuart1_fput/fget_lock/unlockに変更
	優先順位決定関数以外の割り込み制御関数をamPic30Extendに移動
hmrDevice_kk06_v1_06/121028 hmIto
	device_initialize/finalize関数をなくし、module単位での初期化を可能に変更
		device:デバイスの中核部分の初期化/終端化 プログラム開始直後、終了直前に実行
		clock:二つのクロックスピードでの初期化を提供
		uart1/2,spi2,i2c,adc:通信系の初期化関数
hmrDevice_kk06_v1_07/121030 iwahori
	configuration bitの_FPOR()を変更ALTI2C2_ON
	（ASDA2/ASCK2 pins are selected as the I/O pins for I2C2）を追加
	関数の名前空間をhmrに移動,hmr_adc_initializeを実装
hmrDevice_kk06_v1_08/121101 hmIto
	RG3がinput専用だったので、RG2へServoPower出力ピンを変更
*/

#ifndef HMR_24EP512GU810_INC
#	//デバイスが以下の型を使っていることを他ファイルに知らせるためのdefine
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
//割り込み初期化関数
void hmr_interrupt_initalize(void);
//割り込み終端化関数
void hmr_interrupt_finalize(void);
//mainループへの割り込み一時停止
void hmr_main_lock_interrupt(void);
//mainループへの割り込み復帰
void hmr_main_unlock_interrupt(void);
//uart1_recvへの割り込み一時停止
void hmr_uart1_fget_lock_interrupt(void);
//uart1_recvへの割り込み復帰
void hmr_uart1_fget_unlock_interrupt(void);
//uart1_sendへの割り込み一時停止
void hmr_uart1_fput_lock_interrupt(void);
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
