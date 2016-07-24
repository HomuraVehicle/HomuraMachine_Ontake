#ifndef HMR_DEVICE_KK06_C_INC
#define HMR_DEVICE_KK06_C_INC 108
#
#ifndef HMR_DEVICE_KK06_INC
#	include "hmrDevice_kk06.h"
#endif

//================================CONFIG==================================
_FOSCSEL( FNOSC_FRCPLL);//Internal FRC(7.37MHz) + PLL//
_FOSC( FCKSM_CSDCMD & OSCIOFNC_ON & POSCMD_NONE ); //HS//
_FWDT( FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS1024 );//WDT 2s
_FPOR( FPWRT_PWR64 & ALTI2C2_ON);//power on timer 64ms

//=================================interrupt==============================
#define HMR_INTERRUPT_LEVEL_UART1_SEND 3
#define HMR_INTERRPUT_LEVEL_UART1_RECV 4
#define HMR_INTERRUPT_LEVEL_UART2_SEND 6
#define HMR_INTERRPUT_LEVEL_UART2_RECV 7
#define HMR_INTERRUPT_LEVEL_TIMER1 2
#define HMR_INTERRUPT_LEVEL_TIMER5 2
#define HMR_INTERRUPT_LEVEL_MAIN 1
#define HMR_INTERRUPT_LEVEL_LOCK 5
//割り込み初期化関数
void hmr_interrupt_initalize(void){
	//割り込み　ネスティング(多重割り込み)許可(これがないとデータ落ちするみたい)
	INTCON1bits.NSTDIS = 0;
	//割り込み順位設定 優先順に数字が大きい 
	//ここは触るな！！　stop_MainStreamInterrupt,restart/// が依存している！！
	IPC7bits.U2RXIP = HMR_INTERRPUT_LEVEL_UART2_RECV;// UART2(CAMERA) Rx
	IPC7bits.U2TXIP = HMR_INTERRUPT_LEVEL_UART2_SEND;// UART2(CAMERA) Tx
	IPC2bits.U1RXIP = HMR_INTERRPUT_LEVEL_UART1_RECV;// UART1(RF module) Rx  
	IPC3bits.U1TXIP = HMR_INTERRUPT_LEVEL_UART1_SEND;// UART1(RF module) Tx  
	IPC0bits.T1IP = HMR_INTERRUPT_LEVEL_TIMER1;	// Timer1   
	IPC7bits.T5IP = HMR_INTERRUPT_LEVEL_TIMER5;	// Timer5   
	SRbits.IPL = HMR_INTERRUPT_LEVEL_MAIN;		// Main Loop
}
//割り込み終端化関数
void hmr_interrupt_finalize(void){}
//mainループへの割り込み一時停止
void hmr_main_lock_interrupt(void){SRbits.IPL = HMR_INTERRUPT_LEVEL_LOCK;}
//mainループへの割り込み復帰
void hmr_main_unlock_interrupt(void){SRbits.IPL = HMR_INTERRUPT_LEVEL_MAIN;}
//uart1_recvへの割り込み一時停止
void hmr_uart1_fget_lock_interrupt(void){IPC2bits.U1RXIP= HMR_INTERRUPT_LEVEL_LOCK;}
//uart1_recvへの割り込み復帰
void hmr_uart1_fget_unlock_interrupt(void){IPC2bits.U1RXIP = HMR_INTERRPUT_LEVEL_UART1_RECV;}
//uart1_sendへの割り込み一時停止
void hmr_uart1_fput_lock_interrupt(void){IPC3bits.U1TXIP = HMR_INTERRUPT_LEVEL_LOCK;}
//uart1_sendへの割り込み復帰
void hmr_uart1_fput_unlock_interrupt(void){IPC3bits.U1TXIP = HMR_INTERRUPT_LEVEL_UART1_SEND;}
//timer1への割り込み一時停止
void hmr_timer1_lock_interrupt(void){IPC0bits.T1IP = HMR_INTERRUPT_LEVEL_LOCK;}
//timer1への割り込み復帰
void hmr_timer1_unlock_interrupt(void){IPC0bits.T1IP = HMR_INTERRUPT_LEVEL_TIMER1;}
//timer5への割り込み一時停止
void hmr_timer4_5_lock_interrupt(void){IPC7bits.T5IP = HMR_INTERRUPT_LEVEL_LOCK;}
//timer5への割り込み復帰
void hmr_timer4_5_unlock_interrupt(void){IPC7bits.T5IP = HMR_INTERRUPT_LEVEL_TIMER5;}



