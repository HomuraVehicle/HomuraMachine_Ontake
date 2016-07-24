#ifndef HMR_DEBUG_INC
#define HMR_DEBUG_INC 100
#
/*===hmrDebug===
Debug関係の関数群
*/
#include"hmrType.h"
//モーター出力ピンで、0-15の数字を出力する
//	3-0bit目がそれぞれ順にLA,LB,RA,RB
void debug_num4Motor(uint8 num_0_15_);
void debug_num4Motor2(uint8 num_0_15_);
//0-1出力をCO2PumpPwピンを使って出力
void debug_isCO2PumpPw(boolian val_);
//0-1出力をSpritePwピンを使って出力
//void debug_isSpritePw(boolian val_);
//0-1出力をMPPwピンを使って出力
void debug_isMPPw(boolian val_);
#
#endif
