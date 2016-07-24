#ifndef HMR_UTILITY_C_INC
#define HMR_UTILITY_C_INC 101
#
#ifndef HMR_UTILITY_INC
#	include"hmrUtility.h"
#endif
#ifndef HMR_DEVICE_INC
#	include"hmrDevice.h"
#endif
#include <stdlib.h>
//���b�N�����֐�
unsigned char lockcode(void){
	unsigned char Key;
	do{
		Key=(unsigned char)(rand());
	}while(Key==0);
	return Key;
}
//���荞�݈��S��cstring���I�����֐�
void cstring_construct_dynamic_safe(cstring* str,cstring_size_t size){
	mutex Lock;
	Lock=hmr_interrupt_lock();
	cstring_construct_dynamic(str,size);
	hmr_interrupt_unlock(Lock);
}
//���荞�݈��S��cstring�j���֐�
void cstring_destruct_safe(cstring* str){
	mutex Lock;
	Lock=hmr_interrupt_lock();
	cstring_destruct(str);
	hmr_interrupt_unlock(Lock);
}
// ���̊��荞�݂��֎~����A�r���I delay �֐�
void exclusive_delay_ms(uint16 time){
	mutex Lock;
	Lock=hmr_interrupt_lock();
	delay_ms_sub(time);
	hmr_interrupt_unlock(Lock);
}



//CRC8�p�֐�
//�L���萔
#define MSB_CRC8 (0x85)// x8 + x7 + x2 + x0
//CRC8�̌v�Z(����������:0x85,������)
unsigned char getCRC8(unsigned char crc8,unsigned char data){
	unsigned char i;

	crc8 ^= data;
	for(i=0; i<8; i++){
		if(crc8 & 0x80){
			crc8<<=1;
			crc8^=MSB_CRC8; 
		}else{
			crc8<<=1;
		}
	}
	return crc8;
}
#
#endif
