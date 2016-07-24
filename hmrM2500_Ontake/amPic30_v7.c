#ifndef AMPIC30_C_INC
#define AMPIC30_C_INC 701
#
#define HMLIB_NOLIB
//
//＊＊＊＊　dsPIC C30コンパイラ用ライブラリ　＊＊＊＊＊
//							Ver. 3-1
//*データ型(bit)
//	int 16
//  long 32
//  long long 64
//  double 32
//  long double 64
//
//
//
//**覚書　
//	・割り込みについて　
//		割り込みにはそれぞれ割り込みフラグが用意されている	
//		たとえばＵＳＡＲＴの場合はU1TXIF,U1RXIFがある。
//		この割り込みフラグをどのような条件でＯＮにするかは、
//		各機能のレジスタ(USARTレジスタ)で指定する必要がある
//	
//	・送信、受信関数の製作
//		getc,putc,kbhitの製作
//		OERR=0 は受信バッファオーバーフローエラーの取り消し
//
//	・delay関数について　
//　　	delay関数のためにtimer2,3を使ったので、timer2,3は使用できない仕様になっている！！
//
//  ・ＡＤ変換する際は、ポートの入出力を間違いなくすること。
//		Vrefについても入力にする必要がある
//
//
//
//
//
//
//
//:::::::::::::::::::ーー更新履歴ーー:::::::::::::::::::::::::
//
//  100724	UARTについてライブラリ化進める
//	100807	UARTについてライブラリ完成　
//			タイマー、時間関係のライブラリも完成
//	100808　ＡＤ変換についての関数群を作成
//			Vrefがうまくいっていない(回路の問題か？)
//			SPIについては動作チェック済み
//	100809  WDT機能もちゃんと動くことを確認
//	100911	delay_us()を実装
//　　　	set_tris(),output_high,low実装
//  131019  Timer4,5 ini_timer4 にバグを発見(ビットシフトの桁数を間違っていた)　－＞修正
//			Timer の精度を上げるために、プリスケーラを1:1に変更してやった			
//
//
//
//

//#include <p33FJ256GP710.h>
//#include <timer.h>
#include "amPic30_v7.h"

#include "hmrDebug.h"
#include "hmrDevice.h"
/*
//config(メインでやってクリ)　参考例(この関数群の動作確認済み設定例) 
//
//　WDTオフにしているのは、内部でソフトウェア的にON,OFFできるため
//  WDTは内部の別のクロックで決まるので、メインクロックの速さには依存しない
//  WDTのWINDOWモードは、WDT周期の3/4以降に、WDTクリアしないと、リセットがかかるという厄介者なのでOFF
//	内部クロックはPLLによって早くできる。 
//　
//
_FOSCSEL( FNOSC_FRC);		//Internal FRC(7.37MHz)
_FOSC( FCKSM_CSDCMD & OSCIOFNC_ON & POSCMD_NONE ); //HS
_FWDT( FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS512 );//WDT 2s
_FPOR( FPWRT_PWR64 );//power on timer 64ms 
*/

uint64 clock = 7370000;//クロック
uint8 _am_delay_ms_isUsed; // delay_ms を使っているフラグ(2重呼び出しによる無限ループ対策)
uint8 _am_delay_ms2_isUsed;

//必ず必要な初期化関数
void ini_clock(uint64 _clock){//クロックの設定(初期値7.37MH)＋タイマーの設定(必ずしてください)  
	clock = _clock;//クロックを保持

	//Timer2設定^^delay_ms()用^^
	T2CONbits.T32 = 0;			//16bit mode
	__asm__("NOP");
	T2CONbits.TCKPS = 1;		//プリスケーラ１・８
	__asm__("NOP");
	T2CONbits.TCS = 0;			//内部クロック使う
	__asm__("NOP");
	T2CONbits.TGATE = 0;		//ゲート時間累算無効
	PR2 = (uint16)((double)clock / (2.*8.*1000.) + 0.5);//これで1msの周期になる
	TMR2 = 0;					//T2カウントをクリア
	_am_delay_ms_isUsed = 0;	// delay_ms が使われているかどうか

	//Timer3設定^^delay_us()用^^
	T3CONbits.TCKPS = 0;		//プリスケーラ１・1
	__asm__("NOP");
	T3CONbits.TCS = 0;			//内部クロック使う
	__asm__("NOP");
	T3CONbits.TGATE = 0;		//ゲート時間累算無効
	PR3 = (uint16)((double)clock / (2.*100000.) + 0.5);//これで10usの周期になる
	TMR3 = 0;//T3カウントをクリア

	//Timer6設定^^delay_ms2()用^^
	T6CONbits.T32 = 0;			//16bit mode
	__asm__("NOP");
	T6CONbits.TCKPS = 1;		//プリスケーラ１・８
	__asm__("NOP");
	T6CONbits.TCS = 0;			//内部クロック使う
	__asm__("NOP");
	T6CONbits.TGATE = 0;		//ゲート時間累算無効
	PR6 = (uint16)((double)clock / (2.*8.*1000.) + 0.5);//これで1msの周期になる
	TMR6 = 0;					//T6カウントをクリア
	_am_delay_ms2_isUsed = 0;	// delay_ms2 が使われているかどうか
}

