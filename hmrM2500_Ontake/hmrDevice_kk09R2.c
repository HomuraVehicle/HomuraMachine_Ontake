#ifndef HMR_DEVICE_KK09R2_C_INC
#define HMR_DEVICE_KK09R2_C_INC 102
#
#define HMLIB_NOLIB
#ifndef HMR_DEVICE_KK09R2_INC
//__TRACE(id);
#	include "hmrDevice_kk09R2.h"
#endif

//=================================interrupt==============================
#define HMR_INTERRUPT_LEVEL_UART1_SEND 3
#define HMR_INTERRPUT_LEVEL_UART1_RECV 4
#define HMR_INTERRUPT_LEVEL_UART2_SEND 6	//6
#define HMR_INTERRPUT_LEVEL_UART2_RECV 7	//7
#define HMR_INTERRUPT_LEVEL_TIMER1  2		//2
#define HMR_INTERRUPT_LEVEL_TIMER5  2		//2
#define HMR_INTERRUPT_LEVEL_MAIN 1
#define HMR_INTERRUPT_LEVEL_LOCK 5
#define HMR_INTERRUPT_LEVEL_STRONG_LOCK 7
//割り込み初期化関数
void hmr_interrupt_initalize(void){
	//割り込み　ネスティング(多重割り込み)許可(これがないとデータ落ちするみたい)
	INTCON1bits.NSTDIS = 0; //debug 	//0が多重割り込み許可
	// 
	// 優先度つき割り込みの考え方
	// 　１、すべての処理を行うのはCPUであり、それに優先度という概念をつけた
	//   ２、各モジュールの割り込み処理(UART、TIMERなど)も優先度を持っている
	//   ３、もし割り込み要求が起こった時は、CPUの優先度と割り込み処理の優先度を比べて、優先度の高い処理を行う。
	//			この際に、優先度の高い割り込み処理を行う場合はCPUの優先度も同様に上がる
	//   ４、これによって、多重割り込みも、割り込みの優先度次第で可能になった
	//
	//割り込みネスティング禁止処理　NSTDIS = 1
	//　　こいつの仕組みとしては、割り込みが入ると、必ずCPUの処理優先度をMAX 7 にしてくる
	//　　よって多重割り込みは起きない！！
	//　　　しかし、同時に、CPUの処理優先度を一切変更できなくなってしまう SRbits.IPL は書き込み不可になる
	//　　つまり、メイン側で割り込みをロックしたいときは工夫が必要となる
	//

	//割り込み順位設定 優先順に数字が大きい 
	//ここは触るな！！　stop_MainStreamInterrupt,restart/// が依存している！！
	IPC7bits.U2RXIP = HMR_INTERRPUT_LEVEL_UART2_RECV;// UART2(CAMERA) Rx
	__asm__("NOP");
	IPC7bits.U2TXIP = HMR_INTERRUPT_LEVEL_UART2_SEND;// UART2(CAMERA) Tx
	IPC2bits.U1RXIP = HMR_INTERRPUT_LEVEL_UART1_RECV;// UART1(RF module) Rx  
	IPC3bits.U1TXIP = HMR_INTERRUPT_LEVEL_UART1_SEND;// UART1(RF module) Tx  
	IPC0bits.T1IP = HMR_INTERRUPT_LEVEL_TIMER1;	// Timer1   
	IPC7bits.T5IP = HMR_INTERRUPT_LEVEL_TIMER5;	// Timer5   
	SRbits.IPL = HMR_INTERRUPT_LEVEL_MAIN;		// Main Loop
}

//割り込みロック・アンロック関数
//static unsigned char x_hmr_lock_prv=HMR_INTERRUPT_LEVEL_LOCK;
mutex hmr_interrupt_lock(void){
	mutex x_hmr_lock_prv;
	x_hmr_lock_prv = SRbits.IPL; 
	SRbits.IPL = HMR_INTERRUPT_LEVEL_LOCK;

	return x_hmr_lock_prv;
}
void hmr_interrupt_unlock(mutex x_hmr_lock_prv){
	SRbits.IPL = x_hmr_lock_prv;
}

