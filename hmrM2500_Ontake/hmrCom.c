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

//**************com�ϐ��Q******************//
//TRMN���i�߂��ɂ���������߁C�҂��Ȃ���΂����Ȃ�PACTRMNID��
//	finSendPac��TRMNID�������ł��Ȃ������ꍇ��++
//	oBuf_move_push��TRMNID���m�F�ł����ꍇ��--
uint8			_com_FailPac;
//���M�҂�PAC��
//	oBuf_move_push��FailPac�p��TRMNID����������Ȃ������ꍇ��++
//	finSendPac��TRMNID�������ł����ꍇ��--
uint8			_com_WaitPacNum;			//��M���Ė����M��Packet�̐�
uint8			_com_HoldPac;				//���M�X��Packet�̐�
boolian			_com_RecvFailDat=0;			//��M����Dat�̕ۑ�����m�ۂ��Ȃ�����/�ł��Ȃ������ꍇ�ɗ����AfinRecvDat�ŗ�����
boolian			_com_RecvMode=0;			//��M��(Strt��M����Trmn��M�܂ł̊ԁj
cstring			_com_RecvDatStr;
com_did_t		_com_RecvDatID=0;
com_dsize_t		_com_RecvDatCnt=0;			//��M����Dat�̃J�E���^
com_vFp_v		_com_FpInfromFinRecvPac=0;	//��M����Pac��S�Ď�M�������ɌĂяo���֐��|�C���^
boolian			_com_SendMode=0;			//���M��(Strt���M����Trmn���M�܂ł̊ԁj
odata			_com_SendDat;				//���M����Dat
com_dsize_t		_com_SendDatCnt=0;			//���M����Dat�̃J�E���^
com_vFp_v		_com_FpInfromFinSendPac=0;	//���M����Pac��S�Ď�M�������ɌĂяo���֐��|�C���^
//**************com_iBuf�֘A***************//
//��M�o�b�t�@
cqueue _com_iBuf_Que;
idata _com_iBuf_QueBuf[HMR_COM_BufqueSize];
//��M�o�b�t�@�̏�����
void _com_iBuf_initialize(void){
	cqueue_construct_static(
		&_com_iBuf_Que
		,_com_iBuf_QueBuf
		,_com_iBuf_QueBuf+HMR_COM_BufqueSize
		,sizeof(idata)
	);
}
//��M�o�b�t�@�̏I�[��
void _com_iBuf_finalize(void){
	cqueue_iterator itr;

	//�c���Ă���S�f�[�^��j��
	for(itr=cqueue_begin(&_com_iBuf_Que);itr!=cqueue_end(&_com_iBuf_Que);itr=cqueue_next(&_com_iBuf_Que,itr)){
		cstring_destruct_safe(&((idata*)(itr))->Str);
	}

	//iQue,oQue��j��
	cqueue_destruct(&_com_iBuf_Que);
}
//��M�\�����m�F
boolian _com_iBuf_empty(void){return cqueue_empty(&_com_iBuf_Que);}
//��M�ς݃f�[�^���폜����
void _com_iBuf_move_pop(idata* pData_){
	if(_com_iBuf_empty()){
		idata_format(pData_);
		return;
	}
	idata_move((idata*)cqueue_getptr(&_com_iBuf_Que),pData_);
	cqueue_pop(&_com_iBuf_Que);
}
//iBuf�Ƀf�[�^��}���\���m�F
boolian _com_iBuf_full(void){return cqueue_full(&_com_iBuf_Que);}
//iBuf�̃T�C�Y���擾����
cqueue_size_t _com_iBuf_size(void){return cqueue_size(&_com_iBuf_Que);}
//iBuf�Ƀf�[�^��}������
void _com_iBuf_move_push(idata* mData_){
	if(!idata_is_construct(mData_))return;

	if(_com_iBuf_full()){
		idata_destruct(mData_);
		return;
	}

	//���ۏ��move
	cqueue_push(&_com_iBuf_Que,(void*)(mData_));
	idata_format(mData_);
}
//*****************com_oBuf�֘A**************************//
//���M�o�b�t�@
cqueue _com_oBuf_Que;
odata _com_oBuf_QueBuf[HMR_COM_BufqueSize];
//���M�o�b�t�@�̏�����
void _com_oBuf_initialize(void){
	cqueue_construct_static(
		&_com_oBuf_Que
		,_com_oBuf_QueBuf
		,_com_oBuf_QueBuf+HMR_COM_BufqueSize
		,sizeof(odata)
	);
}
//���M�o�b�t�@�̏I�[��
void _com_oBuf_finalize(void){
	cqueue_iterator itr;

	//�c���Ă���S�f�[�^��j��
	for(itr=cqueue_begin(&_com_oBuf_Que);itr!=cqueue_end(&_com_oBuf_Que);itr=cqueue_next(&_com_oBuf_Que,itr)){
		cstring_destruct_safe(&((odata*)(itr))->Str);
	}

	//oQue��j��
	cqueue_destruct(&_com_oBuf_Que);
}
//���M�\�����m�F
boolian _com_oBuf_empty(void){return cqueue_empty(&_com_oBuf_Que);}
//���M�ς݃f�[�^���폜����
void _com_oBuf_move_pop(odata* pData_){
	if(_com_oBuf_empty()){
		odata_format(pData_);
		return;
	}
	odata_move((odata*)cqueue_getptr(&_com_oBuf_Que),pData_);
	cqueue_pop(&_com_oBuf_Que);
}
//oBuf�̃T�C�Y���擾����
cqueue_size_t _com_oBuf_size(void){return cqueue_size(&_com_oBuf_Que);}
//���M�\���̊m�F
boolian _com_oBuf_full(void){return cqueue_full(&_com_oBuf_Que);}
//���M�o�b�t�@�̖������p�P�b�g�I���^�O
boolian _com_oBuf_isPacEnd(void){return ((odata*)cqueue_getptr(&_com_oBuf_Que))->ID==HMR_COM_PACTRMNID;}
//���M�p�֐�(��M�σf�[�^�p)�@���M�f�[�^�̐擪�A�h���X(Ptr)�A���M�f�[�^��(Size)�������ɓ����΁A���̑��M�f�[�^�𑗐M�҂��o�b�t�@�ɑ����B
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
//****************com_wdt����*********************//
uint8 x_com_wdt_WdtCnt=0;	//WDT�̃J�E���^�[
boolian x_com_wdt_WdtEn=0;	//WDT�̗L�������t���O
//com�Ď��pWatchDogTimer�̃��Z�b�g
void com_wdt_restart(){x_com_wdt_WdtCnt=0;}
//com�Ď��pWatchDogTimer���~
void com_wdt_enable(){x_com_wdt_WdtEn=hmLib_true;}
//com�Ď��pWatchDogTimer���~
void com_wdt_disable(){x_com_wdt_WdtEn=hmLib_false;}
//com�Ď��pWatchDogTimer�̃^�X�N�֐�
sint16 com_wdt_task_function(sint16 Sec){
	//�L���łȂ��ꍇ�́A����
	if(!x_com_wdt_WdtEn)return Sec;

	x_com_wdt_WdtCnt+=Sec;

	if(x_com_wdt_WdtCnt>com_wdt_sec)devmng_kill();

	return Sec;
}
//****************VMC�֘A����*********************//
boolian com_vmc_can_iniRecvPac(void){return hmLib_true;}
//PacStrt��M�������ɌĂ΂��
void com_vmc_iniRecvPac(void){
	//��M���[�h��ON�ɂ���
	_com_RecvMode=1;

	//com_WDT�΍�
	com_wdt_restart();
}
//PacTrmn��M�������ɌĂ΂��@�����̓G���[�̗L��
void com_vmc_finRecvPac(unsigned char Err){
	idata IData;
//	cstring Str;
	//HMR_COM_PACTRMNID�R�}���h���쐬���APac�����
//	cstring_construct_dynamic_safe(&Str,1);
//	cstring_putc(&Str,0,Err);
//DEBUG	idata_format(&IData);
//DEBUG	IData.ID=HMR_COM_PACTRMNID;
//DEBUG	_com_iBuf_move_push(&IData);
	
	idata_format(&IData);
	IData.ID=HMR_COM_PACTRMNID;
	_com_iBuf_move_push(&IData);
	
	//��M���[�h��OFF�ɂ���
	_com_RecvMode=0;
	//Pac��M�ʒm�֐����Ăяo��
	if(_com_FpInfromFinRecvPac)_com_FpInfromFinRecvPac();
	//��M�ς݁A���M�҂��̃f�[�^���𑝂₷
	++_com_WaitPacNum;
}
//iniRecvDat�����s���ėǂ����̊m�F�ɌĂ΂��
boolian com_vmc_can_iniRecvDat(void){return !_com_iBuf_full();}
//Dat��M�J�n���ɌĂ΂�� �����͎�M����f�[�^�T�C�Y
unsigned int DebugCnt;
unsigned char* Debug;
void com_vmc_iniRecvDat(com_did_t ID,com_dsize_t Size){
	//ID��Size���������ꍇ�͐�ɔj��
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
//Dat��M�I�����ɌĂ΂�� �����̓G���[�̗L��
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

	//�G���[��������΃o�b�t�@�Ƀf�[�^�ǉ��A����Δj��
	idata_construct(&Data,_com_RecvDatID,&_com_RecvDatStr);
	_com_iBuf_move_push(&Data);

}
//recv�����s���Ă悢���̊m�F�ɌĂ΂��
boolian com_vmc_can_recv(void){return 1;}
//Dat�̒��g��M���ɌĂ΂��@��������M�����f�[�^
void com_vmc_recv(unsigned char c){
	if(_com_RecvFailDat==0)cstring_putc(&_com_RecvDatStr,_com_RecvDatCnt++,c);
//DEBUG	if(_com_RecvFailDat==0)cstring_putc(&_com_RecvDatStr,_com_RecvDatCnt++,c);
}
boolian com_vmc_can_iniSendPac(void){
	return devmng_isFullDuplex() || (_com_WaitPacNum>0);
}
//RacStrt���M�������ɌĂ΂��
void com_vmc_iniSendPac(void){
	//���M���[�h��ON�ɂ���
	_com_SendMode=1;

	if(_com_WaitPacNum)--_com_WaitPacNum;

	//com_WDT�΍�
//	com_wdt_restart();
}
//PacTrmn���M�������ɌĂ΂��@�����̓G���[�̗L��
void com_vmc_finSendPac(unsigned char Err){
	odata Data;

	odata_format(&Data);
	while(1){
		//HMR_COM_PACTRMNID�R�}���h���o�Ă��Ȃ������ꍇ�AHMR_COM_PACTRMNID�R�}���h����o�Ă���܂ł͖�����Pac�ł��邱�Ƃ�ʒm
		if(_com_oBuf_empty()){
			++_com_FailPac;
			break;
		}
		
		//�擪����move&pop
		_com_oBuf_move_pop(&Data);
		//HMR_COM_PACTRMNID�R�}���h==Pac�̏I�[�̍��}���o�Ă����ꍇ
		if(Data.ID==HMR_COM_PACTRMNID){
			//HMR_COM_PACTRMNID�R�}���h���������APac�����
			odata_destruct(&Data);
			--_com_HoldPac;
			break;
		}else{
			odata_destruct(&Data);
		}
	}
	//���M���[�h��OFF�ɂ���
	_com_SendMode=0;
	//Pac��M�ʒm�֐����Ăяo��
	if(_com_FpInfromFinSendPac)_com_FpInfromFinSendPac();
}
//�f�[�^�̗L���̊m�F���\���ǂ���
boolian com_vmc_can_existSendDat(void){return !_com_oBuf_empty();}
//���M���K�v�ȃf�[�^�̗L���̊m�F
boolian com_vmc_existSendDat(void){
	//�o�b�t�@�����f�[�^��PacketTrmn�̏ꍇ�ɂ͑��݂��Ȃ��̂�false
	if(_com_oBuf_isPacEnd())return 0;
	//��L�ȊO�̏ꍇ�̓f�[�^�����݂���̂�true
	return 1;
}
//iniSendDat�����s���ėǂ����̊m�F�ɌĂ΂��
boolian com_vmc_can_iniSendDat(void){
	return !_com_oBuf_isPacEnd();
}
//Dat���M�m�莞�ɌĂ΂��@�T�C�Y��߂�
void com_vmc_iniSendDat(com_did_t* pID,com_dsize_t* pSize){
	if(odata_is_construct(&_com_SendDat)){
		odata_destruct(&_com_SendDat);
	}
	//���M�f�[�^���p�P�b�g�Ɉڂ�
	_com_oBuf_move_pop(&_com_SendDat);

	if(!odata_is_construct(&_com_SendDat)){
		*pID=0;
		*pSize=0;
		return;
	}

	//�J�E���^�N���A
	_com_SendDatCnt=0;
	//�����ɏ���߂��Ă��
	*pID=_com_SendDat.ID;
	*pSize=cstring_size(&(_com_SendDat.Str));
}
//Dat���M�I�����ɌĂ΂��@�����̓G���[�̗L��
void com_vmc_finSendDat(unsigned char Err){
	//Dat���M�I����ʒm����
	if(odata_is_construct(&_com_SendDat)){
		odata_destruct(&_com_SendDat);
	}
}
//send�����s���Ă悢���̊m�F�ɌĂ΂��
boolian com_vmc_can_send(void){
	//FpSendable�����݂��Ă��Ȃ����A���݂��邪���M�\Size�ȓ��ł���ꍇ�A��ɐ^
	return (_com_SendDat.FpSendable==0)
		|| (_com_SendDatCnt<_com_SendDat.FpSendable());
}
//Dat�̒��g���M���ɌĂ΂��
unsigned char com_vmc_send(void){
//	int tmpPAG;
//	char c;
//	if(_com_SendDat.DSRPAG==0){
		//�f�[�^���M
		return cstring_getc(&(_com_SendDat.Str),_com_SendDatCnt++);
/*	}else{
		tmpPAG=_DSRPAG;
		_DSRPAG=__builtin_edspage(_com_SendDat.DSRPAG);
		//�f�[�^���M
		c=cstring_getc(&(_com_SendDat.Str),_com_SendDatCnt++);
		_DSRPAG=tmpPAG;
		return c;
	}
*/
}

