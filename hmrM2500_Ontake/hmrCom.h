#ifndef HMR_COM_INC
#define HMR_COM_INC 205
#
/*
===hmrCom===
����M�o�b�t�@�ɂ��Ď��d�؂�
	@�R�}���h���p�P�b�g�̋�؂�
		�p�P�b�g����M���I���ƁA�����I�Ɏ�M�o�b�t�@��@�R�}���h�f�[�^��push�����
		���M���Ƀp�P�b�g��؂�ɂ�@�R�}���h�f�[�^�𑗐M�o�b�t�@��push���Ă����ƃp�P�b�g��������

v2_05/131102 hmIto
	�S��d/����d�ɉ����āA��Ƀf�[�^�𑗂�/��M�ς݂̐������f�[�^�𑗂�A��ύX
v2_04/131026 hmIto
	com�pWatchDogTimer�@�\���
		com_ini_recvPac/sendPac��restart�����B
		com_wdt_sec�ȏ�̎��ԑ���M���Ȃ��A����restart���Ă΂�Ȃ��ƁAkill����B
		�@�\��L���ɂ���ɂ́Acom_wdt_interrupt_function��task�ɓo�^����K�v������B
v2_03/130928 hmIto
	iBuf/oBuf_move_pop/push�ɂ����āAempty/full�ł����Ă������Ă��܂������C��
	odata::informDestruct�@�\�̏�����odata::destruct�ɈϏ�
v2_02/130803 hmIto
	inform_destruct�@�\�𗘗p���Ă���ۂɁC�܂��inform����Ȃ������C��
		�G���[��������finSendPac���Ă΂�邪�C���̍ۂ�destruct�͂���邪inform�͎��s����Ă��Ȃ������D
		���l�̖�肪�CFailPac�����݂���������ɂ�����move_push�ɂ����Ă����݂��Ă����D
v2_00/121201 hmIto
	odata/iQue_data��cstring���g���悤�ύX
	odata�^��ύX
		NonDeleteFlag��p�~
			cstring��dynamic_destruct��static_destruct���������ꂽ����
		FpSendable��ǉ�
			���M�\���������擾����֐�
		FpInformDestruct��ǉ�
			cstring��destruct���Ɍ���module�ɑ΂��ʒm����֐�
			����static�o�b�t�@�̍ė��p�̉ۂ̊m�F�ɗ��p
v1_09/121128 hmIto
	hmCLib��hmLib�̓����ɔ����AhmLib_v3_03�ɑΉ�����悤�ύX	
v1_08/121101 hmIto
	HMR_COM_PACTRMNID��@�R�}���h�̑���Ɏg�p
	odata����FnSendable��p�~
	odata��NonDeleteFlag��ǉ�
v1_07/121028 hmIto
	boolian com_out_empty(void)�ǉ�
v1_06/121021 hmIto
	����M�o�b�t�@����p�֐�����ύX(In��in�ɁAOut��out�Ɂj
	0byte�R�}���h�𑗐M���Ɉُ�I�����������v���I�o�O���C��
		ID��size�Ɋ܂܂�Ă������c�ŁASize=0���G���[�f�[�^�Ƃ��Ă������
	hmrVMC1_v2_00�ɑΉ�
		vmc1_create,vmc1_initialize�֐��̈����ǉ��ɑΉ�
		���킹�āAcom_createVMC1�ɂ����g��VMCID�w��p��������
v1_05/121020 iwahori
	c�t�@�C����com_IO�֐��Q�̖��̂�h�t�@�C���Ƃ̑����ύX�D(in��In�ɁCout��Out��)
v1_04/121012 hmIto
	com_iQue/oQue��hmrCom.c���ŉB��(__��t��)
	�C���^�[�t�F�C�X��Begin/End����Ptr/Size�ɖ߂�
	���O��Ԃ�Com����com�ɕύX
v1_03/121008 hmIto
	�֐��`�𔲖{�I�ɉ���
		VMC1�֐��Q�����b�s���O���Ă���com_RF�֐��Q�́A�p�~
		VMC1�Ƃ̒ʐM�ɓ�������com_VMC�֐��Q�́A�w�b�_�t�@�C���ł͉B�؂�
		���p�҂�vmc1_create/release�֐����g���āAVMC�𗘗p
		Buf�̑���M����́Acom_In/com_Out�֐��Q�ɏW��
			com_In/Out�ł́A�C���^�[�t�F�C�X��ID/Begin/End�ɓ���
			��M���̃f�[�^�̑��M�\�T�C�Y��Ԃ�sendProgress��p�~
			���M�i�K�ő��M�f�[�^��K�˂�push_later�֐���ǉ�
	lock/unlock�֐��p�~
		Mutex���g���Ȃ����߁A�o�b�t�@�̃��b�N�͊e�X���b�h�̗D�揇�ʂŐ���B
		���̂��߁A���b�N����X���b�h�ɂ���ď������قȂ�B
		�Ȍ�́A�e�X���b�h���Ǝ��Ɏ��g�̗D��x���グ��悤��lock/unlock�������s���B
	121009Test_hmrcom_hmrVMC1.c�ɂāA����m�F�ς�
v1_02/121005 hmIto
	VMC1�̊֐��Q�����b�s���O����̂ł͂Ȃ��AVMC1�𐶐�����֐���񋟂���`�ɕύX
		�ʐM�����ۂɍs���֐��́AVMC1�𗘗p����
v1_01/120922 hmIto
	�ЂƂ܂����삪���肵�Ă��邱�Ƃ��m�F�i120922Test_hmrcom_hmrVMC1.c�ɂē���m�F�ς݁j
v1_00/120921 hmIto
	�o�[�W�����Ǘ��J�n

*/
#ifndef HMR_DATA_INC
#	include"hmrData.h"
#endif
#ifndef HMR_VMC_INC
#	include"hmrVMC1.h"
#endif
#include"hmLib_v3_04/cqueue.h"
#ifdef __cplusplus
using hmLib::cstring;
#endif
#define HMR_COM_PACTRMNID 0xF0
//�f�[�^�^�̒�`
typedef vmc1_dsize_t com_dsize_t;
typedef vmc1_did_t com_did_t;
typedef com_dsize_t(*com_dsizeFp_v)(void);
typedef void(*com_vFp_v)(void);

