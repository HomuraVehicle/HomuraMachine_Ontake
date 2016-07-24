#ifndef HMR_SENSOR_INC
#define HMR_SENSOR_INC 112
#
/*
=== hmrSensor ===
センサー関連関数についてまとめたもの

hmrSensor_v1_00/120925 hmIto
	センサー関連の関数名を整理
hmrSensor_v1_01/121006 hmIto
	MessageにBattery関数のみ対応 ただしVref_GNDは不明
hmrSensor_v1_02/121013 hmIto
	一通り完成
hmrSensor_v1_03/121014 iwahori
	AD_H2SをADC_H2Sに変更
	s_byte,u_intをsint8,uint16に変更
hmrSensor_v1_04/121020 hmIto
	とりあえずエラー修正を確認して更新
	getAveADC等の関数群のコメントアウトを解除
hmrSensor_v1_05/121020 iwahori
	cファイルのgetAveADC()をadc_getAve100()に変更
hmrSensor_v1_06/121021 iwahori
	battery_message(cstring Str)の末尾に return Ans;を追加
	sprite_message()内の409行目cstring_static_constructの引数(unsigned char)(*BufPtr)をBufPtrに変更
hmrSensor_v1_07/121028 iwahori
	ifの条件の中のcstring_getc(&Ans,0)をcstring_getc(&Str,0)に変更
	加速度・磁気センサーの初期化関数を追加
	加速度・磁気センサーのメッセージ関数をkk06に対応
hmrSensor_v1_08/121031 hmIto
	battery_message関数のメモリ破壊を修正
hmrSensor_v1_09/121101 hmIto
	restart_wdtを廃止（main-loopに集約）
hmrSprite_v1_10/121101 hmIto
	カメラ速度重視のsprite_message2を追加　通信規約がまるで別物なので注意
hmrSensor_v1_11/121208 iwahori
	加速度・磁気センサーの初期化およびメッセージ関数を修正
hmrSensor_v1_12/121222 iwahori
	Gyro追加．センサー変更に伴いAxelとCompassの初期化関数変更
*/
#ifndef HMCLIB_BYTEBASETYPE_INC
#	include"hmCLib_v1_00/bytebase_type.h"
#endif
#ifndef HMR_DEVICECONFIG_INC
#	include"hmrDeviceConfig.h"
#endif
#ifndef HMR_MESSAGE_INC
#	include"hmrMessage.h"
#endif
//***************** GPS ************************************
#define SPICMD_GPSGet 0x00
#define SPICMD_GPSChange 0x01
#define SPICMD_GPSGetChannel 0x02
#define SPICMD_NULL 0xfe
//***************** SpriteCmr *********************
#define SPRITE_PACKSIZE 160 //240 debug_
#define SPRITE_BUF_HEADER 4	//0x20,0x21,size[2]
#define sprite_BufferSize_LARGE 1680 //240 debug_
#define sprite_BufferSize_SMALL 160 //240 debug_
#define sprite_BufferHeader 3
#define sprite_BufferNum 2

//******************* Acc ******************
#define AXEL_ADDRESS 0x19
//****************** compass *********************
#define COMPASS_ADDRESS 0x1E
//****************** Gyro *********************
#define GYRO_ADDRESS 0x69
//**************** Battery *********************************
cstring battery_message(cstring Str);
//************************ GPS 
cstring gps_message(cstring Str);
//************************ CO2 
cstring co2_message(cstring Str);
//*********************** H2S 
cstring h2s_message(cstring Str);
//************************ Acc 
void axel_initialize(void);
cstring axel_message(cstring Str);
//************************ Thermo 
cstring thermo_message(cstring Str);
//************************ InfThermo 
cstring infthermo_message(cstring Str);
//************************ Hygro 
cstring hygro_message(cstring Str);
//************************ Compass 
void compass_initialize(void);
cstring compass_message(cstring Str);
//************************ Gyro
void gyro_initialize(void);
cstring gyro_message(cstring Str);
//************************ Get ADC Ave 100
//uint32 adc_getAve100(unsigned char mode, unsigned char ch);
//************************ SpriteCmr
cstring sprite_message(cstring Str);
cstring sprite_message2(cstring Str);
#
#endif
