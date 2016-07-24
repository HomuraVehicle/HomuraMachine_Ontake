#ifndef HMR_LOGGERMESSAGE_C_INC
#define HMR_LOGGERMESSAGE_C_INC 100
#
#include"hmrLoggerMessage_b.h"
#include"hmrBufLogger.h"
#include"hmrUtility.h"

//���K�[�쓮�p�o�^�֐��Q
typedef struct {
	unsigned char ID;
	vFp_v setup_talk;
	logmsg_bFp_ps talk;
	bFp_v is_talking;
}logmessage_t;
typedef struct {
	unsigned InformLogSize:1;
	unsigned InformRequestData:1;
	unsigned InformSendData:1;
}logmsg_flags;
logmsg_flags x_logmsg_Flags;
uint32 x_logmsg_RequestSize;
uint32 x_logmsg_SendSize;
//�֐��Q�o�^�p�o�b�t�@
logmessage_t x_logmsg_Buffer[logmsg_BUFSIZE];
//�֐��Q�o�^�p�o�b�t�@�̃T�C�Y
unsigned char x_logmsg_BufferSize;
//Logger���[�h�ɓ��������̃t���O
boolian x_logmsg_IsLogging;
//Logger�ۑ��ς݃��b�Z�[�W�ʒu
unsigned char x_logmsg_BufferCnt;

