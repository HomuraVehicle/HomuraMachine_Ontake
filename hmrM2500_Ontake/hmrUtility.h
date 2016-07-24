#ifndef HMR_UTILITY_INC
#define HMR_UTILITY_INC 101
#
#ifndef HMRLIB_CSTRING_INC
#	include"hmLib_v3_04/cstring.h"
#endif
#ifndef HMLIB_BYTEBASETYPE_INC
#	include "hmLib_v3_04/bytebase_type.h"
#endif
#include"hmrType.h"
/*
===hmrUtilities===
便利な関数の提供を行う
v1_01/131019 hmIto
	cstringを割り込み安全に動的生成&破棄する関数群を追加
		cstring_construct_dynamic_safe
		cstring_destruct_safe
v1_00/131019 amby
	生まれる
	ロックコード生成関数、lockcodeを追加
*/
//ロックコード生成関数
unsigned char lockcode(void);
//割り込み安全なcstring動的生成関数
void cstring_construct_dynamic_safe(cstring* str,cstring_size_t size);
//割り込み安全なcstring破棄関数
void cstring_destruct_safe(cstring* str);

// 他の割り込みを禁止する、排他的 delay 関数
void exclusive_delay_ms(uint16 time);

#
#endif


















