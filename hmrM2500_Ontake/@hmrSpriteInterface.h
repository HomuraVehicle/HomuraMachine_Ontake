#ifndef HMR_SPRITEINTERFACE_INC
#define HMR_SPRITEINTERFACE_INC 106
#
/*
===hmrSpriteInteface===
SpriteJpegCmr�̏����ɕK�v�Ȋ֐��Q���܂Ƃ߂�����
hmrSpriteInterface:v1_05/130706 iwahori
	�������Z�b�g��A�ʐM�^�C���A�E�g�G���[���o�Ȃ��Ȃ�o�O���C��.�������Z�b�g��̏������Y��
hmrSpriteInterface:v1_02/130406 iwahori
	�������Z�b�g�̃o�O���C���C�J��������̎�M���ł��Ȃ��o�O���C���C�ŏ��T�C�Y�̎ʐ^�擾/�A���擾�ɐ���
hmrSpriteInterface:v1_02/130310 iwahori
	�I�[�g�p���[���Z�b�g�@�\��ǉ��C���̂ق����C��
hmrSpriteInterface:v1_02/130302 iwahori
	�K��̔��C���ɔ����A���J�֐����C���A�p���[���Z�b�g�A�G���[�����A�^�C���A�E�g�@�\������
	�̂���F�I�[�g�p���[���Z�b�g
	�I�[���֐��őS���[���ɂ����肵�Ă��邩��p���[���Z�b�g�ŏI�[���֐������̂܂܂���Ȃ���������
	_spriteitf_fnPowerReset�ł̏��������v�m�F
	_spriteitf_timer_enable�A_spriteitf_timer_interrupt_enable��define�������A���̕ӂ��v�m�F
hmrSpriteInterface:v1_01/130223 iwahori
	���J�֐����쐬
	�̂���F�G���[���������J�֐��ȂǂɎ����A�p���[���Z�b�g�A�I�[�g���Z�b�g
	_spriteitf_informTakePic�A_spriteitf_informReadPicDat�͂��o�^����H
	StopTakePic����ƃf�[�^�擾���ł��Ȃ��Ȃ邩�v�m�F
hmrSpriteInterface:v1_00/130211 hmIto
	hmrSpriteFunctions_v1_05���p��
	�֐��Q�𐮗�
hmrSpriteFunctions:v1_05/121101 hmIto
	256byte�ȏ�̃f�[�^����x�Ɏ擾�ł���悤�ɑΉ�
		dsize�^�Aboolian�^�Aimagetype�^�Aboaudrate�^�ȂǁA�Ӗ��ɉ������^��typedef
		�֐��|�C���^�̌^�����ύX
		�g���Ă��Ȃ������֐��|�C���^�������ɐ���
hmrSpriteFunctions:v1_04/121031 hmIto
	Intr_ini�֐�����const�ϐ����������Ă����o�O���C��
hmrSpriteFunctions:v1_03/121028 hmIto
	Stream����sprite_Stream����Stream_CMR�ɕύX
	initialize�֐���power�֐��𐧌䂵�Ȃ��悤�ɕύX
		�������Apower_reset�֘A�̊֐��Q�͗��p���Ă���̂Œ���
	���荞�݊֐���disable����finalize�Ŏ��s����悤�ύX
		enable���́A�K�v�ƂȂ������_��sprite�֐����Ő������s�����
hmrSpriteFunctions:v1_02/121020 iwahori
	hmrSensor��sprite_getRequestImageSize()�Ȃ�֐���K�v�Ƃ��Ă���̂łƂ肠�����ǉ�
hmrSpriteFunctions:v1_01/121014 iwahori
	sprite_ON()�폜
	p_LED_YELLOW��sprite_setLED�ɕύX
hmrSpriteFunctions:v1_00/121013 hmIto
	hmrSpriteIntr�����ɁASprite�֌W�̊֐��Q�̂ݕ����J�n
	�Ƃ肠��������
*/	
#ifndef HMR_DEVICECONFIG_INC
#	include "hmrDeviceConfig.h"
#endif
#ifndef HMCLIB_BYTEBASETYPE_INC
#	include "hmLib_v3_04/bytebase_type.h"
#endif
//�y�ݒ�z�ʐM�r�┻�f����
#define spriteitf_RecvTimeOut 300	//ms(500�܂�)

//�T�C�Y�^�̒�`
typedef uint16 spriteitf_dsize_t;

//�e�X�e�[�^�X�̒�`
typedef uint8 spriteitf_status_t;
#define spriteitf_Status_Idle				0x00
#define spriteitf_Status_SetImageSize		0x10
#define spriteitf_Status_ActiveImageSize	0x11
#define spriteitf_Status_TakePic			0x12
#define spriteitf_Status_ReadPicInfo		0x13
#define spriteitf_Status_ReadPicData		0x20
#define spriteitf_Status_EndTakePic			0x30
#define spriteitf_Status_CommandReset		0x40
#define spriteitf_Status_PowerReset			0xF0

