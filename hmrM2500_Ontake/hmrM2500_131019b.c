#ifndef HMR_M2500_INC
#define HMR_M2500_INC 200
#
#define HMLIB_NOLIB
/*
===hmrMain2500===
hmrMainPic用のメインプログラム
hmrMain2500:v2_00/130720 hmIto
	デバッグ用の関数群やコメントを消して整理
hmrMain2500_v1_01/130202 iwahori
	main関数内にmsg_setup_talkとmsg_setup_listenを追加
	listen関数に入れているので受信は上手くいっているが返信がない，
	パケットの区切りを作るpacket_talkとかを作っていないのが原因？
	fp_finRecvPac,com_vmc_finRecvPacに委譲？
hmrMain2500_v1_00/130112 hmIto
	hmLib_v3_03に対応
	全二重通信に対応
	センサー部分を分離してinclude
hmrMain2400_v1_09/121101 hmIto
	restart_wdtをmain-loopの一か所に集約
	パケット区切りにHMR_COM_PACTRMNIDを使用
	受信パケットがフルになった場合は、強制リセット
	強制リセットコマンドとしてMessageに依存しない(Comには依存)、0xFF命令を追加
	->バグがあったため一時的に廃止
	sprite_message2を使用するように変更
hmrMain2400_v1_08/121031 hmIto
	devctrに半スリープ時の電源制御用変数とそのコマンドを追加
hmrMain2400_v1_07/121030 iwahori
	加速度のmsg_resistを追加
hmrMain2400_v1_06/121028 hmIto
	スリープ機能とローミング機能（通信デバイス切り替え）を実装
		devctr関数群を用いて操作
		いずれも、device_messageコマンドを用いて制御する
		timer4_5を用いている
			スリープ時に10秒に一度割り込ませて、時間計測
			ローミング時のデバイス切り替えに使用
	送信バッファに基づく送信割り込み割り込み制御を採用
		送信バッファがからになったらdisable
		送信バッファにデータを追加したらenable
		com_out_empty関数を使って、送信バッファが空かどうか確認
hmrMain2400_v1_05/121027 hmIto
	uartの送受信が正常に行われることまで確認
		hmrTaskがわるさをしていたので廃止．
		バッファの制御時に送信割り込みを制御するように変更
hmrMain2400_v1_04/121020 iwahori
	initialize()内にSPI slave slectピンの初期化を追加
		RF module電源オン
hmrMain2400_v1_03/121020 hmIto
	デバッグ修正を確認して、コメントア
	ウトを削除
	packet_message関数が未完成だったのを追加
hmrMain2400_v1_02/121014 iwahori
	#include "hmrSpriteFunctions.h"を追加
	p_DIP1をUserConfigBit1に変更
	uart1_fget_interrupt_retry_task()とuart1_fput_interrupt()の順番を入れ替え
	送信も同様
	packet_message()内cstring Ans{1,1}→cstring Ans={1,1}
	sprite_ONをsprite_powerに変更
hmrMain2400_v1_01/121013 hmIto
	恐らく送受信部分がほぼ完成
	初期化終端化が必要な関数の作成が残る
hmrMain2400_v1_00/120921 hmIto
	バージョン管理開始
	ファイルを複数に分割
*/
#include <stdlib.h>
#include "amPic30_v7.h"
#include "hmrVMC1.h"
#include "hmrCom.h"
#include "hmrMessage.h"
#include "hmrCO2.h"
#include "hmrBattery.h"
#include "hmrGPS.h"
#include "hmrDrive.h"
#include "hmrInertial.h"
#include "hmrInfraRed.h"
#include "hmrSprite.h"
#include "hmrThermo.h"
#include "hmrDeviceManage.h"
//#include "hmrDeviceManage.h"
//#include "hmrSpriteFunctions.h"
#include "hmrSpriteInterface.h"
#include "hmrDebug.h"
//VMC1
VMC1* pVMC;

