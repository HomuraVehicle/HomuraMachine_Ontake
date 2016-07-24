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

//Used�̃t�B���^�[
const unsigned char Used_Sprite=1;
const unsigned char Used_Message=2;
const unsigned char Used_SpriteItf=4;

//spritebuf�̒��j�ƂȂ�\����
typedef struct{
	uint8 Used;	//�g�p�����m�ۂ��Ă��鏊�L�Ґ�
	unsigned char Buf[spritebuf_HeaderSize+spritebuf_DataSize];
	uint16 DataCnt;		//�������܂ꂽ�f�[�^�̐�
	uint16 DataSize;	//�������ޗ\��̃f�[�^�̐�
}sprite_buffer;
sprite_buffer x_spritebuf_Buffer[spritebuf_Num] __attribute__((far));

//=== protected�֐� ===
//�o�b�t�@�g�p���𓾂�
unsigned char _spritebuf_usedNum(void);

//VMC�ɑ����Ă�������ʒm�p
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
//Message�Ƀo�b�t�@�[�̃����[�X�˗��p
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

//�o�b�t�@�֘A������
void spritebuf_initialize(void){
	uint8 i=0;
	for(i=0;i<spritebuf_Num;++i){
		x_spritebuf_Buffer[i].Used=0;
		x_spritebuf_Buffer[i].DataCnt=0;
		x_spritebuf_Buffer[i].DataSize=0;
	}
}
//�o�b�t�@�֘A�I�[��
void spritebuf_finalize(void){}
//�o�b�t�@���m�ۂ���
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
//�o�b�t�@�g�p���𓾂�
unsigned char _spritebuf_usedNum(void){
	uint8 i=0;
	uint8 tmp=0;
	for(i=0;i<spritebuf_Num;++i){
		if(x_spritebuf_Buffer[i].Used)++tmp;
	}
	return tmp;
}
//�o�b�t�@���������(Sprite)
void spritebuf_release(spritebuf_id ID){
	//�A�N�Z�X�\�ȗ̈�łȂ���Ζ���
	if(ID<1 || spritebuf_Num<ID)return;

	//���L��������
	x_spritebuf_Buffer[ID-1].Used&=(~Used_Sprite);
}
//�o�b�t�@�����b�Z�[�W�֐��Ƌ��L����
//	���̊֐����Ă΂ꂽ�ꍇ�A�ȉ��̂����ꂩ�̏������K�v
//	1.FpInformDestruct�ƂƂ��Ƀf�[�^��Message�ɓo�^����
//	2.FpInformDestruct�����s����
void spritebuf_share_withMesasge(spritebuf_id ID,cstring* pStr,uint16 PicDataPos,
	spritebuf_dsizeFp_v* pFpSendableNum,spritebuf_vFp_v* pFpInformDestruct){
	//�A�N�Z�X�\�ȗ̈�łȂ���Ζ���
	if(ID<1 || spritebuf_Num<ID)return;

	//�m�ۂ���Ă��Ȃ��Ă�����
	if((x_spritebuf_Buffer[ID-1].Used&Used_Sprite)==0)return;

	//���łɃ��b�Z�[�W�Ɋm�ۂ���Ă��Ă�����
	if(x_spritebuf_Buffer[ID-1].Used&Used_Message)return;

	//���b�Z�[�W�ɂ����L�����n��
	x_spritebuf_Buffer[ID-1].Used|=Used_Message;

	//cstring�f�[�^�\�z
	x_spritebuf_Buffer[ID-1].Buf[0]=0x01;
	x_spritebuf_Buffer[ID-1].Buf[1]=(uint8)(PicDataPos);
	x_spritebuf_Buffer[ID-1].Buf[2]=(uint8)(PicDataPos>>8);
	cstring_construct_static(pStr,x_spritebuf_Buffer[ID-1].Buf,x_spritebuf_Buffer[ID-1].Buf+x_spritebuf_Buffer[ID-1].DataSize+spritebuf_HeaderSize);

	//�֐��|�C���^���擾
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
//�o�b�t�@��SpriteInteface�֐��Ƌ��L����
//	���̊֐����Ă΂ꂽ�ꍇ�A�ȉ��̏������K�v
//	1.spritebuf_release_fromSpriteInterface
void spritebuf_share_withSpriteInterface(spritebuf_id ID, unsigned char** pPtr,uint16** pCntPtr){
	//�A�N�Z�X�\�ȗ̈�łȂ���Ζ���
	if(ID<1 || spritebuf_Num<ID)return;

	//�m�ۂ���Ă��Ȃ��Ă�����
	if((x_spritebuf_Buffer[ID-1].Used&Used_Sprite)==0)return;

	//���ł�SpriteInterface�Ɋm�ۂ���Ă��Ă�����
	if(x_spritebuf_Buffer[ID-1].Used&Used_SpriteItf)return;

	//SpriteInterface�ɂ����L�����n��
	x_spritebuf_Buffer[ID-1].Used|=Used_SpriteItf;

	//�o�b�t�@���Z�b�g����
	*pPtr = x_spritebuf_Buffer[ID-1].Buf+spritebuf_HeaderSize;
	*pCntPtr = &(x_spritebuf_Buffer[ID-1].DataCnt);
}
//�o�b�t�@��SpriteInteface�֐�����������
void spritebuf_release_fromSpriteInterface(spritebuf_id ID){
	//�A�N�Z�X�\�ȗ̈�łȂ���Ζ���
	if(ID<1 || spritebuf_Num<ID)return;

	//�o�b�t�@�̏������݂���Ă���=�T�C�Y�ƃJ�E���g����v�����Ă���
	x_spritebuf_Buffer[ID-1].DataCnt=x_spritebuf_Buffer[ID-1].DataSize;

	//���L��������
	x_spritebuf_Buffer[ID-1].Used&=(~Used_SpriteItf);
}
//�ʐ^�f�[�^�������ݗ̈���擾����
unsigned char* _x_spritebuf_getBufPtr(spritebuf_id ID){
	//�A�N�Z�X�\�ȗ̈�łȂ���Ζ���
	if(ID<1 || spritebuf_Num<ID)return 0;

	//�m�ۂ���Ă��Ȃ��Ă�����
	if((x_spritebuf_Buffer[ID-1].Used&Used_Sprite)==0)return 0;

	//�o�b�t�@��Ԃ�
	return x_spritebuf_Buffer[ID-1].Buf+spritebuf_HeaderSize;
}
//�ʐ^�f�[�^�T�C�Y�������ݗ̈���擾����
uint16* _x_spritebuf_getBufCnt(spritebuf_id ID){
	//�A�N�Z�X�\�ȗ̈�łȂ���Ζ���
	if(ID<1 || spritebuf_Num<ID)return 0;

	//�m�ۂ���Ă��Ȃ��Ă�����
	if((x_spritebuf_Buffer[ID-1].Used&Used_Sprite)==0)return 0;

	//�o�b�t�@��Ԃ�
	return &(x_spritebuf_Buffer[ID-1].DataCnt);
}
#
#endif
