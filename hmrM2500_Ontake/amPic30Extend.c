#ifndef AMPIC30EXTEND_C_INC
#define AMPIC30EXTEND_C_INC 101
#
#define HMLIB_NOLIB
#ifndef AMPIC30EXTEND_INC
#	include "amPic30Extend.h"
#endif

void uart1_enable_fget_interrupt(void){
	IFS0bits.U1RXIF = U1STAbits.URXDA;
	IEC0bits.U1RXIE = 1;
}
void uart1_disable_fget_interrupt(void){
	IEC0bits.U1RXIE = 0;
}
boolian uart1_is_enable_fget_interrupt(void){return IEC0bits.U1RXIE;}
void uart1_set_fget_interrupt_flag(void){
	IFS0bits.U1RXIF = 1;
}
void uart1_clear_fget_interrupt_flag(void){
	IFS0bits.U1RXIF = 0;
}
void uart1_enable_fput_interrupt(void){
	IEC0bits.U1TXIE = 1;
}
void uart1_disable_fput_interrupt(void){
	IEC0bits.U1TXIE = 0;
}
boolian uart1_is_enable_fput_interrupt(void){return IEC0bits.U1TXIE;}
void uart1_set_fput_interrupt_flag(void){
	IFS0bits.U1TXIF = 1;
}
void uart1_clear_fput_interrupt_flag(void){
	IFS0bits.U1TXIF = 0;
}
void uart2_enable_fget_interrupt(void){
	IFS1bits.U2RXIF = U2STAbits.URXDA;
	IEC1bits.U2RXIE = 1;
}
void uart2_disable_fget_interrupt(void){
	IEC1bits.U2RXIE = 0;
}
boolian uart2_is_enable_fget_interrupt(void){return IEC1bits.U2RXIE;}
void uart2_set_fget_interrupt_flag(void){
	IFS1bits.U2RXIF = 1;
}
void uart2_clear_fget_interrupt_flag(void){
	IFS1bits.U2RXIF = 0;
}
void uart2_enable_fput_interrupt(void){
	IEC1bits.U2TXIE = 1;
}
void uart2_disable_fput_interrupt(void){
	IEC1bits.U2TXIE = 0;
}
boolian uart2_is_enable_fput_interrupt(void){return IEC1bits.U2TXIE;}
void uart2_set_fput_interrupt_flag(void){
	IFS1bits.U2TXIF = 1;
}
void uart2_clear_fput_interrupt_flag(void){
	IFS1bits.U2TXIF = 0;
}
//timer1用関数
void timer1_initialize(uint16 time){ini_timer1(time);}
//timer1の割り込みフラグをクリア
void timer1_clear_interrupt_flag(void){
	IFS0bits.T1IF = 0;			//T1割り込みフラグをクリア
}
//timer1の割り込みフラグをセット
void timer1_set_interrupt_flag(void){
	IFS0bits.T1IF = 1;			//T1割り込みフラグをクリア
}
//timer1のカウントをリセット
void timer1_reset(void){
	TMR1 = 0;					//T1カウントをクリア
}
//timer1が割り込み有効かどうかの確認
boolian timer1_is_enable_interrupt(void){return IEC0bits.T1IE!=0;}
//timer1の割り込み有効
void timer1_enable_interrupt(void){enable_interrupt_timer1();}
//timer1の割り込み無効
void timer1_disable_interrupt(void){disable_interrupt_timer1();}
//timer4&5連動タイマーの初期化
void timer4_5_initialize(uint16 time){//time(ms)（1～10000msの間）に一回割り込む
	ini_timer4(time);
}
//timer4&5連動タイマーの終端化
void timer4_5_finalize(){
	timer4_5_disable_interrupt();
	timer4_5_clear_interrupt_flag();
	timer4_5_reset();
}
//timer4&5の割り込みフラグをクリア
void timer4_5_clear_interrupt_flag(void){
	IFS1bits.T5IF = 0;			
}
//timer4&5の割り込みフラグをセット
void timer4_5_set_interrupt_flag(void){
	IFS1bits.T5IF = 1;			
}
//timer4&5のカウントをリセット
void timer4_5_reset(void){
	TMR5 = 0x00; // Clear 32-bit Timer (msw)
	TMR4 = 0x00; // Clear 32-bit Timer (lsw)
}
//timer4&5が割り込み有効かどうかの確認
boolian timer4_5_is_enable_interrupt(void){return IEC1bits.T5IE!=0;}
//timer4&5の割り込み有効
void timer4_5_enable_interrupt(void){enable_interrupt_timer4();}
//timer4&5の割り込み無効
void timer4_5_disable_interrupt(void){disable_interrupt_timer4();}
//====================RTCC====================
#define _rtcc_lock() 		RCFGCALbits.RTCWREN=0;
#define _rtcc_active()		RCFGCALbits.RTCEN = 1;
#define	_rtcc_inactive()		RCFGCALbits.RTCEN = 0;
//#define EnableSecOsc()	 __builtin_write_OSCCONL(0x02);
void _rtcc_unlock(void){
	asm("MOV #0x55,W0");
	asm("MOV W0,NVMKEY");                      // Write the 0x55 key
	asm("MOV #0xAA,W1");
	asm("MOV W1,NVMKEY");                      // Write the 0xAA key

	RCFGCALbits.RTCWREN = 1;
}
//RTCC用セカンドオシレータ初期化プロセス
void rtcc_oscilator_initialize(void){
	__builtin_write_OSCCONL(0x02);		// EnableSecOsc
}
//RTCC初期化関数 引数が割り込み間隔(秒)
void rtcc_initialize(unsigned char YMD[6],unsigned char WeekNo,unsigned char HMS[6]){
	uint16 Val[4]={0,0,0,0};
	Val[3]|=(((HMS[5]-'0')&0x0F)<<0);
	Val[3]|=(((HMS[4]-'0')&0x0F)<<1);
	Val[3]|=(((HMS[3]-'0')&0x0F)<<2);
	Val[3]|=(((HMS[2]-'0')&0x0F)<<3);
	Val[2]|=(((HMS[1]-'0')&0x0F)<<0);
	Val[2]|=(((HMS[0]-'0')&0x0F)<<1);
	Val[2]|=(((WeekNo-'0')&0x0F)<<2);
	Val[1]|=(((YMD[5]-'0')&0x0F)<<0);
	Val[1]|=(((YMD[4]-'0')&0x0F)<<1);
	Val[1]|=(((YMD[3]-'0')&0x0F)<<2);
	Val[1]|=(((YMD[2]-'0')&0x0F)<<3);
	Val[0]|=(((YMD[1]-'0')&0x0F)<<0);
	Val[0]|=(((YMD[0]-'0')&0x0F)<<1);

	//===RTCC Critical Begin===
	_rtcc_unlock();								//unlock Timer Registers
	_rtcc_inactive();                                  //disable the RTCC peripheral

	/* Configure the alarm settings*/
	ALCFGRPTbits.ALRMEN = 0;					//alarm enable flag
	ALCFGRPTbits.CHIME = 0;  	 				//no rolloever of the repeat count	
	ALCFGRPTbits.AMASK = 0;                     //alarm mask configuration bits
	ALCFGRPTbits.ARPT =  0;                     //alarm repeat counter value bits 	

	RCFGCALbits.RTCOE = 0;   	//専用ピンから外部へのクロック出力を有効にするか? 0:無効
//	PADCFG1 = 0x0000;			//外部への出力を有効にした際の出力内容の設定

	/* Load the initial values to the RTCC value registers*/
	RCFGCALbits.RTCPTR = 0b11;						//point to Year register
	//RTCVALへ書き込むと自動的にRTCVALの参照先がインクリメントされる
	RTCVAL = Val[0];
	RTCVAL = Val[1];
	RTCVAL = Val[2];
	RTCVAL = Val[3];

	RCFGCALbits.CAL=0x0000;                     //クロックの誤差の補正 0なら補正無し

	_rtcc_active();                                   //enable RTCC peripheral
	_rtcc_lock();                                 //lock the RTCC value registers
	//===RTCC Critical End===
}
//RTCCの割り込みフラグをクリア
void rtcc_clear_interrupt_flag(void){IFS3bits.RTCIF = 0; }
//RTCCの割り込みフラグをセット
void rtcc_set_interrupt_flag(void){IFS3bits.RTCIF = 1; }
//RTCCのカウントをリセット
void rtcc_reset_interrupt(void){IFS3bits.RTCIF = 0;}
//RTCCが割り込み有効かどうかの確認
boolian rtcc_is_enable_interrupt(void){return ALCFGRPTbits.ALRMEN;}
//RTCCの割り込み有効
void rtcc_enable_interrupt(unsigned char MD[4],unsigned char WeekNo,unsigned char HMS[6],boolian IterateNoLimitFlag_,uint8 IterateNum_){
	unsigned char MaskFlag=1;
	uint16 Val[3]={0,0,0};
	//実行中なら割り込みから外れる
	if(rtcc_is_enable_interrupt())return;

	MaskFlag=1;		//毎秒

	if(HMS[5]!='-'){
		Val[2]|=(((HMS[5]-'0')&0x0F)<<0);	
		MaskFlag=2;	//毎10秒
	}
	if(HMS[4]!='-'){
		Val[2]|=(((HMS[4]-'0')&0x0F)<<1);	
		MaskFlag=3;	//毎分
	}
	if(HMS[3]!='-'){
		Val[2]|=(((HMS[3]-'0')&0x0F)<<2);	
		MaskFlag=4;	//毎10分
	}
	if(HMS[2]!='-'){
		Val[2]|=(((HMS[2]-'0')&0x0F)<<3);	
		MaskFlag=5;	//毎時
	}
	if(HMS[0]!='-' || HMS[1]!='-'){
		Val[1]|=(((HMS[0]-'0')&0x0F)<<1)+(((HMS[1]-'0')&0x0F)<<0);	
		MaskFlag=6;	//毎日
	}
	if(WeekNo!='-'){
		Val[1]|=(((WeekNo-'0')&0x0F)<<2);
		MaskFlag=7;	//毎週
	}
	if( MD[2]!='-' ||  MD[3]!='-'){
		Val[0]|=((( MD[2]-'0')&0x0F)<<1)+((( MD[3]-'0')&0x0F)<<0);	
		MaskFlag=8;	//毎月
	}
	if( MD[0]!='-' ||  MD[1]!='-'){
		Val[0]|=((( MD[0]-'0')&0x0F)<<3)+((( MD[1]-'0')&0x0F)<<2);
		MaskFlag=9;	//毎年
	}

	//===RTCC Critical Begin===
	_rtcc_unlock();								//unlock Timer Registers
	_rtcc_inactive();                                  //disable the RTCC peripheral

	ALCFGRPTbits.ALRMPTR = 2;  	 				//Point to Month/Day register
	//ALRMVALへ書き込むと自動的にALRMVALの参照先がインクリメントされる
	RTCVAL = Val[0];
	RTCVAL = Val[1];
	RTCVAL = Val[2];

	ALCFGRPTbits.CHIME = IterateNoLimitFlag_;  	//無限繰返しフラグ
	ALCFGRPTbits.ARPT = IterateNum_;			//繰り返し回数
	ALCFGRPTbits.AMASK = MaskFlag;		//アラーム用の参照マスク

	ALCFGRPTbits.ALRMEN = 1;

	_rtcc_active();                                   //enable RTCC peripheral
	_rtcc_lock();                                 //lock the RTCC value registers
	//===RTCC Critical End===

	rtcc_reset_interrupt();
	rtcc_clear_interrupt_flag();
}
//RTCCの割り込み無効(繰り返し回)
void rtcc_disable_interrupt(void){
	//===RTCC Critical Begin===
	_rtcc_unlock();								//unlock Timer Registers
	_rtcc_inactive();                                  //disable the RTCC peripheral
	ALCFGRPTbits.ALRMEN = 0;
	_rtcc_active();                                   //enable RTCC peripheral
	_rtcc_lock();                                 //lock the RTCC value registers
	//===RTCC Critical End===
}

