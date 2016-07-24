#ifndef HMR_MESSAGE_C_INC
#define HMR_MESSAGE_C_INC 204
#
#define HMLIB_NOLIB
#ifndef HMR_MESSAGE_INC
#	include"hmrMessage.h"
#endif
#ifndef HMR_UTILITY_INC
#	include "hmrUtility.h"
#endif
#include"hmrDebug.h"
#
#include<stdlib.h>
#ifndef HMLIB_CQUEUE_INC
#	include"hmLib_v3_03/cqueue.h"
#endif
#ifdef __cplusplus
using hmLib::cqueue;
#endif

//message_t�̃o�b�t�@
message_t _msg_Buf[HMR_MESSAGE_BUFSIZE];
//�o�^����Ă���message_t��
unsigned char _msg_BufSize=0;
//���M�֐��̌��݂̔ԍ�
unsigned char _msg_BufTalkPos=0;
//FpSendable�֐��o�^�p�ꎞ�ϐ�
msg_dsizeFp_v _msg_talkconfig_FpSendable=0;
//FpInformDestruct�֐��o�^�p�ꎞ�ϐ�
msg_vFp_v _msg_talkconfig_FpInformDestruct=0;
//DSRPageFile�o�^�p�ꎞ�ϐ�
int _msg_talkconfig_DSRPAG=0;
//���M�ݒ�p
void _msg_talkconfig_clear(void){
	//msg_talkconfig�֐��Q�̎g�p�ɔ�����
	_msg_talkconfig_FpSendable=0;
	_msg_talkconfig_FpInformDestruct=0;
	_msg_talkconfig_DSRPAG=0;
}
//���M�\�T�C�Y�֐��̎g�p��錾����
void msg_talkconfig_useSendable(msg_dsizeFp_v Fp_){
	_msg_talkconfig_FpSendable=Fp_;
}
//���M�����ʒm�֐����g�p����
void msg_talkconfig_useInformDestruct(msg_vFp_v Fp_){
	_msg_talkconfig_FpInformDestruct=Fp_;
}
//EDS�������̈�Ɋm�ۂ����ꍇ�̃A�N�Z�X�p�ɁADSRPageFile��o�^����
void msg_talkconfig_useDSRPageFile(int DSRPAG_){
	_msg_talkconfig_DSRPAG=DSRPAG_;
}
//����������
void msg_initialize(void){
	_msg_BufSize=0;
}
//�I�[������
void msg_finalize(void){}
//DatType��o�^
hmLib_boolian msg_regist(
	msg_did_t ID_
	,msg_vFp_v FpSetupListen_
	,msg_bFp_s FpListen_
	,msg_vFp_v FpSetupTalk_
	,msg_bFp_ps FpTalk_){
	if(_msg_BufSize==HMR_MESSAGE_BUFSIZE)return 1;
	if(ID_==0)return 1;

	_msg_Buf[_msg_BufSize].ID=ID_;
	_msg_Buf[_msg_BufSize].FpSetupListen=FpSetupListen_;
	_msg_Buf[_msg_BufSize].FpListen=FpListen_;
	_msg_Buf[_msg_BufSize].FpSetupTalk=FpSetupTalk_;
	_msg_Buf[_msg_BufSize].FpTalk=FpTalk_;

	++_msg_BufSize;
	return 0;
}
//��M�����֐�
void msg_setup_listen(void){
	unsigned char i;
	for(i=0;i<_msg_BufSize;++i){
		if(_msg_Buf[i].FpSetupListen)_msg_Buf[i].FpSetupListen();
	}
}
//��M�֐�
hmLib_boolian msg_listen(idata* mData_){
	unsigned char i;
//	cstring str;
	
	for(i=0;i<HMR_MESSAGE_BUFSIZE;++i){
		if(_msg_Buf[i].ID==mData_->ID)break;
	}
	
	if(i>=HMR_MESSAGE_BUFSIZE){
		idata_destruct(mData_);
		return hmLib_true;
	}

	if(_msg_Buf[i].FpListen)_msg_Buf[i].FpListen(mData_->Str);
	idata_destruct(mData_);
	
	return hmLib_false;
}
//���M�����֐�
void msg_setup_talk(void){
	unsigned char i;

	for(i=0;i<_msg_BufSize;++i){
		if(_msg_Buf[i].FpSetupTalk)_msg_Buf[i].FpSetupTalk();
	}
	_msg_BufTalkPos=0;
}
//���M�֐� �Ԃ�l��
hmLib_boolian msg_talk(odata* pData_){
	cstring Str;
	_msg_talkconfig_clear();

	//format�������Ă���
	cstring_format(&Str);
        while(_msg_Buf[_msg_BufTalkPos].FpTalk==0 || _msg_Buf[_msg_BufTalkPos].FpTalk(&Str)==hmLib_true){
		//������m�ۂ���Ă������̂��߂ɁA���
		if(cstring_is_construct(&Str))cstring_destruct_safe(&Str);
		//���łɍŌ�̃o�b�t�@�܂ŏ����ς݂Ȃ�I��
		if(++_msg_BufTalkPos>=_msg_BufSize)return hmLib_true;
	}

	odata_construct(pData_,_msg_Buf[_msg_BufTalkPos].ID,&Str
		,_msg_talkconfig_DSRPAG
		,_msg_talkconfig_FpSendable
		,_msg_talkconfig_FpInformDestruct
	);

	return hmLib_false;
}

/*
//Message�̏����֐�
odata msg_work(idata data_){
	unsigned char tmp=0;
	cstring str,astr;
	odata ans;
	ans.ID=0;
	ans.FpSendable=0;
	ans.FpInformDestruct=0;

	for(tmp=0;tmp<HMR_MESSAGE_BUFSIZE;++tmp){
		if(_msg_Buf[tmp].ID==data_.ID)break;
	}

	if(tmp<HMR_MESSAGE_BUFSIZE){
		_msg_FpSendable=0;
		_msg_FpInformDestruct=0;

		cstring_construct_static(&str,data_.Ptr,data_.Ptr+data_.Size);
		_msg_Buf[tmp].FpMessage(str);
		astr=_msg_Buf[tmp].FpTalk(
		cstring_destruct_safe(&str);
		if(data_.Size)free(data_.Ptr);

		//�T�C�Y�����݂��邩�A1,1�Ȃǂ̏ꍇ�ɗL��
		if(cstring_size(&astr)>0 || cstring_ptr(&astr)){
			ans.ID=_msg_Buf[tmp].ID;
			cstring_move(&astr,&(ans.Str));
			ans.FpSendable=_msg_FpSendable;
			ans.FpInformDestruct=_msg_FpInformDestruct;
		}
	}

	return ans;
}
*/
#
#endif