//����������
void logmsg_initialize(void) {
	x_logmsg_BufferSize=0;
	x_logmsg_IsLogging=0;
	x_logmsg_BufferCnt=0;
}
//�I�[������
void logmsg_finalize(void) {
}
//logger�pmessage�֐���o�^
hmLib_boolian logmsg_regist(logmsg_did_t ID_
	, vFp_v FpSetupTalk_
	, logmsg_bFp_ps FpTalk_
	, bFp_v FpIsTalking_) {
	//�o�^����ɒB���Ă�����͂���
	if(x_logmsg_BufferSize>=logmsg_BUFSIZE)return hmLib_true;

	//�֐���o�^
	x_logmsg_Buffer[x_logmsg_BufferSize].ID=ID_;
	x_logmsg_Buffer[x_logmsg_BufferSize].setup_talk=FpSetupTalk_;
	x_logmsg_Buffer[x_logmsg_BufferSize].talk=FpTalk_;
	x_logmsg_Buffer[x_logmsg_BufferSize].is_talking=FpIsTalking_;

	//�T�C�Y�𑝂₵�Ă���
	++x_logmsg_BufferSize;

	return hmLib_false;
}
//���O����X�^�[�g
void logmsg_logging(void) {
	unsigned char Cnt;
	//setup_talk��S���Ɏ��s������
	for(Cnt=0; Cnt<x_logmsg_BufferSize; ++Cnt) {
		//�֐����o�^����Ă��邱�Ƃ��m���߂Ă���A���s
		if(x_logmsg_Buffer[Cnt].setup_talk)x_logmsg_Buffer[Cnt].setup_talk();
	}

	//���O����̏�����
	x_logmsg_BufferCnt=0;
	x_logmsg_IsLogging=hmLib_true;
}
//���O����p�������H
boolian logmsg_is_logging(void) {
	return x_logmsg_IsLogging;
}
//logmsg���W���[���̃��[�N�֐�
void logmsg_work(void) {
	odata Data;

	odata_format(&Data);

	//���O���[�h�łȂ���Ζ�������
	if(!x_logmsg_IsLogging)return;

	//�������߂Ȃ��Ƃ�����������
	if(buflogger_can_write_bytes(4096)==0)return;

	//�o�^�f�[�^�̖����ɓ��B����܂ő�����
	while (x_logmsg_BufferCnt<x_logmsg_BufferSize) {
		//talk�̎擾�ɐ�������΁A�E�o
		if (x_logmsg_Buffer[x_logmsg_BufferCnt].talk(&(Data.Str)) == 0) {
			break;
		}

		//�O�̂��ߔj�����Ă���
		if (cstring_is_construct(&(Data.Str)))cstring_destruct_safe(&(Data.Str));

		//�܂��b�����ɂ�������炸�ǂݍ��ݎ��s�̏ꍇ�́A����ɂ�����
		if (x_logmsg_Buffer[x_logmsg_BufferCnt].is_talking())return;

		//���̃f�[�^��
		++x_logmsg_BufferCnt;
	}

	//�����܂œ��B�����ꍇ�͂����Ń��O������I����
	if (x_logmsg_BufferCnt == x_logmsg_BufferSize) {
		x_logmsg_IsLogging = hmLib_false;
		return;
	}

/*
	//�f�[�^��ǂݍ��݂ɐ�������܂ő�����
	while(x_logmsg_Buffer[x_logmsg_BufferCnt].talk(&(Data.Str))) {
		//�O�̂��ߔj�����Ă���
		if(cstring_is_construct(&(Data.Str)))cstring_destruct_safe(&(Data.Str));
		
		//�܂��b�����ɂ�������炸�ǂݍ��ݎ��s�̏ꍇ�́A����ɂ�����
		if(x_logmsg_Buffer[x_logmsg_BufferCnt].is_talking())return;

		//���̃f�[�^�����݂��Ȃ��ꍇ�́A�����Ń��O������I����
		if(++x_logmsg_BufferCnt == x_logmsg_BufferSize) {
			x_logmsg_IsLogging=hmLib_false;
			return;
		}
	}
	*/

	//ID���T����
	Data.ID=x_logmsg_Buffer[x_logmsg_BufferCnt].ID;

	//�f�[�^�����K�[�ɂԂ�����
	buflogger_move_write(&Data);
}
//��M�֐� �Ԃ�l�͐���Ƀf�[�^����������Ȃ������ꍇ��true
hmLib_boolian logmsg_listen(cstring Str) {
	if(cstring_size(&Str)==0)return hmLib_true;

	switch(cstring_getc(&Str, 0)) {
	case 0x00:
		x_logmsg_Flags.InformLogSize=1;
		return hmLib_false;
	case 0x01:
		if(cstring_size(&Str)!=3)return hmLib_true;

		//���ɑ��M���̏ꍇ�͊O��
		if(x_logmsg_RequestSize>0)return hmLib_false;

		//�T�C�Y���擾
		x_logmsg_SendSize=0;
		x_logmsg_RequestSize=(uint32)(cstring_getc(&Str, 1))+((uint32)(cstring_getc(&Str, 2))<<8);
		x_logmsg_Flags.InformRequestData=1;
		return hmLib_false;
	default:
		return hmLib_true;
	}
}
//���M�֐� �Ԃ�l�̓f�[�^�����݂��Ȃ��ꍇ��true
hmLib_boolian logmsg_talk(cstring* pStr) {
	uint32 u32;
	odata Data;
	if(x_logmsg_Flags.InformLogSize) {
		cstring_construct_dynamic_safe(pStr, 5);
		cstring_putc(pStr, 0, 0x00);
		u32=buflogger_size();
		cstring_putc(pStr, 1, (uint8)(u32));
		u32>>=8;
		cstring_putc(pStr, 2, (uint8)(u32));
		u32>>=8;
		cstring_putc(pStr, 3, (uint8)(u32));
		u32>>=8;
		cstring_putc(pStr, 4, (uint8)(u32));
		x_logmsg_Flags.InformLogSize=0;
		return hmLib_false;
	} else if(x_logmsg_Flags.InformRequestData) {
		cstring_construct_dynamic_safe(pStr, 3);
		cstring_putc(pStr, 0, 0x00);
		u32=x_logmsg_RequestSize;
		cstring_putc(pStr, 1, (uint8)(u32));
		u32>>=8;
		cstring_putc(pStr, 2, (uint8)(u32));
		x_logmsg_Flags.InformRequestData=0;
		return hmLib_false;
	}
	
	if(x_logmsg_SendSize<x_logmsg_RequestSize) {
		if(!buflogger_can_read()) {
			x_logmsg_Flags.InformSendData=1;
			x_logmsg_SendSize=0;
			x_logmsg_RequestSize=0;
		} else {
			buflogger_move_read(&Data);
			msg_talkconfig_setID(Data.ID);
			cstring_move(&(Data.Str), pStr);
			x_logmsg_SendSize+=cstring_size(pStr)+3;
			if(x_logmsg_SendSize>=x_logmsg_RequestSize) {
				x_logmsg_Flags.InformSendData=1;
				x_logmsg_SendSize=0;
				x_logmsg_RequestSize=0;
			}
			return hmLib_false;
		}
	}
	if(x_logmsg_Flags.InformSendData) {
		u32=buflogger_size();
		cstring_construct_dynamic_safe(pStr, 5);
		cstring_putc(pStr, 0, 0x20);
		cstring_putc(pStr, 1, (uint8)(u32));
		u32>>=8;
		cstring_putc(pStr, 2, (uint8)(u32));
		u32>>=8;
		cstring_putc(pStr, 3, (uint8)(u32));
		u32>>=8;
		cstring_putc(pStr, 4, (uint8)(u32));
		x_logmsg_Flags.InformSendData=0;
		return hmLib_false;
	}
	return hmLib_true;
}
#
#endif
