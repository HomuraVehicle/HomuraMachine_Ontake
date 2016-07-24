#ifndef HMR_SPRITE_INC
#define HMR_SPRITE_INC 16
#
/*===hmrSprite===
�J�����֘A���W���[��
ToDo:talk1,talk2,�c(���܂��Ă���f�[�^��������C��talk)
hmrSprite_v0_15/130727 iwahori
	�e�f�[�^�o�b�t�@��talk�p�Ƀt���O������D�A���B�e��minipacket���Ƃ���Ȃ�̕p�x�Ń��������[�N��������o�O�͕ς�炸�B�ʏ�p�P�b�g�ł����Ȃ�҂ƃ��������[�N��������
hmrSprite_v0_15/130713 iwahori
	�A���B�e�͂ł��邪�A���ʂ�Take���ł��Ȃ��D���ʂ�Take�̂Ƃ���listen����_sprite_buf_empty()���ۂ��Ă���̂ɑ΂��āA
AutoTakePic�ł�setup_talk��!_sprite_buf_full()���ۂ��Ă��邠���肪���������H
hmrSprite_v0_15/130712 iwahori
	end_TakePic���ł��Ȃ��o�O���C���Aend_TakePic�̑O��spriteitf_clear_ReadPicDatFlag�����Ă��Ȃ������̂�����
hmrSprite_v0_14/130706 iwahori
	�J��������f�[�^����M���Ȃ���CTR�ɑ��M�ł���悤�ɕύX�D2��ڈȍ~�̎B�e�����܂������Ȃ��Dend_TakePic�ł��Ă��Ȃ��H
	TalkNo�̈��S���u��_sprite_fnFinReadPicData�ɗv��
hmrSprite_v0_13/130706 hmIto
	PicInfo�̃f�[�^�𑗂�ۂɁAwhile�ő��M�\�ƂȂ�܂ő҂��Ă��������C��
		���炽�ɑ��M�\�ɂȂ������Ƃ�ʒm����ϐ���p�ӂ��Atalk_setup�ŗ����グ��悤�ɂ���
hmrSprite v0_12/130622 iwahori
	�o�O���C���B����ɓ����悤�ɂȂ���
hmrSprite v0_11/130615 iwahori
	ReadPic���inform�֐����g�p����悤�ɕύX�D
	2��ڈȍ~�̎ʐ^�擾�����s����D�������Ԃ��Ă��Ȃ�
hmrSprite_v0_06/130427 iwahori
	�V�ʐM�`���ɑΉ�
hmrSprite_v0_06/130406 iwahori
	���掿��2���擾�����D�~�j�p�P�b�g���[�h�ł��ʐ^�擾�ƘA���B�e���`�F�b�N����
hmrSprite_v0_06/130406 iwahori
	�ŏ��T�C�Y�̎ʐ^�擾/�A���擾�ɐ����D
	���掿��2�͂ł��Ă��Ȃ��D�ʐ^�f�[�^�̃A�h���X�̎w����ԈႦ�Ă��邩��
hmrSprite_v0_06/130310 iwahori
	SpriteInterface�ɑΉ�����
	�f�o�b�O��ƒ��C�p���[���Z�b�g�@�\�̓���m�F�͊���
	�ʐ^�f�[�^�擾�͂��܂������ĂȂ��C�����͂܂��s��
	�p���[���Z�b�g��ɂȂ����ʐM�^�C���A�E�g�̃G���[���͂��Ă���
hmrSprite_v0_06/130211 hmIto
	
hmrSprite_v0_06/130211 iwahori
	_sprite_buf_requestLock()��while����++i����Ă��Ȃ������̂Œǉ�
	sprite_setup_talk()���̘A���B�e���[�h�ŃJ�����T�C�Y��V����̃f�[�^��ϊ����Ă��Ȃ������̂ŕύX
	_sprite_buf_full()��true false�����O�Ƌt�ɂȂ��Ă����̂ł𔽓]�A����ɔ���sprite_setup_talk()�̕�������]
hmrSprite_v0_05/130127 iwahori
	_sprite_is_BufAllUsed��return�l�����
hmrSprite_v0_04/130127 iwahori
	�V�ʐM�K��ɑΉ�
hmrSprite_v0_03/130112 iwahori
	sprite_setup_talk�֐�����GetPicture�t���O�������������ύX
hmrSprite_v0_02/130105 iwahori
	�ʐ^�擾���[�h�������C��
hmrSprite_v0_01/121221 iwahori
	�S�̓I�ɉ��M
hmrSprite_v0_00/121216 iwahori
	��{�֐��쐬
*/
#include"hmLib_v3_04/cstring.h"
#include"hmLib_v3_04/bytebase_type.h"
#ifndef HMR_DEVICE_INC
#	include"hmrDevice.h"
#endif
#ifndef HMR_SPRITEINTERFACE_INC
#	include"hmrSpriteInterface.h"
#endif
#ifndef HMR_MESSAGE_INC
#	include"hmrMessage.h"
#endif
#ifdef __cplusplus
using hmLib::cstring;
#endif
typedef void(*sprite_vFp_v)(void);
typedef msg_dsizeFp_v sprite_dsizeFp_v;
//#define SPRITE_PACKSIZE 160 //240 debug_
//#define SPRITE_BUF_HEADER 4	//0x20,0x21,size[2]

//Message Agent�̃X�e�[�^�X�̒�`
#define _sprite_Status_Idle				0x00
#define _sprite_Status_ReadPicInfo		0x13
#define _sprite_Status_ReadPicData		0x20
#define _sprite_Status_EndTakePic		0x30
#define _sprite_Status_CommandReset		0x40
#define _sprite_Status_PowerReset		0xF0


hmLib_boolian sprite_listen(cstring Str);
hmLib_boolian sprite_talk(cstring* pStr);
void sprite_setup_listen(void);
void sprite_setup_talk(void);
void sprite_initialize(void);
void sprite_finalize(void);
sint16 sprite_task_inform(sint16 interval);
boolian _sprite_itf_readPicData(void);
#
#endif
