#ifndef HMR_SPRITEBUFFER_C_INC
#define HMR_SPRITEBUFFER_C_INC 100
#
#undef __cplusplus
#define HMLIB_NOLIB
#include"hmrType.h"
#include<hmLib_v3_04/cqueue.h>
#ifndef HMR_SPRITE_INC
#	include"hmrSpriteBuffer.h"
#endif
#ifndef HMR_UTILITY_INC
#	include "hmrUtility.h"
#endif

//Usedのフィルター
const unsigned char Used_Sprite=1;
const unsigned char Used_Message=2;
const unsigned char Used_SpriteItf=4;

//spritebufの中核となる構造体
typedef struct{
	uint8 Used;	//使用権を確保している所有者数
	unsigned char Buf[spritebuf_HeaderSize+spritebuf_DataSize];
	uint16 DataCnt;		//書き込まれたデータの数
	uint16 DataSize;	//書き込む予定のデータの数
}sprite_buffer;
sprite_buffer x_spritebuf_Buffer[spritebuf_Num] __attribute__((far));

//=== protected関数 ===
//バッファ使用数を得る
unsigned char _spritebuf_usedNum(void);

//VMCに送っていい数を通知用
uint16 x_spritebuf_Buffer0_SendableNum(void){return (x_spritebuf_Buffer[0].DataCnt+spritebuf_HeaderSize);}
uint16 x_spritebuf_Buffer1_SendableNum(void){return (x_spritebuf_Buffer[1].DataCnt+spritebuf_HeaderSize);}
//uint16 x_spritebuf_Buffer2_SendableNum(void){return (x_spritebuf_Buffer[2].DataCnt+spritebuf_HeaderSize);}
//uint16 x_spritebuf_Buffer3_SendableNum(void){return (x_spritebuf_Buffer[3].DataCnt+spritebuf_HeaderSize);}
//uint16 x_spritebuf_Buffer4_SendableNum(void){return (x_spritebuf_Buffer[4].DataCnt+spritebuf_HeaderSize);}
//uint16 x_spritebuf_Buffer5_SendableNum(void){return (x_spritebuf_Buffer[5].DataCnt+spritebuf_HeaderSize);}
//uint16 x_spritebuf_Buffer6_SendableNum(void){return (x_spritebuf_Buffer[6].DataCnt+spritebuf_HeaderSize);}
//uint16 x_spritebuf_Buffer7_SendableNum(void){return (x_spritebuf_Buffer[7].DataCnt+spritebuf_HeaderSize);}
//uint16 x_spritebuf_Buffer8_SendableNum(void){return (x_spritebuf_Buffer[8].DataCnt+spritebuf_HeaderSize);}
//uint16 x_spritebuf_Buffer9_SendableNum(void){return (x_spritebuf_Buffer[9].DataCnt+spritebuf_HeaderSize);}
//Messageにバッファーのリリース依頼用
void x_spritebuf_Buffer0_inform_destruct(void){x_spritebuf_Buffer[0].Used&=(~Used_Message);}
void x_spritebuf_Buffer1_inform_destruct(void){x_spritebuf_Buffer[1].Used&=(~Used_Message);}
//void x_spritebuf_Buffer2_inform_destruct(void){x_spritebuf_Buffer[2].Used&=(~Used_Message);}
//void x_spritebuf_Buffer3_inform_destruct(void){x_spritebuf_Buffer[3].Used&=(~Used_Message);}
//void x_spritebuf_Buffer4_inform_destruct(void){x_spritebuf_Buffer[4].Used&=(~Used_Message);}
//void x_spritebuf_Buffer5_inform_destruct(void){x_spritebuf_Buffer[5].Used&=(~Used_Message);}
//void x_spritebuf_Buffer6_inform_destruct(void){x_spritebuf_Buffer[6].Used&=(~Used_Message);}
//void x_spritebuf_Buffer7_inform_destruct(void){x_spritebuf_Buffer[7].Used&=(~Used_Message);}
//void x_spritebuf_Buffer8_inform_destruct(void){x_spritebuf_Buffer[8].Used&=(~Used_Message);}
//void x_spritebuf_Buffer9_inform_destruct(void){x_spritebuf_Buffer[9].Used&=(~Used_Message);}

