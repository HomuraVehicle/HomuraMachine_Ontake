#ifndef AMPIC30EXTEND_INC
#define AMPIC30EXTEND_INC 101
#
/*
===amPic30Extend===
amPic30の拡張ライブラリ
	amPic30必要と思われる関数群
	フラグ管理関数群を追加

amPic30Extend_v1_01/121201 iwahori
	I2C関数群を追加・修正
amPic30Extend_v1_00/121027 hmIto
	
*/
#ifndef AMPIC30_INC
#	include"amPic30_v7.h"
#endif
//stream1の受信割込み
#define uart1_fget_interrupt(void) __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
void uart1_enable_fget_interrupt(void);
void uart1_disable_fget_interrupt(void);
boolian uart1_is_enable_fget_interrupt(void);
void uart1_set_fget_interrupt_flag(void);
void uart1_clear_fget_interrupt_flag(void);
//stream1の送信割込み
#define uart1_fput_interrupt(void) __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void)
void uart1_enable_fput_interrupt(void);
void uart1_disable_fput_interrupt(void);
boolian uart1_is_enable_fput_interrupt(void);
void uart1_set_fput_interrupt_flag(void);
void uart1_clear_fput_interrupt_flag(void);
//stream2の受信割込み
#define uart2_fget_interrupt(void) __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void)
void uart2_enable_fget_interrupt(void);
void uart2_disable_fget_interrupt(void);
boolian uart2_is_enable_fget_interrupt(void);
void uart2_set_fget_interrupt_flag(void);
void uart2_clear_fget_interrupt_flag(void);
//stream2の送信割込み
#define uart2_fput_interrupt(void) __attribute__((interrupt, no_auto_psv)) _U2TXInterrupt(void)
void uart2_enable_fput_interrupt(void);
void uart2_disable_fput_interrupt(void);
boolian uart2_is_enable_fput_interrupt(void);
void uart2_set_fput_interrupt_flag(void);
void uart2_clear_fput_interrupt_flag(void);
//timer1の割り込み
#define timer1_interrupt(void) __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
//timer1用関数
void timer1_initialize(uint16 time);
//timer1の割り込みフラグをクリア
void timer1_clear_interrupt_flag(void);
//timer1の割り込みフラグをセット
void timer1_set_interrupt_flag(void);
//timer1のカウントをリセット
void timer1_reset(void);
//timer1が割り込み有効かどうかの確認
boolian timer1_is_enable_interrupt(void);
//timer1の割り込み有効
void timer1_enable_interrupt(void);
//timer1の割り込み無効
void timer1_disable_interrupt(void);
//timer5の割込み
#define timer4_5_interrupt(void) __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void)
//timer4&5連動タイマーの初期化
void timer4_5_initialize(uint16 time);
//timer4&5連動タイマーの終端化
void timer4_5_finalize();
//timer4&5の割り込みフラグをクリア
void timer4_5_clear_interrupt_flag(void);
//timer4&5の割り込みフラグをセット
void timer4_5_set_interrupt_flag(void);
//timer4&5のカウントをリセット
void timer4_5_reset(void);
//timer4&5が割り込み有効かどうかの確認
boolian timer4_5_is_enable_interrupt(void);
//timer4&5の割り込み有効
void timer4_5_enable_interrupt(void);
//timer4&5の割り込み無効
void timer4_5_disable_interrupt(void);
//RTCC用割り込み関数
#define rtcc_interrupt(void) void __attribute__((interrupt)) _RTCCInterrupt(void)
//RTCC用セカンドオシレータ初期化プロセス
void rtcc_oscilator_initialize(void);
//RTCC初期化関数 引数が設定時刻 
//	rtcc_initialize("121027","0","210120"); == 2012年10月27日(日)21:01:20
void rtcc_initialize(unsigned char YMD[6],unsigned char WeekNo,unsigned char HMS[6]);
//RTCCの割り込みフラグをクリア
void rtcc_clear_interrupt_flag(void);
//RTCCの割り込みフラグをセット
void rtcc_set_interrupt_flag(void);
//RTCCのカウントをリセット
void rtcc_reset_interrupt(void);
//RTCCが割り込み有効かどうかの確認
boolian rtcc_is_enable_interrupt(void);
//RTCCの割り込み有効 引数が割り込み時間
//	rtcc_initialize("----","-","--3000"); == 毎時30分ちょうどに割り込み
void rtcc_enable_interrupt(unsigned char MD[4],unsigned char WeekNo,unsigned char HMS[6],boolian IterateNoLimitFlag_,uint8 IterateNum_);
//RTCCの割り込み無効(繰り返し回)
void rtcc_disable_interrupt(void);
//=================================I2C1======================================
#define I2C_CLOCK_100kHz 10000
#define I2C_CLOCK_400kHz 2500
#define I2C_CLOCK_1MHz 1000
//i2c1初期化
void i2c1_initialize(uint32 SystemClock_,uint16 I2C_ClockMode_,uint16 nsPGD_);
//I2C1の終端化処理
void i2c1_finalize(void);
//通信開始(IO=1が受信，0が送信)
boolian i2c1_start(uint8 slave_address,boolian IO);
//通信再開(IO=1が受信，0が送信)(終了処理をせずにIOを変更するときに使う)
boolian i2c1_restart(uint8 slave_address,boolian IO);
//通信終了
void i2c1_stop(void);
//送信関数
boolian i2c1_write(uint8 c);
//受信関数
uint8 i2c1_read(boolian EndFlag);
//ACK送信関数
void _i2c1_send_ACK(void);
//NACK送信関数
void _i2c1_send_NACK(void);
//NACK受信チェック関数(0:ACK,1:NACK)
boolian i2c1_check_NACK(void);
//受信確認関数
boolian i2c1_data_is_in(void);
//I2Cの処理待ち関数（処理の競合を防ぐ）
void i2c1_wait_process(void);

