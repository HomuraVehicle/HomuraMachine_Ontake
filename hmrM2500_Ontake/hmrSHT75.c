#ifndef HMR_SHT75_C_INC
#define HMR_SHT75_C_INC 1
#
#define HMLIB_NOLIB
#ifndef HMR_THERMO_INC
#	include"hmrSHT75.h"
#endif
#ifndef HMR_DEVICE_INC
#	include"hmrDevice.h"
#endif
#ifndef HMR_UTILITY_INC
#	include "hmrUtility.h"
#endif
#include "hmrI2C_SHT75.h"
//SHT75との通信失敗時に、リトライできる回数
#define SHT75_I2C_RETRYNUM		3
//SHT75のアドレス
#define SHT75_ADDRESS_THERMO	1
#define SHT75_ADDRESS_HUMID		2
#define SHT75_ADDRESS_CONFIG	3
#define SHT75_ADDRESS_RESET		0x0F
//SHT75ステータス管理用構造体
struct{
	unsigned Data:1;				//データを送信時に落ちる
	unsigned SendData:1;			//データ送信モード
	unsigned SendData_i:1;			//データ送信モード受理
	unsigned LowAccData:1;			//低精度データモード
	unsigned LowAccData_i:1;		//低精度データモード受理
	unsigned Reset_i:1;				//リセット受理
}_sht75_Mode={0,0,1,0,1};
//通常モードのデータ受信用関数
void sht75_setup_listen(void){return;}
hmLib_boolian sht75_listen(cstring Str){
	unsigned char Cnt;

	//データサイズ確認
	if(cstring_size(&Str)==0)return hmLib_true;

	//1byte目でモードを分ける
	switch(cstring_getc(&Str,0)){
	case 0x10:
		_sht75_Mode.SendData=hmLib_true;
		_sht75_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	case 0x11:
		_sht75_Mode.SendData=hmLib_false;
		_sht75_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	case 0x20:
		if(_sht75_Mode.LowAccData==hmLib_false){
			//ConfigBitを設定
			for(Cnt=0;Cnt<SHT75_I2C_RETRYNUM;++Cnt){
				//書き込み要求
				if(i2c1_start(SHT75_ADDRESS_CONFIG,0))continue;
				//ConfigBits書き込み
				if(i2c1_write(0x01))continue;	
				//i2c1終了
				i2c1_stop();
				break;
			}

			if(Cnt==SHT75_I2C_RETRYNUM)return hmLib_false;
		}
		
		_sht75_Mode.LowAccData=hmLib_true;
		_sht75_Mode.LowAccData_i=hmLib_true;

		return hmLib_false;
	case 0x21:
		if(_sht75_Mode.LowAccData==hmLib_true){
			//ConfigBitを設定
			for(Cnt=0;Cnt<SHT75_I2C_RETRYNUM;++Cnt){
				//書き込み要求
				if(i2c1_start(SHT75_ADDRESS_CONFIG,0))continue;
				//ConfigBits書き込み
				if(i2c1_write(0x00))continue;
				//i2c1終了
				i2c1_stop();
				break;
			}

			if(Cnt==SHT75_I2C_RETRYNUM)return hmLib_false;
		}
		
		_sht75_Mode.LowAccData=hmLib_false;
		_sht75_Mode.LowAccData_i=hmLib_true;

		return hmLib_false;
	case 0xE0:
		if(_sht75_Mode.LowAccData==hmLib_true){
			//ConfigBitを設定
			for(Cnt=0;Cnt<SHT75_I2C_RETRYNUM;++Cnt){
				//書き込み要求
				if(i2c1_start(SHT75_ADDRESS_RESET,0))continue;
				//i2c1終了
				i2c1_stop();
				break;
			}

			if(Cnt==SHT75_I2C_RETRYNUM)return hmLib_false;
		}
		
		_sht75_Mode.Reset_i=hmLib_true;
		_sht75_Mode.LowAccData=hmLib_false;
		_sht75_Mode.LowAccData_i=hmLib_true;

		return hmLib_false;
	default:
		return hmLib_true;
	}
}
//通常モードのデータ送信用関数
void sht75_setup_talk(void){
	_sht75_Mode.Data=_sht75_Mode.SendData;
}
hmLib_boolian sht75_talk(cstring* pStr){
	uint8 Cnt;
	uint8 TLow,THigh,HLow,HHigh;

	//データ送信モード変更命令受諾
	if(_sht75_Mode.SendData_i){
		cstring_construct_dynamic_safe(pStr,1);
		if(_sht75_Mode.SendData)cstring_putc(pStr,0,0x10);
		else cstring_putc(pStr,0,0x11);

		//送信後自動的にフラグを落とす
		_sht75_Mode.SendData_i=hmLib_false;
		return hmLib_false;
	}//高精度データモード変更命令受諾
	else if(_sht75_Mode.LowAccData_i){
		cstring_construct_dynamic_safe(pStr,1);
		if(_sht75_Mode.LowAccData)cstring_putc(pStr,0,0x20);
		else cstring_putc(pStr,0,0x21);

		//送信後自動的にフラグを落とす
		_sht75_Mode.LowAccData_i=hmLib_false;
		return hmLib_false;
	}//リセット命令受諾
	else if(_sht75_Mode.Reset_i){
		cstring_construct_dynamic_safe(pStr,1);
		cstring_putc(pStr,0,0xE0);

		//送信後自動的にフラグを落とす
		_sht75_Mode.Reset_i=hmLib_false;
		return hmLib_false;
	}//データ送信
	else if(_sht75_Mode.Data){
		hmr_i2c_finalize();
		i2cSHT75_initialize();

		//温度データ取得
		for(Cnt=0;Cnt<SHT75_I2C_RETRYNUM;++Cnt){
			//i2cがエラーを返した場合
			if(i2cSHT75_start(SHT75_ADDRESS_THERMO,1)){
				//規定回数以上だと、終了
				if(Cnt==SHT75_I2C_RETRYNUM){
					cstring_construct_dynamic_safe(pStr,2);
					cstring_putc(pStr,0,0xFF);
					cstring_putc(pStr,1,0x10);	//Defaultでは0にしておく
					_sht75_Mode.Data=hmLib_false;
					return hmLib_false;
				}
				continue;
			}
			break;
		}

		while(i2cSHT75_data_can_recv());

		//現在のFIFOSRC取得
		THigh=i2cSHT75_read(0);
		TLow=i2cSHT75_read(0);
		i2cSHT75_read(1);
		i2cSHT75_stop();
		

		//湿度データ取得
		for(Cnt=0;Cnt<SHT75_I2C_RETRYNUM;++Cnt){
			//i2cがエラーを返した場合
			if(i2cSHT75_start(SHT75_ADDRESS_HUMID,1)){
				//規定回数以上だと、終了
				if(Cnt==SHT75_I2C_RETRYNUM){
					cstring_construct_dynamic_safe(pStr,2);
					cstring_putc(pStr,0,0xFF);
					cstring_putc(pStr,1,0x11);	//Defaultでは0にしておく
					_sht75_Mode.Data=hmLib_false;
					return hmLib_false;
				}
				continue;
			}
			break;
		}

		//現在のFIFOSRC取得
		HHigh=i2cSHT75_read(0);
		HLow=i2cSHT75_read(0);
		i2cSHT75_read(1);
		i2cSHT75_stop();

		hmr_i2c_initialize();

		//精度に応じてデータ書き込み
		if(_sht75_Mode.LowAccData){
			cstring_construct_dynamic_safe(pStr,4);
			cstring_putc(pStr,0,0x00);
			cstring_putc(pStr,1,TLow);
			cstring_putc(pStr,2,THigh);
			cstring_putc(pStr,3,HLow);
		}else{
			cstring_construct_dynamic_safe(pStr,5);
			cstring_putc(pStr,0,0x01);
			cstring_putc(pStr,1,TLow);
			cstring_putc(pStr,2,THigh);
			cstring_putc(pStr,3,HLow);
			cstring_putc(pStr,4,HHigh);
		}

		//送信後自動的にフラグを落とす
		_sht75_Mode.Data=hmLib_false;
		return hmLib_false;
	}

	return hmLib_true;
}
//タスクモードのデータ送信用関数
void sht75_task_setup_talk(void){}
sint16 sht75_task_data(sint16 interval){
	_sht75_Mode.Data=_sht75_Mode.SendData;
	return interval;
}
sint16 sht75_task_inform(sint16 interval){
	_sht75_Mode.SendData_i=hmLib_true;
	_sht75_Mode.LowAccData_i=hmLib_true;
	return interval;
}
#
#endif
