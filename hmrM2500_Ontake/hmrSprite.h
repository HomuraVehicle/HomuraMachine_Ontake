#ifndef HMR_SPRITE_INC
#define HMR_SPRITE_INC 16
#
/*===hmrSprite===
カメラ関連モジュール
ToDo:talk1,talk2,…(溜まっているデータ数だけ一気にtalk)
hmrSprite_v0_15/130727 iwahori
	各データバッファにtalk用にフラグをつける．連続撮影をminipacketだとそれなりの頻度でメモリリークがおきるバグは変わらず。通常パケットでもかなり待つとメモリリークがおきる
hmrSprite_v0_15/130713 iwahori
	連続撮影はできるが、普通のTakeができない．普通のTakeのときはlisten内で_sprite_buf_empty()を課しているのに対して、
AutoTakePicではsetup_talkで!_sprite_buf_full()を課しているあたりが怪しいか？
hmrSprite_v0_15/130712 iwahori
	end_TakePicができないバグを修正、end_TakePicの前にspriteitf_clear_ReadPicDatFlagをしていなかったのが原因
hmrSprite_v0_14/130706 iwahori
	カメラからデータを受信しながらCTRに送信できるように変更．2回目以降の撮影がうまくいかない．end_TakePicできていない？
	TalkNoの安全装置が_sprite_fnFinReadPicDataに要る
hmrSprite_v0_13/130706 hmIto
	PicInfoのデータを送る際に、whileで送信可能となるまで待っていた問題を修正
		あらたに送信可能になったことを通知する変数を用意し、talk_setupで立ち上げるようにした
hmrSprite v0_12/130622 iwahori
	バグを修正。正常に動くようになった
hmrSprite v0_11/130615 iwahori
	ReadPic後のinform関数を使用するように変更．
	2回目以降の写真取得が失敗する．反応が返ってこない
hmrSprite_v0_06/130427 iwahori
	新通信形式に対応
hmrSprite_v0_06/130406 iwahori
	高画質の2つも取得成功．ミニパケットモードでも写真取得と連続撮影をチェック完了
hmrSprite_v0_06/130406 iwahori
	最小サイズの写真取得/連続取得に成功．
	高画質の2つはできていない．写真データのアドレスの指定を間違えているかも
hmrSprite_v0_06/130310 iwahori
	SpriteInterfaceに対応完了
	デバッグ作業中，パワーリセット機能の動作確認は完了
	写真データ取得はうまくいってない，原因はまだ不明
	パワーリセット後になぜか通信タイムアウトのエラーをはいてくる
hmrSprite_v0_06/130211 hmIto
	
hmrSprite_v0_06/130211 iwahori
	_sprite_buf_requestLock()のwhile内で++iされていなかったので追加
	sprite_setup_talk()内の連続撮影モードでカメラサイズをVからのデータを変換していなかったので変更
	_sprite_buf_full()のtrue falseが名前と逆になっていたのでを反転、それに伴いsprite_setup_talk()の分岐も反転
hmrSprite_v0_05/130127 iwahori
	_sprite_is_BufAllUsedのreturn値を訂正
hmrSprite_v0_04/130127 iwahori
	新通信規約に対応
hmrSprite_v0_03/130112 iwahori
	sprite_setup_talk関数内のGetPictureフラグをあげる条件を変更
hmrSprite_v0_02/130105 iwahori
	写真取得モード部分を修正
hmrSprite_v0_01/121221 iwahori
	全体的に加筆
hmrSprite_v0_00/121216 iwahori
	基本関数作成
*/
#include"hmLib_v3_04/cstring.h"
#include"hmLib_v3_04/bytebase_type.h"
#ifndef HMR_DEVICE_INC
#	include"hmrDevice.h"
#endif
#ifndef HMR_SPRITEINTERFACE_INC
#	include"hmrSpriteInterface.h"
#endif
#ifndef HMR_MESSAGE_INC
#	include"hmrMessage.h"
#endif
#ifdef __cplusplus
using hmLib::cstring;
#endif
typedef void(*sprite_vFp_v)(void);
typedef msg_dsizeFp_v sprite_dsizeFp_v;
//#define SPRITE_PACKSIZE 160 //240 debug_
//#define SPRITE_BUF_HEADER 4	//0x20,0x21,size[2]

//Message Agentのステータスの定義
#define _sprite_Status_Idle				0x00
#define _sprite_Status_ReadPicInfo		0x13
#define _sprite_Status_ReadPicData		0x20
#define _sprite_Status_EndTakePic		0x30
#define _sprite_Status_CommandReset		0x40
#define _sprite_Status_PowerReset		0xF0


hmLib_boolian sprite_listen(cstring Str);
hmLib_boolian sprite_talk(cstring* pStr);
void sprite_setup_listen(void);
void sprite_setup_talk(void);
void sprite_initialize(void);
void sprite_finalize(void);
sint16 sprite_task_inform(sint16 interval);
boolian _sprite_itf_readPicData(void);
#
#endif
