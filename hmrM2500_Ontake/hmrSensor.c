#ifndef HMR_SENSOR_C_INC
#define HMR_SENSOR_C_INC 112
#
#include<stdlib.h>
#ifndef HMR_SENSOR_INC
#	include"hmrSensor.h"
#endif
#ifndef HMR_SPRITEFUNCTIONS_INC
#	include"hmrSpriteFunctions.h"
#endif
//**************** Battery *********************************
cstring battery_message(cstring Str){
	cstring Ans={0,0};
	unsigned char adcdata_l,adcdata_h,size,j;
	uint32 sumadc;

	//文字が存在しなければエラーを返す
	if(cstring_size(&Str)==0){
		cstring_dynamic_construct(&Ans,1);
		cstring_putc(&Ans,0,0xF0);			//エラーコード:0xF0
		return Ans;
	}

	//モードが0x10の場合
	if(cstring_getc(&Str,0) == 0x10){

		size = 3*(cstring_size(&Str)-1)+1;			//モードを抜いてsize計算
		cstring_dynamic_construct(&Ans,size);	//文字列

		//返信用モード
		cstring_putc(&Ans,0,0x10);

		//要求されたBatteryIDの個数分データ返す
		for(j = 0; j < size; j++){
			//IDの データ取得
			sumadc = adc_get_agerage(Vref_GND,battery_id_to_adc(cstring_getc(&Str,j+1)));//IDをANに変換してそこのデータを読み読み
			adcdata_h = (unsigned char)((sumadc>>8)%256);
			adcdata_l = (unsigned char)(sumadc%256);

			//データ挿入
			cstring_putc(&Ans,3*j+1,cstring_getc(&Str,j+1));//BatteryID
			cstring_putc(&Ans,3*j+2,adcdata_l);				//LowBitsData
			cstring_putc(&Ans,3*j+3,adcdata_h);				//HighBitsData
		}
		return Ans;
	}
	return Ans;
}
//************************ GPS 
cstring gps_message(cstring Str){
	cstring Ans={0,0};
	unsigned char j;
	unsigned char* gPtr;

	spi_selectGPS(1);
	delay_ms(3);

	if(cstring_getc(&Str,0) == 0x00){//GPS交換コマンド(0x00)
		cstring_dynamic_construct(&Ans,1);
		cstring_putc(&Ans,0,0x00);

		gPtr=malloc(2);
		spi2_write(SPICMD_GPSChange);//交換コマンドを送る
		delay_ms(1);
	}else{			//GPSデータ取得コマンド(0x10)
		cstring_dynamic_construct(&Ans,21);
		cstring_putc(&Ans,0,0x10);

		spi2_write(SPICMD_GPSGet);//GPSdataよこせコマンド
		delay_ms(1);

		//restart_wdt();
		for(j=1;j<20;j++){//data受信
			cstring_putc(&Ans,j,spi2_read(SPICMD_NULL));	//コマンド以外の適当なものを送る
			delay_ms(1);
		}
		//restart_wdt();
		spi2_write(SPICMD_GPSGetChannel);//とったGPSのCHをいただく
		//spi2_write(0xBB);
		delay_ms(1);
		cstring_putc(&Ans,20,spi2_read(SPICMD_NULL));//コマンド以外の適当なものを送る
	}
	spi_selectGPS(0);

	return Ans;
}
//************************ CO2 
cstring co2_message(cstring Str){
	cstring Ans={0,0};
	unsigned char	adcdata_l,adcdata_h;//,c;
	uint32 sumadc;

	if(cstring_getc(&Str,0) == 0x30){//pump off
		cstring_dynamic_construct(&Ans,1);
		cstring_putc(&Ans,0,0x30);

		co2_powerPump(0);
	}else if(cstring_getc(&Str,0) == 0x31){//pump on
		cstring_dynamic_construct(&Ans,1);
		cstring_putc(&Ans,0,0x31);

		co2_powerPump(1);
	}else if(cstring_getc(&Str,0) == 0x20){//sensor off
		cstring_dynamic_construct(&Ans,1);
		cstring_putc(&Ans,0,0x20);

		co2_powerSensor(0);
	}else if(cstring_getc(&Str,0) == 0x21){//sensor on
		cstring_dynamic_construct(&Ans,1);
		cstring_putc(&Ans,0,0x21);

		co2_powerSensor(1);
	}else if(cstring_getc(&Str,0) == 0x10){//get CO2 data
		cstring_dynamic_construct(&Ans,3);
		cstring_putc(&Ans,0,0x10);
		
		// CO2データ取得
		sumadc = adc_get_agerage(Vref_GND,ADC_CO2);
		adcdata_h= (unsigned char)((sumadc>>8)%256);
		adcdata_l = (unsigned char)(sumadc%256);

		cstring_putc(&Ans,1,adcdata_l);
		cstring_putc(&Ans,2,adcdata_h);
	}

	return Ans;
}
//*********************** H2S 
cstring h2s_message(cstring Str){
	cstring Ans={0,0};
	unsigned char	adcdata_l,adcdata_h;//,c;
	uint32 sumadc;

	if(cstring_getc(&Str,0) == 0x20){//sensor off
		cstring_dynamic_construct(&Ans,1);
		cstring_putc(&Ans,0,0x20);

		h2s_powerSensor(0);
	}else if(cstring_getc(&Str,0) == 0x21){//sensor on
		cstring_dynamic_construct(&Ans,1);
		cstring_putc(&Ans,0,0x21);

		h2s_powerSensor(1);
	}else if(cstring_getc(&Str,0) == 0x10){//get H2S data
		cstring_dynamic_construct(&Ans,3);
		cstring_putc(&Ans,0,0x10);
		
		// H2Sデータ取得
		sumadc = adc_get_agerage(Vref_GND,ADC_H2S);
		adcdata_h= (unsigned char)((sumadc>>8)%256);
		adcdata_l = (unsigned char)(sumadc%256);

		cstring_putc(&Ans,1,adcdata_l);
		cstring_putc(&Ans,2,adcdata_h);
	}

	return Ans;
}
//************************ Acc 
void axel_initialize(void){
	while(1){
		if(i2c2_start(AXEL_ADDRESS,0))continue;
		//CTRL_REG1なるレジスタへの書き込み要求
		if(i2c2_write(0x20))continue;
		//CTRL_REG1なるレジスタの初期化		（Application Note p.10 のコピペ）
		if(i2c2_write(0x27))continue;
		break;
	}
	i2c2_stop();
/*
	while(1){
		if(i2c2_start(AXEL_ADDRESS,0))continue;
		//CTRL_REG4なるレジスタへの書き込み要求
		if(i2c2_write(0x23))continue;
		//CTRL_REG4なるレジスタの初期化		（Application Note p.10 のコピペ）
		if(i2c2_write(0x40))continue;
		break;
	}
	i2c2_stop();
	*/
}
cstring axel_message(cstring Str){
	cstring Ans={0,0};
	uint8 LowData,HighData;;

	if(cstring_getc(&Str,0) == 0x10){//データ取得
		cstring_dynamic_construct(&Ans,7);
		cstring_putc(&Ans,0,0x10);

		//======== ACC X
		while(1){
			if(i2c2_start(AXEL_ADDRESS,0))continue;
			//x軸加速度LowByte要求
			if(i2c2_write(0x28))continue;
			if (i2c2_restart(AXEL_ADDRESS,1))continue;
			break;
		}
		//x軸加速度LowByte受信
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(AXEL_ADDRESS,0))continue;
			//x軸加速度HighByte要求
			if(i2c2_write(0x29))continue;
			if (i2c2_restart(AXEL_ADDRESS,1))continue;
			break;
		}
		//x軸加速度HighByte受信
		HighData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,1,LowData);
		cstring_putc(&Ans,2,HighData);

		//========= ACC Y
		while(1){
			if(i2c2_start(AXEL_ADDRESS,0))continue;
			//y軸方向加速度LowByte要求
			if(i2c2_write(0x2A))continue;
			if (i2c2_restart(AXEL_ADDRESS,1))continue;
			break;
		}
		//y軸方向加速度LowByte受信
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(AXEL_ADDRESS,0))continue;
			//y軸方向加速度HighByte要求
			if(i2c2_write(0x2B))continue;
			if (i2c2_restart(AXEL_ADDRESS,1))continue;
			break;
		}
		HighData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,3,LowData);
		cstring_putc(&Ans,4,HighData);

		//=========== ACC Z
		while(1){
			if(i2c2_start(AXEL_ADDRESS,0))continue;
			//z軸方向加速度LowByte要求
			if(i2c2_write(0x2C))continue;
			if (i2c2_restart(AXEL_ADDRESS,1))continue;
			break;
		}
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(AXEL_ADDRESS,0))continue;
			//z軸方向加速度HighByte要求
			if(i2c2_write(0x2D))continue;
			if (i2c2_restart(AXEL_ADDRESS,1))continue;
			break;
		}
		LowData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,5,LowData);
		cstring_putc(&Ans,6,HighData);
	}

	return Ans;
}
//************************ Thermo 
cstring thermo_message(cstring Str){
	cstring Ans={0,0};
	unsigned char	adcdata_l,adcdata_h;//,c;
	uint32 sumadc;

	if(cstring_getc(&Str,0) == 0x10){//データ取得(100回平均)コマンド		
		cstring_dynamic_construct(&Ans,3);
		cstring_putc(&Ans,0,0x10);

		//温度データ取得
		sumadc = adc_get_agerage(Vref_GND,ADC_Thermo);
		adcdata_h = (unsigned char)((sumadc>>8)%256);
		adcdata_l = (unsigned char)(sumadc%256);
		cstring_putc(&Ans,1,adcdata_l);
		cstring_putc(&Ans,2,adcdata_h);
	}

	return Ans;
}
//************************ InfThermo 
cstring infthermo_message(cstring Str){
	cstring Ans={0,0};
	unsigned char	adcdata_l,adcdata_h;//,c;
	uint32 sumadc;

	if(cstring_getc(&Ans,0) == 0x10){//データ取得(100回平均)コマンド		
		cstring_dynamic_construct(&Ans,5);
		cstring_putc(&Ans,0,0x10);

		// 外部温度取得
		sumadc = adc_get_agerage(Vref_GND,ADC_InfThermo_thermo);
		adcdata_h = (unsigned char)((sumadc>>8)%256);
		adcdata_l = (unsigned char)(sumadc%256);

		cstring_putc(&Ans,1,adcdata_l);
		cstring_putc(&Ans,2,adcdata_h);

		//内部温度取得
		sumadc = adc_get_agerage(Vref_GND,ADC_InfThermo);
		adcdata_h = (unsigned char)((sumadc>>8)%256);
		adcdata_l = (unsigned char)(sumadc%256);

		cstring_putc(&Ans,3,adcdata_l);
		cstring_putc(&Ans,4,adcdata_h);
	}

	return Ans;
}
//************************ Hygro 
cstring hygro_message(cstring Str){
	cstring Ans={0,0};
	unsigned char	adcdata_l,adcdata_h;//,c;
	uint32 sumadc;

	if(cstring_getc(&Str,0) == 0x10){//データ取得(100回平均)コマンド		
		cstring_dynamic_construct(&Ans,3);
		cstring_putc(&Ans,0,0x10);

		//温度データ取得
		sumadc = adc_get_agerage(Vref_GND,ADC_Hygro);
		adcdata_h = (unsigned char)((sumadc>>8)%256);
		adcdata_l = (unsigned char)(sumadc%256);

		cstring_putc(&Ans,1,adcdata_l);
		cstring_putc(&Ans,2,adcdata_h);
	}

	return Ans;
}
//************************ Compass 
void compass_initialize(void){
	while(1){
		if(i2c2_start(COMPASS_ADDRESS,0))continue;
		//CRA_REG_Mなるレジスタへの書き込み要求
		if(i2c2_write(0x00))continue;
		//CRA_REG_Mなるレジスタを初期化　（Application Note p.11のコピペ）
		if(i2c2_write(0x04))continue;
		break;
	}
	i2c2_stop();

	while(1){
		if(i2c2_start(COMPASS_ADDRESS,0))continue;
		//CRB_REG_Mなるレジスタへの書き込み要求
		if(i2c2_write(0x01))continue;
		//CRB_REG_Mなるレジスタを初期化　（Application Note p.11のコピペ）
		if(i2c2_write(0x01))continue;
		break;
	}
	i2c2_stop();

	while(1){
		if(i2c2_start(COMPASS_ADDRESS,0))continue;
		//CRB_REG_Mなるレジスタへの書き込み要求
		if(i2c2_write(0x02))continue;
		//CRB_REG_Mなるレジスタを初期化　（Application Note p.11のコピペ）
		if(i2c2_write(0x00))continue;
		break;
	}
	i2c2_stop();
}
cstring compass_message(cstring Str){
	cstring Ans={0,0};
	uint8 LowData,HighData;

	if(cstring_getc(&Str,0) == 0x10){//データ取得
		cstring_dynamic_construct(&Ans,7);
		cstring_putc(&Ans,0,0x10);

		//===========COMPASS X
		while(1){
			if(i2c2_start(COMPASS_ADDRESS,0))continue;
			//磁場x成分LowByte要求
			if(i2c2_write(0x04))continue;
			if (i2c2_restart(COMPASS_ADDRESS,1))continue;
			break;
		}
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(COMPASS_ADDRESS,0))continue;
			//磁場x成分HighByte要求
			if(i2c2_write(0x03))continue;
			if (i2c2_restart(COMPASS_ADDRESS,1))continue;
			break;
		}
		HighData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,1,LowData);
		cstring_putc(&Ans,2,HighData);

		//=========COMPASS Y
		while(1){
			if(i2c2_start(COMPASS_ADDRESS,0))continue;
			//磁場y成分LowByte要求
			if(i2c2_write(0x06))continue;
			if (i2c2_restart(COMPASS_ADDRESS,1))continue;
			break;
		}
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(COMPASS_ADDRESS,0))continue;
			//磁場y成分HighByte要求
			if(i2c2_write(0x05))continue;
			if (i2c2_restart(COMPASS_ADDRESS,1))continue;
			break;
		}
		HighData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,3,LowData);
		cstring_putc(&Ans,4,HighData);

		//============COMPASS Z
		while(1){
			if(i2c2_start(COMPASS_ADDRESS,0))continue;
			//磁場z成分LowByte要求
			if(i2c2_write(0x08))continue;
			if (i2c2_restart(COMPASS_ADDRESS,1))continue;
			break;
		}
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(COMPASS_ADDRESS,0))continue;
			//磁場z成分HighByte要求
			if(i2c2_write(0x07))continue;
			if (i2c2_restart(COMPASS_ADDRESS,1))continue;
			break;
		}
		HighData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,5,LowData);
		cstring_putc(&Ans,6,HighData);
	}

	return Ans;
}
//************************ Gyro
void gyro_initialize(void){
	while(1){
		if(i2c2_start(GYRO_ADDRESS,0))continue;
		//CRA_REG1レジスタへの書き込み要求
		if(i2c2_write(0x20))continue;
		//CRA_REG1レジスタを初期化
		if(i2c2_write(0x0F))continue;
		break;
	}
	i2c2_stop();
}
cstring gyro_message(cstring Str){
	cstring Ans={0,0};
	uint8 LowData,HighData;

	if(cstring_getc(&Str,0) == 0x10){//データ取得
		cstring_dynamic_construct(&Ans,7);
		cstring_putc(&Ans,0,0x10);

		//===========Gyro X
		while(1){
			if(i2c2_start(GYRO_ADDRESS,0))continue;
			//x成分LowByte要求
			if(i2c2_write(0x28))continue;
			if (i2c2_restart(GYRO_ADDRESS,1))continue;
			break;
		}
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(GYRO_ADDRESS,0))continue;
			//x成分HighByte要求
			if(i2c2_write(0x29))continue;
			if (i2c2_restart(GYRO_ADDRESS,1))continue;
			break;
		}
		HighData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,1,LowData);
		cstring_putc(&Ans,2,HighData);

		//=========Gyro Y
		while(1){
			if(i2c2_start(GYRO_ADDRESS,0))continue;
			//y成分LowByte要求
			if(i2c2_write(0x2A))continue;
			if (i2c2_restart(GYRO_ADDRESS,1))continue;
			break;
		}
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(GYRO_ADDRESS,0))continue;
			//y成分HighByte要求
			if(i2c2_write(0x2B))continue;
			if (i2c2_restart(GYRO_ADDRESS,1))continue;
			break;
		}
		HighData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,3,LowData);
		cstring_putc(&Ans,4,HighData);

		//============Gyro Z
		while(1){
			if(i2c2_start(GYRO_ADDRESS,0))continue;
			//z成分LowByte要求
			if(i2c2_write(0x2C))continue;
			if (i2c2_restart(GYRO_ADDRESS,1))continue;
			break;
		}
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(GYRO_ADDRESS,0))continue;
			//z成分HighByte要求
			if(i2c2_write(0x2D))continue;
			if (i2c2_restart(GYRO_ADDRESS,1))continue;
			break;
		}
		HighData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,5,LowData);
		cstring_putc(&Ans,6,HighData);
	}

	return Ans;
	
}
//************************ Get ADC Ave 100
/*
uint32 adc_get_agerage(unsigned char mode, unsigned char ch){
	sint8 j = 0;
	uint32 sumadc;

	setup_adc1_Vref(mode);//MODE設定
	//restart_wdt();

	set_adc_channel(ch);//ADCCh選択 
	//restart_wdt();

	sumadc = 0;
	for(j=0;j<100;++j){
		sumadc += (uint16)(read_adc());
		//restart_wdt();
	}
	sumadc = sumadc/100;
	return sumadc;
}*/

