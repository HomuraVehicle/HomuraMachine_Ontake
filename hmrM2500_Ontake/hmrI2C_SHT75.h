#ifndef HMR_I2CSHT75_INC
#define HMR_I2CSHT75_INC 0
/*
hmrI2C_SHT75 SHT75モジュールのI2Cのゴミ独自規格用の関数群
v0_00/131102 iwahori

*/
#include"hmLib_v3_04/cstring.h"
#include"hmrType.h"

//初期化
void i2cSHT75_initialize(void);
//終端化
void i2cSHT75_finalize(void);
//通信開始(IO=1が受信，0が送信)
boolian i2cSHT75_start(uint8 slave_address,boolian IO);
//通信終了
void i2cSHT75_stop(void);
//送信関数
boolian i2cSHT75_write(uint8 c);
//受信関数
uint8 i2cSHT75_read(boolian EndFlag);
//温度・湿度データが用意できたか
boolian i2cSHT75_data_can_recv(void);
#endif