//シリアル通信系関数
void ini_UART1(uint32 baudrate){//UART1設定＋UART1使用許可
	U1MODEbits.UARTEN = 1;		//UART1有効化
	U1STAbits.UTXEN = 1;		//UART送信許可
	U1BRG = (int)((double)clock/(2.*(double)baudrate*16.) + 0.5 -1.) ;//ボーレート設定(この値が8bit超えないように注意！)
	U1STAbits.UTXISEL1 = 0;		//送信割り込みモードの選択：送信バッファに空きができたら割り込む
	__asm__("NOP");
	U1STAbits.UTXISEL0 = 0;		//送信割り込みモードの選択：送信バッファに空きができたら割り込む
	__asm__("NOP");
	U1STAbits.URXISEL = 0;		//UART受信割り込み設定(１つ受信したら割り込み)
}
void ini_UART2(uint32 baudrate){//UART2設定＋UART2使用許可
	U2MODEbits.UARTEN = 1;		//UART1有効化
	U2STAbits.UTXEN = 1;		//UART送信許可
	U2BRG = (int)((double)clock/(2.*(double)baudrate*16.) + 0.5 -1.) ;//ボーレート設定(この値が8bit超えないように注意！)
	U2STAbits.UTXISEL1 = 0;		//送信割り込みモードの選択：送信バッファに空きができたら割り込む
	__asm__("NOP");
	U2STAbits.UTXISEL0 = 0;		//送信割り込みモードの選択：送信バッファに空きができたら割り込む
	__asm__("NOP");
	U2STAbits.URXISEL = 0;		//UART受信割り込み設定(１つ受信したら割り込み)
}
//シリアル通信における割り込みの使い方
/*
割り込みを許可してやる(以下の関数軍)
送信割り込みは、送信バッファに空きができたら割り込んでくる。
受信割り込みは、何か受信したら割り込んでくる。
メイン関数の始まる前のところで、以下のように割り込み処理を書いてやれば良し
steam の書き間違いに注意
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void){//UART1による送信割り込み
	clear_interrupt_fputflag(1);		//割り込みフラグクリア
	fputc(data,1);						//データ送信
}
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void){//UART2による受信割り込み
	clear_interrupt_fgetflag(2); 		//割り込みフラグクリア
	data = fgetc(2);					//データ受信
}
*/
void clear_interrupt_fgetflag(uint8 stream){
	if(stream&1)IFS0bits.U1RXIF = 0;
	if(stream&2)IFS1bits.U2RXIF = 0;
}
void clear_interrupt_fputflag(uint8 stream){
	if(stream&1)IFS0bits.U1TXIF = 0;
	if(stream&2)IFS1bits.U2TXIF = 0;
}
void set_interrupt_fgetflag(uint8 stream){
	if(stream&1)IFS0bits.U1RXIF = 1;
	if(stream&2)IFS1bits.U2RXIF = 1;
}
void set_interrupt_fputflag(uint8 stream){
	if(stream&1)IFS0bits.U1TXIF = 1;
	if(stream&2)IFS1bits.U2TXIF = 1;
}
void enable_interrupt_fget(uint8 stream){
	//UART1受信割り込み許可
	if(stream&1){
		IFS0bits.U1RXIF = U1STAbits.URXDA;
		IEC0bits.U1RXIE = 1;
	}
	//UART2受信割り込み許可
	if(stream&2){
		IFS1bits.U2RXIF = U2STAbits.URXDA;
		IEC1bits.U2RXIE = 1;
	}
}
void enable_interrupt_fput(uint8 stream){//送信割り込み許可
	//UART1送信割り込み許可
	if(stream&1)IEC0bits.U1TXIE = 1;
	//UART2送信割り込み許可
	if(stream&2)IEC1bits.U2TXIE = 1;
}
void disable_interrupt_fget(uint8 stream){
	//UART1受信割り込み禁止
	if(stream&1)IEC0bits.U1RXIE = 0;
	//UART2受信割り込み禁止
	if(stream&2)IEC1bits.U2RXIE = 0;
}
void disable_interrupt_fput(uint8 stream){
	//UART1送信割り込み禁止
	if(stream&1)IEC0bits.U1TXIE = 0;
	//UART2送信割り込み禁止
	if(stream&2)IEC1bits.U2TXIE = 0;
}

