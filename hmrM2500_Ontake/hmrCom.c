#ifndef HMR_COM_C_INC
#define HMR_COM_C_INC 205
#
#include<stdlib.h>
#define HMLIB_NOLIB
#ifndef HMLIB_CQUEUE_INC
#	include"hmLib_v3_04/cqueue.h"
#endif
#ifndef HMR_COM_INC
#	include"hmrCom.h"
#endif
#ifndef HMR_TYPE_INC
#	include"hmrType.h"
#endif
#ifndef HMR_UTILITY_INC
#	include "hmrUtility.h"
#endif
#ifndef HMR_DEVICEMANAGE_INC
#	include "hmrDeviceManage.h"
#endif
#ifdef __cplusplus
using hmLib::cqueue;
using hmLib::cqueue_iterator;
using hmLib::cqueue_size_t;
#endif
#define HMR_COM_BufqueSize 20

//**************com変数群******************//
//TRMNを絞めずにおわったため，待たなければいけないPACTRMNID数
//	finSendPacでTRMNIDが処理できなかった場合に++
//	oBuf_move_pushでTRMNIDが確認できた場合に--
uint8			_com_FailPac;
//送信待ちPAC数
//	oBuf_move_pushでFailPac用にTRMNIDが処理されなかった場合に++
//	finSendPacでTRMNIDが処理できた場合に--
uint8			_com_WaitPacNum;			//受信して未送信のPacketの数
uint8			_com_HoldPac;				//送信街のPacketの数
boolian			_com_RecvFailDat=0;			//受信中のDatの保存先を確保しなかった/できなかった場合に立ち、finRecvDatで落ちる
boolian			_com_RecvMode=0;			//受信中(Strt受信からTrmn受信までの間）
cstring			_com_RecvDatStr;
com_did_t		_com_RecvDatID=0;
com_dsize_t		_com_RecvDatCnt=0;			//受信中のDatのカウンタ
com_vFp_v		_com_FpInfromFinRecvPac=0;	//受信中のPacを全て受信完了時に呼び出す関数ポインタ
boolian			_com_SendMode=0;			//送信中(Strt送信からTrmn送信までの間）
odata			_com_SendDat;				//送信中のDat
com_dsize_t		_com_SendDatCnt=0;			//送信中のDatのカウンタ
com_vFp_v		_com_FpInfromFinSendPac=0;	//送信中のPacを全て受信完了時に呼び出す関数ポインタ
//**************com_iBuf関連***************//
//受信バッファ
cqueue _com_iBuf_Que;
idata _com_iBuf_QueBuf[HMR_COM_BufqueSize];
//受信バッファの初期化
void _com_iBuf_initialize(void){
	cqueue_construct_static(
		&_com_iBuf_Que
		,_com_iBuf_QueBuf
		,_com_iBuf_QueBuf+HMR_COM_BufqueSize
		,sizeof(idata)
	);
}
//受信バッファの終端化
void _com_iBuf_finalize(void){
	cqueue_iterator itr;

	//残っている全データを破棄
	for(itr=cqueue_begin(&_com_iBuf_Que);itr!=cqueue_end(&_com_iBuf_Que);itr=cqueue_next(&_com_iBuf_Que,itr)){
		cstring_destruct_safe(&((idata*)(itr))->Str);
	}

	//iQue,oQueを破棄
	cqueue_destruct(&_com_iBuf_Que);
}
//受信可能かを確認
boolian _com_iBuf_empty(void){return cqueue_empty(&_com_iBuf_Que);}
//受信済みデータを削除する
void _com_iBuf_move_pop(idata* pData_){
	if(_com_iBuf_empty()){
		idata_format(pData_);
		return;
	}
	idata_move((idata*)cqueue_getptr(&_com_iBuf_Que),pData_);
	cqueue_pop(&_com_iBuf_Que);
}
//iBufにデータを挿入可能か確認
boolian _com_iBuf_full(void){return cqueue_full(&_com_iBuf_Que);}
//iBufのサイズを取得する
cqueue_size_t _com_iBuf_size(void){return cqueue_size(&_com_iBuf_Que);}
//iBufにデータを挿入する
void _com_iBuf_move_push(idata* mData_){
	if(!idata_is_construct(mData_))return;

	if(_com_iBuf_full()){
		idata_destruct(mData_);
		return;
	}

	//実際上のmove
	cqueue_push(&_com_iBuf_Que,(void*)(mData_));
	idata_format(mData_);
}
//*****************com_oBuf関連**************************//
//送信バッファ
cqueue _com_oBuf_Que;
odata _com_oBuf_QueBuf[HMR_COM_BufqueSize];
//送信バッファの初期化
void _com_oBuf_initialize(void){
	cqueue_construct_static(
		&_com_oBuf_Que
		,_com_oBuf_QueBuf
		,_com_oBuf_QueBuf+HMR_COM_BufqueSize
		,sizeof(odata)
	);
}
//送信バッファの終端化
void _com_oBuf_finalize(void){
	cqueue_iterator itr;

	//残っている全データを破棄
	for(itr=cqueue_begin(&_com_oBuf_Que);itr!=cqueue_end(&_com_oBuf_Que);itr=cqueue_next(&_com_oBuf_Que,itr)){
		cstring_destruct_safe(&((odata*)(itr))->Str);
	}

	//oQueを破棄
	cqueue_destruct(&_com_oBuf_Que);
}
//送信可能かを確認
boolian _com_oBuf_empty(void){return cqueue_empty(&_com_oBuf_Que);}
//送信済みデータを削除する
void _com_oBuf_move_pop(odata* pData_){
	if(_com_oBuf_empty()){
		odata_format(pData_);
		return;
	}
	odata_move((odata*)cqueue_getptr(&_com_oBuf_Que),pData_);
	cqueue_pop(&_com_oBuf_Que);
}
//oBufのサイズを取得する
cqueue_size_t _com_oBuf_size(void){return cqueue_size(&_com_oBuf_Que);}
//送信可能かの確認
boolian _com_oBuf_full(void){return cqueue_full(&_com_oBuf_Que);}
//送信バッファの末尾がパケット終了タグ
boolian _com_oBuf_isPacEnd(void){return ((odata*)cqueue_getptr(&_com_oBuf_Que))->ID==HMR_COM_PACTRMNID;}
//送信用関数(受信済データ用)　送信データの先頭アドレス(Ptr)、送信データ長(Size)を引数に入れれば、その送信データを送信待ちバッファに送れる。
void _com_oBuf_move_push(odata* mData_){
	if(!odata_is_construct(mData_))return;

	if(_com_FailPac){
		if(mData_->ID==HMR_COM_PACTRMNID){
			--_com_FailPac;
		}
		odata_destruct(mData_);
	}else if(_com_oBuf_full()){
		odata_destruct(mData_);
	}else{
		if(mData_->ID==HMR_COM_PACTRMNID){
			++_com_HoldPac;
		}
		cqueue_push(&_com_oBuf_Que,(void*)mData_);
		odata_format(mData_);
	}
}
//****************com_wdt実装*********************//
uint8 x_com_wdt_WdtCnt=0;	//WDTのカウンター
boolian x_com_wdt_WdtEn=0;	//WDTの有効無効フラグ
//com監視用WatchDogTimerのリセット
void com_wdt_restart(){x_com_wdt_WdtCnt=0;}
//com監視用WatchDogTimerを停止
void com_wdt_enable(){x_com_wdt_WdtEn=hmLib_true;}
//com監視用WatchDogTimerを停止
void com_wdt_disable(){x_com_wdt_WdtEn=hmLib_false;}
//com監視用WatchDogTimerのタスク関数
sint16 com_wdt_task_function(sint16 Sec){
	//有効でない場合は、無視
	if(!x_com_wdt_WdtEn)return Sec;

	x_com_wdt_WdtCnt+=Sec;

	if(x_com_wdt_WdtCnt>com_wdt_sec)devmng_kill();

	return Sec;
}
//****************VMC関連実装*********************//
boolian com_vmc_can_iniRecvPac(void){return hmLib_true;}
//PacStrt受信完了時に呼ばれる
void com_vmc_iniRecvPac(void){
	//受信モードをONにする
	_com_RecvMode=1;

	//com_WDT対策
	com_wdt_restart();
}
//PacTrmn受信完了時に呼ばれる　引数はエラーの有無
void com_vmc_finRecvPac(unsigned char Err){
	idata IData;
//	cstring Str;
	//HMR_COM_PACTRMNIDコマンドを作成し、Pacを閉じる
//	cstring_construct_dynamic_safe(&Str,1);
//	cstring_putc(&Str,0,Err);
//DEBUG	idata_format(&IData);
//DEBUG	IData.ID=HMR_COM_PACTRMNID;
//DEBUG	_com_iBuf_move_push(&IData);
	
	idata_format(&IData);
	IData.ID=HMR_COM_PACTRMNID;
	_com_iBuf_move_push(&IData);
	
	//受信モードをOFFにする
	_com_RecvMode=0;
	//Pac受信通知関数を呼び出す
	if(_com_FpInfromFinRecvPac)_com_FpInfromFinRecvPac();
	//受信済み、送信待ちのデータ数を増やす
	++_com_WaitPacNum;
}
//iniRecvDatを実行して良いかの確認に呼ばれる
boolian com_vmc_can_iniRecvDat(void){return !_com_iBuf_full();}
//Dat受信開始時に呼ばれる 引数は受信するデータサイズ
unsigned int DebugCnt;
unsigned char* Debug;
void com_vmc_iniRecvDat(com_did_t ID,com_dsize_t Size){
	//IDやSizeがおかし場合は先に破棄
	if(ID==0 || 64<Size){
		_com_RecvFailDat=1;
		return;
	}

	_com_RecvDatID=ID;
	_com_RecvDatCnt=0;

	if(cstring_is_construct(&_com_RecvDatStr)){
		cstring_destruct_safe(&_com_RecvDatStr);
	}

	cstring_construct_dynamic_safe(&_com_RecvDatStr,Size);

	if(!cstring_is_construct(&_com_RecvDatStr) && Size>0){
		_com_RecvFailDat=1;
		return;
	}

//	_com_RecvDatStr.Begin=(unsigned char*)malloc(Size);
//	free(_com_RecvDatStr.Begin);
//	_com_RecvDatStr.Begin=0;

//DebugCnt=(1+DebugCnt)%32+10;
//Debug=(unsigned char*)malloc(DebugCnt);
//free(Debug);
}
//Dat受信終了時に呼ばれる 引数はエラーの有無
void com_vmc_finRecvDat(unsigned char Err){
	idata Data;
	idata_format(&Data);

	if(_com_RecvFailDat){
		_com_RecvFailDat=0;
		return;
	}

	if(Err){
		if(cstring_is_construct(&_com_RecvDatStr))cstring_destruct_safe(&_com_RecvDatStr);
		return;
	}

	//エラーが無ければバッファにデータ追加、あれば破棄
	idata_construct(&Data,_com_RecvDatID,&_com_RecvDatStr);
	_com_iBuf_move_push(&Data);

}
//recvを実行してよいかの確認に呼ばれる
boolian com_vmc_can_recv(void){return 1;}
//Datの中身受信時に呼ばれる　引数が受信したデータ
void com_vmc_recv(unsigned char c){
	if(_com_RecvFailDat==0)cstring_putc(&_com_RecvDatStr,_com_RecvDatCnt++,c);
//DEBUG	if(_com_RecvFailDat==0)cstring_putc(&_com_RecvDatStr,_com_RecvDatCnt++,c);
}
boolian com_vmc_can_iniSendPac(void){
	return devmng_isFullDuplex() || (_com_WaitPacNum>0);
}
//RacStrt送信完了時に呼ばれる
void com_vmc_iniSendPac(void){
	//送信モードをONにする
	_com_SendMode=1;

	if(_com_WaitPacNum)--_com_WaitPacNum;

	//com_WDT対策
//	com_wdt_restart();
}
//PacTrmn送信完了時に呼ばれる　引数はエラーの有無
void com_vmc_finSendPac(unsigned char Err){
	odata Data;

	odata_format(&Data);
	while(1){
		//HMR_COM_PACTRMNIDコマンドが出てこなかった場合、HMR_COM_PACTRMNIDコマンドが一つ出てくるまでは無効のPacであることを通知
		if(_com_oBuf_empty()){
			++_com_FailPac;
			break;
		}
		
		//先頭からmove&pop
		_com_oBuf_move_pop(&Data);
		//HMR_COM_PACTRMNIDコマンド==Pacの終端の合図が出てきた場合
		if(Data.ID==HMR_COM_PACTRMNID){
			//HMR_COM_PACTRMNIDコマンドを処理し、Pacを閉じる
			odata_destruct(&Data);
			--_com_HoldPac;
			break;
		}else{
			odata_destruct(&Data);
		}
	}
	//送信モードをOFFにする
	_com_SendMode=0;
	//Pac受信通知関数を呼び出す
	if(_com_FpInfromFinSendPac)_com_FpInfromFinSendPac();
}
//データの有無の確認が可能かどうか
boolian com_vmc_can_existSendDat(void){return !_com_oBuf_empty();}
//送信が必要なデータの有無の確認
boolian com_vmc_existSendDat(void){
	//バッファ末尾データがPacketTrmnの場合には存在しないのでfalse
	if(_com_oBuf_isPacEnd())return 0;
	//上記以外の場合はデータが存在するのでtrue
	return 1;
}
//iniSendDatを実行して良いかの確認に呼ばれる
boolian com_vmc_can_iniSendDat(void){
	return !_com_oBuf_isPacEnd();
}
//Dat送信確定時に呼ばれる　サイズを戻す
void com_vmc_iniSendDat(com_did_t* pID,com_dsize_t* pSize){
	if(odata_is_construct(&_com_SendDat)){
		odata_destruct(&_com_SendDat);
	}
	//送信データをパケットに移す
	_com_oBuf_move_pop(&_com_SendDat);

	if(!odata_is_construct(&_com_SendDat)){
		*pID=0;
		*pSize=0;
		return;
	}

	//カウンタクリア
	_com_SendDatCnt=0;
	//引数に情報を戻してやる
	*pID=_com_SendDat.ID;
	*pSize=cstring_size(&(_com_SendDat.Str));
}
//Dat送信終了時に呼ばれる　引数はエラーの有無
void com_vmc_finSendDat(unsigned char Err){
	//Dat送信終了を通知する
	if(odata_is_construct(&_com_SendDat)){
		odata_destruct(&_com_SendDat);
	}
}
//sendを実行してよいかの確認に呼ばれる
boolian com_vmc_can_send(void){
	//FpSendableが存在していないか、存在するが送信可能Size以内である場合、常に真
	return (_com_SendDat.FpSendable==0)
		|| (_com_SendDatCnt<_com_SendDat.FpSendable());
}
//Datの中身送信時に呼ばれる
unsigned char com_vmc_send(void){
//	int tmpPAG;
//	char c;
//	if(_com_SendDat.DSRPAG==0){
		//データ送信
		return cstring_getc(&(_com_SendDat.Str),_com_SendDatCnt++);
/*	}else{
		tmpPAG=_DSRPAG;
		_DSRPAG=__builtin_edspage(_com_SendDat.DSRPAG);
		//データ送信
		c=cstring_getc(&(_com_SendDat.Str),_com_SendDatCnt++);
		_DSRPAG=tmpPAG;
		return c;
	}
*/
}

