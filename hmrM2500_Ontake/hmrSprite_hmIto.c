#ifndef HMR_SPRITE_C_INC
#define HMR_SPRITE_C_INC 16
#
#define HMLIB_NOLIB
#include"hmrType.h"
#include<hmLib_v3_04/cqueue.h>
#ifndef HMR_SPRITE_INC
#	include"hmrSprite.h"
#endif
#ifndef HMR_HMR_SPRITEBUFFER_INC_INC
#	include "hmrSpriteBuffer.h"
#endif
#ifndef HMR_UTILITY_INC
#	include "hmrUtility.h"
#endif
/*
#ifndef HMR_DEVICECONFIG_INC
#	include"hmrDeviceConfig.h"
#endif
#ifndef HMR_SPRITEINTERFACE_INC
#	include"hmrSpriteInterface.h"
#endif
#ifndef HMR_MESSAGE_INC
#	include"hmrMessage.h"
#endif
*/
#ifdef __cplusplus
using namespace hmLib;
#endif
//写真データパケットの情報
typedef struct{
	spritebuf_id BufID;	//使用バッファID
	uint16 DataPos;		//書き込まれたデータの位置
}sprite_pac_info;
//================ Sprite制御変数 =================
//takl待ちのバッファキュー
sprite_pac_info x_sprite_TalkableBufData[spritebuf_Num+1];
cqueue _sprite_TalkableBufQue;
//自動フラッシュON/OFF
boolian sprite_AutoLight=0;
//現在のミニパケットモード
boolian sprite_MiniPacMode=0;
//ミニパケットモードのV側からの要求
boolian _sprite_reserve_MiniPacMode=0;
//ライトのON/OFF
boolian _sprite_LightData=0;
//連続撮影モードON/OFF
boolian _sprite_AutoTakePic=0;
//写真取得時の写真サイズ
uint8 _sprite_Pixel=0;
//連続撮影モード時の写真サイズ
uint8 _sprite_AutoTakePixel=0;
//今までで読み込んだ全データ数
uint16 _sprite_PicDataPos=0;
//写真の全サイズ
uint16 _sprite_PicDataSize=0;
//現在データ受信中のバッファID
spritebuf_id _sprite_BufID=0;
//現在要求中のデータのサイズ
uint16 _sprite_RequestSize=0;

