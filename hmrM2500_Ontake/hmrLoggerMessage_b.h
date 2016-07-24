#ifndef HMR_LOGGERMESSAGE_INC
#define HMR_LOGGERMESSAGE_INC 100
#
/*
===hmrLoggerMessage===
���K�[�o�R�Ńf�[�^�����߁APC����̃��b�Z�[�W�Œ~�����f�[�^����o���郂�W���[��

�g����
�܂��Amessage�`����setup_talk, talk�A�����inform_talk_end�֐���regist�֐��œo�^����
	setup_talk�F���̃��O����O�Ɉ�x�����Ă΂��A���O����ʒm�֐��@���̊֐����쎞�ɁA�f�[�^�擾���J�n���Ă������Ƃ��]�܂���
	talk�F���O����֐��@�L�^�������f�[�^��n���@�܂������ł��Ă��Ȃ���΁Atrue��Ԃ��΂���
	is_talking�F�����talk���삪���݂��p�����Ă��邩�������֐�
logmsg_logging�֐������s����ƁAlog���삪�X�^�[�g����
	setup_talk����x����΂��
	IsTalking�t���O�������オ��Alogmsg_work�֐�����talk��������s����
logmsg_work�֐����ŁA���ۂ̃��O���삪���s�����
	�o�^���ꂽtalk�֐����Ă΂�A�o�Ă������b�Z�[�W��logger�ɋL�^����
	is_talk_end�֐��������オ��ƁA���ɓo�^���ꂽtalk�֐���ǂ݂ɍs��
logmsg_is_logging�֐��ŁA���ׂẴ��O���삪�������Ă��邩�m�F�ł���
logmsg_message�֐��Q��message�ɓo�^����ƁA�����I�Ƀ��O�f�[�^�̓ǂݏo�����̋@�\�����s�ł���
	>> 0x00
		�ۑ�����Ă���f�[�^�T�C�Y��v��
	<< 0x00 LogSize[4]
		�f�[�^�T�C�Y��4byte�Ŗ߂�
	<> 0x01 RequestSize[2]
		�Œ�RequestSize���̃f�[�^��v������/�������i�������u�ԏI���A����Ȃ��Ă��������I���j
	<< 0x02 LogSize[4]
		���O�f�[�^�̑��M������ʒm����ƂƂ��ɁA�c��̃��O�f�[�^�T�C�Y��ʒm
===hmrLoggerMessage===
hmrLoggerMessage_v1_00
	hmrMessage:v2_04���p�����č쐬
*/
#include"hmLib_v3_04/bytebase_type.h"
#include"hmLib_v3_04/cstring.h"
#ifndef HMR_DATA_INC
#	include"hmrData.h"
#endif
#ifndef HMR_COM_INC
#	include"hmrCom.h"
#endif
#ifndef HMR_MESSAGE_INC
#	include"hmrMessage.h"
#endif

typedef com_did_t logmsg_did_t;
//==============�ݒ�p=================//
#define logmsg_BUFSIZE 8
//=====================================//
typedef hmLib_boolian(*logmsg_bFp_ps)(cstring*);	//Talk�p�֐�
//typedef void(*vFp_v)(void);
//typedef boolian(*bFp_v)(void);

//����������
void logmsg_initialize(void);
//�I�[������
void logmsg_finalize(void);
//logger�pmessage�֐���o�^
hmLib_boolian logmsg_regist(logmsg_did_t ID_
	, vFp_v FpSetupTalk_
	, logmsg_bFp_ps FpTalk_
	, bFp_v FpIsTalking_);
//���O����X�^�[�g
void logmsg_logging(void);
//���O����p�������H
boolian logmsg_is_logging(void);
//logmsg���W���[���̃��[�N�֐�
void logmsg_work(void);
//��M�����֐�
void logmsg_setup_listen(void);
//��M�֐� �Ԃ�l�͐���Ƀf�[�^����������Ȃ������ꍇ��true
hmLib_boolian logmsg_listen(cstring Str);
//���M�����֐�
void logmsg_setup_talk(void);
//���M�֐� �Ԃ�l�̓f�[�^�����݂��Ȃ��ꍇ��true
hmLib_boolian logmsg_talk(cstring* pStr);
#
#endif
