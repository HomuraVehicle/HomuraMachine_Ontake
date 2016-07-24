#ifndef HMR_SPRITEINTERFACE_C_INC
#define HMR_SPRITEINTERFACE_C_INC 107
#
#define HMLIB_NOLIB
#ifndef HMR_SPRITEINTERFACE_INC
#	include "hmrSpriteInterface.h"
#endif

//=====================LED用define==========================//
#define _spriteitf_led_set(val)						sprite_setLED(val)
//=====================電源用define=========================//
#define _spriteitf_power(val)						sprite_power(val)
//=====================timer用define========================//
#define _spriteitf_timer_clear()					timer1_reset()
#define _spriteitf_timer_interrupt					timer1_interrupt
#define _spriteitf_timer_interrupt_disable()		timer1_disable_interrupt()
#define _spriteitf_timer_interrupt_enable()			timer1_enable_interrupt()
#define _spriteitf_timer_interrupt_clear()			timer1_clear_interrupt_flag()
#define _spriteitf_timer_interrupt_initialize(Time)	timer1_initialize(Time)
//==========================================================//

//=====================uart用define=========================//
#define x_spriteitf_uart_initialize()				hmr_uart2_initialize()
#define _spriteitf_uart_fputc(c)					rs_fputc(c,Stream_CMR)
#define _spriteitf_uart_fput_interrupt				uart2_fput_interrupt
#define _spriteitf_uart_fput_interrupt_set()		set_interrupt_fputflag(Stream_CMR)
#define _spriteitf_uart_fput_interrupt_clear()		clear_interrupt_fputflag(Stream_CMR)
#define _spriteitf_uart_fput_interrupt_enable()		enable_interrupt_fput(Stream_CMR)
#define _spriteitf_uart_fput_interrupt_disable()	disable_interrupt_fput(Stream_CMR)
//amPic30からPicC30への移行までの一時措置　spriteitf_PowerReset関数で使用
#define x_spriteitf_uart_disable()					U2MODEbits.UARTEN=0

#define _spriteitf_uart_fgetc()						rs_fgetc(Stream_CMR)
#define _spriteitf_uart_fget_interrupt				uart2_fget_interrupt
#define _spriteitf_uart_fget_interrupt_set()		set_interrupt_fgetflag(Stream_CMR)
#define _spriteitf_uart_fget_interrupt_clear()		clear_interrupt_fgetflag(Stream_CMR)
#define _spriteitf_uart_fget_interrupt_disable()		disable_interrupt_fget(Stream_CMR)
#define _spriteitf_uart_fget_interrupt_enable()	enable_interrupt_fget(Stream_CMR)
//==========================================================//

//===カメラ通信用コマンド群===
//リセット
static unsigned char _spriteitf_Cmd_RESET_CMR[4] = {0x56, 0x00, 0x26, 0x00};
spriteitf_command _spriteitf_Cmd_ResetCmr={_spriteitf_Cmd_RESET_CMR,4,70};
//写真撮影
static unsigned char _spriteitf_Cmd_TAKE_PICTURE[5] = {0x56, 0x00, 0x36, 0x01, 0x00};
spriteitf_command _spriteitf_Cmd_TakePicture={_spriteitf_Cmd_TAKE_PICTURE,5,5};
//データサイズを得る
static unsigned char _spriteitf_Cmd_GET_DATA_SIZE[5] = {0x56, 0x00, 0x34, 0x01, 0x00};
spriteitf_command _spriteitf_Cmd_GetDataSize={_spriteitf_Cmd_GET_DATA_SIZE,5,9};
//写真撮るのやめる
static unsigned char _spriteitf_Cmd_STOP_TAKE_PICTURE[5] = {0x56, 0x00, 0x36, 0x01, 0x03};
spriteitf_command _spriteitf_Cmd_StopTakePicture={_spriteitf_Cmd_STOP_TAKE_PICTURE,5,5};
//省電力モード
static unsigned char _spriteitf_Cmd_SAVE_PWR[7] = {0x56,0x00,0x3E,0x03,0x00,0x01,0x01};
spriteitf_command _spriteitf_Cmd_SavePwr={_spriteitf_Cmd_SAVE_PWR,7,4};
//省電力から復帰モード
static unsigned char _spriteitf_Cmd_STOP_SAVE_PWR[7] = {0x56,0x00,0x3E,0x03,0x00,0x01,0x00};
spriteitf_command _spriteitf_Cmd_StopSavePwr={_spriteitf_Cmd_STOP_SAVE_PWR,7,5};
//データを得るコマンド(初めのADDRESS(8の倍数)と、データ数指定)
static unsigned char _spriteitf_varCmd_GET_DATA[16] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00,0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x0B};
spriteitf_command _spriteitf_varCmd_GetData(spriteitf_dsize_t Address_,spriteitf_dsize_t PackSize_){
	spriteitf_command Ans;
	//PackSizeは８の倍数なので、8の倍数にする！！　超えた分は読み捨てる方針で
	if(PackSize_%8 != 0){
		PackSize_ = PackSize_ + 8 - PackSize_%8;
	}

	//受信開始アドレス指定
	_spriteitf_varCmd_GET_DATA[8] = (unsigned char)(Address_/256);
	_spriteitf_varCmd_GET_DATA[9] = (unsigned char)(Address_%256);

	//受信パケットのサイズ指定
	_spriteitf_varCmd_GET_DATA[12] = (unsigned char)(PackSize_/256);
	_spriteitf_varCmd_GET_DATA[13] = (unsigned char)(PackSize_%256);

	Ans.Ptr=_spriteitf_varCmd_GET_DATA;
	Ans.Size=16;
	Ans.InSize=10 + PackSize_;

	return Ans;
}
//圧縮率の指定(defo 0x36)
static unsigned char _spriteitf_varCmd_SET_COMP_RATE[9]={0x56,0x00,0x31,0x05,0x01,0x01,0x12,0x04,0x36};
spriteitf_command _spriteitf_varCmd_setCompRate(unsigned char ComRate_){
	spriteitf_command Ans;
	//受信開始アドレス指定
	_spriteitf_varCmd_SET_COMP_RATE[8] = ComRate_;

	Ans.Ptr=_spriteitf_varCmd_SET_COMP_RATE;
	Ans.Size=9;
	Ans.InSize=6;

	return Ans;
}
//画像サイズの指定(定義した３通り)要リセット
/*static unsigned char _spriteitf_varCmd_SET_IMAGE_SIZE_1[9]={0x56,0x00,0x31,0x05,0x04,0x01,0x00,0x19,0xFF};
spriteitf_command _spriteitf_varCmd_SetImageSize(spriteitf_imagesize_t PictureSize_){
	spriteitf_command Ans;
	_spriteitf_varCmd_SET_IMAGE_SIZE_1[8]=PictureSize_;
	Ans.Ptr=_spriteitf_varCmd_SET_IMAGE_SIZE_1;
	Ans.Size=9;
	Ans.InSize=5;
	return Ans;
}*/
//画像サイズの指定(定義した３通り)リセット不要
static unsigned char _spriteitf_varCmd_SET_IMAGE_SIZE[5]={0x56,0x00,0x54,0x01,0xFF};
spriteitf_command _spriteitf_varCmd_SetImageSize(spriteitf_imagesize_t PictureSize_){
	spriteitf_command Ans;
	_spriteitf_varCmd_SET_IMAGE_SIZE[4]=PictureSize_;
	Ans.Ptr=_spriteitf_varCmd_SET_IMAGE_SIZE;
	Ans.Size=5;
	Ans.InSize=5;
	return Ans;
}
//ボーレートの指定(定義した５通り)
static unsigned char _spriteitf_varCmd_CHANGE_BAUD_RATE[7]={0x56,0x00,0x24,0x03,0x01,0x00,0x00};
spriteitf_command _spriteitf_varCmd_ChangeBaudRate(spriteitf_baudrate_t BaudRate_){
	spriteitf_command Ans;
	_spriteitf_varCmd_CHANGE_BAUD_RATE[5]= (char)(BaudRate_/256);
	_spriteitf_varCmd_CHANGE_BAUD_RATE[6]= (char)(BaudRate_%256);
	Ans.Ptr=_spriteitf_varCmd_CHANGE_BAUD_RATE;
	Ans.Size=7;
	Ans.InSize=5;
	return Ans;
}