//************************ SpriteCmr
boolian sprite_AutoLight=0;
boolian sprite_SpeedMode=0;
#define sprite_powerLight(onoff) HMR_PIN_HEADLIGHT_PW_set(onoff)
/*void sprite_powerLight(boolian b){
	delay_ms(1);
	spi_selectDrive(1);
	if(b){
		//restart_wdt();
		delay_ms(1);
		spi2_read('<');
		delay_ms(1);
		spi2_read(0xFF);
		delay_ms(1);
	}else{
		//restart_wdt();
		delay_ms(1);
		spi2_read('<');
		delay_ms(1);
		spi2_read(0xFE);
		delay_ms(1);
	}
	spi_selectDrive(0);
}*/
cstring sprite_message(cstring Str){
	cstring Ans={0,0};
	static unsigned char* BufPtr = 0;
	unsigned char c1;
//	int i;
	if(sprite_AutoLight){
		if(cstring_getc(&Str,0)==0x10)sprite_powerLight(1);
		else sprite_powerLight(0);
	}

	//エラーがないことを確認
	//エラーがあればエラー処理を行う
	if(sprite_getErrorFlag()){
		//メモリを確保していたなら捨てる
		if(BufPtr != 0){
			free(BufPtr);
			BufPtr = 0;
		}

		//エラーを伝える
		cstring_dynamic_construct(&Ans,7);
		cstring_putc(&Ans,0,0xFF);						//Error
		cstring_putc(&Ans,1,sprite_getStatus());		// 現在のステータス
		cstring_putc(&Ans,2,sprite_getSubStatus());		//現在のサブステータス
		cstring_putc(&Ans,3, sprite_isError());			//現在エラー状態かどうか
		cstring_putc(&Ans,4,sprite_getErrorStatus());	//エラー時のステータス
		cstring_putc(&Ans,5,sprite_getErrorSubStatus());//エラー時のサブステータス
		cstring_putc(&Ans,6,sprite_getErrorPosition());	//エラー時のポジション

		//エラーフラグを消去
		sprite_clearErrorFlag(); 
	}
	//強制リセット中か
	else if(sprite_getStatus() == 0xF0){
		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,cstring_getc(&Str,0));
		cstring_putc(&Ans,1,0xF0);
	}
	//ここからコマンド通常処理に移る
	//内部情報取得 
	else if(cstring_getc(&Str,0) == 0x00){
		cstring_dynamic_construct(&Ans,9);
		cstring_putc(&Ans,0,0x00);
		cstring_putc(&Ans,1,sprite_getStatus());		// 現在のステータス
		cstring_putc(&Ans,2,sprite_getSubStatus());		//現在のサブステータス
		cstring_putc(&Ans,3,sprite_isError());			//現在エラー状態かどうか
		cstring_putc(&Ans,4,sprite_getErrorStatus());	//エラー時のステータス
		cstring_putc(&Ans,5,sprite_getErrorSubStatus());//エラー時のサブステータス
		cstring_putc(&Ans,6,sprite_getErrorPosition());	//エラー時のポジション
		cstring_putc(&Ans,7,sprite_getRequestImageSize());//imageSizeを得る
		cstring_putc(&Ans,8,sprite_getAutoResetMode());	//AutoResetを得る 
	}
	//写真撮影
	else if(cstring_getc(&Str,0) == 0x10){
		//写真撮っていい状態なら
		if(sprite_isIdle() == 1){
			//バッファがセットされていなければ
			if(sprite_is_setPicDataBuf()==0){
				//バッファセット
				BufPtr = malloc(SPRITE_PACKSIZE + SPRITE_BUF_HEADER);
				//バッファをSPRITEに教えてあげる
				sprite_setPicDataBuf(&(BufPtr[SPRITE_BUF_HEADER]), SPRITE_PACKSIZE);
			}

			//PictureSize設定
			if(cstring_getc(&Str,1)>=2){
				c1=sprite_PictureSize_640_480;
			}else if(cstring_getc(&Str,1)==1){
				c1=sprite_PictureSize_320_240;
			}else{
				c1=sprite_PictureSize_160_120;
			}
			
			
			//SPRITEで写真撮る命令送る
			sprite_beginTakePic(c1);

			//とりあえず状態返す
			cstring_dynamic_construct(&Ans,2);
			cstring_putc(&Ans,0,0x10);
			cstring_putc(&Ans,1,0x10);
		//写真のデータ数撮っていい状態なら
		}else if(sprite_can_sizeAllPicData() == 1){
			//とりあえず状態返す
			cstring_dynamic_construct(&Ans,4);
			cstring_putc(&Ans,0,0x10);
			cstring_putc(&Ans,1,0x11);
			cstring_putc(&Ans,2,(unsigned char)(sprite_sizeAllPicData()));
			cstring_putc(&Ans,3,(unsigned char)((sprite_sizeAllPicData())/256));
		}
		//それ以外…処理待ち
		else{
			cstring_dynamic_construct(&Ans,2);
			cstring_putc(&Ans,0,0x10);
			cstring_putc(&Ans,1,0x10);
		}
	}
	//撮影データ取得
	else if(cstring_getc(&Str,0) == 0x20){
		//データすべて受信したとき
		if(sprite_getStatus() == sprite_Status_FinishTakePic){
			cstring_dynamic_construct(&Ans,2);
			cstring_putc(&Ans,0,0x20);
			cstring_putc(&Ans,1,0x30);
		}
		//データ受信可能な時
		else if(sprite_can_getPicData() == 1){
			//バッファからデータをもらうので、staticに作成
			cstring_static_construct(&Ans,BufPtr,BufPtr+SPRITE_BUF_HEADER+sprite_sizePicData());
			cstring_putc(&Ans,0,0x20);
			cstring_putc(&Ans,1,0x21);
			//取得済みデータサイズ[2]
			cstring_putc(&Ans,2,(unsigned char)(sprite_sizeGotPicData()%256));
			cstring_putc(&Ans,3,(unsigned char)((sprite_sizeGotPicData()>>8)%256));

			//データ受信完了を伝える
			sprite_getPicData();

			//次のデータがある時
			if(sprite_is_getAllPicData()==0){
				//メモリ確保//バッファセット
				BufPtr = malloc(SPRITE_PACKSIZE + SPRITE_BUF_HEADER);
				//バッファをSPRITEに教えてあげる 取得開始
				sprite_setPicDataBuf(BufPtr +  SPRITE_BUF_HEADER, SPRITE_PACKSIZE);
			}
		}
		//データ受信まだ不可能のとき
		else{
			cstring_dynamic_construct(&Ans,4);
			cstring_putc(&Ans,0,0x20);
			cstring_putc(&Ans,1,sprite_getStatus());
			//取得済みデータサイズ[2]
			cstring_putc(&Ans,2,(unsigned char)(sprite_sizeGotPicData()%256));
			cstring_putc(&Ans,3,(unsigned char)((sprite_sizeGotPicData()>>8)%256));
		}
	}
	//撮影データ取得完了
	else if(cstring_getc(&Str,0) == 0x30){
		//写真撮影を終えるコマンド
		sprite_endTakePic();

		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x30);
		cstring_putc(&Ans,1,0x00);
	}

	//内部リセット要求
	else if(cstring_getc(&Str,0) == 0x40){
		//リセットを行う
		sprite_requestCommandReset();

		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x40);
		cstring_putc(&Ans,1, sprite_getStatus());
	}
	//エラー時自動強制リセットON・OFF
	else if(cstring_getc(&Str,0) == 0x50){
		//AutoResetMode設定
		sprite_setAutoResetMode(cstring_getc(&Str,1));
		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x50);
		cstring_putc(&Ans,1, sprite_getAutoResetMode());
	}
	//強制リセット
	else if(cstring_getc(&Str,0) == 0xF0){
		//強制リセットをかける
		sprite_requestPowerReset();

		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0xF0);
		cstring_putc(&Ans,1, sprite_getStatus());
	}
	//ライトの設定
	else if(cstring_getc(&Str,0) == 0x60){
		sprite_AutoLight=cstring_getc(&Str,1);

		//ライトをつける
		if(sprite_AutoLight==0)sprite_powerLight(0);

		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x60);
		cstring_putc(&Ans,1, sprite_AutoLight);
	}

	return Ans;
}
//static unsigned char Buffer1[sprite_BufferHeader+sprite_BufferSize_LARGE];
//static unsigned char Buffer2[sprite_BufferHeader+sprite_BufferSize_LARGE];
static unsigned char Buffer[sprite_BufferNum][sprite_BufferHeader+sprite_BufferSize_LARGE];
cstring sprite_message2(cstring Str){
	cstring Ans={0,0};
	static boolian BufferUsed=0;
	static unsigned char* BufferPtr=0;
	static unsigned char BufferNo=0;
	unsigned char c1;

	if(cstring_size(&Str)<1)return;

	//エラーがないことを確認
	//エラーがあればエラー処理を行う
	if(sprite_getErrorFlag()){
		//メモリを確保していたなら捨てる
//		if(BufPtr != 0){
//			free(BufPtr);
//			BufPtr = 0;
//		}

		//エラーを伝える
		cstring_dynamic_construct(&Ans,7);
		cstring_putc(&Ans,0,0xFF);						//Error
		cstring_putc(&Ans,1,sprite_getStatus());		// 現在のステータス
		cstring_putc(&Ans,2,sprite_getSubStatus());		//現在のサブステータス
		cstring_putc(&Ans,3, sprite_isError());			//現在エラー状態かどうか
		cstring_putc(&Ans,4,sprite_getErrorStatus());	//エラー時のステータス
		cstring_putc(&Ans,5,sprite_getErrorSubStatus());//エラー時のサブステータス
		cstring_putc(&Ans,6,sprite_getErrorPosition());	//エラー時のポジション

		//エラーフラグを消去
		sprite_clearErrorFlag(); 
	}
	//強制リセット中か
	else if(sprite_getStatus() == 0xF0){
		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,cstring_getc(&Str,0));
		cstring_putc(&Ans,1,0xF0);
	}
	//ここからコマンド通常処理に移る
	//内部情報取得 
	else if(cstring_getc(&Str,0) == 0x00){
		cstring_dynamic_construct(&Ans,9);
		cstring_putc(&Ans,0,0x00);
		cstring_putc(&Ans,1,sprite_getStatus());		// 現在のステータス
		cstring_putc(&Ans,2,sprite_getSubStatus());		//現在のサブステータス
		cstring_putc(&Ans,3,sprite_isError());			//現在エラー状態かどうか
		cstring_putc(&Ans,4,sprite_getErrorStatus());	//エラー時のステータス
		cstring_putc(&Ans,5,sprite_getErrorSubStatus());//エラー時のサブステータス
		cstring_putc(&Ans,6,sprite_getErrorPosition());	//エラー時のポジション
		cstring_putc(&Ans,7,sprite_getRequestImageSize());//imageSizeを得る
		cstring_putc(&Ans,8,sprite_getAutoResetMode());	//AutoResetを得る 
	}
	//写真撮影orデータ要求モード
	else if((cstring_getc(&Str,0)&0xF0) == 0x10){
		//写真撮っていい状態で、撮影要求が来た場合
		if(sprite_isIdle() == 1 && BufferUsed==0){
			//バッファがセットされていなければ
			if(sprite_is_setPicDataBuf()==0){
				//バッファセット
				//バッファをSPRITEに教えてあげる
				BufferPtr=Buffer[(BufferNo++)%sprite_BufferNum];
				if(sprite_SpeedMode){
					sprite_setPicDataBuf(BufferPtr+sprite_BufferHeader, sprite_BufferSize_LARGE);
				}else{
					sprite_setPicDataBuf(BufferPtr+sprite_BufferHeader, sprite_BufferSize_SMALL);
				}
				BufferUsed=1;
			}

			//PictureSize設定
			if(cstring_getc(&Str,1)>=2){
				c1=sprite_PictureSize_640_480;
			}else if(cstring_getc(&Str,1)==1){
				c1=sprite_PictureSize_320_240;
			}else{
				c1=sprite_PictureSize_160_120;
			}
			
			if(sprite_AutoLight){
				sprite_powerLight(1);
				delay_ms(200);
			}

			//SPRITEで写真撮る命令送る
			sprite_beginTakePic(c1);

			//写真データを取っていい状態まで待つ
			while(!sprite_can_sizeAllPicData()){
				if(sprite_isError()){
					sprite_powerLight(0);
					return;
				}
			}

			if(sprite_AutoLight){
				sprite_powerLight(0);
			}

			//データが来るまで待つ
			while(!sprite_can_getPicData()){
				if(sprite_isError())return;
			}

			//バッファからデータをもらうので、staticに作成
			msg_set_nondelete_mode();
			cstring_static_construct(&Ans,BufferPtr,BufferPtr+sprite_BufferHeader+sprite_sizePicData());
			cstring_putc(&Ans,0,(unsigned char)(0x10));
			cstring_putc(&Ans,1,(unsigned char)(sprite_sizeAllPicData()));
			cstring_putc(&Ans,2,(unsigned char)(sprite_sizeAllPicData()>>8));

			//データ受信完了を伝える
			sprite_getPicData();
			BufferUsed=0;
			delay_ms(5);
			//次のデータがある時
			if(sprite_is_getAllPicData()){
				//写真撮影を終えるコマンド
				sprite_endTakePic();
			}//次のデータがない時
			else{
				//バッファセット
				BufferPtr=Buffer[(BufferNo++)%sprite_BufferNum];
				if(sprite_SpeedMode){
					sprite_setPicDataBuf(BufferPtr+sprite_BufferHeader, sprite_BufferSize_LARGE);
				}else{
					sprite_setPicDataBuf(BufferPtr+sprite_BufferHeader, sprite_BufferSize_SMALL);
				}
				BufferUsed=1;
			}
		}else{
			//データが来るまで待つ
			while(!sprite_can_getPicData()){
				if(sprite_isError())return;
			}

			//バッファからデータをもらうので、staticに作成
			msg_set_nondelete_mode();
			cstring_static_construct(&Ans,BufferPtr,BufferPtr+3+sprite_sizePicData());
			cstring_putc(&Ans,0,(unsigned char)(0x11));
			cstring_putc(&Ans,1,(unsigned char)(sprite_sizeGotPicData()));
			cstring_putc(&Ans,2,(unsigned char)(sprite_sizeGotPicData()>>8));

			//データ受信完了を伝える
			sprite_getPicData();
			BufferUsed=0;
			delay_ms(5);

			//次のデータがある時
			if(sprite_is_getAllPicData()){
				//写真撮影を終えるコマンド
				sprite_endTakePic();
			}//次のデータがない時
			else{
				//バッファセット
				BufferPtr=Buffer[(BufferNo++)%sprite_BufferNum];
				if(sprite_SpeedMode){
					sprite_setPicDataBuf(BufferPtr+sprite_BufferHeader, sprite_BufferSize_LARGE);
				}else{
					sprite_setPicDataBuf(BufferPtr+sprite_BufferHeader, sprite_BufferSize_SMALL);
				}
				BufferUsed=1;
			}
		}
	}//内部リセット要求
	else if(cstring_getc(&Str,0) == 0x40){
		//リセットを行う
		sprite_requestCommandReset();

		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x40);
		cstring_putc(&Ans,1, sprite_getStatus());
	}//エラー時自動強制リセットON・OFF
	else if(cstring_getc(&Str,0) == 0x51){
		//AutoResetMode設定
		sprite_setAutoResetMode(cstring_getc(&Str,1));
		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x51);
		cstring_putc(&Ans,1, sprite_getAutoResetMode());
	}//強制リセット
	else if(cstring_getc(&Str,0) == 0x50){
		//強制リセットをかける
		sprite_requestPowerReset();

		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x50);
		cstring_putc(&Ans,1, sprite_getStatus());
	}//ライトの自動ON/OFFの設定
	else if(cstring_getc(&Str,0) == 0x61){
		sprite_AutoLight=cstring_getc(&Str,1);

		//ライトを消す時
		if(sprite_AutoLight==0)sprite_powerLight(0);

		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x61);
		cstring_putc(&Ans,1, sprite_AutoLight);
	}//ライトのON/OFF
	else if(cstring_getc(&Str,0) == 0x60){
		if(!sprite_AutoLight){
			sprite_powerLight(cstring_getc(&Str,1));
			cstring_dynamic_construct(&Ans,2);
			cstring_putc(&Ans,0,0x60);
			cstring_putc(&Ans,1, cstring_getc(&Str,1));
		}else{
			cstring_dynamic_construct(&Ans,2);
			cstring_putc(&Ans,0,0x60);
			cstring_putc(&Ans,1,0xFF);
		}
	}//スピードモードかどうかの設定
	else if(cstring_getc(&Str,0)==0x70){
		sprite_SpeedMode=cstring_getc(&Str,1);
		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x70);
		cstring_putc(&Ans,1, sprite_SpeedMode);
	}
	return Ans;
}
//受信しながら送信する場合にMessageに登録するための関数
//	現在のは受信完了後にのみ送信しているので、不使用
msg_dsize_t sprite_sendable_bufdata(void){return sprite_sizePicData();}
#
#endif