// 全ての割り込みを禁止
void hmr_interrupt_disable(void){INTCON2bits.GIE = 0;}
// 全ての割り込みを許可
void hmr_interrupt_enable(void){INTCON2bits.GIE = 1;}

//================================CONFIG==================================
_FOSCSEL( FNOSC_FRCPLL);//Internal FRC(7.37MHz) + PLL//
_FOSC( FCKSM_CSDCMD & OSCIOFNC_ON & POSCMD_NONE ); //HS//
_FWDT( FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS1024 );//WDT 2s
_FPOR( FPWRT_PWR64 & ALTI2C1_ON & ALTI2C2_ON);//power on timer 64ms

//===========================デバイス=========================
#define HMR_SYSTEM_CLOCK 73700000

void HMR_core_initialize(void){
//=============CLOCK設定======================
	
	// Pattern 1
	// Fin = 7.37 /2 * 40 / 2 = 73.7 MHz
	CLKDIVbits.PLLPRE = 0; //N1 =  1/2
	__asm__("NOP");
	CLKDIVbits.PLLPOST = 0;// N2 = 1/2
	PLLFBD = 38; // M = 40
	

	/*
	// Pattern 2
	// Fin = 7.37 /2 * 40 /4 = 36.35 MHz
	CLKDIVbits.PLLPRE = 0; //N1 =  1/2
	__asm__("NOP");
	CLKDIVbits.PLLPOST = 1;// N2 = 1 / 2(1 + 1)
	PLLFBD = 38; // M = 40
	*/


//==============TRIS設定====================
//1がin
	TRISA=0x04C0;
	TRISB=0xCE1F;
	TRISC=0x6004;
	TRISD=0x0001;
	TRISE=0x0392;
	TRISF=0x0007;
	TRISG=0x1083;
	
//=============ADC設定=======================
//1がADCを有効　注意：DataSheetに記載されていないピンも設定できることがあるのでヘッダファイルでどのピンが設定可能か確認した方がいい
	ANSELA=0x00C0;	//A portはA6,7,9,10がAD設定可
	ANSELB=0x403E;	//B portはB0から15まですべてAD設定可
	ANSELC=0x0004;	//C portはC1,2,3,4,13,14がAD設定可
	ANSELD=0x0000;	//D portはD6,7がAD設定可
	ANSELE=0x0390;	//E portはE0から9までAD設定可
	ANSELG=0x0000;	//G portはG6,7,8,9がAD設定可
	//ポートFにADピンはない

	//クロックの設定
	//ini_clock(73700000);
	set_clock(HMR_SYSTEM_CLOCK);

	//ポストスケーラー設定
	changeClock(MAIN_CLK_Div_4);
}