//**************com関数群*************//
//初期化関数
void com_initialize(){
	_com_iBuf_initialize();
	_com_oBuf_initialize();

	_com_FailPac=0;
	_com_HoldPac=0;
	_com_WaitPacNum=0;

	_com_RecvMode=0;
	cstring_format(&_com_RecvDatStr);
	_com_RecvDatID=0;
	_com_RecvDatCnt=0;

	_com_SendMode=0;
	odata_format(&_com_SendDat);
	_com_SendDatCnt=0;

	com_wdt_restart();
}
//終端化関数
void com_finalize(){
	com_wdt_disable();

	cstring_destruct_safe(&(_com_RecvDatStr));
	cstring_destruct_safe(&(_com_SendDat.Str));

	_com_iBuf_finalize();
	_com_oBuf_finalize();
}
//受信モードか確認
boolian com_isRecvMode(void){return _com_RecvMode;}
//送信モードか確認
boolian com_isSendMode(void){return _com_SendMode;}
//VMC1を作成する
VMC1* com_createVMC1(){
	return vmc1_create(
		com_vmc_can_iniRecvPac,
		com_vmc_iniRecvPac,
		com_vmc_finRecvPac,
		com_vmc_can_iniRecvDat,
		com_vmc_iniRecvDat,
		com_vmc_finRecvDat,
		com_vmc_can_recv,
		com_vmc_recv,
		com_vmc_can_iniSendPac,
		com_vmc_iniSendPac,
		com_vmc_finSendPac,
		com_vmc_can_existSendDat,
		com_vmc_existSendDat,
		com_vmc_can_iniSendDat,
		com_vmc_iniSendDat,
		com_vmc_finSendDat,
		com_vmc_can_send,
		com_vmc_send
	);
}
//VMC1を破棄する
void com_releaseVMC1(VMC1* pVMC1){
	vmc1_release(pVMC1);
}
//****************com_IO関数群**************************//
//受信可能であるか確認
boolian com_in_empty(void){return _com_iBuf_empty();}
//受信バッファがいっぱいであるかの確認
boolian com_in_full(void){return _com_iBuf_full();}
//受信バッファの先頭データ取り出し
void com_in_move_pop(idata* pData_){_com_iBuf_move_pop(pData_);}
//送信バッファがいっぱいであるかの確認
boolian com_out_full(void){return _com_oBuf_full();}
//送信バッファが空かどうかの確認
boolian com_out_empty(void){return _com_oBuf_empty();}
//送信バッファにデータ追加
void com_out_move_push(odata* mData_){_com_oBuf_move_push(mData_);}
//****************** for Debug ***************************//
uint8 _com_getFailPacNum(void){return _com_FailPac;}
uint8 _com_getHoldPacNum(void){return _com_HoldPac;}
boolian com_isWaitSendPacket(void){return _com_HoldPac!=0;}
#
#endif