//割り込みフラグを保持する関数　＝＝　危険。使うな！！
uint8 tmp_disable_interrupt_fget(uint8 stream){
	uint8 ans=0;
	if((stream&1) && IEC0bits.U1RXIE){
		disable_interrupt_fget(stream);
		ans|=1;
	}
	if((stream&2) && IEC1bits.U2RXIE){
		disable_interrupt_fget(stream);
		ans|=2;
	}
	return ans;
}
uint8 tmp_disable_interrupt_fput(uint8 stream){
	uint8 ans=0;
	if((stream&1) && IEC0bits.U1TXIE){
		disable_interrupt_fget(stream);
		ans|=4;
	}
	if((stream&2) && IEC1bits.U2TXIE){
		disable_interrupt_fget(stream);
		ans|=8;
	}
	return ans;
}
uint8 tmp_disable_interrupt_fgetput(uint8 stream){
	uint8 ans=0;
	if(stream&1){
		if(IEC0bits.U1TXIE && IEC0bits.U1RXIE){
			IEC0bits.U1RXIE = 0;
			__asm__("NOP");
			IEC0bits.U1TXIE = 0;
			ans|=1+4;
		}else if(IEC0bits.U1TXIE){
			IEC0bits.U1TXIE = 0;
			ans|=4;
		}else if(IEC0bits.U1RXIE){
			IEC0bits.U1RXIE = 0;
			ans|=1;
		}
	}
	if(stream&2){
		if(IEC1bits.U2TXIE && IEC1bits.U2RXIE){
			IEC1bits.U2RXIE = 0;
			__asm__("NOP");
			IEC1bits.U2TXIE = 0;
			ans|=2+8;
		}else if(IEC1bits.U2TXIE){
			IEC1bits.U2TXIE = 0;
			ans|=8;
		}else if(IEC1bits.U2RXIE){
			IEC1bits.U2RXIE = 0;
			ans|=2;
		}
	}
	return ans;
}
void replace_interrupt_fget(uint8 c,uint8 stream){
	if(stream&1) IEC0bits.U1RXIE=(c&1);
	if(stream&2) IEC1bits.U2RXIE=((c&2)==2);
}
void replace_interrupt_fput(uint8 c,uint8 stream){
	if(stream&1) IEC0bits.U1TXIE=((c&4)==4);
	if(stream&2) IEC1bits.U2TXIE=((c&8)==8);
}
void replace_interrupt_fgetput(uint8 c,uint8 stream){
	uint8 u;
	if(stream&1){
		u=IFS0bits.U1RXIF+2*IFS0bits.U1TXIF;
		IFS0bits.U1RXIF=0;
		IFS0bits.U1TXIF=0;
		IEC0bits.U1RXIE=(c&1);
		IEC0bits.U1TXIE=((c&4)==4);
		IFS0bits.U1RXIF=(u&1);
		IFS0bits.U1TXIF=((u&2)==2);
	}
	if(stream&2){
		u=IFS1bits.U2RXIF+2*IFS1bits.U2TXIF;
		IFS1bits.U2RXIF=0;
		IFS1bits.U2TXIF=0;
		IEC1bits.U2RXIE=((c&2)==2);
		IEC1bits.U2TXIE=((c&8)==8);
		IFS1bits.U2RXIF=(u&1);
		IFS1bits.U2TXIF=((u&2)==2);
	}
}