//===error エラー処理===
//errorが生じた位置の固有番号(defineを使用）
spriteitf_error_t x_spriteitf_error_ErrCode=0;
//catch
spriteitf_vFp_v x_spriteitf_error_Fp_catch=0;
//error機能初期化
void _spriteitf_error_initialize(void){
	x_spriteitf_error_ErrCode=0;
	x_spriteitf_error_Fp_catch=0;
}
//error機能の終端化
void _spriteitf_error_finalize(void){
	x_spriteitf_error_ErrCode=0;
	x_spriteitf_error_Fp_catch=0;
}
//エラーを発報
void _spriteitf_throw(spriteitf_error_t ErrCode_){
	//エラーを記録
	x_spriteitf_error_ErrCode=ErrCode_;
	//キャッチを実行
	if(x_spriteitf_error_Fp_catch)(*x_spriteitf_error_Fp_catch)();
}
//Error状態か確認
boolian spriteitf_isError(void){return x_spriteitf_error_ErrCode!=0;}
//Errorフラグ確認
spriteitf_error_t spriteitf_getErrorCode(void){return x_spriteitf_error_ErrCode;}
//エラーをリセット
void spriteitf_clearErrorCode(void){x_spriteitf_error_ErrCode=0;}
//エラーcatch関数を登録
void _spritetif_set_catch(spriteitf_vFp_v Fp_catch_){x_spriteitf_error_Fp_catch=Fp_catch_;}


//===timer タイマー割込み===//
/***注意：timer内部の処理は未実装*/
//タイムアウト時呼出し関数ポインタ
spriteitf_vFp_v x_spriteitf_timer_fnTimeOut;
//timer機能の初期化
void _spriteitf_timer_initialize(void){
	x_spriteitf_timer_fnTimeOut=0;
	_spriteitf_timer_interrupt_initialize(spriteitf_RecvTimeOut);
	_spriteitf_timer_clear();
	_spriteitf_timer_interrupt_disable();
//	_spriteitf_timer_interrupt_enable();
}
//timer機能の終端化
void _spriteitf_timer_finalize(void){
	x_spriteitf_timer_fnTimeOut=0;
	_spriteitf_timer_interrupt_disable();
}
//TimeOut機能が利用可能か確認
boolian _spriteitf_timer_can_start(void){return x_spriteitf_timer_fnTimeOut==0;}
//TimeOut機能を有効化
void _spriteitf_timer_start(spriteitf_vFp_v fnTimeOut_){
	if(!_spriteitf_timer_can_start())return;
	x_spriteitf_timer_fnTimeOut=fnTimeOut_;
	_spriteitf_timer_clear();
	_spriteitf_timer_interrupt_clear();
	_spriteitf_timer_interrupt_enable();
}
//前回と同じ内容でタイマーを動かす(stopせずに再スタート)
void _spriteitf_timer_restart(void){
	_spriteitf_timer_clear();
	_spriteitf_timer_interrupt_clear();
	_spriteitf_timer_interrupt_enable();
}
//TimeOut機能を無効化
void _spriteitf_timer_stop(void){
	x_spriteitf_timer_fnTimeOut=0;
	_spriteitf_timer_interrupt_disable();
}
//タイマー割り込み関数//spriteitf_RecvTimeOut ms に一回
void _spriteitf_timer_interrupt(void){
	//割り込みフラグ処理
	_spriteitf_timer_interrupt_clear();
	//受信時タイムアウト時呼出し関数を実行
	if(x_spriteitf_timer_fnTimeOut){
		(*x_spriteitf_timer_fnTimeOut)();
	}else _spriteitf_timer_stop();

}


