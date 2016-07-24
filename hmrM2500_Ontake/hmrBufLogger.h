#ifndef HMR_BUFbuflogger_INC
#define HMR_BUFbuflogger_INC 100
#

/*
=== hmrBufbuflogger =====================================================
�o�b�t�@���g�p�����^�����K�[

v1_00/140318 hmIto
	buflogger�Ɠ����C���^�[�t�F�[�X+���O��Ԃňꉞ�쐬
*/
#include"hmLib_v3_04/bytebase_type.h"
#include"hmrData.h"

#define buflogger_BUFFERSIZE 12288

// �������֐�(ch�͎������g��ch,�߂�l��SD��detect���s����true)
boolian buflogger_initialize(void);

// �I�[���֐�
void buflogger_finalize(void);

// ���O�\���̊m�F�֐�(������vcom_data���̃f�[�^�T�C�Y�m�F�p)
boolian buflogger_can_write(const odata* pData);

// ���O�\���̊m�F�֐��i�o�C�g�w��o�[�W�����j
boolian buflogger_can_write_bytes(uint16 DataBytes_);

// ���O�֐�(���̒���destruct�����)
boolian buflogger_move_write(odata* mData_);

// �ǂݏo���\���̊m�F�֐�
boolian buflogger_can_read(void);

// �ǂ݂����֐�(���̒���construct�����)
boolian buflogger_move_read(odata* pData_);

//�ǂݎc����byte���擾
uint32 buflogger_size(void);

//buflogger�Ƃ̌݊��p�@��Ɏ��s
boolian buflogger_save_position(void);

//buflogger�Ƃ̌݊��p�@��Ɏ��s
boolian buflogger_load_position(void);

#
#endif