//===============================通信設定系=============================
//#define HMR_UART1_RF_BAUDRATE 9600
//#define HMR_UART1_RF_BAUDRATE_DIP4ON 38400
//#define HMR_UART1_RF_BAUDRATE_DIP4OFF 9600
//#define HMR_UART1_MP_BAUDRATE 38400
//#define HMR_UART2_BAUDRATE 38400
void HMR_uart1_initialize_withRF0_with_flowCtr(uint16 BaudRate){
//____________UART1(for RF modeule)____________
	_IOLOCK=0;		//pheriperal pin設定のためレジスタのロックを解除

	_RP125R=0;			//MP側のTx設定を0に戻しておく
	_RP126R=0;			//MP側のRTS設定を0に戻しておく
	
	_U1RXR=41;			//RPI41をUART1受信ピンに設定 U1RXRが受信ピン設定レジスタ 44がRPI44の番号(要データシート参照)
	_U1CTSR=42;			//RPI42をUART1のCTSに設定
	_RP79R=1;			//RP100をUART1送信ピンに設定 送信ピン設定に1を入れる
	_RP100R=2;			//RP79をUART1のRTSピンに設定
	
	
	/*
	CTS,RTSの有効化のメモ
	UxMODEbits.UEN = 0:UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLK pins controlled by PORT latches
					 1:UxTX, UxRX and UxRTS pins are enabled and used; UxCTS pin controlled by PORT latches
					 2:UxTX, UxRX, UxCTS and UxRTS pins are enabled and used
					 3:UxTX, UxRX and BCLK pins are enabled and used; UxCTS pin controlled by PORT latches
	*/
//	U1MODEbits.UEN=0x00;	//U1CTS,RTSを無効にする
	U1MODEbits.UEN = 0x02;	//U1CTS,RTSを有効にする
	
	_IOLOCK=1;		//pheriperal pin設定のためレジスタをロック
	uart1_initialize(BaudRate);
}
void HMR_uart1_initialize_withRF0_without_flowCtr(uint16 BaudRate){
	//____________UART1(for RF modeule)____________
	_IOLOCK = 0;		//pheriperal pin設定のためレジスタのロックを解除

	_RP125R = 0;			//MP側のTx設定を0に戻しておく
	_RP126R = 0;			//MP側のRTS設定を0に戻しておく

	_U1RXR = 41;			//RPI41をUART1受信ピンに設定 U1RXRが受信ピン設定レジスタ 44がRPI44の番号(要データシート参照)
	_U1CTSR = 42;			//RPI42をUART1のCTSに設定
	_RP79R = 1;				//RP100をUART1送信ピンに設定 送信ピン設定に1を入れる
	_RP100R = 2;			//RP79をUART1のRTSピンに設定


	/*
	CTS,RTSの有効化のメモ
	UxMODEbits.UEN = 0:UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLK pins controlled by PORT latches
	1:UxTX, UxRX and UxRTS pins are enabled and used; UxCTS pin controlled by PORT latches
	2:UxTX, UxRX, UxCTS and UxRTS pins are enabled and used
	3:UxTX, UxRX and BCLK pins are enabled and used; UxCTS pin controlled by PORT latches
	*/
	U1MODEbits.UEN=0x00;	//U1CTS,RTSを無効にする

	_IOLOCK = 1;		//pheriperal pin設定のためレジスタをロック
	uart1_initialize(BaudRate);
}
void HMR_uart1_initialize_withRF1_with_flowCtr(uint16 BaudRate){
//____________UART1(for MobilePhone modeule)____________
	_IOLOCK=0;			//pheriperal pin設定のためレジスタのロックを解除
	
	_RP79R=0;			//RF側のTx設定を0に戻しておく
	_RP100R=0;			//RF側のRTS設定を0に戻しておく

	_U1RXR=124;			//RP124をUART1受信ピンに設定 U1RXRが受信ピン設定レジスタ 0x17がRPI23の番号
	_U1CTSR=81;		//RPI81をCTSに設定
						// 注意：携帯モジュールMPMにもCTS用の線をつないであげる必要があるが、この線は8ピンにつなぐ！！
						// 　ふつうは7ピンでよいのだが、MPMでは8ピンにつなぐということらしい・・・・
	_RP125R=1;			//RP125をUART1送信ピンに設定
	_RP126R=2;			//RP126をUART1のRTSピンに設定
	
	
//	U1MODEbits.UEN=0x00;	//U1CTS,RTSを無効にする
	U1MODEbits.UEN = 0x02;	//U1CTS,RTSを有効にする
	
	_IOLOCK=1;			//pheriperal pin設定のためレジスタをロック	
	uart1_initialize(BaudRate);
}
void HMR_uart1_initialize_withRF1_without_flowCtr(uint16 BaudRate){
//____________UART1(for MobilePhone modeule)____________
	_IOLOCK=0;			//pheriperal pin設定のためレジスタのロックを解除
	
	_RP79R=0;			//RF側のTx設定を0に戻しておく
	_RP100R=0;			//RF側のRTS設定を0に戻しておく

	_U1RXR=124;			//RP124をUART1受信ピンに設定 U1RXRが受信ピン設定レジスタ 0x17がRPI23の番号
	_U1CTSR=81;		//RPI81をCTSに設定
						// 注意：携帯モジュールMPMにもCTS用の線をつないであげる必要があるが、この線は8ピンにつなぐ！！
						// 　ふつうは7ピンでよいのだが、MPMでは8ピンにつなぐということらしい・・・・
	_RP125R=1;			//RP125をUART1送信ピンに設定
	_RP126R=2;			//RP126をUART1のRTSピンに設定
	
	
	U1MODEbits.UEN=0x00;	//U1CTS,RTSを無効にする
//	U1MODEbits.UEN = 0x02;	//U1CTS,RTSを有効にする
	
	_IOLOCK=1;			//pheriperal pin設定のためレジスタをロック	
	uart1_initialize(BaudRate);
}
void HMR_uart1_finalize(void){
	uart1_finalize();
}

