#ifndef HMR_I2CSHT75_C_INC
#define HMR_I2CSHT75_C_INC 0

#include "hmrI2C_SHT75.h"
#include "hmrDevice_kk08.h"

#define I2C_SHT75_SCL HMR_PIN_I2C_EXTERNA_SCL
#define I2C_SHT75_SCL_TRIS HMR_PIN_I2C_EXTERNA_SCL_TRIS
#define I2C_SHT75_SDA HMR_PIN_I2C_EXTERNA_SDA
#define I2C_SHT75_SDA_TRIS HMR_PIN_I2C_EXTERNA_SDA_TRIS

#define _i2cSHT75_NOP5() __asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop")
#define _i2cSHT75_NOP10() __asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop")

void i2cSHT75_initialize(void){
	__asm("nop");
	I2C_SHT75_SCL_TRIS=0;
	__asm("nop");
	I2C_SHT75_SDA_TRIS=0;
	__asm("nop");
}
void i2cSHT75_finalize(void){}
void _i2cSHT75_write_data(uint8 data){
	uint8 i=0;
	I2C_SHT75_SDA=0;
	__asm("nop");
	I2C_SHT75_SCL=0;
_i2cSHT75_NOP10();
	//スレーブアドレス、IO送信
	for(i=0x80;i>0;i>>=1){
		if(data & i) I2C_SHT75_SDA=1;
		else I2C_SHT75_SDA=0;
_i2cSHT75_NOP10();
	I2C_SHT75_SCL=1;
_i2cSHT75_NOP10();
_i2cSHT75_NOP10();
_i2cSHT75_NOP10();
		I2C_SHT75_SCL=0;
_i2cSHT75_NOP10();
	}
}
boolian i2cSHT75_start(uint8 slave_address,boolian IO){
	uint8 data=((0x01&IO)|(slave_address<<1));
	boolian err=hmLib_false;

	//ピンの」IOをアウトプットにする
	__asm("nop");
	I2C_SHT75_SCL_TRIS=0;
	__asm("nop");
	I2C_SHT75_SDA_TRIS=0;
	__asm("nop");

//通信開始波形送信
//		_____		  ________
// SDA :	 |_______|
//			___		 ___
// SCL :___|	|___|	|______ 

	//初期状態
	I2C_SHT75_SDA=1;
	__asm("nop");
	I2C_SHT75_SCL=0;
	
_i2cSHT75_NOP10();
	I2C_SHT75_SCL=1;
_i2cSHT75_NOP10();
	I2C_SHT75_SDA=0;
_i2cSHT75_NOP10();
	I2C_SHT75_SCL=0;
_i2cSHT75_NOP10();
_i2cSHT75_NOP10();
_i2cSHT75_NOP10();
	I2C_SHT75_SCL=1;
_i2cSHT75_NOP10();
	I2C_SHT75_SDA=1;
_i2cSHT75_NOP10();
	I2C_SHT75_SCL=0;

_i2cSHT75_NOP10();
_i2cSHT75_NOP10();
_i2cSHT75_NOP10();

	//8bit送信
	_i2cSHT75_write_data(data);
	
	I2C_SHT75_SDA=0;		//SDAをリリース
_i2cSHT75_NOP10();
	I2C_SHT75_SCL=1;		//ACK用の9個目のクロック送信
	__asm("nop");

	I2C_SHT75_SCL_TRIS=1;	//ACK受信のためにinに設定
_i2cSHT75_NOP5();

	err=I2C_SHT75_SDA;		//ACK受信
_i2cSHT75_NOP5();

	I2C_SHT75_SCL=0;		//9個目のクロック送信終了
	__asm("nop");

	I2C_SHT75_SDA_TRIS=0;	//TRIS設定をoutに戻す
	return err;
}
void i2cSHT75_stop(void){
	I2C_SHT75_SCL_TRIS=0;
	__asm("nop");
	I2C_SHT75_SDA_TRIS=0;
	__asm("nop");
}
boolian i2cSHT75_write(uint8 c){
	uint8 data=(0x7F&c);		//7bit目は書き込めないので消す
	boolian err=hmLib_false;

	//ピンの」IOをアウトプットにする
	__asm("nop");
	I2C_SHT75_SCL_TRIS=0;
	__asm("nop");
	I2C_SHT75_SDA_TRIS=0;
	__asm("nop");

	//8bit送信
	_i2cSHT75_write_data(data);

	I2C_SHT75_SDA=0;		//SDAをリリース
_i2cSHT75_NOP10();
	I2C_SHT75_SCL=1;		//ACK用の9個目のクロック送信
	__asm("nop");

	I2C_SHT75_SCL_TRIS=1;	//ACK受信のためにinに設定
_i2cSHT75_NOP5();

	err=I2C_SHT75_SDA;		//ACK受信
_i2cSHT75_NOP5();

	I2C_SHT75_SCL=0;		//9個目のクロック送信終了
	__asm("nop");

	I2C_SHT75_SDA_TRIS=0;	//TRIS設定をoutに戻す
	return err;
}
uint8 i2cSHT75_read(boolian EndFlag){
	uint8 i=0;
	uint8 data=0;

	I2C_SHT75_SCL=0;
	__asm("nop");
	I2C_SHT75_SDA=1;		//SDAをリリース
	__asm("nop");

	I2C_SHT75_SDA_TRIS=1;	//受信のためにTRISをinに設定
_i2cSHT75_NOP10();

	for(i=0x80;i>0;i>>=1){
		I2C_SHT75_SCL=1;
		if(I2C_SHT75_SDA)data|=i;
_i2cSHT75_NOP10();
		I2C_SHT75_SCL=0;
	}

	I2C_SHT75_SDA_TRIS=0;
	//__asm("nop");
	__asm("nop");
	I2C_SHT75_SDA=EndFlag;	//終了時はSDAを挙げたままにしてNACKを送る
_i2cSHT75_NOP10();
	I2C_SHT75_SCL=1;		//9回目のクロック生成
_i2cSHT75_NOP10();
_i2cSHT75_NOP10();
_i2cSHT75_NOP10();
	I2C_SHT75_SCL=0;
_i2cSHT75_NOP10();
	I2C_SHT75_SDA=1;		//SDAをリリース
	return data;
}
boolian i2cSHT75_data_can_recv(void){
	boolian Flag=0;
	I2C_SHT75_SDA_TRIS=1;
	__asm("nop");
	Flag=I2C_SHT75_SDA;
	__asm("nop");
	I2C_SHT75_SDA_TRIS=0;
	return Flag;
}
#endif
