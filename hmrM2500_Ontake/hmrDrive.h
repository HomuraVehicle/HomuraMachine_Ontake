#ifndef HMR_DRIVE_INC
#define HMR_DRIVE_INC 0
#
/*
===hmrDrive===
モーター、マニュピュレータ系をコントロール

hmrDrive_v0_00/130112 iwahori
	ファイルを作成・モーターのメッセージ関数をtalkとlistenに分解
*/
#ifndef HMCLIB_CSTRING_INC
#	include"hmLib_v3_04/cstring.h"
#endif
#ifndef HMR_DEVICE_INC
#	include "hmrDevice.h"
#endif
//*********************** 駆動
hmLib_boolian motor_listen(cstring Str);
hmLib_boolian motor_talk(cstring* pStr);
void motor_setup_listen(void);
void motor_setup_talk(void);
sint16 motor_task_wdt(sint16 interval);
//*********************** Manipulator
cstring manipu_message(cstring Str);
//*********************** MmSPI
uint8 MmSPI_read(uint8 u);
void MmSPI_strt(uint8 cmd);
void MmSPI_trmn(uint8 cmd);
void MmSPI_motor(uint8 left,uint8 l_breake, uint8 right, uint8 r_breake);
void MmSPI_manipu(uint16 arm1, uint16 arm2, uint16 arm3);
void MmSPI_setLog(uint8 io_);
uint8 MmSPI_getLog(uint8* ptr_);


#
#endif

