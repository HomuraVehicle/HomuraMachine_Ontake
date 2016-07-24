#ifndef HMR_SPRITEBUFFER_INC
#define HMR_SPRITEBUFFER_INC 16
#
/*
===hmrSpriteBuffer===
v1_00/131019 hmIto
	hmrSprite����o�b�t�@�Ǘ��̈�̂ݕ���

===hmrSprite===
v0_15/130727 iwahori
	�e�f�[�^�o�b�t�@��talk�p�Ƀt���O������D�A���B�e��minipacket���Ƃ���Ȃ�̕p�x�Ń��������[�N��������o�O�͕ς�炸�B�ʏ�p�P�b�g�ł����Ȃ�҂ƃ��������[�N��������
v0_15/130713 iwahori
	�A���B�e�͂ł��邪�A���ʂ�Take���ł��Ȃ��D���ʂ�Take�̂Ƃ���listen����_sprite_buf_empty()���ۂ��Ă���̂ɑ΂��āA
	AutoTakePic�ł�setup_talk��!_sprite_buf_full()���ۂ��Ă��邠���肪���������H
v0_15/130712 iwahori
	end_TakePic���ł��Ȃ��o�O���C���Aend_TakePic�̑O��spriteitf_clear_ReadPicDatFlag�����Ă��Ȃ������̂�����
v0_14/130706 iwahori
	�J��������f�[�^����M���Ȃ���CTR�ɑ��M�ł���悤�ɕύX�D2��ڈȍ~�̎B�e�����܂������Ȃ��Dend_TakePic�ł��Ă��Ȃ��H
	TalkNo�̈��S���u��_sprite_fnFinReadPicData�ɗv��
v0_13/130706 hmIto
	PicInfo�̃f�[�^�𑗂�ۂɁAwhile�ő��M�\�ƂȂ�܂ő҂��Ă��������C��
		���炽�ɑ��M�\�ɂȂ������Ƃ�ʒm����ϐ���p�ӂ��Atalk_setup�ŗ����グ��悤�ɂ���
v0_12/130622 iwahori
	�o�O���C���B����ɓ����悤�ɂȂ���
v0_11/130615 iwahori
	ReadPic���inform�֐����g�p����悤�ɕύX�D
	2��ڈȍ~�̎ʐ^�擾�����s����D�������Ԃ��Ă��Ȃ�
v0_06/130427 iwahori
	�V�ʐM�`���ɑΉ�
v0_06/130406 iwahori
	���掿��2���擾�����D�~�j�p�P�b�g���[�h�ł��ʐ^�擾�ƘA���B�e���`�F�b�N����
v0_06/130406 iwahori
	�ŏ��T�C�Y�̎ʐ^�擾/�A���擾�ɐ����D
	���掿��2�͂ł��Ă��Ȃ��D�ʐ^�f�[�^�̃A�h���X�̎w����ԈႦ�Ă��邩��
v0_06/130310 iwahori
	SpriteInterface�ɑΉ�����
	�f�o�b�O��ƒ��C�p���[���Z�b�g�@�\�̓���m�F�͊���
	�ʐ^�f�[�^�擾�͂��܂������ĂȂ��C�����͂܂��s��
	�p���[���Z�b�g��ɂȂ����ʐM�^�C���A�E�g�̃G���[���͂��Ă���
v0_06/130211 hmIto
	
v0_06/130211 iwahori
	_sprite_buf_requestLock()��while����++i����Ă��Ȃ������̂Œǉ�
	sprite_setup_talk()���̘A���B�e���[�h�ŃJ�����T�C�Y��V����̃f�[�^��ϊ����Ă��Ȃ������̂ŕύX
	_sprite_buf_full()��true false�����O�Ƌt�ɂȂ��Ă����̂ł𔽓]�A����ɔ���sprite_setup_talk()�̕�������]
v0_05/130127 iwahori
	_sprite_is_BufAllUsed��return�l�����
v0_04/130127 iwahori
	�V�ʐM�K��ɑΉ�
v0_03/130112 iwahori
	sprite_setup_talk�֐�����GetPicture�t���O�������������ύX
v0_02/130105 iwahori
	�ʐ^�擾���[�h�������C��
v0_01/121221 iwahori
	�S�̓I�ɉ��M
v0_00/121216 iwahori
	��{�֐��쐬
*/
#include"hmLib_v3_04/cstring.h"
#ifndef HMR_TYPE_INC
#	include"hmrType.h"
#endif
#ifndef HMR_MESSAGE_INC
#	include"hmrMessage.h"
#endif

#define spritebuf_Num 3	//Debug 2
#define spritebuf_DataSize 4088		//8�̔{��
#define spritebuf_DataSize_Small 240	//8�̔{��
#define spritebuf_HeaderSize 3

//typedef�֘A
typedef uint8 spritebuf_id;
typedef msg_vFp_v spritebuf_vFp_v;
typedef msg_dsizeFp_v spritebuf_dsizeFp_v;

//�o�b�t�@�֘A������
void spritebuf_initialize(void);
//�o�b�t�@�֘A�I�[��
void spritebuf_finalize(void);
//�o�b�t�@���m�ۂ���
spritebuf_id spritebuf_create(uint16 DataSize);
//�o�b�t�@���������
void spritebuf_release(spritebuf_id ID);
//�o�b�t�@�����b�Z�[�W�֐��Ƌ��L����
//	���̊֐����Ă΂ꂽ�ꍇ�A�ȉ��̂����ꂩ�̏������K�v
//	1.FpInformDestruct�ƂƂ��Ƀf�[�^��Message�ɓo�^����
//	2.FpInformDestruct�����s����
void spritebuf_share_withMesasge(spritebuf_id BufferNo,cstring* pStr,uint16 PicDataPos,spritebuf_dsizeFp_v* pFpSendableNum,spritebuf_vFp_v* pFpInformDestruct);
//�o�b�t�@��SpriteInteface�֐��Ƌ��L����
//	���̊֐����Ă΂ꂽ�ꍇ�A�ȉ��̏������K�v
//	1.spritebuf_release_fromSpriteInterface
void spritebuf_share_withSpriteInterface(spritebuf_id ID, unsigned char** pPtr, uint16** pCntPtr);
//�o�b�t�@��SpriteInteface�֐�����������
void spritebuf_release_fromSpriteInterface(spritebuf_id ID);

#
#endif
