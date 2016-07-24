#ifndef HMR_INERTIAL_INC
#define HMR_INERTIAL_INC 103
#
/*===hmrInertial===
慣性航法モジュール制御用
hmrInertial_v0_03/131026 amby
	この場合は電源ON,OFFするたびにI２Cでデバイスを初期化する必要があるので、
	そのための関数device_initialize を追加した。
hmrInertial_v0_02/131019 iwahori
	タスクシステム用にtask_setup_talkとtask_interruptを作成
hmrInertial_v1_00/130907 iwahori
	kk08移行に伴い，使用するI2Cが変わったためi2c1,i2c2を変更．initializeは完了，talkはまだ
hmrInertial_v1_00/130622 hmIto
	ほむらの設置位置に対応するように軸を変更
hmrInertial_v0_01/130105 iwahori
	work関数をsetup_lisenとsetup_talkに分離
	Gyroのtalk関数を実装
	KK07でのセンサー変更に伴いinitialze関数とアドレスを変更
hmrInertial_v0_00/121208 hmIto
	基本関数作成
*/
#include"hmLib_v3_04/cstring.h"
#include"hmrType.h"
#ifdef __cplusplus
using hmLib::cstring;
#endif
void inertial_initialize(void);
void inertial_finalize(void);
//******************* Axel ******************
#define AXEL_ADDRESS 0x19
#define AXEL_REG_CTRL 0x20
#define AXEL_REG_DATA 0x28
#define AXEL_REGMODE_SINGLE 0x00
#define AXEL_REGMODE_MULTI 0x80
#define AXEL_I2C_RETRY_NUM 3
void axel_device_initialize(void);// PWR ON にしたときに必要な初期化関数
void axel_device_finalize(void);// PWR OFF にする前に必要な初期化関数
hmLib_boolian axel_listen(cstring Str);
hmLib_boolian axel_talk(cstring* pStr);
void axel_setup_listen(void);
void axel_setup_talk(void);
void axel_task_setup_talk(void);
sint16 axel_task_interrupt(sint16 interval);
sint16 axel_task_data(sint16 interval);
sint16 axel_task_inform(sint16 interval);
//****************** compass *********************
#define CMPS_ADDRESS 0x1E
#define CMPS_REG_CTRL 0x00
#define CMPS_REG_DATA 0x03
#define CMPS_REG_DATATEMP 0x31
#define CMPS_REGMODE_SINGLE 0x00
#define CMPS_REGMODE_MULTI 0x80
#define CMPS_I2C_RETRY_NUM 3
void compass_device_initialize(void);
void compass_device_finalize(void);
hmLib_boolian compass_listen(cstring Str);
hmLib_boolian compass_talk(cstring* pStr);
void compass_setup_listen(void);
void compass_setup_talk(void);
void compass_task_setup_talk(void);
sint16 compass_task_data(sint16 interval);
sint16 compass_task_inform(sint16 interval);
//=====================Gyro=======================
#define GYRO_ADDRESS 0x69
#define GYRO_REG_CTRL		0x20	//各種設定用レジスタ
#define GYRO_REG_DATA		0x28	//XYZ各2バイト，計6byteの先頭データ読み出し用レジスタ
#define GYRO_REG_FIFOCTRL	0x2E	//FIFO設定用レジスタ
#define GYRO_REG_FIFOSRC	0x2F	//FIFO管理用レジスタ
#define gyro_bufsize(FIFOSRC) (FIFOSRC&0x1F)	//FIFO管理用レジスタから，FIFO内のデータ数を調べる、
#define gyro_empty(FIFOSRC) (FIFOSRC&0x20)		//FIFO管理用レジスタから，FIFOが空か調べる
#define gyro_overflow(FIFOSRC) (FIFOSRC&0x40)	//FIFO管理用レジスタから，FIFOがオーバーフローしているか調べる
#define GYRO_REGMODE_SINGLE	0x00	//REGとともに|演算で使う　単バイト受信
#define GYRO_REGMODE_MULTI	0x80	//REGとともに|演算で使う　複数バイト受信
#define GYRO_I2C_RETRY_NUM 3
void gyro_device_initialize(void); // gyro はデフォルトでパワーダウンモードなので、起動する関数
void gyro_device_finalize(void);
hmLib_boolian gyro_listen(cstring Str);
hmLib_boolian gyro_talk(cstring* pStr);
void gyro_setup_listen(void);
void gyro_setup_talk(void);
void gyro_task_setup_talk(void);
sint16 gyro_task_data(sint16 interval);
sint16 gyro_task_inform(sint16 interval);

#
#endif