//======================I2C1==============================
boolian I2C1_BusyFlag;		//I2C1処理中：1
void i2c1_initialize(uint32 SystemClock_,uint16 I2C_ClockMode_,uint16 nsPGD_){
	I2C1_BusyFlag=hmLib_false;
	I2C1CON=0x00;

	//Configre SCA/SDA pin as open-drain
	ODCAbits.ODCA2=0;
	ODCAbits.ODCA3=0;
	
	//アドレスのマスクの有無の設定 マスタとして動作する場合は常にクリア
	I2C1CONbits.IPMIEN=0;
	//クロックストレッチを無効化
	I2C1CONbits.STREN=0;
	//スレーブのアドレスのビット数を7bitにする（これを1にすると10bit）
	I2C1CONbits.A10M=0;
	//クロックをリリース
	I2C1CONbits.SCLREL=1;

	//ボーレートからI2Cクロックを生成するための何か設定
	//	BRG= ( 1/FSCL - PGD ) * FCY -1
	//	FSCL：I2C側のクロックスピード 100k,400k,1Mの3種類が主流らしい
	//		100k:1.0e-5s=10000ns
	//		400k:2.5e-6s= 2500ns
	//		  1M:1.0e-6s= 1000ns
	//	PGD : Pulse_Gobbler_Delay デバイス固有　データシート参照
	//		Pic4EP512GU810:1.30e-7
	//	FCY : システム側のクロック PLL後　ただし，DOZE利用の場合はCPUクロックと一致しない
	I2C1BRG=((uint32)(I2C_ClockMode_+nsPGD_)*(SystemClock_/100000)-10000)/10000;

	//スレーブのアドレスを初期化？
	I2C1ADD=0;
	//スレーブのアドレスマスク機能を禁止？
	I2C1MSK=0;

	//I2C有効化
	I2C1CONbits.I2CEN=1;

//	IEC3bits.MI2C1IE = 1;
//	IFS3bits.MI2C1IF = 0;
}
//I2C1の終端化処理
void i2c1_finalize(void){
	I2C1_BusyFlag=hmLib_false;
	I2C1CON=0x00;
}
//通信開始(IO=1が受信，0が送信)
boolian i2c1_start(uint8 slave_address,boolian IO){
	//以前の通信処理待ち
	i2c1_wait_process();

	//Busyフラグを上げる
	I2C1_BusyFlag=1;
	//通信開始
	i2c1_interrupt_setup_start();
	
	//以前の通信処理待ち(＝start処理待ち)
	i2c1_wait_process();
	
	//Busyフラグを上げる
	I2C1_BusyFlag=1;
	//slave addressとIO(write or read)を送信
	i2c1_interrupt_do_start(slave_address,IO);

	//以前の通信処理待ち
	i2c1_wait_process();
	return i2c1_check_NACK();
}
//通信再開(IO=1が受信，0が送信)(終了処理をせずにIOを変更するときに使う)
boolian i2c1_restart(uint8 slave_address,boolian IO){
	//以前の通信処理待ち
	i2c1_wait_process();

	//Busyフラグを上げる
	I2C1_BusyFlag=1;
	//通信再開
	i2c1_interrupt_setup_restart();

	//以前の通信処理待ち（＝restart処理待ち）
	i2c1_wait_process();

	//Busyフラグを上げる
	I2C1_BusyFlag=1;
	//slave addressとIO(write or read)を送信
	i2c1_interrupt_do_restart(slave_address,IO);

	//以前の通信処理待ち
	i2c1_wait_process();
	return i2c1_check_NACK();
}
//通信終了
void i2c1_stop(void){
	//以前の通信処理待ち
	i2c1_wait_process();

	//Busyフラグを上げる
	I2C1_BusyFlag=1;
	//通信終了処理開始
	i2c1_interrupt_stop();
}
//送信関数
boolian i2c1_write(uint8 c){
	//以前の通信処理待ち
	i2c1_wait_process();

	//Busyフラグを上げる
	I2C1_BusyFlag=1;
	//バッファに書き込み
	i2c1_interrupt_write(c);

	//以前の通信処理待ち(slave側からの応答待ち)
	i2c1_wait_process();

	//ACK or NACK を返す
	return i2c1_check_NACK(); 
}
//受信関数
uint8 i2c1_read(boolian EndFlag){
	//以前の通信処理待ち
	i2c1_wait_process();

	//Busyフラグを上げる
	I2C1_BusyFlag=1;
	//受信enable
	i2c1_interrupt_settup_read();

	//以前の通信処理待ち（＝受信待ち）
	i2c1_wait_process();

	//Busyフラグを上げる
	I2C1_BusyFlag=1;
	//データ受信およびACK/NACK送信
	return i2c1_interrupt_do_read(EndFlag);
}
//ACK送信関数
void _i2c1_send_ACK(void){
	//ACK送信モードに設定
	I2C1CONbits.ACKDT=0;
	//ACK送信
	I2C1CONbits.ACKEN=1;
}
//NACK送信関数
void _i2c1_send_NACK(void){
	//NACK送信モードに設定
	I2C1CONbits.ACKDT=1;
	//NACK送信
	I2C1CONbits.ACKEN=1;
}
//NACK受信チェック関数(0:ACK,1:NACK)
boolian i2c1_check_NACK(void){
	//以前の通信処理待ち
	i2c1_wait_process();	
	return I2C1STATbits.ACKSTAT;
}
//受信確認関数
boolian i2c1_data_is_in(void){return I2C1STATbits.RBF;}
//I2Cの処理待ち関数（処理の競合を防ぐ）
void i2c1_wait_process(void){
	if(I2C1_BusyFlag){
		//割り込みフラグが上がるまで待つ（処理終了時に自動的に割り込み生成される）
		while(!_MI2C1IF);
		//busyフラグクリア
		I2C1_BusyFlag=0;
		//割り込みフラグクリア
		_MI2C1IF=0;
	}
}
//割り込み用通信開始準備関数
void i2c1_interrupt_setup_start(void){I2C1CONbits.SEN=1;}
//割り込み用通信開始関数
void i2c1_interrupt_do_start(uint8 slave_address,boolian IO){I2C1TRN=((0b00000001&IO)|(slave_address<<1));};
//割り込み用通信再開準備関数
void i2c1_interrupt_setup_restart(void){I2C1CONbits.RSEN=1;}
//割り込み用通信終了関数
void i2c1_interrupt_stop(void){I2C1CONbits.PEN=1;}
//割り込み用送信関数
void i2c1_interrupt_write(uint8 c){I2C1TRN=c;}
//割り込み用受信準備関数
void i2c1_interrupt_settup_read(void){I2C1CONbits.RCEN=1;}
//割り込み用受信関数
uint8 i2c1_interrupt_do_read(boolian EndFlag){
	//slaveへACK/NACK送信
	if(EndFlag)_i2c1_send_NACK();	//NACK送信（受信処理終了）
	else _i2c1_send_ACK();			//ACK送信

	//受信レジスタを返す
	return I2C1RCV;
}


