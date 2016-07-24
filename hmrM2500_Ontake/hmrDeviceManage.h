#ifndef HMR_DEVICEMANAGE_INC
#define HMR_DEVICEMANAGE_INC 103
#
#ifndef HMR_TYPE_INC
#	include"hmrType.h"
#endif
#ifndef HMR_TASK_INC
#	include"hmrTask.h"
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
v1_03/131026 amby
	���荞�݋��֐��R��ǉ��A���̃N���X�ŊǗ�����
v1_02/131019 hmIto
	clockmode��clock�ɕύX
	isSleep, isRoaming��ǉ�
v1_01/131013 hmIto
	�C���^�[�t�F�[�X��啝�ɕύX������
		���[�h����N���b�N���x�w������O
*/
typedef void(*devmng_vFp_v)(void);

//=== ��{��` ===
#define devmng_RF_BaudRate 9600
#define devmng_RFDebug_BaudRate 38400
#define devmng_MP_BaudRate 38400

//=== ��{����֐� ===
//�f�o�C�X�������֐�
void devmng_initialize(void);
//�f�o�C�X�I�[���֐�
void devmng_finalize(void);
//�f�o�C�X�����I��
void devmng_kill(void);
//���݂̒ʐM�悪�S��d�ʐM�Ή�����Ԃ�
boolian devmng_isFullDuplex(void);

//=== ���[�h����@�\ ===
typedef enum{NormalMode,SleepMode,RoamingMode}devmng_mode;
//�f�o�C�X���[�h��ύX����
void devmng_mode_set(devmng_mode Mode_);
//�f�o�C�X���[�h���擾����
volatile devmng_mode devmng_mode_get(void);

//=== �N���b�N����@�\ ===
typedef enum{LowClock,NormalClock,HighClock}devmng_clock;
//�N���b�N��ύX����
void devmng_clock_set(devmng_clock Clock_);
//�N���b�N���擾����
volatile devmng_clock devmng_clock_get(void);

//=== �^�X�N���� ===
//�^�X�N��V�K�o�^�ł���m�F����
boolian devmng_task_can_start();
//�^�X�N��o�^�i��d�o�^�͉���j
boolian devmng_task_start(sint16 Interval,task_function TaskFp,task_inform TaskInformStopFp);
//�^�X�N��o�^�i��d�o�^�ł��Ă��܂��̂ŁA�댯�j
boolian devmng_task_quick_start(sint16 Interval,task_function TaskFp,task_inform TaskInformStopFp);
//�^�X�N��ݒ�ύX
boolian devmng_task_restart(task_function TaskFp,sint16 Interval);
//�^�X�N���폜
boolian devmng_task_stop(task_function TaskFp);

//=== �X���[�v���[�h���� ===
//�X���[�v���[�h�̃p�����[�^���擾����
void devmng_sleep_getInterval(sint16* secNonRem,sint16* secRem);
//�X���[�v���[�h�̃p�����[�^��ݒ肷��
void devmng_sleep_setInterval(sint16 secNonRem, sint16 secRem);
//���������N�����ɌĂяo�����ʒm�֐���o�^�ł��邩
boolian devmng_sleep_can_registSleepInformFunction();
//���������N�����ɌĂяo�����ʒm�֐���o�^����
boolian devmng_sleep_registSleepInformFunction(devmng_vFp_v InformSleep,devmng_vFp_v InformWakeUp);
//�����X���[�v�ɓ���/�o��ۂ̒ʒm�֐���o�^�ł��邩
boolian devmng_sleep_can_registRemInformFunction();
//�����X���[�v�ɓ���/�o��ۂ̒ʒm�֐���o�^����
boolian devmng_sleep_registRemInformFunction(devmng_vFp_v InformRem,devmng_vFp_v InformNonRem);
//�X���[�v��Ԃ��ǂ������擾����
volatile boolian devmng_sleep_isSleep(void);
//�����X���[�v��Ԃ��ǂ������擾����
volatile boolian devmng_sleep_isRem(void);

//=== ���[�~���O���[�h���� ===
//���[�~���O���[�h�̃p�����[�^���擾����
void devmng_roaming_getInterval(sint16* secInterval);
//���[�~���O���[�h�̃p�����[�^��ݒ肷��
void devmng_roaming_setInterval(sint16 secInterval);
//���[�~���O��Ԃ��ǂ������擾����
volatile boolian devmng_roaming_isRoaming(void);

//=== ���荞�݊֐� ===
//�^�X�N�A�X���[�v�A����у��[�~���O���s�p�֐�
void devmng_interrupt_function(sint16 Interval_);
//���荞�݂�main���[�v�ւ̈Ϗ������֐�
boolian devmng_interrupt_yield(void);

//=== ���荞�ݐ���p�֐�
// ���荞�݋��ς��Ă悢���ǂ����̃`�F�b�N�֐��@�璷�߂��������E�E
boolian devmng_interrupt_can_enable_streamVMC_fget_interrupt(void);
boolian devmng_interrupt_can_disable_streamVMC_fget_interrupt(void);
boolian devmng_interrupt_can_enable_streamVMC_fput_interrupt(void);
boolian devmng_interrupt_can_disable_streamVMC_fput_interrupt(void);

boolian devmng_interrupt_can_enable_streamCMR_fget_interrupt(void);
boolian devmng_interrupt_can_disable_streamCMR_fget_interrupt(void);
boolian devmng_interrupt_can_enable_streamCMR_fput_interrupt(void);
boolian devmng_interrupt_can_disable_streamCMR_fput_interrupt(void);

boolian devmng_interrupt_can_enable_timerCMR_interrupt(void);
boolian devmng_interrupt_can_disable_timerCMR_interrupt(void);
boolian devmng_interrupt_can_enable_timerDevmng_interrupt(void);
boolian devmng_interrupt_can_disable_timerDevmng_interrupt(void);

// main stream ����M���荞�݋��֐�
boolian devmng_interrupt_enable_streamVMC_fget_interrupt(void);
boolian devmng_interrupt_disable_streamVMC_fget_interrupt(void);
boolian devmng_interrupt_enable_streamVMC_fput_interrupt(void);
boolian devmng_interrupt_disable_streamVMC_fput_interrupt(void);

// camera stream ����M���荞�݋��֐�
boolian devmng_interrupt_enable_streamCMR_fget_interrupt(void);
boolian devmng_interrupt_disable_streamCMR_fget_interrupt(void);
boolian devmng_interrupt_enable_streamCMR_fput_interrupt(void);
boolian devmng_interrupt_disable_streamCMR_fput_interrupt(void);

// timer CMR�@����M���荞�݋��֐�
boolian devmng_interrupt_enable_timerCMR_interrupt(void);
boolian devmng_interrupt_disable_timerCMR_interrupt(void);
// timer Device Manager ����M���荞�݋��֐�
boolian devmng_interrupt_enable_timerDevmng_interrupt(void);
boolian devmng_interrupt_disable_timerDevmng_interrupt(void);

#
#endif
