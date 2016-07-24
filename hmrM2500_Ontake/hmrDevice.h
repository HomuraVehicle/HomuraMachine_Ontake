#ifndef HMR_DEVICE_INC
#define HMR_DEVICE_INC 102
#
/*
===hmrDevice===
hmrDevice_XXX.hをincludeして、
「デバイス非依存だが、運用方法等で変更され、広範に影響が出る可能性がある」
ような宣言、変数、関数等を提供する。
例
	* Device提供のPin名宣言に役割を与えていく宣言
	* Device提供の関数を複数まとめるなどしてラッピング
	* Device提供の関数を利用した、より一般的な利用形態の関数提供
*/
/*
=== hmrDevice ===
v1_02/131102 hmIto
	全二重通信に対応しているかを示す、hmr_isFullDuplexを追加
v1_01/131026 amby
	この中で、カメラがタイマー1を使って・・・というようなデバイス依存性をすべて定義できるようにおしこめていく途中
	割り込み関数について主に対応中
v1_00/131019 hmIto
	DeviceConfigから改名
=== hmrDeviceConfig ===
v1_09/130112 iwahori
	モーター制御のピンdefine追加、hmrDevice_kk06がincludeされていたのをkk07に変更
	カメラのヘッドライトのピンdefine追加
v1_08/121101 hmIto
	restart_wdt廃止（main-loopに集約）
v1_07/121028 hmIto
	sprite_StreamをStream_CMRに変更
	co2/h2sのpower関数をマクロに置換
	関数adc_BatteryID_to_chをbattery_ID_to_adcに変更
	servoとi2cのpower関数マクロを追加
	uart1_initialize,uart1_getModuleIDを追加
		ModuleIDにて切換え制御が可能
		一括して、電源などを管理
v1_06/121020 iwahori
	cファイルのほうのadc_BatteryID_to_adchをhファイルに合わせてadc_BatteryID_to_chに変更
	cファイルのほうのadc_BatteryID_to_chの引数名がIDだったのでBattelyIDに変更
	h2s_powerSensor()追加ピン等未定なので未実装
	rf_power,manipu_power,subPic_power cファイルの大文字はじまりを小文字に変更
v1_05/121020 hmIto
	ADCのdefineにとりあえずのチャンネル番号を宣言
		ただし、実際にこれらのセンサーを使う場合は、ADCを指定する必要あり
v1_04/121014 iwahori
	Streamの名称変更
	void spi_selectDrive(boolian onoff){HMR_PIN_GPS_DRIVE_SELECT_set(onoff)
	HMR_PIN_GPS_DRIVE_SELECTをHMR_PIN_DRIVE_SPI_SELECTに変更
	ADピンのdefineを追加
v1_03/121013 hmIto
	ADC_Battery#の名称を変更
	power関数を追加
v1_02/121005 hmIto
	ちゃんと、バージョン番号上げろ
	初期化系関数群をhmrDevice_kk06のDevice_initializeに移行
v1_01/120922 iwahori
	編集開始
	AD_ini()未実装，ADポートの設定はhmrDevice_kk**.c 内Device_initialize()に実装済み
v1_00/120921 hmIto
	とりあえず基本イメージだけ書き出し。
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

//================ 割り込み制御系 ====================
// Main stream 用制御関数
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

// CMR stream 用制御関数
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

// CMR timer 用制御関数
#define timerCMR_reset()					timer1_reset()
#define timerCMR_interrupt					timer1_interrupt
#define timerCMR_disable_interrupt()		timer1_disable_interrupt()
#define timerCMR_enable_interrupt()			timer1_enable_interrupt()
#define timerCMR_clear_interrupt_flag()		timer1_clear_interrupt_flag()
#define timerCMR_set_interrupt_flag()		timer1_set_interrupt_flag()
#define timerCMR_initialize(Time)			timer1_initialize(Time)

// DeviceManager Timer 用制御関数
#define timerDevmng_reset()					timer4_5_reset()
#define timerDevmng_interrupt				timer4_5_interrupt
#define timerDevmng_disable_interrupt()		timer4_5_disable_interrupt()
#define timerDevmng_enable_interrupt()		timer4_5_enable_interrupt()
#define timerDevmng_clear_interrupt_flag()	timer4_5_clear_interrupt_flag()
#define timerDevmng_set_interrupt_flag()	timer4_5_set_interrupt_flag()
#define timerDevmng_initialize(Time)		timer4_5_initialize(Time)

//================Delay系関数=========================
//#define delay_ms_main(Time) delay_ms(Time)
#define delay_ms_sub(Time) delay_ms2(Time) 

//================ADC=================================
#define adc_power(onoff) HMR_PIN_ADCPW_CH0to6_set(onoff)
//ADCチャンネル設定
#define ADC_CO2 HMR_ADC_CH8					//CO2 ADチャンネル
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
//ADCの100回平均取得用関数
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
//ModuleIDのモジュールで初期化する
void hmr_uart1_initialize(unsigned char ModuleID_,uint16 BaudRate_);
//ModuleIDに電源を変更する
void hmr_uart1_power_switch(boolian onoff,unsigned char ModuleID_);
//現在のModuleIDを取得する
unsigned char hmr_uart1_getModuleID(void);

//==============Clock==========================
#define CLOCK_NULL 0x00
#define CLOCK_SLOW 0x01
#define CLOCK_NORMAL 0x02
#define CLOCK_FAST 0x03
//クロックを指定モードで初期化する
void hmr_clock_initialize(unsigned char Mode_);
//現在のModuleIDを取得する
unsigned char hmr_clock_getModuleID(void);
#
#endif
