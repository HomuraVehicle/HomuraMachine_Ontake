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
//ロック生成関数
unsigned char lockcode(void){
	unsigned char Key;
	do{
		Key=(unsigned char)(rand());
	}while(Key==0);
	return Key;
}
//割り込み安全なcstring動的生成関数
void cstring_construct_dynamic_safe(cstring* str,cstring_size_t size){
	mutex Lock;
	Lock=hmr_interrupt_lock();
	cstring_construct_dynamic(str,size);
	hmr_interrupt_unlock(Lock);
}
//割り込み安全なcstring破棄関数
void cstring_destruct_safe(cstring* str){
	mutex Lock;
	Lock=hmr_interrupt_lock();
	cstring_destruct(str);
	hmr_interrupt_unlock(Lock);
}
// 他の割り込みを禁止する、排他的 delay 関数
void exclusive_delay_ms(uint16 time){
	mutex Lock;
	Lock=hmr_interrupt_lock();
	delay_ms_sub(time);
	hmr_interrupt_unlock(Lock);
}



//CRC8用関数
//記号定数
#define MSB_CRC8 (0x85)// x8 + x7 + x2 + x0
//CRC8の計算(生成多項式:0x85,左送り)
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