//Error
typedef uint8 spriteitf_error_t;
//���ʃr�b�g�}�X�N
#define spriteitf_error_LowBitMask									0xF0
//�ʐ^��b�f�[�^��M�������̃G���[
#define spriteitf_error_TakePic										0x10
//�ʐ^��b�f�[�^��M�������A�r���ŕςȃX�e�[�^�X�ɂȂ���
#define spriteitf_error_TakePic_invalidStatus_Idle					0x11
#define spriteitf_error_TakePic_invalidStatus_ReadPicData			0x12
#define spriteitf_error_TakePic_invalidStatus_EndTakePic			0x13
#define spriteitf_error_TakePic_invalidStatus_CommandReset			0x14
#define spriteitf_error_TakePic_invalidStatus_PowerReset			0x15
//�ʐ^��b�f�[�^��M�������A�J�����ւ̃R�}���h���M�����ۂ��ꂽ
#define spriteitf_error_TakePic_cannotsend_SetImageSize				0x16
#define spriteitf_error_TakePic_cannotsend_ActiveImageSize			0x17
#define spriteitf_error_TakePic_cannotsend_TakePic					0x18
#define spriteitf_error_TakePic_cannotsend_ReadPicInfo				0x19

//�ʐ^�f�[�^�ǂݍ��ݏ������̃G���[
#define spriteitf_error_ReadPicData									0x20
//�ʐ^�f�[�^�ǂݍ��ݏ������A�r���ŕςȃX�e�[�^�X�ɂȂ���
#define spriteitf_error_ReadPicData_invalidStatus_Idle				0x21
#define spriteitf_error_ReadPicData_invalidStatus_SetImageSize		0x22
#define spriteitf_error_ReadPicData_invalidStatus_ActiveImageSize	0x23
#define spriteitf_error_ReadPicData_invalidStatus_TakePic			0x24
#define spriteitf_error_ReadPicData_invalidStatus_ReadPicInfo		0x25
#define spriteitf_error_ReadPicData_invalidStatus_EndTakePic		0x26
#define spriteitf_error_ReadPicData_invalidStatus_CommandReset		0x27
#define spriteitf_error_ReadPicData_invalidStatus_PowerReset		0x28
//�ʐ^�f�[�^�ǂݍ��ݎ��������A�J�����ւ̃R�}���h���M�����ۂ��ꂽ
#define spriteitf_error_ReadPicData_cannotsend_ReadPicData			0x29

//�B�e�I���������̃G���[
#define spriteitf_error_EndTakePic									0x30
//�B�e�I���������A�r���ŕςȃX�e�[�^�X�ɂȂ���
#define spriteitf_error_EndTakePic_invalidStatus_Idle				0x31
#define spriteitf_error_EndTakePic_invalidStatus_SetImageSize		0x32
#define spriteitf_error_EndTakePic_invalidStatus_ActiveImageSize	0x33
#define spriteitf_error_EndTakePic_invalidStatus_TakePic			0x34
#define spriteitf_error_EndTakePic_invalidStatus_ReadPicInfo		0x35
#define spriteitf_error_EndTakePic_invalidStatus_ReadPicData		0x36
#define spriteitf_error_EndTakePic_invalidStatus_CommandReset		0x37
#define spriteitf_error_EndTakePic_invalidStatus_PowerReset			0x38
//�B�e�I���������A�J�����ւ̃R�}���h���M�����ۂ��ꂽ
#define spriteitf_error_EndTakePic_cannotsend_EndPicData			0x39

//Command Reset���ł̃G���[
#define spriteitf_error_CommandReset								0x40
//�R�}���h���Z�b�g�������A�r���ŕςȃX�e�[�^�X�ɂȂ���
#define spriteitf_error_CommandReset_invalidStatus_Idle				0x41
#define spriteitf_error_CommandReset_invalidStatus_SetImageSize		0x42
#define spriteitf_error_CommandReset_invalidStatus_ActiveImageSize	0x43
#define spriteitf_error_CommandReset_invalidStatus_TakePic			0x44
#define spriteitf_error_CommandReset_invalidStatus_ReadPicInfo		0x45
#define spriteitf_error_CommandReset_invalidStatus_ReadPicData		0x46
#define spriteitf_error_CommandReset_invalidStatus_EndTakePic		0x47
#define spriteitf_error_CommandReset_invalidStatus_PowerReset		0x48
//�R�}���h���Z�b�g�������A�J�����ւ̃R�}���h���M�����ۂ��ꂽ
#define spriteitf_error_CommandReset_cannotsend_CommandReset		0x49

//�J�����Ƃ̒ʐM�^�C���A�E�g
#define spriteitf_error_TimeOut										0x50
#define spriteitf_error_TimeOut_SetImageSize						0x51
#define spriteitf_error_TimeOut_TakePic								0x52
#define spriteitf_error_TimeOut_GetDataSize							0x53
#define spriteitf_error_TimeOut_GetData								0x54
#define spriteitf_error_TimeOut_StopTakePic							0x55
#define spriteitf_error_TimeOut_CommandReset						0x56
#define spriteitf_error_TimeOut_unUsedCommand						0x57


