#ifndef AMPIC30_INC
#define AMPIC30_INC 701
#

/*
amPic30_v7_02/130112 hmIto
	hmLib_v3_03に対応
amPic30_v7_01/121028 hmIto
	ini_timer4で、timerをロックせずいじっていたのを修正
	ini_timer4の演算を少しだけ高速化
	changeClockに注意書きを付加
		この関数を用いてクロック数を変更しても、影響を受けるのはCPUのみ
		timerなどは影響を受けない可能性
amPic30_v7_00/121027 iwahori
	clock周波数の変更関数changeClock()追加
	set_adc_sampleMode()追加

*/
//
//＊＊＊＊　dsPIC C30コンパイラ用ライブラリ　＊＊＊＊＊
//ヘッダーファイル版　Ver. 3-4
//
//*******　データ型(bit)　************
//  dsPic型		bit		hmr型
//				8		byte
//	int			16		short
//  long		32		int
//	long long	64		long
//  double		32		float
//  long double 64		double
//
//*******　delay関数について　********　
//　delay関数のためにtimer2,3を使ったので、timer2,3は使用できない仕様になっている！！
//  
//  JPEG割り込み内で再コネクトの際にdelayを使う必要があったので、専用にdelay関数を作ることにした
//　timer 6,7 を使用する　
//
//*******　output_high()関数について　*********
//　今回はこのような出力関数群の実装はあきらめた
//  その代わり、#define p_LED PORTAbits.RA0 のように定義することで、
//　p_LED = 1 のようにそのまま変数として出力をいじくる事になる。
//
//*******  AD converter *******************
//　AD変換の場合、ポートの入出力設定は自動では行わないので注意する必要がある
//　ややこしそうであれば、関数に今後実装する
//  現在はADC1についてのみ実装(これで十分と思われる)
//
//*******  SPI関数  ************************
//	初期設定してから、read,　write　するだけ、MASTERモードについては動作確認済み
//	SLAVEモードについては動作確認はしていないので注意してほしい。　
//
//*******  WDT関係　*************************
//	WDTはソフトウェアでONOFFできる。しかし、これはコンフィギュ依存なので注意
//　(具体的にはWDTをコンフィギュでOFFにする必要がある)
//
//*******  Timer関係　***********************
// Timer１は、16ビットタイマ　クロック５０Mで500ms待ちは不可能になるので注意
//
// Timer４は、32ビットタイマ　クロックはいくらでも、10s待ちできます！！　　
//   ただ、割り込みの制御は timer5 のレジスタを使う。割り込み関数の名前に注意！！
//
//*******  CONFIG　の　参考例　**************
/*
//config(メインでやってクリ)　参考例(この関数群の動作確認済み設定例) 
//
//　WDTオフにしているのは、内部でソフトウェア的にON,OFFできるため
//  WDTは内部の別のクロックで決まるので、メインクロックの速さには依存しない
//  WDTのWINDOWモードは、WDT周期の3/4以降に、WDTクリアしないと、リセットがかかるという厄介者なのでOFF
//	内部クロックはPLLによって早くできる。 
//　
#include <p33FJ256GP710.h>
_FOSCSEL( FNOSC_FRC);		//Internal FRC(7.37MHz)
_FOSC( FCKSM_CSDCMD & OSCIOFNC_ON & POSCMD_NONE ); //HS
_FWDT( FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS512 );//WDT 2s
_FPOR( FPWRT_PWR64 );//power on timer 64ms 
*/

//::::::::::::::::::　更新履歴　:::::::::::::::::::::::::
//
//  100724	UARTについてライブラリ化進める
//	100807	UARTについてライブラリ完成　
//			タイマー、時間関係のライブラリも完成
//	100808　ＡＤ変換についての関数群を作成
//			Vrefがうまくいっていない(回路の問題か？)
//			SPIは動作チェック済み
//

/* amPic30を使っていることを示す宣言 */
#define HMLIB_NOLIB
#include"hmrType.h"

//基本データサイズ型
#ifdef P33FJ256GP710_INC
#	define P33FJ256GP710_INC
#	include <p33FJ256GP710.h>
#endif
#include <timer.h>

#define AN0 0x01
#define AN1 0x02
#define AN2 0x04
#define AN3 0x08
#define AN4 0x10
#define AN5 0x20
#define AN6 0x40
#define AN7 0x80
#define AN8 0x0100
#define AN9 0x0200
#define AN10 0x0400
#define AN11 0x0800
#define AN12 0x1000
#define AN13 0x2000
#define AN14 0x4000
#define AN15 0x8000

#define AN16 0x010000
#define AN17 0x020000
#define AN18 0x040000
#define AN19 0x080000
#define AN20 0x100000
#define AN21 0x200000
#define AN22 0x400000
#define AN23 0x800000
#define AN24 0x01000000
#define AN25 0x02000000
#define AN26 0x04000000
#define AN27 0x08000000
#define AN28 0x10000000
#define AN29 0x20000000
#define AN30 0x40000000
#define AN31 0x80000000