//===========================デバイス=========================
void hmr_device_initialize(void){
//=============CLOCK設定======================
	// Fin = 7.37 /2 * 40 / 2 = 73.7 MHz
	CLKDIVbits.PLLPRE = 0; //N1 =  1/2
	__asm__("NOP");
	CLKDIVbits.PLLPOST = 0;// N2 = 1/2
	PLLFBD = 38; // M = 40

//==============TRIS設定====================
	TRISA=0x04C0;
	TRISB=0x8F43;	//0x8F7F adc2,3,4,5を切った
	TRISC=0x000E;
	TRISD=0x4020;
	TRISE=0x01FF;	//0x3FF adc1を切った
	TRISF=0x0017;
	TRISG=0x008F;
	
//=============ADC設定=======================
//1がADCを有効
	ANSELA=0x0040;
	ANSELB=0x0743;	//0x77F ch2,3,4,5を切った
	ANSELC=0x000E;
	ANSELD=0x0000;
	ANSELE=0x01FF;	//0x03FF ch1を切った
	ANSELG=0x0000;
	//ポートFにADピンはない
}
void hmr_device_finalize(void){}

//===============================クロック===============================
void hmr_clock_initialize_withNormal(void){
	//ポストスケーラー設定
	changeClock(MAIN_CLK_Div_1);
	//クロックの設定
	ini_clock(73700000);
}
void hmr_clock_initialize_withSlow(void){
	//ポストスケーラー設定
	changeClock(MAIN_CLK_Div_16);
	//クロックの設定 (Postスケーラーで設定しても影響しない)
	ini_clock(73700000);
}
void hmr_clock_finalize(void){}

//===============================通信設定系=============================
#define HMR_UART1_RF_BAUDRATE 9600
#define HMR_UART1_MP_BAUDRATE 38400
#define HMR_UART2_BAUDRATE 38400
#define HMR_I2C2_CLOCK I2C_CLOCK_1MHz
void hmr_uart1_initialize_withRF(void){
//____________UART1(for RF modeule)____________
	OSCCONbits.IOLOCK=0;		//pheriperal pin設定のためレジスタのロックを解除
	RPINR18bits.U1RXR=0x62;		//RP98をUART1受信ピンに設定 U1RXRが受信ピン設定レジスタ 0x62がRP98の番号
	RPOR8bits.RP99R=1;			//RP99をUART1送信ピンに設定 送信ピン設定に1を入れる
	RPOR14bits.RP125R=0;		//MP側の送信ピン設定を下しておく
	OSCCONbits.IOLOCK=1;		//pheriperal pin設定のためレジスタをロック
	ini_UART1(HMR_UART1_RF_BAUDRATE);
}
void hmr_uart1_initialize_withMP(void){
//____________UART1(for MobilePhone modeule)____________
	OSCCONbits.IOLOCK=0;		//pheriperal pin設定のためレジスタのロックを解除
	RPINR18bits.U1RXR=0x17;		//RPI23をUART1受信ピンに設定 U1RXRが受信ピン設定レジスタ 0x17がRPI23の番号
	RPOR14bits.RP125R=1;		//RP125をUART1送信ピンに設定
	RPOR8bits.RP99R=0;			//RF側の送信ピン設定を0に戻しておく
	OSCCONbits.IOLOCK=1;		//pheriperal pin設定のためレジスタをロック	
	ini_UART1(HMR_UART1_MP_BAUDRATE);
}
void hmr_uart1_finalize(void){}
void hmr_uart2_initialize(void){
//_____________UART2(for CAMERA)_______________
	OSCCONbits.IOLOCK=0;		//pheriperal pin設定のためレジスタのロックを解除
	RPINR19bits.U2RXR=0x64;		//RP100をUART2受信ピンに設定
	RPOR9bits.RP101R=3;			//RP101をUART2受信ピンに設定
	OSCCONbits.IOLOCK=1;		//pheriperal pin設定のためレジスタをロック	
	ini_UART2(HMR_UART2_BAUDRATE);
}
void hmr_uart2_finalize(void){}
void hmr_spi2_initialize(void){
	setup_spi2(SPI_MASTER | SPI_L_TO_H | SPI_CLK_DIV_64|SPI_CLK_DIV2_5);//MASTER, L to H(共通), クロック100Kbps位
}
void hmr_spi2_finalize(void){}
void hmr_i2c_initialize(void){i2c2_initialize(73700000,HMR_I2C2_CLOCK,0);}
void hmr_i2c_finalize(void){i2c2_finalize();}
//==============================AD変換==================================
void hmr_adc_initialize(void){
	setup_adc1_12bit(1);
	set_adc_sampleMode(EVERY);
	setup_adc1_time_auto();
	setup_adc1_Vref(Vref_GND);
}
void hmr_adc_finalize(void){}
#
#endif