//割り込み用通信開始準備関数
void i2c1_interrupt_setup_start(void);
//割り込み用通信開始関数
void i2c1_interrupt_do_start(uint8 slave_address,boolian IO);
//割り込み用通信再開準備関数
void i2c1_interrupt_setup_restart(void);
//割り込み用通信再開関数
#define i2c1_interrupt_do_restart i2c1_interrupt_do_start
//割り込み用送信関数
void i2c1_interrupt_write(uint8 c);
//割り込み用受信準備関数
void i2c1_interrupt_settup_read(void);
//割り込み用受信関数
uint8 i2c1_interrupt_do_read(boolian EndFlag);
//割り込み用通信終了関数
void i2c1_interrupt_stop(void);

//================================I2C2=======================================
//i2c2初期化
void i2c2_initialize(uint32 SystemClock_,uint16 I2C_ClockMode_,uint16 nsPGD_);
//I2C2の終端化処理
void i2c2_finalize(void);
//通信開始(IO=1が受信，0が送信)
boolian i2c2_start(uint8 slave_address,boolian IO);
//通信再開(IO=1が受信，0が送信)(終了処理をせずにIOを変更するときに使う)
boolian i2c2_restart(uint8 slave_address,boolian IO);
//通信終了
void i2c2_stop(void);
//送信関数
boolian i2c2_write(uint8 c);
//受信関数
uint8 i2c2_read(boolian EndFlag);
//ACK送信関数
void _i2c2_send_ACK(void);
//NACK送信関数
void _i2c2_send_NACK(void);
//NACK受信チェック関数(0:ACK,1:NACK)
boolian i2c2_check_NACK(void);
//受信確認関数
boolian i2c2_data_is_in(void);
//I2Cの処理待ち関数（処理の競合を防ぐ）
void i2c2_wait_process(void);

//割り込み用通信開始準備関数
void i2c2_interrupt_setup_start(void);
//割り込み用通信開始関数
void i2c2_interrupt_do_start(uint8 slave_address,boolian IO);
//割り込み用通信再開準備関数
void i2c2_interrupt_setup_restart(void);
//割り込み用通信再開関数
#define i2c2_interrupt_do_restart i2c2_interrupt_do_start
//割り込み用送信関数
void i2c2_interrupt_write(uint8 c);
//割り込み用受信準備関数
void i2c2_interrupt_settup_read(void);
//割り込み用受信関数
uint8 i2c2_interrupt_do_read(boolian EndFlag);
//割り込み用通信終了関数
void i2c2_interrupt_stop(void);

/*
//I2Cの使い方
	while(1){
		if(i2c2_start(slave_adress,0))continue;
		if(i2c2_write('A'))continue;
		if(i2c2_write('B'))continue;
		if(i2c2_write('C'))continue;
		if (i2c2_restart(slave_adress,1))continue;
		break;
	}
	Buf[1]=i2c2_read(0);
	Buf[2]=i2c2_read(0);
	Buf[3]=i2c2_read(0);
	Buf[4]=i2c2_read(1);
	i2c2_stop();

//I2C割り込み使い方
void __attribute__((interrupt, no_auto_psv)) _MI2C2Interrupt(void){
	static boolian ACKFlag=0;
	static uint8 status=0;
	static uint8 SendCnt=0;
	static uint8 RecvCnt=0;
	IFS3bits.MI2C2IF = 0;		//Clear the DMA0 Interrupt Flag;
	
	if(ACKFlag){
		if(i2c2_check_NACK())status=0;
	}
	ACKFlag=0;

	switch (status){
		case 1:
			i2c2_interrupt_setup_start();
			++status;
			ACKFlag=1;
			break;
		case 2:
			i2c2_interrupt_do_start(slave_address,0);
			++status;
			ACKFlag=1;
			break;
		case 3:
			i2c2_interrupt_write(Buf[SendCnt++]);
			if(SendCnt>X)++status;
			ACKFlag=1;
			break;
		case 4:
			i2c2_interrupt_setup_restart();
			++status;
			ACKFlag=1;
			break;
		case 5:
			i2c2_interrupt_do_restart(slave_address,1);
			++status;
			ACKFlag=1;
			break;
		case 6:
			if(RecvCnt%2==0)i2c2_interrupt_settup_read();
			else Buf[RecvCnt]=i2c2_interrupt_do_read;
			++RecvCnt;
			if(RecvCnt>Y)++status;
			break;
		case 7:
			i2c2_interrupt_stop();
			status=0;
			break;
		default:
			break;
	}
}
*/


#
#endif