//******* 必ず必要な初期化関数 ******
//一番初めに必ずこのクロック指定が必要
//クロック自体は、コンフィギュレーションビットで指定
//あくまでも、クロックを設定するのではなく、ただ関数にクロックを教えているだけであることに注意！！

void ini_clock(uint64 _clock);//クロックを指定してやる

//******* シリアル通信系関数 *******
//初期化して、fputc,fgetcで使う
//あるいは、初期化して割り込み許可して割り込みで使う

void ini_UART1(uint32 baudrate);//初期化＋UART1使用許可
void ini_UART2(uint32 baudrate);//初期化＋２使用許可
//シリアル通信における割り込みの使い方
/*
１、割り込みを許可してやる(以下の関数軍)
　　　ちなみに　送信割り込みは、送信バッファに空きがあれば割り込んでくる。
　　　　　　　　受信割り込みは、何か受信したら割り込んでくる。
２、メイン関数の始まる前のところで、以下のように割り込み処理を書いてやれば良し
　　割り込み関数内では、フラグさえ消してやれば、何をしてもよい！！
　　steam の書き間違いに注意（フラグ消して、処理するだけ）

void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void){//UART1による送信割り込み
	//必須処理
	clear_interrupt_fputflag(1);		//割り込みフラグクリア(必須)
	//独自処理
	fputc(data,1);						//データ送信
}
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void){//UART2による受信割り込み
	clear_interrupt_fgetfalg(2); 		//割り込みフラグクリア
	data = fgetc(2);					//データ受信
}
*/

/* 割り込み関数のコンパイラ定義型置換マクロ */
//stream1の受信割込み
#define interrupt_fget_rs1(void) __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
//stream1の送信割込み
#define interrupt_fput_rs1(void) __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void)
//stream2の受信割込み
#define interrupt_fget_rs2(void) __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void)
//stream2の送信割込み
#define interrupt_fput_rs2(void) __attribute__((interrupt, no_auto_psv)) _U2TXInterrupt(void)

//割り込みフラグを消す
void clear_interrupt_fgetflag(uint8 stream);
void clear_interrupt_fputflag(uint8 stream);
void set_interrupt_fgetflag(uint8 stream);
void set_interrupt_fputflag(uint8 stream);
//割り込みをdisable/enable
void enable_interrupt_fget(uint8 stream);
void enable_interrupt_fput(uint8 stream);
void disable_interrupt_fget(uint8 stream);
void disable_interrupt_fput(uint8 stream);

//一時的にstatusを退避して割り込みをdisable　危険使うな
// これは、statusを退避したあとに、割り込まれてstatusの状態が変わる可能性がある　
// 危険だから使うな！！！！！！！　　　　　　　　　　　　　　　　　　　　　　　　
uint8 tmp_disable_interrupt_fget(uint8 stream);
uint8 tmp_disable_interrupt_fput(uint8 stream);
uint8 tmp_disable_interrupt_fgetput(uint8 stream);
//tmp関数で退避したstatusを元に戻す　危険　使うな
void replace_interrupt_fget(uint8 c,uint8 stream);
void replace_interrupt_fput(uint8 c,uint8 stream);
void replace_interrupt_fgetput(uint8 c,uint8 stream);
//送受信関数
void rs_fputc(uint8 data, uint8 stream);
uint8 rs_fgetc(uint8 stream);
//受信チェック関数
uint8 rs_kbhit(uint8 stream);

//********* 便利関数 **********
//時間待ち(ｍｓ)
void delay_ms(uint16 time);
void delay_us(uint16 time);//クロックが低速なので10us刻みでしか待てないことに注意
void delay_ms2(uint16 time);//予備

//********* timer1用関数 ***************
//初期化して、割り込み許可するとタイマースタート

//初期化関数　何msに一回割り込むか(1～500で指定可能)
void ini_timer1(uint16 time);
/*
//割り込みの使い方
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void){//T1IFによる送信割り込み
	//決まった処理
	IFS0bits.T1IF = 0;			//T1割り込みフラグをクリア
	TMR1 = 0;					//T1カウントをクリア
	//以下処理内容
	PORTAbits.RA0 += 1;			//LEDの状態を変化(処理)
}
*/
void reset_timer1();//タイマーカウンタリセット
void enable_interrupt_timer1();//タイマー割り込み許可
void disable_interrupt_timer1();//タイマー割り込み禁止
uint8 can_interrupt_timer1();
uint8 tmp_disable_interrupt_timer1();
void replace_interrupt_timer1(uint8 c);

// ********** timer4用関数　**************** //
//初期化して、割り込み許可するとタイマースタート//
//初期化関数　何msに一回割り込むか（1～10000ms）で指定可能//
void ini_timer4(uint16 time);

/*
//割り込みの使い方
void __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void){//T5(4ではない注意)IFによる送信割り込み
	IFS1bits.T5IF = 0;			//T5割り込みフラグをクリア
	TMR5 = 0;
	TMR4 = 0;                   //T4カウントをクリア
	PORTAbits.RA0 += 1;			//LEDの状態を変化(処理)
}
*/
void enable_interrupt_timer4();
void disable_interrupt_timer4();
void reset_timer4();