void HMR_uart2_initialize(uint16 BaudRate){
//_____________UART2(for CAMERA)_______________
	OSCCONbits.IOLOCK=0;		//pheriperal pin設定のためレジスタのロックを解除
	RPINR19bits.U2RXR=98;		//RP98をUART2受信ピンに設定
	_RP101R=3;					//RP101をUART2送信ピンに設定


	OSCCONbits.IOLOCK=1;		//pheriperal pin設定のためレジスタをロック	
	uart2_initialize(BaudRate);
}
//debug用　uart1のRF1と重複するので，通常は使ってはいけない
/*
void HMR_uart3_initialize(uint16 BaudRate){
	//_____________UART2(for CAMERA)_______________
	OSCCONbits.IOLOCK = 0;		//pheriperal pin設定のためレジスタのロックを解除

	//debug begin0
	RPINR27bits.U3RXR = 124;			//RP124をUART2受信ピンに設定 U1RXRが受信ピン設定レジスタ 0x17がRPI23の番号
	_RP125R = 0x1B;			//RP125をUART2送信ピンに設定
	U3MODEbits.UEN = 0x00;
	//debug end

	OSCCONbits.IOLOCK = 1;		//pheriperal pin設定のためレジスタをロック	
	uart3_initialize(BaudRate); 
}
*/
void HMR_uart2_finalize(void){
	uart2_finalize();
}

void HMR_spi1_initialize(void){
	spi1_initialize(SPI_MASTER | SPI_L_TO_H | SPI_CLK_DIV_64|SPI_CLK_DIV2_5);	//MASTER, L to H(共通), クロック100Kbps位
}
void HMR_spi2_initialize(void) {
	spi2_initialize(SPI_MASTER | SPI_H_TO_L | SPI_CLK_DIV_64|SPI_CLK_DIV2_5);
}
void HMR_spi1_finalize(void) {}
void HMR_spi2_finalize(void) {}

#define HMR_I2C1_CLOCK I2C_CLOCK_1MHz
#define HMR_I2C2_CLOCK I2C_CLOCK_1MHz
void HMR_i2c_initialize(void){
	i2c1_initialize(HMR_SYSTEM_CLOCK,HMR_I2C1_CLOCK,0);
	//i2c2_initialize(HMR_SYSTEM_CLOCK,HMR_I2C2_CLOCK,0);
}
void HMR_i2c_finalize(void){
	i2c1_finalize();
	//i2c2_finalize();
}

//==============================AD変換==================================
void HMR_adc_initialize(void){
	setup_adc1_12bit(1);
	set_adc_sampleMode(EVERY);
	setup_adc1_time_auto();
	setup_adc1_Vref(Vref_GND);
}
void HMR_adc_finalize(void){}
//==============================Camera===================================
void HMR_PIN_CAM_PW_set(unsigned char val){
	if(val==0){
		HMR_PIN_CAM_SELECT0_TRIS_set(1);
		HMR_PIN_CAM_SELECT1_TRIS_set(1);
	}else{
		HMR_PIN_CAM_SELECT0_TRIS_set(0);
		HMR_PIN_CAM_SELECT1_TRIS_set(0);
	}
}
#
#endif