//===uart送受信割込み型===//
//コマンド送信状況　待機:0 送信:1 受信:2
uint8 x_spriteitf_uart_Mode=0;
//現在の担当コマンド
spriteitf_command x_spriteitf_uart_Cmd;
//コマンドカウンター
spriteitf_dsize_t x_spriteitf_uart_Cnt=0;
//受信完了時呼び出し関数 引数はACK
spriteitf_vFp_v x_spriteitf_uart_fnFinRecv=0;
//受信時呼び出し関数　引数はdataとposition
spriteitf_bFp_uc_dsize x_spriteitf_uart_fnRecv=0;
//x_spriteitf_uart_fnTimeOut_Recv()で実行されるエラー処理関数、引数は通信時のコマンドポインタ
spriteitf_vFp_ucp x_spriteitf_uart_fnTimeOutErr=0;
//uart無効化
#define _spriteitf_uart_disable() x_spriteitf_uart_disable()
//uart関連の変数初期化
void _spriteitf_uart_iniVal(void){
	x_spriteitf_uart_Mode=0;
	x_spriteitf_uart_Cnt=0;
	x_spriteitf_uart_Cmd.Ptr=0;
	x_spriteitf_uart_Cnt=0;
	x_spriteitf_uart_fnFinRecv=0;
	x_spriteitf_uart_fnRecv=0;
	x_spriteitf_uart_fnTimeOutErr=0;
}
#define _spriteitf_uart_finVal() _spriteitf_uart_iniVal()
//送受信割り込み初期化
void _spriteitf_uart_initialize(void){
	//各変数初期化
	_spriteitf_uart_iniVal();
	//uart module初期化
	x_spriteitf_uart_initialize();
	//割り込み予約
	_spriteitf_uart_fput_interrupt_clear();
	_spriteitf_uart_fput_interrupt_disable();
	_spriteitf_uart_fget_interrupt_clear();
	_spriteitf_uart_fget_interrupt_disable();
}
//送受信割り込み終端化
void _spriteitf_uart_finalize(void){
	//uart割り込み関連をクリア
	_spriteitf_uart_fput_interrupt_disable();
	_spriteitf_uart_fget_interrupt_disable();
	_spriteitf_uart_fput_interrupt_clear();
	_spriteitf_uart_fget_interrupt_clear();
	//uart無効化
	x_spriteitf_uart_disable();
	//変数終端化
	_spriteitf_uart_finVal();
}
//割込み型コマンドを送信可能かの確認
boolian _spriteitf_uart_can_sendCommand(void){return x_spriteitf_uart_Mode==0;}
//割込み型コマンド送信
void _spriteitf_uart_sendCommand(const spriteitf_command Cmd_,spriteitf_vFp_v fnFinRecv_,spriteitf_bFp_uc_dsize fnRecv_){
	//コマンドが送信可能か確認
	if(!_spriteitf_uart_can_sendCommand())return;

	//受信時関数
	x_spriteitf_uart_Mode=1;
	x_spriteitf_uart_Cmd=Cmd_;
	x_spriteitf_uart_Cnt=0;
	x_spriteitf_uart_fnFinRecv=fnFinRecv_;
	x_spriteitf_uart_fnRecv=fnRecv_;

	//送信割り込み許可
	_spriteitf_uart_fput_interrupt_set();
	_spriteitf_uart_fput_interrupt_enable();
}
//タイムアウト時実行関数
void x_spriteitf_uart_fnTimeOut_Recv(void){
	_spriteitf_timer_stop();
	//割り込み中止
	_spriteitf_uart_fput_interrupt_disable();
	_spriteitf_uart_fget_interrupt_disable();
	//エラー処理
	if(x_spriteitf_uart_fnTimeOutErr)x_spriteitf_uart_fnTimeOutErr(x_spriteitf_uart_Cmd.Ptr);
	//uart用各種変数をクリア
	x_spriteitf_uart_Mode=0;
	x_spriteitf_uart_Cmd.Ptr=0;
	x_spriteitf_uart_Cnt=0;

}
//送信用割り込み関数
void _spriteitf_uart_fput_interrupt(void){
	//割り込みフラグクリア
	_spriteitf_uart_fput_interrupt_clear();

	//送信処理
	_spriteitf_uart_fputc(x_spriteitf_uart_Cmd.Ptr[x_spriteitf_uart_Cnt++]);
	//送信終了処理
	if(x_spriteitf_uart_Cnt >= x_spriteitf_uart_Cmd.Size){
		//LED点灯
		_spriteitf_led_set(1);
		//カウンターを0に戻す
		x_spriteitf_uart_Cnt=0;
		//割込みモードを受信に変更
		x_spriteitf_uart_Mode=2;
		//送信割り込み禁止
		_spriteitf_uart_fput_interrupt_disable();
		//タイマー作動可能か？（PowerResetが使ってないか？）
		if(_spriteitf_timer_can_start()){
			//タイムアウト監視開始
			_spriteitf_timer_start(x_spriteitf_uart_fnTimeOut_Recv);
		}
		//受信割り込み許可
		_spriteitf_uart_fget_interrupt_enable();
	}
}
//受信用割り込み関数
void _spriteitf_uart_fget_interrupt(void){
	static unsigned char c;

	//割り込みフラグクリア
	_spriteitf_uart_fget_interrupt_clear();

	//1文字受信
	c=_spriteitf_uart_fgetc();
	++x_spriteitf_uart_Cnt; 

	//正しく受信コマンド先頭を読んでいなければ、読み捨て
	if(x_spriteitf_uart_Cnt==1 && c!=0x76){
		//カウント0にして読んだ値は無視する
		x_spriteitf_uart_Cnt = 0;
		//タイマーリスタート
		_spriteitf_timer_restart();
		return;
	}

	//受信時呼出し関数
	if(x_spriteitf_uart_fnRecv){
		//終了が要求が行われた場合、Cntを直ちにMaxに変更
		if((*x_spriteitf_uart_fnRecv)(c,x_spriteitf_uart_Cnt-1))x_spriteitf_uart_Cnt=x_spriteitf_uart_Cmd.InSize;
	}

	//受信終了処理
	if(x_spriteitf_uart_Cnt >= x_spriteitf_uart_Cmd.InSize){
		//LED消灯
		_spriteitf_led_set(0);
		//割込みモードを通常に変更
		x_spriteitf_uart_Mode=0;
		//カウンターを0に戻す
		x_spriteitf_uart_Cnt=0;
		//受信割り込み禁止
		_spriteitf_uart_fget_interrupt_disable();
		//タイマーストップ
		_spriteitf_timer_stop();
		//受信終了時呼出し関数
		if(x_spriteitf_uart_fnFinRecv)(*x_spriteitf_uart_fnFinRecv)();
		//ここでぬける。タイムアウト監視をONしない
		return;
	}
	//タイマー再開
	_spriteitf_timer_restart();
}
//受信タイムアウト時のエラー処理関数設定
void _spriteitf_uart_setErrFunc(spriteitf_vFp_ucp fnTimeOutErr){
	x_spriteitf_uart_fnTimeOutErr=fnTimeOutErr;
}