//rs関数
void rs_fputc(uint8 data, uint8 stream){//１文字送信処理
	if(stream == 1){
		while(U1STAbits.UTXBF == 1);
		U1TXREG = data;
	}else if(stream == 2){
		while(U2STAbits.UTXBF == 1);
		U2TXREG = data;
	}
}

uint8 rs_fgetc(uint8 stream){//１文字受信処理
	if(stream == 1){
		while(U1STAbits.URXDA == 0);
		if(U1STAbits.OERR==1)U1STAbits.OERR = 0;//もしバッファーがオーバーフローしていたら、クリアする
		return U1RXREG;
	}else if(stream == 2){
		while(U2STAbits.URXDA == 0);
		if(U2STAbits.OERR==1)U2STAbits.OERR = 0;//もしバッファーがオーバーフローしていたら、クリアする
		return U2RXREG;
	}else return 0;
}

uint8 rs_kbhit(uint8 stream){//受信バッファーに入っているか？１ならyes
	if(stream == 1){
		return (uint8)U1STAbits.URXDA; 
	}else if(stream == 2){
		return (uint8)U2STAbits.URXDA; 
	}else return 0;
}



//便利関数
void delay_ms(uint16 time){//time msだけ時間待ちしてやる	
	uint32 j;
	if(_am_delay_ms_isUsed != 0)return;
	_am_delay_ms_isUsed = 1;
	TMR2 = 0;
	T2CONbits.TON = 1;//timer2作動
	for(j=0;j<time;++j){//time ms待つ
		TMR2 = 0;
		IFS0bits.T2IF = 0;
		while(!(IFS0bits.T2IF));//1ms待ち
	}
	T2CONbits.TON = 0;//timer2止める
	_am_delay_ms_isUsed = 0;
}
void delay_us(uint16 time){// time usだけ時間待ちしてやる(刻みは10us)
	uint32 j;
	TMR3 = 0;
	T3CONbits.TON = 1;//timer3作動
	for(j=0;j<(time/10);++j){//time us待つ
		TMR3 = 0;
		IFS0bits.T3IF = 0;
		while(IFS0bits.T3IF==0);//10us待ち						 
	}
	T3CONbits.TON = 0;//timer3止める
}

void delay_ms2(uint16 time){//time msだけ時間待ちしてやる	
	uint32 j;
	if(_am_delay_ms2_isUsed != 0)return;
	_am_delay_ms2_isUsed = 1;
	TMR6 = 0;
	T6CONbits.TON = 1;//timer6作動
	for(j=0;j<time;++j){//time ms待つ
		TMR6 = 0;
		IFS2bits.T6IF = 0;
		while(IFS2bits.T6IF==0);//1ms待ち						 
	}
	T6CONbits.TON = 0;//timer6止める
	_am_delay_ms2_isUsed = 0;
}

//timer1用関数
void ini_timer1(uint16 time){//time(ms)（1～500msの間）に一回割り込む
	T1CONbits.TCKPS = 3;		//prescaller 1/256
	PR1 = (int)((double)clock*(double)time / (2.*256.*1000.) + 0.5);//PR1を指定
	TMR1= 0;					//T1カウントをクリア
}
/*
//割り込みの使い方
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void){//T1IFによる送信割り込み
	IFS0bits.T1IF = 0;			//T1割り込みフラグをクリア
	TMR1 = 0;					//T1カウントをクリア
	PORTAbits.RA0 += 1;			//LEDの状態を変化(処理)
}
*/
void enable_interrupt_timer1(){//割り込み許可
	if(IEC0bits.T1IE==0){
		TMR1 = 0;					//T1カウントをクリア
		IEC0bits.T1IE = 1;			//T1割り込みを許可	
		T1CONbits.TON = 1;			//T1カウント開始
	}
}
void disable_interrupt_timer1(){//割り込み終了
	if(IEC0bits.T1IE!=0){
		IEC0bits.T1IE = 0;			//T1割り込みを禁止	
		T1CONbits.TON = 0;			//T1カウント終了
		TMR1 = 0;					//T1カウントをクリア
	}
}
uint8 can_interrupt_timer1(){
	return IEC0bits.T1IE;
}
uint8 tmp_disable_interrupt_timer1(){
	if(IEC0bits.T1IE==0)return 0;
	disable_interrupt_timer1();
	return 1;
}
void replace_interrupt_timer1(uint8 c){
	if(IEC0bits.T1IE==0 && c!=0){
		TMR1 = 0;					//T1カウントをクリア
		IEC0bits.T1IE = 1;			//T1割り込みを許可	
		T1CONbits.TON = 1;			//T1カウント開始
	}else if(IEC0bits.T1IE!=0 && c==0){
		IEC0bits.T1IE = 0;			//T1割り込みを禁止	
		T1CONbits.TON = 0;			//T1カウント終了
		TMR1 = 0;					//T1カウントをクリア
	}
}
void reset_timer1(){
	TMR1 = 0;					//T1カウントをクリア
}