//Picture�w��p
typedef uint8 spriteitf_imagesize_t;
#define spriteitf_PictureSize_160_120 0x22//          0  
#define spriteitf_PictureSize_320_240 0x11//imagesize 1
#define spriteitf_PictureSize_640_480 0x00//          2 

//Baudrate�w��p
typedef uint16 spriteitf_baudrate_t;
#define spriteitf_BaudRate_9600 0xAEC8
#define spriteitf_BaudRate_19200 0x56E4
#define spriteitf_BaudRate_38400 0x2AF2
#define spriteitf_BaudRate_57600 0x1C4C
#define spriteitf_BaudRate_115200 0x0DA6

//�R�}���h�\����
typedef struct{
	unsigned char* Ptr;			//���M����R�}���h
	spriteitf_dsize_t Size;	//���M����R�}���h�̃T�C�Y
	spriteitf_dsize_t InSize;	//��M����R�}���h�̃T�C�Y
}spriteitf_command;

//���荞�݃R�}���h�֐��|�C���^
typedef void (*spriteitf_vFp_v) (void);
//typedef void (*spriteitf_vFp_uc_uc)(unsigned char,unsigned char);
typedef boolian (*spriteitf_bFp_uc_dsize)(unsigned char,spriteitf_dsize_t);
typedef void (*spriteitf_vFp_ucp)(unsigned char*);
//typedef void (*spriteitf_vFp_ui)(unsigned int);
//typedef unsigned char*(*spriteitf_ucpFp_uc)(unsigned char);
//typedef void (*spriteitf_vFp_uc)(unsigned char);

//�������֐�
void spriteitf_initialize(void);
//�I�[���֐�
void spriteitf_finalize(void);

//***Status ��ԑJ�ڕϐ���`***//
//===Status::public:
//Status�`�F�b�N�֐�
spriteitf_status_t spriteitf_getStatus(void);
//��Ԃ��ҋ@��Ԓ����H
boolian spriteitf_isIdle(void);

//***Data �J�����f�[�^***//
//===Data::public:
//�ʐ^�B�e�J�n�v�� �G���[���ɖ߂�l1
boolian spriteitf_beginTakePic(spriteitf_imagesize_t ImageSize_,spriteitf_vFp_v fnInformTakePic);
//�ʐ^�B�e�I���v�� �G���[���ɖ߂�l1
boolian spriteitf_endTakePic(spriteitf_vFp_v fnInformEndTakePic);
//�ʐ^�f�[�^�ǂݍ��ݗv�� �G���[���ɖ߂�l1 ,BufPtr:�f�[�^����Ăق����ꏊ,size_ptr_:�������݊��������f�[�^�������ꏊ,begin:�ʐ^�f�[�^�̓ǂݏo���J�n�ʒu�i8�̔{���j,size:begin����ǂݏo���f�[�^��
boolian spriteitf_beginReadPicDat(unsigned char* BufPtr_,spriteitf_dsize_t begin_,spriteitf_dsize_t* size_ptr_,spriteitf_dsize_t size_,spriteitf_vFp_v fnInformReadPicData);
//Request����ImageSize���擾����֐�
spriteitf_imagesize_t spriteitf_getRequestImageSize(void);
//�ʐ^�S�f�[�^�T�C�Y��v���\���H
boolian spriteitf_can_getPicInfo(void);
//�ʐ^�S�f�[�^�T�C�Y�v��
spriteitf_dsize_t spriteitf_getAllPicDataSize(void);
//�ʐ^�f�[�^�p�o�b�t�@�Ƀf�[�^�͂��ׂė��܂��Ă��邩�H
boolian spriteitf_can_getPicData();
//�ʐ^�f�[�^�p�o�b�t�@�T�C�Y�v��
spriteitf_dsize_t spriteitf_sizePicData();
//�ʐ^��b�f�[�^��M�����t���O���N���A
void spriteitf_clear_TakePicFlag(void);
//�f�[�^��M�����t���O���N���A
void spriteitf_clear_ReadPicDatFlag(void);
//�ʐ^�B���Ă������H
boolian spriteitf_can_TakePic(void);
//�ʐ^�ǂݍ��ݍς݃f�[�^���擾
uint16 spriteitf_getReadDataNum(void);

//***Reset ���Z�b�g����***//
//===Reset::public:
//���Z�b�g�v���֐��@�G���[���ɖ߂�l1
boolian spriteitf_CommandReset(void);
//�p���[���Z�b�g�v��
boolian spriteitf_PowerReset(void);
//�I�[�g���Z�b�g���[�h�̗L��
boolian spriteitf_getAutoResetMode(void);
//�I�[�g���Z�b�g���[�h�ɂ���
void spriteitf_setAutoResetMode(boolian val);

//***Error �G���[����***//
//===Error::public:
//Error��Ԃ��m�F
boolian spriteitf_isError(void);
//ErrorCode�擾�֐�
spriteitf_error_t spriteitf_getErrorCode(void);
//ErrorCode�N���A
void spriteitf_clearErrorCode(void);
#
#endif
