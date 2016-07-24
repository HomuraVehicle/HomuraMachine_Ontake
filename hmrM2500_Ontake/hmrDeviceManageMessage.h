#ifndef HMR_DEVICEMANAGEMESSAGE_INC
#define HMR_DEVICEMANAGEMESSAGE_INC 102

/*
===hmrDeviceManage===

v1_02/131019 amby
	���b�Z�[�W�֐��̒ǉ� ���̑��̋@�\�̓C���^�[�t�F�[�X�ցc

v1_01/131013 hmIto
	�C���^�[�t�F�[�X��啝�ɕύX������
		���[�h����N���b�N���x�w������O
*/

#ifndef HMR_TYPE_INC
#	include"hmrType.h"
#endif
#ifndef HMR_DEVICECONFIG_INC
#	include"hmrDevice.h"
#endif
#ifndef HMR_DEVICEMANAGE_INC
#	include"hmrDeviceManage.h"
#endif
#ifndef HMR_TASK_INC
#	include"hmrTask.h"
#endif
#include"hmLib_v3_04/bytebase_type.h"
#include"hmLib_v3_04/cstring.h"

#ifdef __cplusplus
using hmLib::cstring;
#endif

//void devmngmsg_initialize(void);
//void devmngmsg_finalize(void);

hmLib_boolian devmngmsg_listen(cstring Str);
hmLib_boolian devmngmsg_talk(cstring* pStr);
void devmngmsg_setup_listen(void);
void devmngmsg_setup_talk(void);

sint16 devmngmsg_task_inform(sint16 interval);
#
#endif


/*
Device����p�̋@�\��񋟂���
	Kill�@�\
		�������g�Ƃ��ׂĂ̂̓d����؂�
	�X���[�v�@�\
		����I�ɓd�����I��/�I�t���J��Ԃ�
	���[�~���O�@�\
		Roaming���[�h�ɓ���ƁARF(RoamingTime)=>MB(RoamingTime)=>RF�̂悤�ɁA�g�p�ʐM�@�̑J�ڂ��J��Ԃ��B
		RoamingTime�͓����ϐ��Ō��肷��i�R�}���h�Őݒ�\�j
	�N���b�N���[�h�ύX�@�\
		�ݒ�N���b�N���x��ύX����

===hmrDeviceManage===
v1_01/131013 hmIto
	�C���^�[�t�F�[�X��啝�ɕύX������
		���[�h����N���b�N���x�w������O
*/

/*
typedef void(*devmng_vFp_v)(void);
//=== ��{����֐� ===
//�f�o�C�X�������֐�
void devmng_initialize(void);
//�f�o�C�X�I�[���֐�
void devmng_finalize(void);
//�f�o�C�X�����I��
void devmng_kill(void);

//=== ���[�h����@�\ ===
typedef enum{NormalMode,SleepMode,RoamingMode}devmng_mode;
//�f�o�C�X���[�h��ύX����
void devmng_mode_set(devmng_mode Mode_);
//�f�o�C�X���[�h���擾����
devmng_mode devmng_mode_get(void);

//=== �N���b�N���[�h����@�\ ===
typedef enum{LowClockMode,NormalClockMode,HighClockMode}devmng_clockmode;
//�N���b�N���[�h��ύX����
void devmng_clockmode_set(devmng_clockmode ClockMode_);
//�N���b�N���[�h���擾����
devmng_clockmode devmng_clockmode_get(void);

//=== �X���[�v���[�h���� ===
//�X���[�v���[�h�̃p�����[�^���擾����
void devmng_sleep_getInterval(uint16& secNonRem,uint16& secRem);
//�X���[�v���[�h�̃p�����[�^��ݒ肷��
void devmng_sleep_setInterval(uint16 secNonRem, uint16 secRem);
//�X���[�v�ɓ���/�o��ۂ̒ʒm�֐���o�^����
void devmng_sleep_registSleepInformFunction(devmng_vFp_v InformSleep,devmng_vFp_v InformWakeUp);
//�����X���[�v�ɓ���/�o��ۂ̒ʒm�֐���o�^����
void devmng_sleep_registRemInformFunction(devmng_vFp_v InformRem,devmng_vFp_v InformNonRem);

//=== ���[�~���O���[�h���� ===
//���[�~���O���[�h�̃p�����[�^���擾����
void devmng_roaming_getInterval(uint16& secInterval);
//���[�~���O���[�h�̃p�����[�^��ݒ肷��
void devmng_roaming_setInterval(uint16 secInterval);
#
#endif
*/