//===通常関数群===//
//状態
spriteitf_status_t _spriteitf_Status=spriteitf_Status_Idle;
//カメラの画像サイズ
spriteitf_imagesize_t _spriteitf_ImageSize=0;
//getDataSize完了後上がる(ユーザーが下す)
boolian _spriteitf_TakePicFlag=hmLib_false;
//getData or stopPic完了後に挙がる(ユーザーが下す)
boolian _spriteitf_readPicDatFlag=hmLib_false;
//通信タイムアウト時自動パワーリセット
boolian _spriteitf_AutoResetFlag=hmLib_false;
//写真のデータサイズ
spriteitf_dsize_t _spriteitf_PicDataSize=0;
//写真データの受信バッファへのポインタ：ユーザーがbeginReadPicDatで教えてくれる
unsigned char* _spriteitf_BufPtr=0;
//ユーザー指定の受信データ数＝ユーザーがくれるバッファの大きさ
spriteitf_dsize_t _spriteitf_BufSize=0;
//ユーザーがくれるバッファに書き込んだデータ数
spriteitf_dsize_t* _spriteitf_BufWriteNumPtr=0;
//写真基礎データ受信完了時に呼ばれるユーザー定義関数
spriteitf_vFp_v _spriteitf_fnInformTakePic=0;
//写真データ受信完了時に呼ばれるユーザー定義関数
spriteitf_vFp_v _spriteitf_fnInformReadPicDat=0;
//写真撮影終了処理完了時に呼ばれるユーザー定義関数
spriteitf_vFp_v _spriteitf_fnInformEndTakePic=0;
//エラー処理関数
void _spriteitf_fnCatchErr(void){
	//エラーコードをもらう
	spriteitf_error_t ErrCode=spriteitf_getErrorCode();
	//通信タイムアウト系のエラーの時、ステータスをIdleにもどす
	if((ErrCode & spriteitf_error_LowBitMask) == spriteitf_error_TimeOut)_spriteitf_Status=spriteitf_Status_Idle;
}
//状態変数初期化
void _spriteitf_iniStatusVal(void){
	_spriteitf_Status=spriteitf_Status_Idle;
	_spriteitf_ImageSize=0;
	_spriteitf_TakePicFlag=hmLib_false;
	_spriteitf_readPicDatFlag=hmLib_false;
	_spriteitf_AutoResetFlag=hmLib_false;
	_spriteitf_PicDataSize=0;
	_spriteitf_BufPtr=0;
	_spriteitf_BufSize=0;
}
//状態変数終端化
#define _spriteitf_finStatusVal() _spriteitf_iniStatusVal()
//ステータス取得関数
spriteitf_status_t spriteitf_getStatus(void){return _spriteitf_Status;}
//状態が待機状態中か？
boolian spriteitf_isIdle(void){return spriteitf_getStatus() == spriteitf_Status_Idle;}
//Request中のImageSizeを取得する関数
spriteitf_imagesize_t spriteitf_getRequestImageSize(void){return _spriteitf_ImageSize;}
//写真全データサイズを要求可能か？
boolian spriteitf_can_getPicInfo(void){return _spriteitf_TakePicFlag;}
//写真全データサイズ要求
spriteitf_dsize_t spriteitf_getAllPicDataSize(void){return _spriteitf_PicDataSize;}
//写真データ用バッファにデータはすべて溜まっているか？
boolian spriteitf_can_getPicData(void){return _spriteitf_readPicDatFlag;}
//写真データ用バッファサイズ要求
spriteitf_dsize_t spriteitf_sizePicData(void){return _spriteitf_BufSize;}
//写真基礎データ受信完了フラグをクリア
void spriteitf_clear_TakePicFlag(void){_spriteitf_TakePicFlag=hmLib_false;}
//データ受信完了フラグをクリア
void spriteitf_clear_ReadPicDatFlag(void){_spriteitf_readPicDatFlag=hmLib_false;}
//写真撮っていいか？
boolian spriteitf_can_TakePic(void){return (!_spriteitf_TakePicFlag && !_spriteitf_readPicDatFlag && spriteitf_isIdle());}
//受信タイムアウト時のエラー関数
void _spriteitf_fnRecvTimeOutErr(unsigned char* ptr){
	_spriteitf_uart_fget_interrupt_disable();
	//エラーを投げる
	if(ptr==_spriteitf_varCmd_SET_IMAGE_SIZE){
		_spriteitf_ImageSize=0;
		_spriteitf_throw(spriteitf_error_TimeOut_SetImageSize);
	}else if(ptr==_spriteitf_Cmd_TAKE_PICTURE){
		_spriteitf_throw(spriteitf_error_TimeOut_TakePic);
	}else if(ptr==_spriteitf_Cmd_GET_DATA_SIZE){
		_spriteitf_throw(spriteitf_error_TimeOut_GetDataSize);
	}else if(ptr==_spriteitf_varCmd_GET_DATA){
		_spriteitf_throw(spriteitf_error_TimeOut_GetData);
		//データ取得用変数終端化
		_spriteitf_BufWriteNumPtr=0;
		_spriteitf_BufPtr=0;
		_spriteitf_BufSize=0;
	}else if(ptr==_spriteitf_Cmd_STOP_TAKE_PICTURE){
		_spriteitf_throw(spriteitf_error_TimeOut_StopTakePic);
	}else if(ptr==_spriteitf_Cmd_RESET_CMR){
		_spriteitf_throw(spriteitf_error_TimeOut_CommandReset);
	}else _spriteitf_throw(spriteitf_error_TimeOut_unUsedCommand);

	if(_spriteitf_AutoResetFlag)spriteitf_PowerReset();

}

