#ifndef HMR_BUFLOGGER_C_INC
#define HMR_BUFLOGGER_C_INC 100
#
#include"hmrBufLogger.h"
#include"hmrUtility.h"

#define x_buflogger_HEADBYTES 3
#define x_buflogger_MAXDATASIZE 128
unsigned char x_buflogger_Buffer[buflogger_BUFFERSIZE]  __attribute__((far));
unsigned char* x_buflogger_Begin;
unsigned char* x_buflogger_End;
uint16 x_buflogger_Size;

//インクリメント
unsigned char*  x_buflogger_next(unsigned char* Itr) {
	return (Itr==x_buflogger_Buffer+buflogger_BUFFERSIZE-1?x_buflogger_Buffer:Itr+1);
}
//デクリメント
unsigned char* x_buflogger_prev(unsigned char* Itr) {
	return (Itr==x_buflogger_Buffer?x_buflogger_Buffer+buflogger_BUFFERSIZE-1:Itr-1);
}

// 初期化関数(chは自分自身のch,戻り値はSDのdetect失敗時にtrue)
boolian buflogger_initialize(void) {
	x_buflogger_Begin=x_buflogger_Buffer;
	x_buflogger_End=x_buflogger_Buffer;
	x_buflogger_Size=0;
}

// 終端化関数
void buflogger_finalize(void) {
	x_buflogger_Begin=x_buflogger_Buffer;
	x_buflogger_End=x_buflogger_Buffer;
	x_buflogger_Size=0;
	return hmLib_false;
}

// ログ可能かの確認関数(引数はvcom_data内のデータサイズ確認用)
boolian buflogger_can_write(const odata* pData) {
	return buflogger_can_write_bytes(cstring_size(&(pData->Str)));
}

// ログ可能かの確認関数（バイト指定バージョン）
boolian buflogger_can_write_bytes(uint16 DataBytes_) {
	return (x_buflogger_Size+DataBytes_+x_buflogger_HEADBYTES<buflogger_BUFFERSIZE-1);
}

// ログ関数(この中でdestructされる)
boolian buflogger_move_write(odata* mData_) {
	uint16 Size;
	uint16 Cnt;
	//書き込めないとはじく
	if(buflogger_can_write(mData_)==0)return hmLib_true;

	//サイズ取得
	Cnt=0;
	Size=cstring_size(&(mData_->Str));

	//サイズ上限を超えるものははじく
	if(Size>x_buflogger_MAXDATASIZE)return hmLib_true;

	//送信不可能の時もはじく
	if(mData_->FpSendable && mData_->FpSendable()!=Size)return hmLib_true;

	//IDをまず書き込む
	*x_buflogger_End=mData_->ID;
	x_buflogger_End=x_buflogger_next(x_buflogger_End);

	//Sizeを書き込む
	*x_buflogger_End=(uint8)(Size);
	x_buflogger_End=x_buflogger_next(x_buflogger_End);
	*x_buflogger_End=(uint8)(Size>>8);

	//Strを書き込む
	for(Cnt=0; Cnt<Size; ++Cnt) {
		*x_buflogger_End=cstring_getc(&(mData_->Str), Cnt);
		x_buflogger_End=x_buflogger_next(x_buflogger_End);
	}
	//Destruct
	cstring_destruct(&(mData_->Str));

	//Sizeを加算しておく
	x_buflogger_Size+=x_buflogger_HEADBYTES+Size;

	//destruct通知
	if(mData_->FpInformDestruct)mData_->FpInformDestruct();

	return hmLib_false;
}

// 読み出し可能かの確認関数
boolian buflogger_can_read(void) {
	return x_buflogger_Size>0;
}

// 読みだす関数(この中でconstructされる)
boolian buflogger_move_read(odata* pData_) {
	uint16 Size;
	uint16 Cnt;

	//読み出せないとはじく
	if(buflogger_can_read()==0)return hmLib_true;

	//1byte目がID
	pData_->ID=*x_buflogger_Begin;
	x_buflogger_Begin=x_buflogger_next(x_buflogger_Begin);

	//2,3byte目がSize
	Size=(uint16)(*x_buflogger_Begin);
	x_buflogger_Begin=x_buflogger_next(x_buflogger_Begin);
	Size+=((uint16)(*x_buflogger_Begin)<<8);
	x_buflogger_Begin=x_buflogger_next(x_buflogger_Begin);

	if(Size>x_buflogger_MAXDATASIZE) {
		x_buflogger_Size=0;
		x_buflogger_Begin=0;
		x_buflogger_End=0;
		return hmLib_true;
	}

	//Construct
	cstring_construct_dynamic_safe(&(pData_->Str), Size);

	//Strデータ読み出し
	for(Cnt=0; Cnt<Size; ++Cnt) {
		cstring_putc(&(pData_->Str), Cnt, *x_buflogger_Begin);
		x_buflogger_Begin=x_buflogger_next(x_buflogger_Begin);
	}

	//Sizeを減算しておく
	x_buflogger_Size-=x_buflogger_HEADBYTES+Size;

	//ほかのodata初期化
	pData_->FpInformDestruct=0;
	pData_->FpSendable=0;

	return hmLib_false;
}

//読み残しのbyte数取得
uint32 buflogger_size(void) {
	return x_buflogger_Size;
}

//loggerとの互換用　常に失敗
boolian buflogger_save_position(void) {
	return hmLib_true;
}

//loggerとの互換用　常に失敗
boolian buflogger_load_position(void) {
	return hmLib_true;
}
#
#endif