//バッファ関連初期化
void spritebuf_initialize(void){
	uint8 i=0;
	for(i=0;i<spritebuf_Num;++i){
		x_spritebuf_Buffer[i].Used=0;
		x_spritebuf_Buffer[i].DataCnt=0;
		x_spritebuf_Buffer[i].DataSize=0;
	}
}
//バッファ関連終端化
void spritebuf_finalize(void){}
//バッファを確保する
spritebuf_id spritebuf_create(uint16 DataSize){
	spritebuf_id i=0;
	for(i=0;i<spritebuf_Num;++i){
		if(x_spritebuf_Buffer[i].Used==0){
			x_spritebuf_Buffer[i].Used=Used_Sprite;
			x_spritebuf_Buffer[i].DataCnt=0;
			x_spritebuf_Buffer[i].DataSize=DataSize;
			return i+1;
		}
	}
	return 0;
}
//バッファ使用数を得る
unsigned char _spritebuf_usedNum(void){
	uint8 i=0;
	uint8 tmp=0;
	for(i=0;i<spritebuf_Num;++i){
		if(x_spritebuf_Buffer[i].Used)++tmp;
	}
	return tmp;
}
//バッファを解放する(Sprite)
void spritebuf_release(spritebuf_id ID){
	//アクセス可能な領域でなければ無視
	if(ID<1 || spritebuf_Num<ID)return;

	//所有権を失う
	x_spritebuf_Buffer[ID-1].Used&=(~Used_Sprite);
}
//バッファをメッセージ関数と共有する
//	この関数が呼ばれた場合、以下のいずれかの処理が必要
//	1.FpInformDestructとともにデータをMessageに登録する
//	2.FpInformDestructを実行する
void spritebuf_share_withMesasge(spritebuf_id ID,cstring* pStr,uint16 PicDataPos,
	spritebuf_dsizeFp_v* pFpSendableNum,spritebuf_vFp_v* pFpInformDestruct){
	//アクセス可能な領域でなければ無視
	if(ID<1 || spritebuf_Num<ID)return;

	//確保されていなくても無視
	if((x_spritebuf_Buffer[ID-1].Used&Used_Sprite)==0)return;

	//すでにメッセージに確保されていても無視
	if(x_spritebuf_Buffer[ID-1].Used&Used_Message)return;

	//メッセージにも所有権が渡る
	x_spritebuf_Buffer[ID-1].Used|=Used_Message;

	//cstringデータ構築
	x_spritebuf_Buffer[ID-1].Buf[0]=0x01;
	x_spritebuf_Buffer[ID-1].Buf[1]=(uint8)(PicDataPos);
	x_spritebuf_Buffer[ID-1].Buf[2]=(uint8)(PicDataPos>>8);
	cstring_construct_static(pStr,x_spritebuf_Buffer[ID-1].Buf,x_spritebuf_Buffer[ID-1].Buf+x_spritebuf_Buffer[ID-1].DataSize+spritebuf_HeaderSize);

	//関数ポインタを取得
	switch(ID-1){
	case 0:
		*pFpSendableNum=x_spritebuf_Buffer0_SendableNum;
		*pFpInformDestruct=x_spritebuf_Buffer0_inform_destruct;
		break;
	case 1:
		*pFpSendableNum=x_spritebuf_Buffer1_SendableNum;
		*pFpInformDestruct=x_spritebuf_Buffer1_inform_destruct;
		break;
/*	case 2:
		*pFpSendableNum=x_spritebuf_Buffer2_SendableNum;
		*pFpInformDestruct=x_spritebuf_Buffer2_inform_destruct;
		break;
	case 3:
		*pFpSendableNum=x_spritebuf_Buffer3_SendableNum;
		*pFpInformDestruct=x_spritebuf_Buffer3_inform_destruct;
		break;
	case 4:
		*pFpSendableNum=x_spritebuf_Buffer4_SendableNum;
		*pFpInformDestruct=x_spritebuf_Buffer4_inform_destruct;
		break;
	case 5:
		*pFpSendableNum=x_spritebuf_Buffer5_SendableNum;
		*pFpInformDestruct=x_spritebuf_Buffer5_inform_destruct;
		break;
	case 6:
		*pFpSendableNum=x_spritebuf_Buffer6_SendableNum;
		*pFpInformDestruct=x_spritebuf_Buffer6_inform_destruct;
		break;
	case 7:
		*pFpSendableNum=x_spritebuf_Buffer7_SendableNum;
		*pFpInformDestruct=x_spritebuf_Buffer7_inform_destruct;
		break;
	case 8:
		*pFpSendableNum=x_spritebuf_Buffer8_SendableNum;
		*pFpInformDestruct=x_spritebuf_Buffer8_inform_destruct;
		break;
	case 9:
		*pFpSendableNum=x_spritebuf_Buffer9_SendableNum;
		*pFpInformDestruct=x_spritebuf_Buffer9_inform_destruct;
		break;
*/	default:
		*pFpSendableNum=0;
		*pFpInformDestruct=0;
		break;
	}
}
//バッファをSpriteInteface関数と共有する
//	この関数が呼ばれた場合、以下の処理が必要
//	1.spritebuf_release_fromSpriteInterface
void spritebuf_share_withSpriteInterface(spritebuf_id ID, unsigned char** pPtr,uint16** pCntPtr){
	//アクセス可能な領域でなければ無視
	if(ID<1 || spritebuf_Num<ID)return;

	//確保されていなくても無視
	if((x_spritebuf_Buffer[ID-1].Used&Used_Sprite)==0)return;

	//すでにSpriteInterfaceに確保されていても無視
	if(x_spritebuf_Buffer[ID-1].Used&Used_SpriteItf)return;

	//SpriteInterfaceにも所有権が渡る
	x_spritebuf_Buffer[ID-1].Used|=Used_SpriteItf;

	//バッファをセットする
	*pPtr = x_spritebuf_Buffer[ID-1].Buf+spritebuf_HeaderSize;
	*pCntPtr = &(x_spritebuf_Buffer[ID-1].DataCnt);
}
//バッファをSpriteInteface関数から解放する
void spritebuf_release_fromSpriteInterface(spritebuf_id ID){
	//アクセス可能な領域でなければ無視
	if(ID<1 || spritebuf_Num<ID)return;

	//バッファの書き込みを閉じておく=サイズとカウントを一致させておく
	x_spritebuf_Buffer[ID-1].DataCnt=x_spritebuf_Buffer[ID-1].DataSize;

	//所有権を失う
	x_spritebuf_Buffer[ID-1].Used&=(~Used_SpriteItf);
}
//写真データ書き込み領域を取得する
unsigned char* _x_spritebuf_getBufPtr(spritebuf_id ID){
	//アクセス可能な領域でなければ無視
	if(ID<1 || spritebuf_Num<ID)return 0;

	//確保されていなくても無視
	if((x_spritebuf_Buffer[ID-1].Used&Used_Sprite)==0)return 0;

	//バッファを返す
	return x_spritebuf_Buffer[ID-1].Buf+spritebuf_HeaderSize;
}
//写真データサイズ書き込み領域を取得する
uint16* _x_spritebuf_getBufCnt(spritebuf_id ID){
	//アクセス可能な領域でなければ無視
	if(ID<1 || spritebuf_Num<ID)return 0;

	//確保されていなくても無視
	if((x_spritebuf_Buffer[ID-1].Used&Used_Sprite)==0)return 0;

	//バッファを返す
	return &(x_spritebuf_Buffer[ID-1].DataCnt);
}
#
#endif