//======================I2C2==============================
boolian I2C2_BusyFlag;		//I2C2処理中：1
void i2c2_initialize(uint32 SystemClock_,uint16 I2C_ClockMode_,uint16 nsPGD_){
	I2C2_BusyFlag=hmLib_false;
	I2C2CON=0x00;

	//Configre SCA/SDA pin as open-drain
	ODCAbits.ODCA2=0;
	ODCAbits.ODCA3=0;
	
	//アドレスのマスクの有無の設定 マスタとして動作する場合は常にクリア
	I2C2CONbits.IPMIEN=0;
	//クロックストレッチを無効化
	I2C2CONbits.STREN=0;
	//スレーブのアドレスのビット数を7bitにする（これを1にすると10bit）
	I2C2CONbits.A10M=0;
	//クロックをリリース
	I2C2CONbits.SCLREL=1;

	//ボーレートからI2Cクロックを生成するための何か設定
	//	BRG= ( 1/FSCL - PGD ) * FCY -1
	//	FSCL：I2C側のクロックスピード 100k,400k,1Mの3種類が主流らしい
	//		100k:1.0e-5s=10000ns
	//		400k:2.5e-6s= 2500ns
	//		  1M:1.0e-6s= 1000ns
	//	PGD : Pulse_Gobbler_Delay デバイス固有　データシート参照
	//		Pic4EP512GU810:1.30e-7
	//	FCY : システム側のクロック PLL後　ただし，DOZE利用の場合はCPUクロックと一致しない
	I2C2BRG=((uint32)(I2C_ClockMode_+nsPGD_)*(SystemClock_/100000)-10000)/10000;

	//スレーブのアドレスを初期化？
	I2C2ADD=0;
	//スレーブのアドレスマスク機能を禁止？
	I2C2MSK=0;

	//I2C有効化
	I2C2CONbits.I2CEN=1;

//	IEC3bits.MI2C2IE = 1;
//	IFS3bits.MI2C2IF = 0;
}
//I2C2の終端化処理
void i2c2_finalize(void){
	I2C2_BusyFlag=hmLib_false;
	I2C2CON=0x00;
}
//通信開始(IO=1が受信，0が送信)
boolian i2c2_start(uint8 slave_address,boolian IO){
	//以前の通信処理待ち
	i2c2_wait_process();

	//Busyフラグを上げる
	I2C2_BusyFlag=1;
	//通信開始
	i2c2_interrupt_setup_start();
	
	//以前の通信処理待ち(＝start処理待ち)
	i2c2_wait_process();
	
	//Busyフラグを上げる
	I2C2_BusyFlag=1;
	//slave addressとIO(write or read)を送信
	i2c2_interrupt_do_start(slave_address,IO);

	//以前の通信処理待ち
	i2c2_wait_process();
	return i2c2_check_NACK();
}
//通信再開(IO=1が受信，0が送信)(終了処理をせずにIOを変更するときに使う)
boolian i2c2_restart(uint8 slave_address,boolian IO){
	//以前の通信処理待ち
	i2c2_wait_process();

	//Busyフラグを上げる
	I2C2_BusyFlag=1;
	//通信再開
	i2c2_interrupt_setup_restart();

	//以前の通信処理待ち（＝restart処理待ち）
	i2c2_wait_process();

	//Busyフラグを上げる
	I2C2_BusyFlag=1;
	//slave addressとIO(write or read)を送信
	i2c2_interrupt_do_restart(slave_address,IO);

	//以前の通信処理待ち
	i2c2_wait_process();
	return i2c2_check_NACK();
}
//通信終了
void i2c2_stop(void){
	//以前の通信処理待ち
	i2c2_wait_process();

	//Busyフラグを上げる
	I2C2_BusyFlag=1;
	//通信終了処理開始
	i2c2_interrupt_stop();
}
//送信関数
boolian i2c2_write(uint8 c){
	//以前の通信処理待ち
	i2c2_wait_process();

	//Busyフラグを上げる
	I2C2_BusyFlag=1;
	//バッファに書き込み
	i2c2_interrupt_write(c);

	//以前の通信処理待ち(slave側からの応答待ち)
	i2c2_wait_process();

	//ACK or NACK を返す
	return i2c2_check_NACK(); 
}
//受信関数
uint8 i2c2_read(boolian EndFlag){
	//以前の通信処理待ち
	i2c2_wait_process();

	//Busyフラグを上げる
	I2C2_BusyFlag=1;
	//受信enable
	i2c2_interrupt_settup_read();

	//以前の通信処理待ち（＝受信待ち）
	i2c2_wait_process();

	//Busyフラグを上げる
	I2C2_BusyFlag=1;
	//データ受信およびACK/NACK送信
	return i2c2_interrupt_do_read(EndFlag);
}
//ACK送信関数
void _i2c2_send_ACK(void){
	//ACK送信モードに設定
	I2C2CONbits.ACKDT=0;
	//ACK送信
	I2C2CONbits.ACKEN=1;
}
//NACK送信関数
void _i2c2_send_NACK(void){
	//NACK送信モードに設定
	I2C2CONbits.ACKDT=1;
	//NACK送信
	I2C2CONbits.ACKEN=1;
}
//NACK受信チェック関数(0:ACK,1:NACK)
boolian i2c2_check_NACK(void){
	//以前の通信処理待ち
	i2c2_wait_process();	
	return I2C2STATbits.ACKSTAT;
}
//受信確認関数
boolian i2c2_data_is_in(void){return I2C2STATbits.RBF;}
//I2Cの処理待ち関数（処理の競合を防ぐ）
void i2c2_wait_process(void){
	if(I2C2_BusyFlag){
		//割り込みフラグが上がるまで待つ（処理終了時に自動的に割り込み生成される）
		while(!IFS3bits.MI2C2IF);
		//busyフラグクリア
		I2C2_BusyFlag=0;
		//割り込みフラグクリア
		IFS3bits.MI2C2IF=0;
	}
}
//割り込み用通信開始準備関数
void i2c2_interrupt_setup_start(void){I2C2CONbits.SEN=1;}
//割り込み用通信開始関数
void i2c2_interrupt_do_start(uint8 slave_address,boolian IO){I2C2TRN=((0b00000001&IO)|(slave_address<<1));};
//割り込み用通信再開準備関数
void i2c2_interrupt_setup_restart(void){I2C2CONbits.RSEN=1;}
//割り込み用通信終了関数
void i2c2_interrupt_stop(void){I2C2CONbits.PEN=1;}
//割り込み用送信関数
void i2c2_interrupt_write(uint8 c){I2C2TRN=c;}
//割り込み用受信準備関数
void i2c2_interrupt_settup_read(void){I2C2CONbits.RCEN=1;}
//割り込み用受信関数
uint8 i2c2_interrupt_do_read(boolian EndFlag){
	//slaveへACK/NACK送信
	if(EndFlag)_i2c2_send_NACK();	//NACK送信（受信処理終了）
	else _i2c2_send_ACK();			//ACK送信

	//受信レジスタを返す
	return I2C2RCV;
}


//I2C1のマスター割り込みフラグ：IFS1bits.MI2C1IF
//I2C1のマスター割り込みenableビット：IEC1bits.MI2C1IE
//I2C2のマスター割り込みフラグ：IFS3bits.MI2C2IF
//I2C2のマスター割り込みenableビット：IEC3bits.MI2C2IE

void __attribute__((interrupt, no_auto_psv)) _MI2C2Interrupt(void){
        IFS3bits.MI2C2IF = 0;		//Clear the DMA0 Interrupt Flag;

}

#
#endif