//=============== SpriteInterface側の奴隷 ==================
//カメラに渡せるバッファが無い時にtrue
boolian x_sprite_itf_IsWaitBuf=hmLib_false;
boolian _sprite_itf_is_waitBuf(void){return x_sprite_itf_IsWaitBuf;}
//写真基礎データ取得終了時に呼ばれる関数、SpriteInterfaceに渡す
void _sprite_itf_fnFinGetPicInfo(void){
	//写真基礎データ取得
	_sprite_PicDataPos=0;
	_sprite_PicDataSize=spriteitf_getAllPicDataSize();

	//データを取りに行く
	_sprite_itf_readPicData();
}
//写真読み込み完了時に呼ばれる関数，SpriteInterfaceに渡す
void _sprite_itf_fnFinReadPicData(void){
	//割り込み的フラグをクリア
	spriteitf_clear_ReadPicDatFlag();
	
	//interfaceからバッファリリース
	spritebuf_release_fromSpriteInterface(_sprite_BufID);

	//エラー、強制リセット中は進まない
	if(spriteitf_isError() || spriteitf_getStatus()==spriteitf_Status_PowerReset)return;

	//取得し終わったデータを加算
	_sprite_PicDataPos+=_sprite_RequestSize;

	//全てデータを取得済みなら終了、していなければ次のデータを取りに行く
	if(_sprite_PicDataPos>=_sprite_PicDataSize)spriteitf_endTakePic(0);
	else _sprite_itf_readPicData();
}
//写真データ読み込み
boolian _sprite_itf_readPicData(){
	uint16 RequestNum=0;
	sprite_pac_info PacketInfo;
	unsigned char* BufPtr=0;
	uint16* CntPtr=0;

	//すでに撮影完了している場合はエラー
	if(_sprite_PicDataSize<=_sprite_PicDataPos)return hmLib_true;

	//要求サイズを確定
	RequestNum=(uint16)(_sprite_PicDataSize-_sprite_PicDataPos);
	if(sprite_MiniPacMode){
		if(RequestNum>spritebuf_DataSize_Small)RequestNum=(uint16)(spritebuf_DataSize_Small/8)*8;
	}else{
		if(RequestNum>spritebuf_DataSize)RequestNum=(uint16)(spritebuf_DataSize/8)*8;
	}

	_sprite_RequestSize=RequestNum;

	//バッファ確保
	_sprite_BufID=spritebuf_create(RequestNum);

	//確保に失敗した場合は、WaitBufフラグを立てておく
	if(_sprite_BufID==0){
		x_sprite_itf_IsWaitBuf=hmLib_true;
		return hmLib_true;
	}

	//talk時のためにこのパケットの情報を保持
	PacketInfo.BufID=_sprite_BufID;
	PacketInfo.DataPos=_sprite_PicDataPos;

	
	spritebuf_share_withSpriteInterface(_sprite_BufID,&BufPtr,&CntPtr);

	if(BufPtr==0 || CntPtr==0){
		spritebuf_release_fromSpriteInterface(_sprite_BufID);
		spritebuf_release(_sprite_BufID);
		return hmLib_true;
	}
	//interface側の始末
	if(spriteitf_beginReadPicDat(BufPtr,_sprite_PicDataPos,CntPtr,RequestNum,_sprite_itf_fnFinReadPicData)){
		spritebuf_release_fromSpriteInterface(_sprite_BufID);
		spritebuf_release(_sprite_BufID);
		return hmLib_true;
	}
	x_sprite_itf_IsWaitBuf=hmLib_false;

	//キューに詰める
	cqueue_push(&_sprite_TalkableBufQue,&PacketInfo);

	return hmLib_false;
}
//sprite_itf初期化関数
void _sprite_itf_initialzie(){
	sprite_power(1);
	spriteitf_initialize();
	_spriteitf_resistInformPwReset(_sprite_itf_initialzie,_sprite_itf_finalize);
}
//sprite_itf終端化関数
void _sprite_itf_finalize(){
	if(_sprite_BufID){
		spritebuf_release_fromSpriteInterface(_sprite_BufID);
		_sprite_BufID=0;
	}
	spriteitf_finalize();
	sprite_power(0);
}
//=============== Message側の奴隷 ==================
struct{
	unsigned SendPicInfo:1;				//写真情報をすでに送ったかどうか
	unsigned GetPicInfo:1;				//写真情報取得の有無
	unsigned GetPicture:1;				//写真取得モード
	unsigned SetAutoTakePic:1;			//連続撮影モード設定
	unsigned SetLightPW:1;				//ライトのON/OFFをするモード
	unsigned SetAutoLight:1;			//ライト自動ON/OFF設定モード
	unsigned SetMiniPacMode:1;			//ミニパケットモード設定モード
	unsigned SendMiniPacACK:1;			//ミニパケットモード設定ACK送信モード（カメラデータ取得中のパケットサイズ変更を避ける）
	unsigned RequestIntReset:1;			//内部リセット要求モード
	unsigned SetAutoReset:1;			//エラー時自動強制リセット設定モード
	unsigned SendError:1;				//エラー送信モード
	unsigned SendDevicePw:1;				//内部データ送信
	unsigned DevicePw:1;			//内部データ送信モード
	unsigned DevicePw_i:1;			//内部データ送信モード受理
}_sprite_Mode={0,0,0,1,1,1,0,1,0,1,0,0,0,1};
void sprite_setup_listen(void){return;}
void sprite_setup_talk(void){
	//エラー，強制リセットの時はバッファリリース
	if(spriteitf_isError()){
		_sprite_PicDataPos=0;
		_sprite_PicDataSize=0;
		spritebuf_release_fromSpriteInterface(_sprite_BufID);
	}

	//写真データ読み込み待ちがあるとき
	if(_sprite_itf_is_waitBuf() && spriteitf_isIdle()){
		_sprite_itf_readPicData();
	}

	//内部情報送信モード時
	if(_sprite_Mode.DevicePw)_sprite_Mode.SendDevicePw=hmLib_true;

	//ミニパケットモード設定（カメラデータ取得中のパケットサイズ変更を防ぐ）
	if(spriteitf_can_TakePic() && _sprite_Mode.SetMiniPacMode){
		_sprite_Mode.SetMiniPacMode=hmLib_false;
		sprite_MiniPacMode=_sprite_reserve_MiniPacMode;
		_sprite_Mode.SendMiniPacACK=hmLib_true;
	}

	//TakePicInfoが準備されているか
	if(_sprite_Mode.SendPicInfo && spriteitf_can_getPicInfo()){
		_sprite_Mode.SendPicInfo=hmLib_false;
		_sprite_Mode.GetPicInfo=hmLib_true;
	}

	//Messageに渡せるデータがあるか？
	if(!cqueue_empty(&_sprite_TalkableBufQue))_sprite_Mode.GetPicture=hmLib_true;

	//連続撮影モード時に写真撮影してもいいとき、撮影要求をする
	if(_sprite_AutoTakePic && spriteitf_can_TakePic()){
		_sprite_Mode.SendPicInfo=hmLib_true;

		//ライトを点灯する
		if(sprite_AutoLight){
			sprite_powerLight(1);
			delay_ms(200);
		}

		//SPRITEで写真撮る命令送る
		if(spriteitf_beginTakePic(_sprite_AutoTakePixel,_sprite_itf_fnFinGetPicInfo)){
			_sprite_Mode.SendPicInfo=hmLib_false;
			return;
		}
		_sprite_Pixel=_sprite_AutoTakePixel;

		//ライトを消灯する
		if(sprite_AutoLight){
			sprite_powerLight(0);
		}
	}
}
hmLib_boolian sprite_listen(cstring Str){
	static uint8 c=0;
	
	//データサイズ確認
	if(cstring_size(&Str)==0)return hmLib_true;

	c=cstring_getc(&Str,0);

	//1byte目でモードを分ける
	switch(c){
	case 0x00://写真取得モード
		if(cstring_size(&Str)!=2)return hmLib_true;
		//写真撮っていい状態で、撮影要求が来た場合
		if(spriteitf_can_TakePic()){
			_sprite_Mode.SendPicInfo=hmLib_true;
			//PictureSize設定
			if(cstring_getc(&Str,1)>=2){
				c=spriteitf_PictureSize_640_480;
			}else if(cstring_getc(&Str,1)==1){
				c=spriteitf_PictureSize_320_240;
			}else{
				c=spriteitf_PictureSize_160_120;
			}
			
			//ライトを点灯
			if(sprite_AutoLight){
				sprite_powerLight(1);
				delay_ms(200);
			}

			//SPRITEで写真撮る命令送る
			if(spriteitf_beginTakePic(c,_sprite_itf_fnFinGetPicInfo)){
				_sprite_Mode.SendPicInfo=hmLib_false;
				return hmLib_false;
			}
			//PictureSizeを取っておく
			_sprite_Pixel=c;

			//ライトを消灯
			if(sprite_AutoLight){
				sprite_powerLight(0);
			}
 		}
		return hmLib_false;
	case 0x10://連続撮影モードON
		if(cstring_size(&Str)<2)return hmLib_true;
		_sprite_Mode.SetAutoTakePic=hmLib_true;
		_sprite_AutoTakePic=1;

		//PictureSize設定
		if(cstring_getc(&Str,1)>=2){
			c=spriteitf_PictureSize_640_480;
		}else if(cstring_getc(&Str,1)==1){
			c=spriteitf_PictureSize_320_240;
		}else{
			c=spriteitf_PictureSize_160_120;
		}

		_sprite_AutoTakePixel = c;
		return hmLib_false;

	case 0x11://連続撮影モードOFF
		_sprite_Mode.SetAutoTakePic=hmLib_true;
		_sprite_AutoTakePic=0;
		return hmLib_false;
	case 0x30:	//ライトのON要求
		_sprite_Mode.SetLightPW=hmLib_true;
		//ライトON
		sprite_powerLight(1);
		//要求設定をもらう
		_sprite_LightData=1;
		return hmLib_false;
	case 0x31://ライトのOFF要求
		_sprite_Mode.SetLightPW=hmLib_true;
		//ライトOFF
		sprite_powerLight(0);
		//要求設定をもらう
		_sprite_LightData=0;
		return hmLib_false;
	case 0x40:	//自動フラッシュON設定
		_sprite_Mode.SetAutoLight=hmLib_true;
		//自動フラッシュモードON
		sprite_AutoLight=1;
		return hmLib_false;
	case 0x41:	//自動フラッシュOFF設定
		_sprite_Mode.SetAutoLight=hmLib_true;
		//ライトを消しておく
		sprite_powerLight(0);
		//自動フラッシュモードOFF
		sprite_AutoLight=0;
		return hmLib_false;
	case 0x50:	//ミニパケットモードON設定
		_sprite_Mode.SetMiniPacMode=hmLib_true;
		//ミニパケットモードON予約
		_sprite_reserve_MiniPacMode=1;
		return hmLib_false;
	case 0x51:	//ミニパケットモードOFF設定
		_sprite_Mode.SetMiniPacMode=hmLib_true;
		//ミニパケットモードOFF予約
		_sprite_reserve_MiniPacMode=0;
		return hmLib_false;
	case 0x60:	//内部リセット要求
		_sprite_Mode.RequestIntReset=hmLib_true;
		//リセットを行う
		spriteitf_CommandReset();
		return hmLib_false;
	case 0x70:	//強制リセット要求
		sprite_finalize();
		spriteitf_resistInformPwReset(_sprite_itf_finalize,_sprite_itf_initialize);
		//強制リセットをかける
		spriteitf_PowerReset();
		return hmLib_false;
	case 0x80:	//エラー時自動強制リセットON設定
		_sprite_Mode.SetAutoReset=hmLib_true;
		//AutoReset機能ON
		spriteitf_setAutoResetMode(1);
		return hmLib_false;
	case 0x81:	//エラー時自動強制リセットOFF設定
		_sprite_Mode.SetAutoReset=hmLib_true;
		//AutoReset機能OFF
		spriteitf_setAutoResetMode(0);
		return hmLib_false;
	case 0xB0:	//内部データ送信モードON
		_sprite_Mode.DevicePw=hmLib_true;
		_sprite_Mode.DevicePw_i=hmLib_true;
		_sprite_itf_initialize();
		return hmLib_false;
	case 0xB1:	//内部データ送信モードOFF
		_sprite_Mode.DevicePw=hmLib_false;
		_sprite_Mode.DevicePw_i=hmLib_true;
		_sprite_itf_finalize();
		return hmLib_false;
	default:
		return hmLib_true;
	}
}
hmLib_boolian sprite_talk(cstring* pStr){
	sprite_pac_info PacketInfo;
	msg_vFp_v FpInformDestruct;
	msg_dsizeFp_v FpSendableNum;

	//連続撮影モードのON/OFF設定
	if(_sprite_Mode.SetAutoTakePic){
		//フラグをおろす
		_sprite_Mode.SetAutoTakePic=hmLib_false;
		cstring_construct_dynamic_safe(pStr,1);
		if(_sprite_AutoTakePic)cstring_putc(pStr,0,0x10);
		else cstring_putc(pStr,0,0x11);

		return hmLib_false;
	}
	//ライトのON/OFF
	else if(_sprite_Mode.SetLightPW){
		//フラグをおろす
		_sprite_Mode.SetLightPW=hmLib_false;

		cstring_construct_dynamic_safe(pStr,1);
		if(_sprite_LightData)cstring_putc(pStr,0,0x30);
		else cstring_putc(pStr,0,0x31);
		return hmLib_false;
	}
	//フラッシュ機能ON/OFFの設定
	else if(_sprite_Mode.SetAutoLight){
		//フラグをおろす
		_sprite_Mode.SetAutoLight=hmLib_false;

		cstring_construct_dynamic_safe(pStr,1);
		if(sprite_AutoLight)cstring_putc(pStr,0,0x40);
		else cstring_putc(pStr,0,0x41);

		return hmLib_false;
	}
	//ミニパケットモードかどうかの設定
	else if(_sprite_Mode.SendMiniPacACK){
		//フラグをおろす
		_sprite_Mode.SendMiniPacACK=hmLib_false;

		cstring_construct_dynamic_safe(pStr,1);
		if(sprite_MiniPacMode)cstring_putc(pStr,0,0x50);
		else cstring_putc(pStr,0,0x51);

		return hmLib_false;
	}
	//内部リセット要求
	else if(_sprite_Mode.RequestIntReset){
		//フラグをおろす．
		_sprite_Mode.RequestIntReset=hmLib_false;

		cstring_construct_dynamic_safe(pStr,2);
		cstring_putc(pStr,0,0x60);
		cstring_putc(pStr,1, spriteitf_getStatus());

		return hmLib_false;
	}
	//エラー時自動強制リセットON・OFF設定
	else if(_sprite_Mode.SetAutoReset){
		//フラグをおろす
		_sprite_Mode.SetAutoReset=hmLib_false;

		cstring_construct_dynamic_safe(pStr,1);

		if(spriteitf_getAutoResetMode())cstring_putc(pStr,0,0x80);
		else cstring_putc(pStr,0,0x81);

		return hmLib_false;
	}else if(_sprite_Mode.DevicePw_i){
		_sprite_Mode.DevicePw_i=hmLib_false;

		cstring_construct_dynamic_safe(pStr,1);
		if(_sprite_Mode.DevicePw)cstring_putc(pStr,0,0xB0);
		else cstring_putc(pStr,0,0xB1);

		return hmLib_false;
	
	//内部情報取得
	}else if(_sprite_Mode.SendDevicePw){
		//フラグをおろす．
		_sprite_Mode.SendDevicePw=hmLib_false;

		cstring_construct_dynamic_safe(pStr,4);
		cstring_putc(pStr,0,0xA0);
		cstring_putc(pStr,1,spriteitf_getStatus());			//現在のステータス
		cstring_putc(pStr,2,spriteitf_isError());				//現在エラー状態かどうか
		cstring_putc(pStr,3,spriteitf_getErrorCode());		//エラー時のステータス

		return hmLib_false;
	}
	//写真取得モードACK返信
	else if(_sprite_Mode.GetPicInfo){
		//フラグをおろす
		_sprite_Mode.GetPicInfo=hmLib_false;

		if(spriteitf_isError()){
			sprite_powerLight(0);
			return hmLib_false;
		}

		//ACK送信
		cstring_construct_dynamic_safe(pStr,4);
		cstring_putc(pStr,0,(unsigned char)(0x00));
		cstring_putc(pStr,1,(unsigned char)(_sprite_Pixel));
		cstring_putc(pStr,2,(unsigned char)(spriteitf_getAllPicDataSize()));
		cstring_putc(pStr,3,(unsigned char)(spriteitf_getAllPicDataSize()>>8));

		return hmLib_false;
	}
	//写真撮影orデータ要求モード
	else if(_sprite_Mode.GetPicture){
		//フラグをおろす
		_sprite_Mode.GetPicture=hmLib_false;

		if(spriteitf_isError() || spriteitf_getStatus()==spriteitf_Status_PowerReset)return hmLib_false;

		//talk待ちキューから吸い出し
		PacketInfo=(*(sprite_pac_info*)cqueue_getptr(&_sprite_TalkableBufQue));
		cqueue_pop(&_sprite_TalkableBufQue);
		if(PacketInfo.BufID==0)return hmLib_true;

		//messageに渡す
		spritebuf_share_withMesasge(PacketInfo.BufID,pStr,PacketInfo.DataPos,&FpSendableNum,&FpInformDestruct);

		// 終端処理予約
		msg_talkconfig_useInformDestruct(FpInformDestruct);
		//VMCへの送信可能数通知
		msg_talkconfig_useSendable(FpSendableNum);

		//リリース
		spritebuf_release(PacketInfo.BufID);

		delay_ms(5);

		return hmLib_false;
	}
	return hmLib_true;
}
void _sprite_iniVal(void){
	_sprite_PicDataSize=0;
	sprite_AutoLight=0;
	sprite_MiniPacMode=0;
	_sprite_reserve_MiniPacMode=0;
	_sprite_LightData=0;
	_sprite_AutoTakePic=0;
	_sprite_Pixel=0;
	_sprite_AutoTakePixel=0;
	_sprite_PicDataPos=0;
	_sprite_PicDataSize=0;
	_sprite_BufID=0;
	_sprite_RequestSize=0;
	//キューコンストラクト
	cqueue_construct_static(&_sprite_TalkableBufQue,x_sprite_TalkableBufData,x_sprite_TalkableBufData+spritebuf_Num+1,sizeof(sprite_pac_info));
}
void _sprite_finVal(void){
	_sprite_PicDataSize=0;
	sprite_AutoLight=0;
	sprite_MiniPacMode=0;
	_sprite_reserve_MiniPacMode=0;
	_sprite_LightData=0;
	_sprite_AutoTakePic=0;
	_sprite_Pixel=0;
	_sprite_AutoTakePixel=0;
	_sprite_PicDataPos=0;
	_sprite_PicDataSize=0;
	_sprite_BufID=0;
	_sprite_RequestSize=0;
	//キューデストラクト
	cqueue_destruct(&_sprite_TalkableBufQue);
}
//=============== Sprite公開関数 ===================
void sprite_initialize(void){
	_sprite_itf_initialize();
	_sprite_iniVal();

	//cqueue_construct_static(&_sprite_TalkableBufQue,x_sprite_TalkableBufData,x_sprite_TalkableBufData+spritebuf_Num+2,sizeof(sprite_pac_info));
}
void sprite_finalize(void){
	sprite_pac_info PacketInfo;

	//バッファリリース
	while(!cqueue_empty(&_sprite_TalkableBufQue)){
		PacketInfo=(*(sprite_pac_info*)cqueue_getptr(&_sprite_TalkableBufQue));
		spritebuf_release(PacketInfo.BufID);
		cqueue_pop(&_sprite_TalkableBufQue);
	}

	_sprite_finVal();
	_sprite_itf_finalize();
}
sint16 sprite_task_inform(sint16 interval){
	_sprite_Mode.DevicePw_i=hmLib_true;
	return interval;
}
#
#endif
