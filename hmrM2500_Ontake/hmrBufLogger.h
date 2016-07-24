#ifndef HMR_BUFbuflogger_INC
#define HMR_BUFbuflogger_INC 100
#

/*
=== hmrBufbuflogger =====================================================
バッファを使用した疑似ロガー

v1_00/140318 hmIto
	bufloggerと同じインターフェース+名前空間で一応作成
*/
#include"hmLib_v3_04/bytebase_type.h"
#include"hmrData.h"

#define buflogger_BUFFERSIZE 12288

// 初期化関数(chは自分自身のch,戻り値はSDのdetect失敗時にtrue)
boolian buflogger_initialize(void);

// 終端化関数
void buflogger_finalize(void);

// ログ可能かの確認関数(引数はvcom_data内のデータサイズ確認用)
boolian buflogger_can_write(const odata* pData);

// ログ可能かの確認関数（バイト指定バージョン）
boolian buflogger_can_write_bytes(uint16 DataBytes_);

// ログ関数(この中でdestructされる)
boolian buflogger_move_write(odata* mData_);

// 読み出し可能かの確認関数
boolian buflogger_can_read(void);

// 読みだす関数(この中でconstructされる)
boolian buflogger_move_read(odata* pData_);

//読み残しのbyte数取得
uint32 buflogger_size(void);

//bufloggerとの互換用　常に失敗
boolian buflogger_save_position(void);

//bufloggerとの互換用　常に失敗
boolian buflogger_load_position(void);

#
#endif
