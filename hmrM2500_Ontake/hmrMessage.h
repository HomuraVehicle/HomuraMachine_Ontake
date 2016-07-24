#ifndef HMR_MESSAGE_INC
#define HMR_MESSAGE_INC 204
#
/*
===hmrMessage===
ID��ǂݏo���AID��������������listen�֐��ɓn��
talk�֐��ő��M���ׂ��f�[�^���󂯎��CID��t�����ĕԂ�

hmrMessage:v2_04/130212 hmIto
	msg_listen()
		idata��destruct�����Y��Ă��������C���B
		idata�̈��������ԃR�s�[����move�|�C���^�ɕύX�B
	msg_talk()
		true��FpTalk����߂��Ă����ۂɁA������Str���m�ۂ���Ă���ꍇdestruct����悤�C��
hmrMessage:v2_03/130202 iwahori
	msg_setup_talk()�Ffor���[�v�̏���l��HMR_MESSAGE_BUFSIZE����_msg_BufSize�ɏC��
	msg_setup_listen()�Ffor���[�v�̏���l��HMR_MESSAGE_BUFSIZE����_msg_BufSize�ɏC��
hmrMessage:v2_02/130127 iwahori
	msg_setup_talk()����_msg_Buf[i].FpSetupListen()�ƂȂ��Ă���������
	_msg_Buf[i].FpSetupTalk()�ɏC��
hmrMessage:v2_01/130105 hmIto
	hmLib_v3_03�ɑΉ�
	SetupListen/SetupTalk�֐���ǉ�
		Listen��Talk���s�O�Ɉ�x�����Ă΂��
		����M�t���O�̐ݒ�p�֐�
hmrMessage:v2_00/121201 hmIto
	hmrCom_v2_00�ɑΉ�
	���b�Z�[�W�����֐����ATalk�֐���Listen�֐��ɕ���
hmrMessage:v1_05/121128 hmIto
	hmCLib��hmLib�̓����ɔ����AhmLib_v3_03�ɑΉ�����悤�ύX		
hmrMessage:v1_04/121103 hmIto
	NonDeleteFlag�@�\�ɑΉ�
		msg_set_nondelete_mode();
hmrMessage:v1_03/121013 hmIto
	���O���Message��msg�ɕύX
hmrMessage:v1_02/121012 hmIto
	MessageType�\���̂�Sendable�֐����Ǘ�����`�ɕύX
hmrMessage:v1_01/121009 hmIto
	�C���^�[�t�F�C�X��傫���ύX
		msg_range�͔p�~
		work�֐��̈������|�C���^�ɕύX
hmrMessage:v1_00/121005 hmIto
	�ЂƂ܂��A����
*/
#ifndef HMCLIB_BYTEBASETYPE_INC
#	include"hmLib_v3_04/bytebase_type.h"
#endif
#ifndef HMCLIB_CSTRING_INC
#	include"hmLib_v3_04/cstring.h"
#endif
#ifndef HMR_COM_INC
#	include"hmrCom.h"
#endif
#ifdef __cplusplus
using hmLib::cstring;
#endif
typedef com_did_t msg_did_t;
typedef com_dsize_t msg_dsize_t;
typedef com_dsizeFp_v msg_dsizeFp_v;
typedef com_vFp_v msg_vFp_v;
//==============�ݒ�p=================//
#define HMR_MESSAGE_BUFSIZE 32
//=====================================//

typedef hmLib_boolian(*msg_bFp_s)(cstring);				//Listen�p�֐�
typedef hmLib_boolian(*msg_bFp_ps)(cstring*);	//Talk�p�֐�
//message_t�\����
typedef struct{
	msg_did_t	ID;				//���b�Z�[�W���ʗpID
	msg_vFp_v	FpSetupListen;	//���b�Z�[�W��M�����p�֐�
	msg_bFp_s	FpListen;		//���b�Z�[�W��M�p�֐� �߂�l�̓��b�Z�[�W�����̉�
	msg_vFp_v	FpSetupTalk;	//���b�Z�[�W���M�����p�֐�
	msg_bFp_ps	FpTalk;			//���b�Z�[�W���M�p�֐� �߂�l�̓��b�Z�[�W�̗L��
}message_t;
//����������
void msg_initialize(void);
//�I�[������
void msg_finalize(void);
//DatType��o�^
hmLib_boolian msg_regist(msg_did_t ID_
						 ,msg_vFp_v FpSetupListen_
						 ,msg_bFp_s FpListen_
						 ,msg_vFp_v FpSetupTalk_
						 ,msg_bFp_ps FpTalk_);
//��M�����֐�
void msg_setup_listen(void);
//��M�֐� �Ԃ�l�͐���Ƀf�[�^����������Ȃ������ꍇ��true
hmLib_boolian msg_listen(idata* mdata_);
//���M�����֐�
void msg_setup_talk(void);
//���M�֐� �Ԃ�l�̓f�[�^�����݂��Ȃ��ꍇ��true
hmLib_boolian msg_talk(odata* pdata_);
//���M�\�T�C�Y�֐��̎g�p��錾����
void msg_talkconfig_useSendable(msg_dsizeFp_v Fp_);
//���M�����ʒm�֐����g�p����
void msg_talkconfig_useInformDestruct(msg_vFp_v Fp_);
//EDS�������̈�Ɋm�ۂ����ꍇ�̃A�N�Z�X�p�ɁADSRPageFile��o�^����
void msg_talkconfig_useDSRPageFile(int DSRPAG_);
#
#endif