//**************Com�֐��Q*************//
//�������֐�
void com_initialize(void);
//�I�[���֐�
void com_finalize(void);
//��M���[�h���m�F
boolian com_isRecvMode(void);
//���M���[�h���m�F
boolian com_isSendMode(void);
//VMC1���쐬����
VMC1* com_createVMC1(void);
//VMC1��j������
void com_releaseVMC1(VMC1* pVMC1);

//****************com_IO�֐��Q******************//
//��M�\�ł��邩�m�F
boolian com_in_empty(void);
//��M�o�b�t�@�ɋ󂫂����邩�m�F
boolian com_in_full(void);
//�f�[�^����M
idata com_in_front(void);
//�f�[�^����M���I��
void com_in_move_pop(idata* pData_);
//���M�\�ł��邩���m�F
boolian com_out_full(void);
//���M�o�b�t�@���󂩂ǂ����̊m�F
boolian com_out_empty(void);
//�f�[�^�𑗐M(��M�σf�[�^�p)
void com_out_move_push(odata* mData_);

//**************ComWDT�֘A************//
#define com_wdt_sec 180
//com�Ď��pWatchDogTimer�̃��Z�b�g
void com_wdt_restart();
//com�Ď��pWatchDogTimer���~
void com_wdt_enable();
//com�Ď��pWatchDogTimer���~
void com_wdt_disable();
//com�Ď��pWatchDogTimer�̃^�X�N�֐�
sint16 com_wdt_task_function(sint16 Sec);

//******************com_ioBuf�����֐��Q*****************//
//iBuf�̃T�C�Y���擾����
cqueue_size_t _com_iBuf_size(void);
cqueue_size_t _com_oBuf_size(void);
uint8 _com_getFailPacNum(void);
uint8 _com_getHoldPacNum(void);
boolian com_isWaitSendPacket(void);
boolian _com_oBuf_empty(void);
boolian _com_oBuf_isPacEnd(void);

//*****************debug****************//
boolian com_vmc_can_send(void);
#
#endif