//timer4用関数
void ini_timer4(uint16 time){//time(ms)（1～50000msの間）に一回割り込む
	uint32 val=(uint32)((double)clock*(double)time / (2.* 1. *1000.));
	T5CONbits.TON = 0; // Stop any 16-bit Timer3 operation
	T4CONbits.TON = 0; // Stop any 16/32-bit Timer3 operation
	__asm__("NOP");
	T4CONbits.T32 = 1;			//32bit mode	
	__asm__("NOP");
	T4CONbits.TCKPS = 0;		//prescaller 1/1 
	__asm__("NOP");	
	T4CONbits.TCS = 0;			//内部クロック使う
	__asm__("NOP");
	T4CONbits.TGATE = 0;		//ゲート時間累算無効
	PR5 = (uint16)(val>>16);//上位PR
	PR4 = (uint16)(val);//下位PRを指定
	TMR5 = 0;					//T4カウントをクリア
	TMR4 = 0;
	T4CONbits.TON = 1; // Start 32-bit Timer
}
/*
//割り込みの使い方
void __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void){//T5(4ではない注意)IFによる送信割り込み
	IFS1bits.T5IF = 0;			//T5割り込みフラグをクリア
	TMR5 = 0;
	TMR4 = 0;                   //T4カウントをクリア
	PORTAbits.RA0 += 1;			//LEDの状態を変化(処理)
}
*/
void enable_interrupt_timer4(){//割り込み許可
	if(IEC1bits.T5IE==0){//割り込みはtimer５レジスタで制御する仕様
		TMR5 = 0;
		TMR4 = 0; //T4カウントをクリア
		IEC1bits.T5IE = 1;			//T4割り込みを許可	
		T4CONbits.TON = 1;			//T4カウント開始
	}
}
void disable_interrupt_timer4(){//割り込み終了
	if(IEC1bits.T5IE!=0){
		IEC1bits.T5IE = 0;			//T1割り込みを禁止	
		T4CONbits.TON = 0;			//T1カウント終了
		TMR5 = 0;					//T1カウントをクリア
		TMR4 = 0; 
	}
}
void reset_timer4(){
	TMR5 = 0;					//T1カウントをクリア
	TMR4 = 0;
}



//AD変換関連関数
/*
void setup_adc_ports(uint32 port){//AD変換に使うポート決め（例 port = AN1|AN3|AN23) 
	uint32 port1 = 0xFFFF0000;
	uint32 port2 = 0x0000FFFF;  
	port1 = port & port1;		//上位16ビットのみ残す
	port1 = port1>>16;			//16ビット右にシフト
	AD1PCFGH = ~((uint32)port1);	//反転して、ポート構成レジスタに入れる。0ならADポートとして使う。
								//AN16~31ピンの設定
	port2 = port & port2;		
	AD1PCFGL = ~((uint32)port2);	//AN0~15ピンの設定
}
*/
//AD変換に使うVref決め(ADC1)
void setup_adc1_Vref(uint8 mode){
	AD1CON2bits.VCFG = mode;
}
//12bitモードにするか否か
void setup_adc1_12bit(uint8 Y_N){
	if(Y_N == 1){
		AD1CON1bits.AD12B = 1;		//12bitモード
	}else{
		AD1CON1bits.AD12B = 0;		//10bitモード
	}
}
//AD変換時間が20us位になるように自動設定する関数
void setup_adc1_time_auto(){
	AD1CON1bits.SSRC = 7;		//内臓カウンタでサンプリング終了して変換開始
	AD1CON3bits.SAMC = 0;		//Autoサンプル時間　０
	__asm__("NOP");
	AD1CON3bits.ADCS = (int)((double)clock/(2.*1000000.)) + 21;//変換クロック選択
															//１ターン変換(Tｃｙ＊ADCS)が１μs以上になるように		
	AD1CON1bits.ADON = 1;		//ADC有効
}
//チャンネル指定関数(この後時間待ちは必要なし)
void set_adc_channel(uint8 ch){
	AD1CHS0bits.CH0SA = ch;//チャンネル指定
}
//ADCデータ読み込み関数(２０usくらいかかる)
uint32 read_adc(){
	AD1CON1bits.SAMP = 1;	 //サンプリングスタート
	while(!AD1CON1bits.DONE);//変換終了まで待つ
	AD1CON1bits.DONE=0;		//終了ステータスを戻す
	return ADC1BUF0;		//結果を返信
}
// AD変換のモードの設定，1チャンネルごとに１スキャンするか，まとめて16個スキャンするか
void set_adc_sampleMode(uint32 mode){
    if(mode == EVERY){
        AD1CON2bits.SMPI = 0;// set DONE flag every 1 sample operation
        __asm__("NOP");
		AD1CON2bits.BUFM = 0;//Always starts filling the buffer from the start address
    }else{
        //未実装
    }
}