//**************com�֐��Q*************//
//�������֐�
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
//�I�[���֐�
void com_finalize(){
	com_wdt_disable();

	cstring_destruct_safe(&(_com_RecvDatStr));
	cstring_destruct_safe(&(_com_SendDat.Str));

	_com_iBuf_finalize();
	_com_oBuf_finalize();
}
//��M���[�h���m�F
boolian com_isRecvMode(void){return _com_RecvMode;}
//���M���[�h���m�F
boolian com_isSendMode(void){return _com_SendMode;}
//VMC1���쐬����
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
//VMC1��j������
void com_releaseVMC1(VMC1* pVMC1){
	vmc1_release(pVMC1);
}
//****************com_IO�֐��Q**************************//
//��M�\�ł��邩�m�F
boolian com_in_empty(void){return _com_iBuf_empty();}
//��M�o�b�t�@�������ς��ł��邩�̊m�F
boolian com_in_full(void){return _com_iBuf_full();}
//��M�o�b�t�@�̐擪�f�[�^���o��
void com_in_move_pop(idata* pData_){_com_iBuf_move_pop(pData_);}
//���M�o�b�t�@�������ς��ł��邩�̊m�F
boolian com_out_full(void){return _com_oBuf_full();}
//���M�o�b�t�@���󂩂ǂ����̊m�F
boolian com_out_empty(void){return _com_oBuf_empty();}
//���M�o�b�t�@�Ƀf�[�^�ǉ�
void com_out_move_push(odata* mData_){_com_oBuf_move_push(mData_);}
//****************** for Debug ***************************//
uint8 _com_getFailPacNum(void){return _com_FailPac;}
uint8 _com_getHoldPacNum(void){return _com_HoldPac;}
boolian com_isWaitSendPacket(void){return _com_HoldPac!=0;}
#
#endif