//--------------------写真基礎情報読み込み関数たち------------------
//GetDataSize時処理関数
boolian _spriteitf_fnRecv_getPicInfo(unsigned char c,spriteitf_dsize_t pos){
	//DataSize_high受信
	if(pos == 7){
		_spriteitf_PicDataSize=(((spriteitf_dsize_t)c)<<8)&0xFF00;
	}//DataSize_low受信
	else if(pos == 8){
		//DataSize初期化完了
		_spriteitf_PicDataSize+=((spriteitf_dsize_t)c)&0x00FF;
	}
	return 0;	
}
//写真基礎データ受信処理駆動関数
void _spriteitf_fnFinTakePicCommand(void){
	//カメラへコマンド送信できない時エラーを投げる
	if(!_spriteitf_uart_can_sendCommand()){
		switch (_spriteitf_Status)
		{
		case spriteitf_Status_SetImageSize:
			return _spriteitf_throw(spriteitf_error_TakePic_cannotsend_ActiveImageSize);
		case spriteitf_Status_ActiveImageSize:
			return _spriteitf_throw(spriteitf_error_TakePic_cannotsend_TakePic);
		case spriteitf_Status_TakePic:
			return _spriteitf_throw(spriteitf_error_TakePic_cannotsend_ReadPicInfo);
		default:
			return;
		}
	}

	//ステータス遷移＋コマンド送信
	switch (_spriteitf_Status)
	{
	case spriteitf_Status_SetImageSize:
		_spriteitf_Status=spriteitf_Status_TakePic;
		return _spriteitf_uart_sendCommand(_spriteitf_Cmd_TakePicture,_spriteitf_fnFinTakePicCommand,0);
	case spriteitf_Status_TakePic:
		_spriteitf_Status=spriteitf_Status_ReadPicInfo;
		return _spriteitf_uart_sendCommand(_spriteitf_Cmd_GetDataSize,_spriteitf_fnFinTakePicCommand,_spriteitf_fnRecv_getPicInfo);
	case spriteitf_Status_ReadPicInfo:
		_spriteitf_Status=spriteitf_Status_Idle;
		_spriteitf_TakePicFlag=hmLib_true;
		if(_spriteitf_fnInformTakePic!=0)(*_spriteitf_fnInformTakePic)();
//		_spriteitf_fnInformTakePic=0;
		return;
	default:
		//想定外のステータス時、エラーを投げる
		if(_spriteitf_Status==spriteitf_Status_Idle)_spriteitf_throw(spriteitf_error_TakePic_invalidStatus_Idle);
		else if(_spriteitf_Status==spriteitf_Status_ReadPicData)_spriteitf_throw(spriteitf_error_TakePic_invalidStatus_ReadPicData);
		else if(_spriteitf_Status==spriteitf_Status_EndTakePic)_spriteitf_throw(spriteitf_error_TakePic_invalidStatus_EndTakePic);
		else if(_spriteitf_Status==spriteitf_Status_CommandReset)_spriteitf_throw(spriteitf_error_TakePic_invalidStatus_CommandReset);
//		else if(_spriteitf_Status==spriteitf_Status_PowerReset)_spriteitf_throw(spriteitf_error_TakePic_invalidStatus_PowerReset);
		return;
	}
}
//写真撮影開始要求
boolian spriteitf_beginTakePic(spriteitf_imagesize_t ImageSize_,spriteitf_vFp_v fnInformTakePic){
	//写真撮影開始していけない時はスルー
	if(_spriteitf_TakePicFlag)return hmLib_true;
	if(_spriteitf_Status!=spriteitf_Status_Idle)return hmLib_true;
	if(!_spriteitf_uart_can_sendCommand())return hmLib_true;

	_spriteitf_fnInformTakePic=fnInformTakePic;
	//ImageSize_が異常な場合には160_120に設定
	if(ImageSize_ != spriteitf_PictureSize_160_120 && ImageSize_!=spriteitf_PictureSize_320_240 && ImageSize_!=spriteitf_PictureSize_640_480){
		ImageSize_ = spriteitf_PictureSize_160_120;
	}

	//要求撮影サイズが同じ時
	if(ImageSize_==_spriteitf_ImageSize){
		_spriteitf_Status=spriteitf_Status_TakePic;
		//カメラに撮影要求
		_spriteitf_uart_sendCommand(_spriteitf_Cmd_TakePicture,_spriteitf_fnFinTakePicCommand,0);
	}//撮影要求サイズが前回と異なるとき
	else{
		_spriteitf_Status=spriteitf_Status_SetImageSize;
		_spriteitf_ImageSize=ImageSize_;
		//カメラに写真サイズ変更要求
		_spriteitf_uart_sendCommand(_spriteitf_varCmd_SetImageSize(_spriteitf_ImageSize),_spriteitf_fnFinTakePicCommand,0);

	}
	return hmLib_false;
}

