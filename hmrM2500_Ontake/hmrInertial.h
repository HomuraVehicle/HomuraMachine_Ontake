#ifndef HMR_INERTIAL_INC
#define HMR_INERTIAL_INC 103
#
/*===hmrInertial===
�����q�@���W���[������p
hmrInertial_v0_03/131026 amby
	���̏ꍇ�͓d��ON,OFF���邽�т�I�QC�Ńf�o�C�X������������K�v������̂ŁA
	���̂��߂̊֐�device_initialize ��ǉ������B
hmrInertial_v0_02/131019 iwahori
	�^�X�N�V�X�e���p��task_setup_talk��task_interrupt���쐬
hmrInertial_v1_00/130907 iwahori
	kk08�ڍs�ɔ����C�g�p����I2C���ς��������i2c1,i2c2��ύX�Dinitialize�͊����Ctalk�͂܂�
hmrInertial_v1_00/130622 hmIto
	�قނ�̐ݒu�ʒu�ɑΉ�����悤�Ɏ���ύX
hmrInertial_v0_01/130105 iwahori
	work�֐���setup_lisen��setup_talk�ɕ���
	Gyro��talk�֐�������
	KK07�ł̃Z���T�[�ύX�ɔ���initialze�֐��ƃA�h���X��ύX
hmrInertial_v0_00/121208 hmIto
	��{�֐��쐬
*/
#include"hmLib_v3_04/cstring.h"
#include"hmrType.h"
#ifdef __cplusplus
using hmLib::cstring;
#endif
void inertial_initialize(void);
void inertial_finalize(void);
//******************* Axel ******************
#define AXEL_ADDRESS 0x19
#define AXEL_REG_CTRL 0x20
#define AXEL_REG_DATA 0x28
#define AXEL_REGMODE_SINGLE 0x00
#define AXEL_REGMODE_MULTI 0x80
#define AXEL_I2C_RETRY_NUM 3
void axel_device_initialize(void);// PWR ON �ɂ����Ƃ��ɕK�v�ȏ������֐�
void axel_device_finalize(void);// PWR OFF �ɂ���O�ɕK�v�ȏ������֐�
hmLib_boolian axel_listen(cstring Str);
hmLib_boolian axel_talk(cstring* pStr);
void axel_setup_listen(void);
void axel_setup_talk(void);
void axel_task_setup_talk(void);
sint16 axel_task_interrupt(sint16 interval);
sint16 axel_task_data(sint16 interval);
sint16 axel_task_inform(sint16 interval);
//****************** compass *********************
#define CMPS_ADDRESS 0x1E
#define CMPS_REG_CTRL 0x00
#define CMPS_REG_DATA 0x03
#define CMPS_REG_DATATEMP 0x31
#define CMPS_REGMODE_SINGLE 0x00
#define CMPS_REGMODE_MULTI 0x80
#define CMPS_I2C_RETRY_NUM 3
void compass_device_initialize(void);
void compass_device_finalize(void);
hmLib_boolian compass_listen(cstring Str);
hmLib_boolian compass_talk(cstring* pStr);
void compass_setup_listen(void);
void compass_setup_talk(void);
void compass_task_setup_talk(void);
sint16 compass_task_data(sint16 interval);
sint16 compass_task_inform(sint16 interval);
//=====================Gyro=======================
#define GYRO_ADDRESS 0x69
#define GYRO_REG_CTRL		0x20	//�e��ݒ�p���W�X�^
#define GYRO_REG_DATA		0x28	//XYZ�e2�o�C�g�C�v6byte�̐擪�f�[�^�ǂݏo���p���W�X�^
#define GYRO_REG_FIFOCTRL	0x2E	//FIFO�ݒ�p���W�X�^
#define GYRO_REG_FIFOSRC	0x2F	//FIFO�Ǘ��p���W�X�^
#define gyro_bufsize(FIFOSRC) (FIFOSRC&0x1F)	//FIFO�Ǘ��p���W�X�^����CFIFO���̃f�[�^���𒲂ׂ�A
#define gyro_empty(FIFOSRC) (FIFOSRC&0x20)		//FIFO�Ǘ��p���W�X�^����CFIFO���󂩒��ׂ�
#define gyro_overflow(FIFOSRC) (FIFOSRC&0x40)	//FIFO�Ǘ��p���W�X�^����CFIFO���I�[�o�[�t���[���Ă��邩���ׂ�
#define GYRO_REGMODE_SINGLE	0x00	//REG�ƂƂ���|���Z�Ŏg���@�P�o�C�g��M
#define GYRO_REGMODE_MULTI	0x80	//REG�ƂƂ���|���Z�Ŏg���@�����o�C�g��M
#define GYRO_I2C_RETRY_NUM 3
void gyro_device_initialize(void); // gyro �̓f�t�H���g�Ńp���[�_�E�����[�h�Ȃ̂ŁA�N������֐�
void gyro_device_finalize(void);
hmLib_boolian gyro_listen(cstring Str);
hmLib_boolian gyro_talk(cstring* pStr);
void gyro_setup_listen(void);
void gyro_setup_talk(void);
void gyro_task_setup_talk(void);
sint16 gyro_task_data(sint16 interval);
sint16 gyro_task_inform(sint16 interval);

#
#endif
