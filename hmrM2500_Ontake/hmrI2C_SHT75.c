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
	//�X���[�u�A�h���X�AIO���M
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

	//�s���́vIO���A�E�g�v�b�g�ɂ���
	__asm("nop");
	I2C_SHT75_SCL_TRIS=0;
	__asm("nop");
	I2C_SHT75_SDA_TRIS=0;
	__asm("nop");

//�ʐM�J�n�g�`���M
//		_____		  ________
// SDA :	 |_______|
//			___		 ___
// SCL :___|	|___|	|______ 

	//�������
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

	//8bit���M
	_i2cSHT75_write_data(data);
	
	I2C_SHT75_SDA=0;		//SDA�������[�X
_i2cSHT75_NOP10();
	I2C_SHT75_SCL=1;		//ACK�p��9�ڂ̃N���b�N���M
	__asm("nop");

	I2C_SHT75_SCL_TRIS=1;	//ACK��M�̂��߂�in�ɐݒ�
_i2cSHT75_NOP5();

	err=I2C_SHT75_SDA;		//ACK��M
_i2cSHT75_NOP5();

	I2C_SHT75_SCL=0;		//9�ڂ̃N���b�N���M�I��
	__asm("nop");

	I2C_SHT75_SDA_TRIS=0;	//TRIS�ݒ��out�ɖ߂�
	return err;
}
void i2cSHT75_stop(void){
	I2C_SHT75_SCL_TRIS=0;
	__asm("nop");
	I2C_SHT75_SDA_TRIS=0;
	__asm("nop");
}
boolian i2cSHT75_write(uint8 c){
	uint8 data=(0x7F&c);		//7bit�ڂ͏������߂Ȃ��̂ŏ���
	boolian err=hmLib_false;

	//�s���́vIO���A�E�g�v�b�g�ɂ���
	__asm("nop");
	I2C_SHT75_SCL_TRIS=0;
	__asm("nop");
	I2C_SHT75_SDA_TRIS=0;
	__asm("nop");

	//8bit���M
	_i2cSHT75_write_data(data);

	I2C_SHT75_SDA=0;		//SDA�������[�X
_i2cSHT75_NOP10();
	I2C_SHT75_SCL=1;		//ACK�p��9�ڂ̃N���b�N���M
	__asm("nop");

	I2C_SHT75_SCL_TRIS=1;	//ACK��M�̂��߂�in�ɐݒ�
_i2cSHT75_NOP5();

	err=I2C_SHT75_SDA;		//ACK��M
_i2cSHT75_NOP5();

	I2C_SHT75_SCL=0;		//9�ڂ̃N���b�N���M�I��
	__asm("nop");

	I2C_SHT75_SDA_TRIS=0;	//TRIS�ݒ��out�ɖ߂�
	return err;
}
uint8 i2cSHT75_read(boolian EndFlag){
	uint8 i=0;
	uint8 data=0;

	I2C_SHT75_SCL=0;
	__asm("nop");
	I2C_SHT75_SDA=1;		//SDA�������[�X
	__asm("nop");

	I2C_SHT75_SDA_TRIS=1;	//��M�̂��߂�TRIS��in�ɐݒ�
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
	I2C_SHT75_SDA=EndFlag;	//�I������SDA���������܂܂ɂ���NACK�𑗂�
_i2cSHT75_NOP10();
	I2C_SHT75_SCL=1;		//9��ڂ̃N���b�N����
_i2cSHT75_NOP10();
_i2cSHT75_NOP10();
_i2cSHT75_NOP10();
	I2C_SHT75_SCL=0;
_i2cSHT75_NOP10();
	I2C_SHT75_SDA=1;		//SDA�������[�X
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
