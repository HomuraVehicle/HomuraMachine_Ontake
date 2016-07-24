#ifndef HMR_SPRITEBUFFER_INC
#define HMR_SPRITEBUFFER_INC 16
#
/*
===hmrSpriteBuffer===
v1_00/131019 hmIto
	hmrSpriteからバッファ管理領域のみ分離

===hmrSprite===
v0_15/130727 iwahori
	各データバッファにtalk用にフラグをつける．連続撮影をminipacketだとそれなりの頻度でメモリリークがおきるバグは変わらず。通常パケットでもかなり待つとメモリリークがおきる
v0_15/130713 iwahori
	連続撮影はできるが、普通のTakeができない．普通のTakeのときはlisten内で_sprite_buf_empty()を課しているのに対して、
	AutoTakePicではsetup_talkで!_sprite_buf_full()を課しているあたりが怪しいか？
v0_15/130712 iwahori
	end_TakePicができないバグを修正、end_TakePicの前にspriteitf_clear_ReadPicDatFlagをしていなかったのが原因
v0_14/130706 iwahori
	カメラからデータを受信しながらCTRに送信できるように変更．2回目以降の撮影がうまくいかない．end_TakePicできていない？
	TalkNoの安全装置が_sprite_fnFinReadPicDataに要る
v0_13/130706 hmIto
	PicInfoのデータを送る際に、whileで送信可能となるまで待っていた問題を修正
		あらたに送信可能になったことを通知する変数を用意し、talk_setupで立ち上げるようにした
v0_12/130622 iwahori
	バグを修正。正常に動くようになった
v0_11/130615 iwahori
	ReadPic後のinform関数を使用するように変更．
	2回目以降の写真取得が失敗する．反応が返ってこない
v0_06/130427 iwahori
	新通信形式に対応
v0_06/130406 iwahori
	高画質の2つも取得成功．ミニパケットモードでも写真取得と連続撮影をチェック完了
v0_06/130406 iwahori
	最小サイズの写真取得/連続取得に成功．
	高画質の2つはできていない．写真データのアドレスの指定を間違えているかも
v0_06/130310 iwahori
	SpriteInterfaceに対応完了
	デバッグ作業中，パワーリセット機能の動作確認は完了
	写真データ取得はうまくいってない，原因はまだ不明
	パワーリセット後になぜか通信タイムアウトのエラーをはいてくる
v0_06/130211 hmIto
	
v0_06/130211 iwahori
	_sprite_buf_requestLock()のwhile内で++iされていなかったので追加
	sprite_setup_talk()内の連続撮影モードでカメラサイズをVからのデータを変換していなかったので変更
	_sprite_buf_full()のtrue falseが名前と逆になっていたのでを反転、それに伴いsprite_setup_talk()の分岐も反転
v0_05/130127 iwahori
	_sprite_is_BufAllUsedのreturn値を訂正
v0_04/130127 iwahori
	新通信規約に対応
v0_03/130112 iwahori
	sprite_setup_talk関数内のGetPictureフラグをあげる条件を変更
v0_02/130105 iwahori
	写真取得モード部分を修正
v0_01/121221 iwahori
	全体的に加筆
v0_00/121216 iwahori
	基本関数作成
*/
#include"hmLib_v3_04/cstring.h"
#ifndef HMR_TYPE_INC
#	include"hmrType.h"
#endif
#ifndef HMR_MESSAGE_INC
#	include"hmrMessage.h"
#endif

#define spritebuf_Num 3	//Debug 2
#define spritebuf_DataSize 4088		//8の倍数
#define spritebuf_DataSize_Small 240	//8の倍数
#define spritebuf_HeaderSize 3

//typedef関連
typedef uint8 spritebuf_id;
typedef msg_vFp_v spritebuf_vFp_v;
typedef msg_dsizeFp_v spritebuf_dsizeFp_v;

//バッファ関連初期化
void spritebuf_initialize(void);
//バッファ関連終端化
void spritebuf_finalize(void);
//バッファを確保する
spritebuf_id spritebuf_create(uint16 DataSize);
//バッファを解放する
void spritebuf_release(spritebuf_id ID);
//バッファをメッセージ関数と共有する
//	この関数が呼ばれた場合、以下のいずれかの処理が必要
//	1.FpInformDestructとともにデータをMessageに登録する
//	2.FpInformDestructを実行する
void spritebuf_share_withMesasge(spritebuf_id BufferNo,cstring* pStr,uint16 PicDataPos,spritebuf_dsizeFp_v* pFpSendableNum,spritebuf_vFp_v* pFpInformDestruct);
//バッファをSpriteInteface関数と共有する
//	この関数が呼ばれた場合、以下の処理が必要
//	1.spritebuf_release_fromSpriteInterface
void spritebuf_share_withSpriteInterface(spritebuf_id ID, unsigned char** pPtr, uint16** pCntPtr);
//バッファをSpriteInteface関数から解放する
void spritebuf_release_fromSpriteInterface(spritebuf_id ID);

#
#endif