//--------------------写真データ読み込み関数たち--------------------
#define _spriteitf_GetPicCmdHeaderSize 5
#define _spriteitf_JPEGTrmnSize 2
static const unsigned char _spriteitf_JPEGTrmn[_spriteitf_JPEGTrmnSize]={0xFF,0xD9};
boolian _spriteitf_fnRecv_getPicData(unsigned char c,spriteitf_dsize_t cnt){
	if(cnt>=(_spriteitf_GetPicCmdHeaderSize) && cnt<(_spriteitf_GetPicCmdHeaderSize+_spriteitf_BufSize)){
		//データを受信
		_spriteitf_BufPtr[cnt-(_spriteitf_GetPicCmdHeaderSize)]=c;
		if(_spriteitf_BufWriteNumPtr)(*_spriteitf_BufWriteNumPtr)+=1;
/*
		//念のためデータ数がおかしくないか確認
		if(cnt>_spriteitf_GetPicCmdHeaderSize+_spriteitf_JPEGTrmnSize && _spriteitf_BufPtr[cnt-(_spriteitf_GetPicCmdHeaderSize+1)-1]==_spriteitf_JPEGTrmn[0] && _spriteitf_BufPtr[cnt-(_spriteitf_GetPicCmdHeaderSize+1)]==_spriteitf_JPEGTrmn[1]){
		}
*/
	}
	return hmLib_false;
}
//写真データ読み込み駆動関数
void _spriteitf_fnFinReadPicDat(void){
	switch (_spriteitf_Status)
	{
	case spriteitf_Status_ReadPicData:
		//Idle状態に戻る
		_spriteitf_Status=spriteitf_Status_Idle;
		//読み込み可能フラグをあげる
		_spriteitf_readPicDatFlag=hmLib_true;
		//ReadTakePic完了時に呼び出す関数がユーザーから与えられていれば実行
		if(_spriteitf_fnInformReadPicDat!=0)(*_spriteitf_fnInformReadPicDat)();
		//変数初期化
//		_spriteitf_fnInformReadPicDat=0;
//		_spriteitf_BufPtr=0;
//		_spriteitf_BufSize=0;
//		_spriteitf_BufWriteNumPtr=0;
		return;
	default:
		//想定外のステータス時、エラーを投げる
		if(_spriteitf_Status==spriteitf_Status_Idle)_spriteitf_throw(spriteitf_error_ReadPicData_invalidStatus_Idle);
		else if(_spriteitf_Status==spriteitf_Status_SetImageSize)_spriteitf_throw(spriteitf_error_ReadPicData_invalidStatus_SetImageSize);
		else if(_spriteitf_Status==spriteitf_Status_ActiveImageSize)_spriteitf_throw(spriteitf_error_ReadPicData_invalidStatus_ActiveImageSize);
		else if(_spriteitf_Status==spriteitf_Status_TakePic)_spriteitf_throw(spriteitf_error_ReadPicData_invalidStatus_TakePic);
		else if(_spriteitf_Status==spriteitf_Status_ReadPicInfo)_spriteitf_throw(spriteitf_error_ReadPicData_invalidStatus_ReadPicInfo);
		else if(_spriteitf_Status==spriteitf_Status_EndTakePic)_spriteitf_throw(spriteitf_error_ReadPicData_invalidStatus_EndTakePic);
		else if(_spriteitf_Status==spriteitf_Status_CommandReset)_spriteitf_throw(spriteitf_error_ReadPicData_invalidStatus_CommandReset);
//		else if(_spriteitf_Status==spriteitf_Status_PowerReset)_spriteitf_throw(spriteitf_error_ReadPicData_invalidStatus_PowerReset);
		return;
	}
}
//写真データ読み込み要求	BufPtr:データ入れてほしい場所,begin:写真データの読み出し開始位置（8の倍数）,size:beginから読み出すデータ数
boolian spriteitf_beginReadPicDat(unsigned char* BufPtr_,spriteitf_dsize_t begin_,spriteitf_dsize_t* size_ptr_,spriteitf_dsize_t size_,spriteitf_vFp_v fnInformReadPicData){
	//読み込み要求していけない時はスルー
	if(!_spriteitf_TakePicFlag || _spriteitf_readPicDatFlag)return hmLib_true;
	if(_spriteitf_Status!=spriteitf_Status_Idle)return hmLib_true;
	if(!_spriteitf_uart_can_sendCommand())return hmLib_true;
	//読み出し位置が8の倍数でないときは拒否
	if(begin_%8 !=0)return hmLib_true;

	_spriteitf_fnInformReadPicDat=fnInformReadPicData;
	//受信バッファとサイズをもらう
	_spriteitf_BufPtr=BufPtr_;
	_spriteitf_BufSize=size_;
	_spriteitf_BufWriteNumPtr=size_ptr_;
	if(_spriteitf_BufPtr==0)return hmLib_true;
	_spriteitf_Status=spriteitf_Status_ReadPicData;
	//カメラに写真データを要求
	_spriteitf_uart_sendCommand(_spriteitf_varCmd_GetData(begin_,size_),_spriteitf_fnFinReadPicDat,_spriteitf_fnRecv_getPicData);
	return hmLib_false;
}
//写真読み込み済みデータ数取得
uint16 spriteitf_getReadDataNum(void){
	if(_spriteitf_BufWriteNumPtr)return (*_spriteitf_BufWriteNumPtr);
	else return 0;
}

