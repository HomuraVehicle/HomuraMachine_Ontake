#ifndef HMR_BUFLOGGER_C_INC
#define HMR_BUFLOGGER_C_INC 100
#
#include"hmrBufLogger.h"
#include"hmrUtility.h"

#define x_buflogger_HEADBYTES 3
#define x_buflogger_MAXDATASIZE 128
unsigned char x_buflogger_Buffer[buflogger_BUFFERSIZE]  __attribute__((far));
unsigned char* x_buflogger_Begin;
unsigned char* x_buflogger_End;
uint16 x_buflogger_Size;

//�C���N�������g
unsigned char*  x_buflogger_next(unsigned char* Itr) {
	return (Itr==x_buflogger_Buffer+buflogger_BUFFERSIZE-1?x_buflogger_Buffer:Itr+1);
}
//�f�N�������g
unsigned char* x_buflogger_prev(unsigned char* Itr) {
	return (Itr==x_buflogger_Buffer?x_buflogger_Buffer+buflogger_BUFFERSIZE-1:Itr-1);
}

// �������֐�(ch�͎������g��ch,�߂�l��SD��detect���s����true)
boolian buflogger_initialize(void) {
	x_buflogger_Begin=x_buflogger_Buffer;
	x_buflogger_End=x_buflogger_Buffer;
	x_buflogger_Size=0;
}

// �I�[���֐�
void buflogger_finalize(void) {
	x_buflogger_Begin=x_buflogger_Buffer;
	x_buflogger_End=x_buflogger_Buffer;
	x_buflogger_Size=0;
	return hmLib_false;
}

// ���O�\���̊m�F�֐�(������vcom_data���̃f�[�^�T�C�Y�m�F�p)
boolian buflogger_can_write(const odata* pData) {
	return buflogger_can_write_bytes(cstring_size(&(pData->Str)));
}

// ���O�\���̊m�F�֐��i�o�C�g�w��o�[�W�����j
boolian buflogger_can_write_bytes(uint16 DataBytes_) {
	return (x_buflogger_Size+DataBytes_+x_buflogger_HEADBYTES<buflogger_BUFFERSIZE-1);
}

// ���O�֐�(���̒���destruct�����)
boolian buflogger_move_write(odata* mData_) {
	uint16 Size;
	uint16 Cnt;
	//�������߂Ȃ��Ƃ͂���
	if(buflogger_can_write(mData_)==0)return hmLib_true;

	//�T�C�Y�擾
	Cnt=0;
	Size=cstring_size(&(mData_->Str));

	//�T�C�Y����𒴂�����̂͂͂���
	if(Size>x_buflogger_MAXDATASIZE)return hmLib_true;

	//���M�s�\�̎����͂���
	if(mData_->FpSendable && mData_->FpSendable()!=Size)return hmLib_true;

	//ID���܂���������
	*x_buflogger_End=mData_->ID;
	x_buflogger_End=x_buflogger_next(x_buflogger_End);

	//Size����������
	*x_buflogger_End=(uint8)(Size);
	x_buflogger_End=x_buflogger_next(x_buflogger_End);
	*x_buflogger_End=(uint8)(Size>>8);

	//Str����������
	for(Cnt=0; Cnt<Size; ++Cnt) {
		*x_buflogger_End=cstring_getc(&(mData_->Str), Cnt);
		x_buflogger_End=x_buflogger_next(x_buflogger_End);
	}
	//Destruct
	cstring_destruct(&(mData_->Str));

	//Size�����Z���Ă���
	x_buflogger_Size+=x_buflogger_HEADBYTES+Size;

	//destruct�ʒm
	if(mData_->FpInformDestruct)mData_->FpInformDestruct();

	return hmLib_false;
}

// �ǂݏo���\���̊m�F�֐�
boolian buflogger_can_read(void) {
	return x_buflogger_Size>0;
}

// �ǂ݂����֐�(���̒���construct�����)
boolian buflogger_move_read(odata* pData_) {
	uint16 Size;
	uint16 Cnt;

	//�ǂݏo���Ȃ��Ƃ͂���
	if(buflogger_can_read()==0)return hmLib_true;

	//1byte�ڂ�ID
	pData_->ID=*x_buflogger_Begin;
	x_buflogger_Begin=x_buflogger_next(x_buflogger_Begin);

	//2,3byte�ڂ�Size
	Size=(uint16)(*x_buflogger_Begin);
	x_buflogger_Begin=x_buflogger_next(x_buflogger_Begin);
	Size+=((uint16)(*x_buflogger_Begin)<<8);
	x_buflogger_Begin=x_buflogger_next(x_buflogger_Begin);

	if(Size>x_buflogger_MAXDATASIZE) {
		x_buflogger_Size=0;
		x_buflogger_Begin=0;
		x_buflogger_End=0;
		return hmLib_true;
	}

	//Construct
	cstring_construct_dynamic_safe(&(pData_->Str), Size);

	//Str�f�[�^�ǂݏo��
	for(Cnt=0; Cnt<Size; ++Cnt) {
		cstring_putc(&(pData_->Str), Cnt, *x_buflogger_Begin);
		x_buflogger_Begin=x_buflogger_next(x_buflogger_Begin);
	}

	//Size�����Z���Ă���
	x_buflogger_Size-=x_buflogger_HEADBYTES+Size;

	//�ق���odata������
	pData_->FpInformDestruct=0;
	pData_->FpSendable=0;

	return hmLib_false;
}

//�ǂݎc����byte���擾
uint32 buflogger_size(void) {
	return x_buflogger_Size;
}

//logger�Ƃ̌݊��p�@��Ɏ��s
boolian buflogger_save_position(void) {
	return hmLib_true;
}

//logger�Ƃ̌݊��p�@��Ɏ��s
boolian buflogger_load_position(void) {
	return hmLib_true;
}
#
#endif