//====== デバッグ系列 ======
hmLib_boolian DebugFlag;
void debug_setup_talk(){DebugFlag=hmLib_true;}
void debug_setup_listen(){}
hmLib_boolian debug_listen(cstring Str){return hmLib_false;}
hmLib_boolian debug_talk(cstring* pStr){
   static unsigned char Buf[7];
	if(DebugFlag){
		DebugFlag=hmLib_false;
		cstring_construct_static(pStr,Buf,Buf+7);
		cstring_putc(pStr,0,_cstring_debug_CreateCnt);
		cstring_putc(pStr,1,_cstring_debug_StaticCnt);
		cstring_putc(pStr,2,_cstring_debug_DynamicCnt);
		cstring_putc(pStr,3,_cstring_debug_ErrorStatic);
		cstring_putc(pStr,4,_cstring_debug_ErrorDynamic);
		cstring_putc(pStr,5,_cstring_debug_ErrorPre);
		cstring_putc(pStr,6,_cstring_debug_ErrorDestruct);
		return hmLib_false;
	}
	return hmLib_true;
}

//====== 初期化/終端化 ======
//初期化関数
void initialize(void){
	//デバイスの初期化
	devmng_initialize();

	//通信確認用
	rs_fputc('O',Stream_VMC);
	rs_fputc('K',Stream_VMC);
	rs_fputc(0x0d,Stream_VMC);
	rs_fputc(0x0a,Stream_VMC);

	//通信関連機能の初期化
	pVMC=com_createVMC1();
	vmc1_initialize(pVMC,(const unsigned char*)("hmr"),(const unsigned char*)("ctr"));
	com_initialize();
	msg_initialize();

	rs_fputc('C',Stream_VMC);
	rs_fputc('m',Stream_VMC);
	rs_fputc(0x0d,Stream_VMC);
	rs_fputc(0x0a,Stream_VMC);

	//モジュールの初期化
	sprite_initialize();
	inertial_initialize();

	rs_fputc('M',Stream_VMC);
	rs_fputc('d',Stream_VMC);
	rs_fputc(0x0d,Stream_VMC);
	rs_fputc(0x0a,Stream_VMC);

	//割り込み整理
	timer4_5_initialize(1000);
	timer4_5_reset();
	timer4_5_enable_interrupt();

	set_interrupt_fputflag(Stream_VMC);//割り込み予約
	enable_interrupt_fget(Stream_VMC);
	enable_interrupt_fput(Stream_VMC);
}
//終端化関数
void finalize(void){
	//割り込み整理
	timer4_5_disable_interrupt();
	timer4_5_finalize();

	disable_interrupt_fget(Stream_VMC);
	disable_interrupt_fput(Stream_VMC);

	//モジュールの終端化処理
	sprite_finalize();
	inertial_finalize();
	
	//通信関連の終端化処理
	msg_finalize();
	vmc1_finalize(pVMC);
	com_finalize();
	com_releaseVMC1(pVMC);

	//デバイス終端化
	devmng_finalize();
}

//====== 割り込み関数 ======
//受信割り込み関数
void uart1_fget_interrupt(void){
	//割込みフラグをクリア
	clear_interrupt_fgetflag(Stream_VMC);

	//データを受信し、Comに処理させる
	vmc1_recv(pVMC,rs_fgetc(Stream_VMC));
}
//送信割り込み関数
void uart1_fput_interrupt(void){
	//割り込みフラグをクリア
	clear_interrupt_fputflag(Stream_VMC);

	//送信可能なら、送信
	rs_fputc(vmc1_send(pVMC),Stream_VMC);

	//送信可能でなくなった場合は、割り込みを切る
	if(!vmc1_can_send(pVMC)){
		uart1_disable_fput_interrupt();
	}
};
//タイマー4_5割り込み関数
void timer4_5_interrupt(void){
	//フラグクリアと時刻のリセット
	timer4_5_clear_interrupt_flag();
	timer4_5_reset();

	//デバイスマネージの定期実行処理（引数を秒間隔で使用）
	devmng_interrupt_function(1);
}