//----------------------撮影終了関数たち----------------------------
//撮影終了処理駆動関数
void _spriteitf_fnFinEndTakePic(void){
	switch (_spriteitf_Status)
	{
	case spriteitf_Status_EndTakePic:
		//Idle状態に戻る
		_spriteitf_Status=spriteitf_Status_Idle;
		//Take Pic系のフラグを全てクリア
		_spriteitf_TakePicFlag=hmLib_false;
		_spriteitf_readPicDatFlag=hmLib_false;
		//EndTakePic完了時に呼び出す関数がユーザーから与えられていれば実行
		if(_spriteitf_fnInformEndTakePic!=0)(*_spriteitf_fnInformEndTakePic)();
		//変数初期化
//		_spriteitf_fnInformEndTakePic=0;
		return;
	default:
		//想定外のステータス時、エラーを投げる
		if(_spriteitf_Status==spriteitf_Status_Idle)_spriteitf_throw(spriteitf_error_EndTakePic_invalidStatus_Idle);
		else if(_spriteitf_Status==spriteitf_Status_SetImageSize)_spriteitf_throw(spriteitf_error_EndTakePic_invalidStatus_SetImageSize);
		else if(_spriteitf_Status==spriteitf_Status_ActiveImageSize)_spriteitf_throw(spriteitf_error_EndTakePic_invalidStatus_ActiveImageSize);
		else if(_spriteitf_Status==spriteitf_Status_TakePic)_spriteitf_throw(spriteitf_error_EndTakePic_invalidStatus_TakePic);
		else if(_spriteitf_Status==spriteitf_Status_ReadPicInfo)_spriteitf_throw(spriteitf_error_EndTakePic_invalidStatus_ReadPicInfo);
		else if(_spriteitf_Status==spriteitf_Status_ReadPicData)_spriteitf_throw(spriteitf_error_EndTakePic_invalidStatus_ReadPicData);
		else if(_spriteitf_Status==spriteitf_Status_CommandReset)_spriteitf_throw(spriteitf_error_EndTakePic_invalidStatus_CommandReset);
//		else if(_spriteitf_Status==spriteitf_Status_PowerReset)_spriteitf_throw(spriteitf_error_EndTakePic_invalidStatus_PowerReset);
		return;
	}
}
//撮影終了要求
boolian spriteitf_endTakePic(spriteitf_vFp_v fnInformEndTakePic){
	//終了要求していけない時はスルー
	if(!_spriteitf_TakePicFlag || _spriteitf_readPicDatFlag)return hmLib_true;
	if(_spriteitf_Status!=spriteitf_Status_Idle)return hmLib_true;
	if(!_spriteitf_uart_can_sendCommand())return hmLib_true;
	
	_spriteitf_fnInformEndTakePic=fnInformEndTakePic;
	_spriteitf_Status=spriteitf_Status_EndTakePic;
	_spriteitf_uart_sendCommand(_spriteitf_Cmd_StopTakePicture,_spriteitf_fnFinEndTakePic,0);
	return hmLib_false;
}