//********** AD変換関連 ***************
//初期			：ポート設定して、参照電圧決めて、12bitにするか否か決めて、クロック設定をする
//データとるとき：ｃｈを指定して、読み込む関数を使う

//Port設定
void setup_adc_ports(uint32 port);//AD変換に使うポート決め（例 port = AN1|AN3|AN23)

//AD変換に使う参照電圧決め　Vrefのポートを入力にするのを忘れるな！！
//Vref設定　＋　 －
#define		Vin_GND 0x00
#define		Vref_GND 0x01
#define		Vin_Vref 0x02
#define		Vref_Vref 0x03
void setup_adc1_Vref(uint8 mode);// ex) setup_adc1_Vref(Vin_GND) (ADC1についてのみ) 

//12bitAD変換するか否か 1ならＹｅｓ、それ以外Ｎｏ　
void setup_adc1_12bit(uint8 Y_N);

//AD変換のためのクロック設定関数(自動)＋AD変換ON
void setup_adc1_time_auto();

//AD変換するCH(0~31)の指定(この後の時間待ちはいらない)
void set_adc_channel(uint8 ch);

//AD変換したデータを読み込む(20usかかります)
uint32 read_adc();
// AD変換のモードの設定，1チャンネルごとに１スキャンするか，まとめて16個スキャンするか選択できる
// まとめてスキャンは，AD変換関数にも変更が必要なので未実装
#define EVERY 0x00
void set_adc_sampleMode(uint32 mode);
//*********** SPI関連関数 ***********************
//SPIの初期設定を行った後、書き込んで、読み取って関数を使う
// 選択していないPINは highで保持しておく必要がある 
//

//初期設定
//MASTER or SLAVE
#define SPI_MASTER 0x00
#define SPI_SLAVE  0x01

//クロック読むタイミング
#define SPI_L_TO_H 0x00
#define SPI_H_TO_L 0x02

//クロック分周指定(Fｃｙ/?　= Fosc/(2*?)) MASTERのみ
//分周１
#define SPI_CLK_DIV_1 0x00
#define SPI_CLK_DIV_4 0x04
#define SPI_CLK_DIV_16 0x08
#define SPI_CLK_DIV_64 0x10

//分周2
#define SPI_CLK_DIV2_1 0x00
#define SPI_CLK_DIV2_2 0x20
#define SPI_CLK_DIV2_3 0x40
#define SPI_CLK_DIV2_4 0x60
#define SPI_CLK_DIV2_5 0x80
#define SPI_CLK_DIV2_6 0xA0
#define SPI_CLK_DIV2_7 0xC0
#define SPI_CLK_DIV2_8 0xE0

//SSpin　無効化　SLAVEのみ
#define SPI_SS_DISABLED 0x10

void setup_spi(uint8 mode);//SPI1 初期化関数
void setup_spi2(uint8 mode);//SPI2 初期化関数
//ex) setup_spi1(SPI_MASTER | SPI_L_TO_H | SPI_CLK_DIV_16 )

//SPIに書き込み、読み捨て
void spi_write(uint16 data);
void spi2_write(uint16 data);

//SPIから読んで書き込み
uint32 spi_read(uint16 data);
uint32 spi2_read(uint16 data);

//SPIにデータが来ているかどうか？きてたら 1
uint8 spi_data_is_in();
uint8 spi2_data_is_in();

//*********** WDT関連関数 ***********************
//enbale disable をソフトウェアで決めることができる(初期disable)
////restart_wdt()　でウォッチドッグクリア

//WDT許可
void enable_wdt();
//WDT拒否
void disable_wdt();
//WDT 定期的クリア
void restart_wdt();

/* output_XXXが使えるようにマクロ宣言 */
#define output_low(x) (x=0)
#define output_high(x) (x=1)
#define output_bit(x,a) (x=a)

/* set_tris系列宣言 */
void set_tris_a(uint16 Bits);
void set_tris_b(uint16 Bits);
void set_tris_c(uint16 Bits);
void set_tris_d(uint16 Bits);
void set_tris_e(uint16 Bits);
void set_tris_f(uint16 Bits);
void set_tris_g(uint16 Bits);
//#endif

//change clock Freq
//　Divの値だけメインクロックの周波数を分割してくれる
// これを行ったら，タイマー系の初期化を忘れずに・・・・
// ini_clock, ini_UART*, ini_timer* setup_adc_time_auto set_up_spi*
// ※121028 hmIto この機能はCPUのみ間引いた後のクロック数を利用する機能なので、
//   再初期化は不要の可能性  少なくとも、timerはオリジナルの速度を利用している
#define MAIN_CLK_Div_128 0x07
#define MAIN_CLK_Div_64 0x06
#define MAIN_CLK_Div_32 0x05
#define MAIN_CLK_Div_16 0x04
#define MAIN_CLK_Div_8 0x03
#define MAIN_CLK_Div_4 0x02
#define MAIN_CLK_Div_2 0x01
#define MAIN_CLK_Div_1 0x00

void changeClock(uint8 mode);

#
#endif