//SPI用関数
void setup_spi(uint8 mode){

	//クロック開始プロトコルの設定
	if((mode & 0x02) == 0x02){//CLOCK_H_TO_L でスタート
		SPI1CON1bits.CKP = 1;	
	}else{//CLOCK L to H でスタート
		SPI1CON1bits.CKP = 0;
	}
	
	//MASTER or SLAVE
	if((mode & 0x01)%2 == 0){//MASTER mode
		SPI1CON1bits.MSTEN = 1;		//マスターモードに設定
		//クロック分周１
		if((mode & 0x1C)==0x00){
			SPI1CON1bits.PPRE = 3;		//1st = 1:1
		}else if((mode & 0x1C)==0x04){
			SPI1CON1bits.PPRE = 2;		//1st = 4:1
		}else if((mode & 0x1C)==0x08){
			SPI1CON1bits.PPRE = 1;		//1st = 16:1
		}else if((mode & 0x1C)==0x10){
			SPI1CON1bits.PPRE = 0;		//1st = 64:1
		}
		//クロック分周２
		SPI1CON1bits.SPRE = ((~((mode & 0xE0)>>5)) & 0x07);//分周２の設定

		//SPI１起動	
		SPI1STATbits.SPIROV = 0;	//受信クリア
		__asm__("NOP");
		SPI1STATbits.SPIEN = 1;		//SPI1有効

	}else{//SLAVE mode
		SPI1BUF = 0x00;				//バッファをクリア
		SPI1CON1bits.MSTEN = 0;		//スレーブモードに設定
		SPI1CON1bits.SMP = 0;		//smpクリア
		if((mode & 0x10) != 0x10){	//ssモードを使うとき
			SPI1CON1bits.SSEN = 1;	//SSpinをONにする
		}
		//SPI１起動	
		SPI1STATbits.SPIROV = 0;	//受信クリア
		__asm__("NOP");
		SPI1STATbits.SPIEN = 1;		//SPI1有効
	}
}

void setup_spi2(uint8 mode){
	//クロック開始プロトコルの設定
	if((mode & 0x02) == 0x02){//CLOCK_H_TO_L でスタート
		SPI2CON1bits.CKP = 1;	
	}else{//CLOCK L to H でスタート
		SPI2CON1bits.CKP = 0;
	}
	
	//MASTER or SLAVE
	if((mode & 0x01)%2 == 0){//MASTER mode
		SPI2CON1bits.MSTEN = 1;		//マスターモードに設定
		//クロック分周１
		if((mode & 0x1C)==0x00){
			SPI2CON1bits.PPRE = 3;		//1st = 1:1
		}else if((mode & 0x1C)==0x04){
			SPI2CON1bits.PPRE = 2;		//1st = 4:1
		}else if((mode & 0x1C)==0x08){
			SPI2CON1bits.PPRE = 1;		//1st = 16:1
		}else if((mode & 0x1C)==0x10){
			SPI2CON1bits.PPRE = 0;		//1st = 64:1
		}
		//クロック分周２
		SPI2CON1bits.SPRE = ((~((mode & 0xE0)>>5)) & 0x07);//分周２の設定

		//SPI2起動	
		SPI2STATbits.SPIROV = 0;	//受信クリア
		__asm__("NOP");
		SPI2STATbits.SPIEN = 1;		//SPI2有効

	}else{//SLAVE mode
		SPI2BUF = 0x00;				//バッファをクリア
		SPI2CON1bits.MSTEN = 0;		//スレーブモードに設定
		__asm__("NOP");
		SPI2CON1bits.SMP = 0;		//smpクリア
		if((mode & 0x10) != 0x10){	//ssモードを使うとき
			SPI2CON1bits.SSEN = 1;	//SSpinをONにする
		}
		//SPI2起動	
		SPI2STATbits.SPIROV = 0;	//受信クリア
		__asm__("NOP");
		SPI2STATbits.SPIEN = 1;		//SPI2有効
	}
}