//------------------------コマンドリセット関数たち---------------------
//コマンドリセット駆動関数
void _spriteitf_fnFin_CommandReset(void){
	switch (_spriteitf_Status)
	{
	case spriteitf_Status_CommandReset:
		//フラグをクリア
		_spriteitf_TakePicFlag=hmLib_false;
		_spriteitf_readPicDatFlag=hmLib_false;
		_spriteitf_Status=spriteitf_Status_Idle;
		return;
	default:
		//想定外のステータス時、エラーを投げる
		if(_spriteitf_Status==spriteitf_Status_Idle)_spriteitf_throw(spriteitf_error_CommandReset_invalidStatus_Idle);
		else if(_spriteitf_Status==spriteitf_Status_SetImageSize)_spriteitf_throw(spriteitf_error_CommandReset_invalidStatus_SetImageSize);
		else if(_spriteitf_Status==spriteitf_Status_ActiveImageSize)_spriteitf_throw(spriteitf_error_CommandReset_invalidStatus_ActiveImageSize);
		else if(_spriteitf_Status==spriteitf_Status_TakePic)_spriteitf_throw(spriteitf_error_CommandReset_invalidStatus_TakePic);
		else if(_spriteitf_Status==spriteitf_Status_ReadPicInfo)_spriteitf_throw(spriteitf_error_CommandReset_invalidStatus_ReadPicInfo);
		else if(_spriteitf_Status==spriteitf_Status_ReadPicData)_spriteitf_throw(spriteitf_error_CommandReset_invalidStatus_ReadPicData);
		else if(_spriteitf_Status==spriteitf_Status_EndTakePic)_spriteitf_throw(spriteitf_error_CommandReset_invalidStatus_EndTakePic);
		else if(_spriteitf_Status==spriteitf_Status_PowerReset)_spriteitf_throw(spriteitf_error_CommandReset_invalidStatus_PowerReset);
		return;
	}
}
//コマンドリセット関数
boolian spriteitf_CommandReset(void){
	//コマンドリセットしていけない時はスルー
	if(!_spriteitf_uart_can_sendCommand())return hmLib_true;
	if(_spriteitf_Status!=spriteitf_Status_Idle)return hmLib_true;

	_spriteitf_Status=spriteitf_Status_CommandReset;
	_spriteitf_uart_sendCommand(_spriteitf_Cmd_ResetCmr,_spriteitf_fnFin_CommandReset,0);

	return hmLib_false;	
}

//-------------------------強制リセット関数たち-------------------------
//パワーリセット駆動関数
void _spriteitf_fnPowerReset(void){
	static unsigned char count = 0;
	static unsigned char power = 0;
	boolian tmpAutoPwResetFlag=0;
	//カウンタを進める
	++count;

	//5秒以上待機した場合は逃げる
	if(count > (unsigned char)(5000/spriteitf_RecvTimeOut + 1)){
		count = 0;		//強制リセット終了
		power = 0;
		//ここでパワーリセット完了、Idleに戻る
		_spriteitf_Status=spriteitf_Status_Idle;
		_spriteitf_timer_stop();
		return;
	}//待機が3秒以上で電源が入っていない場合
	else if(power==0 && count > (unsigned char)(3000/spriteitf_RecvTimeOut + 1)){
		//終端化でフラグの状態を忘れてしまうので一時退避
		tmpAutoPwResetFlag=_spriteitf_AutoResetFlag;
		//spriteitf初期化
		spriteitf_initialize();
		//退避させていたフラグ回収
		_spriteitf_AutoResetFlag=tmpAutoPwResetFlag;
		//uart再接続(再接続と同時にカメラが何か送ってくるので，タイマーストップは最後にしないと，受信割り込みが起こって受信割り込み関数内のタイマーリセットに消されてリセットから復帰できなくなる)
		power=1;						//電源を入れた証
	}
	_spriteitf_timer_stop();
	_spriteitf_timer_start(_spriteitf_fnPowerReset);
//	_spriteitf_timer_restart();//再接続と同時にカメラが何か送ってくるので，タイマーストップは最後にしないと，受信割り込みが起こって受信割り込み関数内のタイマーリセットに消されてリセットから復帰できなくなる
	return;
}
//パワーリセット関数
boolian spriteitf_PowerReset(void){
	//終端化でフラグの状態を忘れてしまうので一時退避
	boolian tmpAutoPwResetFlag=_spriteitf_AutoResetFlag;
	//spriteitf終端化
	spriteitf_finalize();
	//退避させていたフラグ回収
	_spriteitf_AutoResetFlag=tmpAutoPwResetFlag;
	//Status遷移
	_spriteitf_Status=spriteitf_Status_PowerReset;
	//タイマー設定
	_spriteitf_timer_start(_spriteitf_fnPowerReset);
	return hmLib_false;
}
//オートパワーリセット機能のセット
void spriteitf_setAutoResetMode(boolian val){_spriteitf_AutoResetFlag=val;}
//オートパワーリセット機能の状態確認
boolian spriteitf_getAutoResetMode(void){return _spriteitf_AutoResetFlag;}

//------------------------初期化・終端化関数--------------------------
//初期化
void spriteitf_initialize(void){
	//カメラモジュール電源ON
	_spriteitf_power(1);
	//uart初期化
	_spriteitf_uart_initialize();
	//timer初期化
	_spriteitf_timer_initialize();
	//error初期化
	_spriteitf_error_initialize();
	//変数初期化
	_spriteitf_iniStatusVal();
	_spriteitf_fnInformTakePic=0;
	_spriteitf_fnInformReadPicDat=0;
	_spriteitf_fnInformEndTakePic=0;
	//エラー処理登録
	_spritetif_set_catch(_spriteitf_fnCatchErr);
	_spriteitf_uart_setErrFunc(_spriteitf_fnRecvTimeOutErr);
}
//終端化
void spriteitf_finalize(void){
	//変数終端化
	_spriteitf_finStatusVal();
	_spriteitf_fnInformTakePic=0;
	_spriteitf_fnInformReadPicDat=0;
	_spriteitf_fnInformEndTakePic=0;
	//エラー処理終端化
	_spritetif_set_catch(0);
	_spriteitf_uart_setErrFunc(0);
	//error終端化
	_spriteitf_error_finalize();
	//timer終端化
	_spriteitf_timer_finalize();
	//uart終端化
	_spriteitf_uart_finalize();
	//カメラモジュール電源OFF
	_spriteitf_power(0);
}

#endif