//====== main 関数 ======
int main(void){
	idata IData;
	boolian IPacketMode=0;
	odata OData;
	boolian OPacketMode=0;
	sint8 debug_cnt=0;

	//初期化
	initialize();

	//メッセージ登録
	msg_regist('m',motor_setup_listen,motor_listen,motor_setup_talk,motor_talk);
	msg_regist('j',sprite_setup_listen,sprite_listen,sprite_setup_talk,sprite_talk);
	msg_regist('b',battery_setup_listen,battery_listen,battery_setup_talk,battery_talk);
	msg_regist('g',gps_setup_listen,gps_listen,gps_setup_talk,gps_talk);
	msg_regist('C',co2_setup_listen,co2_listen,co2_setup_talk,co2_talk);
//	msg_regist('S',h2s_message);
	msg_regist('t',thermo_setup_listen,thermo_listen,thermo_setup_talk,thermo_talk);
//	msg_regist('T',infrared_setup_listen,infrared_listen,infrared_setup_talk,infrared_talk);
	msg_regist('h',0,0,0,0);
	msg_regist('c',compass_setup_listen,compass_listen,compass_setup_talk,compass_talk);
	msg_regist('a',axel_setup_listen,axel_listen,axel_setup_talk,axel_talk);
	msg_regist('G',gyro_setup_listen,gyro_listen,gyro_setup_talk,gyro_talk);
	msg_regist('!',debug_setup_listen,debug_listen,debug_setup_talk,debug_talk);

	HMR_PIN_RedLED_set(0);
	HMR_PIN_YellowLED_set(0);
	delay_ms(400);
	HMR_PIN_RedLED_set(1);
	HMR_PIN_YellowLED_set(1);
    delay_ms(400);
	HMR_PIN_RedLED_set(0);
	HMR_PIN_YellowLED_set(0);
    delay_ms(400);
	HMR_PIN_RedLED_set(1);
	HMR_PIN_YellowLED_set(1);
    delay_ms(400);
	HMR_PIN_RedLED_set(0);
	HMR_PIN_YellowLED_set(0);
    delay_ms(400);

	HMR_PIN_RedLED_set(1);

	while(1){
		//Watch Dog Timer リセット
		restart_wdt();

		//早すぎるので待機
		delay_ms(5);

		//受信可能なデータがある場合
		if(!com_in_empty()){
			//受信データを取得
			com_in_move_pop(&IData);

			//中身が入っている場合
			if(idata_is_construct(&IData)){
				//Packetをまだ開いていない場合
				if(!IPacketMode){
					//Packetをここで開く
					IPacketMode=hmLib_true;

					//受信用メッセージモジュール準備
					msg_setup_listen();
				}

				//Packet終了IDだった場合
				if(IData.ID==HMR_COM_PACTRMNID){
					//Packetをここで閉じる
					IPacketMode=hmLib_false;

					//メッセージ破棄
					idata_destruct(&IData);
				}else{
					//メッセージ処理;
					msg_listen(&IData);
				}
			}
		}

		//早すぎるので待機
		delay_ms(5);

		//送信待ちのPacketがなく、comの送信バッファがいっぱいでもないとき
		if(!com_isWaitSendPacket() && !com_out_full()){
			//Packetをまだ開いていない場合
			if(!OPacketMode){
				//Packetをここで開く
				OPacketMode=hmLib_true;
				//送信用メッセージモジュール準備
				msg_setup_talk();
			}

			//送信用データを準備する
			odata_format(&OData);

			//送信データの取得に失敗した場合
			if(msg_talk(&OData)){
				//中身が作成されていれば破棄
				if(odata_is_construct(&OData))odata_destruct(&OData);
				//Packetをここで閉じる
				OPacketMode=hmLib_false;
				OData.ID=HMR_COM_PACTRMNID;
			}

			//送信データを送り出す
			com_out_move_push(&OData);		
		}

		//送信割り込みが切られていて、かつ送信可能状態のときには、送信割り込みをオンにする
		if(!uart1_is_enable_fput_interrupt()){
			if(vmc1_can_send(pVMC))uart1_enable_fput_interrupt();
		}
	}

	finalize();

	return 0;
}
#
#endif