/*
void spi_write(uint16 data){   
	if(SPI1CON1bits.MODE16){       
        SPI1BUF = data;			//16bit送信モード
	}else{ 
        SPI1BUF = data & 0xff;  //8bit送信モード　(default) 
	}
    while(SPI1STATbits.SPITBF);		//送信可能状態になるまで待つ. SPITBF 1:TXB is full 0:TXB is enpty
    data = SPI1BUF;             //読み捨て
}
*/
/*
void spi2_write(uint16 data){   
	if(SPI2CON1bits.MODE16){       
        SPI2BUF = data;			//16bit送信モード
	}else{ 
        SPI2BUF = data & 0xff;  //8bit送信モード　(default) 
	}
    while(SPI2STATbits.SPITBF);		//送信可能状態になるまで待つ. SPITBF 1:TXB is full 0:TXB is enpty
    data = SPI2BUF;             //読み捨て
}
*/

uint32 spi_read(uint16 data){   
	SPI1STATbits.SPIROV = 0;			//受信オーバーフローフラグリセット
	if(SPI1CON1bits.MODE16){       
        SPI1BUF = data;			//16bit送信モード
	}else{ 
        SPI1BUF = data & 0xff;  //8bit送信モード　(default) 
	}
	while(!SPI1STATbits.SPIRBF);		//受信待ち
	return SPI1BUF;						//結果リターン
}

uint32 spi2_read(uint16 data){   
	SPI2STATbits.SPIROV = 0;			//受信オーバーフローフラグリセット
//	spi2_write(data);					//書き込み
	if(SPI2CON1bits.MODE16){       
        SPI2BUF = data;			//16bit送信モード
	}else{ 
        SPI2BUF = data & 0xff;  //8bit送信モード　(default) 
	}
	while(!SPI2STATbits.SPIRBF);		//受信待ち
	return SPI2BUF;						//結果リターン
}

void spi2_write(uint16 data){
	spi2_read(data);
}

void spi_write(uint16 data){
	spi_read(data);
}


unsigned char spi_data_is_in(){
	return SPI1STATbits.SPIRBF;//データが来ているかを確認、来てたら1を返す
}


unsigned char spi2_data_is_in(){
	return SPI2STATbits.SPIRBF;//データが来ているかを確認、来てたら1を返す
}


//Watch dog timer 関連関数
void enable_wdt(){
	RCONbits.SWDTEN = 1;
}
void disable_wdt(){
	RCONbits.SWDTEN = 0;
}
void restart_wdt(){
	ClrWdt();
}

/* set_tris系列宣言 */
void set_tris_a(uint16 Bits){TRISA=Bits;}
void set_tris_b(uint16 Bits){TRISB=Bits;}
void set_tris_c(uint16 Bits){TRISC=Bits;}
void set_tris_d(uint16 Bits){TRISD=Bits;}
void set_tris_e(uint16 Bits){TRISE=Bits;}
void set_tris_f(uint16 Bits){TRISF=Bits;}
void set_tris_g(uint16 Bits){TRISG=Bits;}


//change clock Freq
//　Divの値だけメインクロックの周波数を分割してくれる
// これを行ったら，タイマー系の初期化を忘れずに・・・・
// ini_clock, ini_UART*, ini_timer* setup_adc_time_auto set_up_spi*
void changeClock(uint8 mode){
	CLKDIVbits.ROI = 0; // 割り込みでクロックが戻るのを阻止
	__asm__("NOP");
	// DOZE への書き込みフェーズ
	CLKDIVbits.DOZEN = 0;
	__asm__("NOP");
	CLKDIVbits.DOZE =  mode; // set
	__asm__("NOP");
	CLKDIVbits.DOZEN = 1; // make DOZE enable
}

#
#endif
