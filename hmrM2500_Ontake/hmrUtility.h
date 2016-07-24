#ifndef HMR_UTILITY_INC
#define HMR_UTILITY_INC 101
#
#ifndef HMRLIB_CSTRING_INC
#	include"hmLib_v3_04/cstring.h"
#endif
#ifndef HMLIB_BYTEBASETYPE_INC
#	include "hmLib_v3_04/bytebase_type.h"
#endif
#include"hmrType.h"
/*
===hmrUtilities===
�֗��Ȋ֐��̒񋟂��s��
v1_01/131019 hmIto
	cstring�����荞�݈��S�ɓ��I����&�j������֐��Q��ǉ�
		cstring_construct_dynamic_safe
		cstring_destruct_safe
v1_00/131019 amby
	���܂��
	���b�N�R�[�h�����֐��Alockcode��ǉ�
*/
//���b�N�R�[�h�����֐�
unsigned char lockcode(void);
//���荞�݈��S��cstring���I�����֐�
void cstring_construct_dynamic_safe(cstring* str,cstring_size_t size);
//���荞�݈��S��cstring�j���֐�
void cstring_destruct_safe(cstring* str);

// ���̊��荞�݂��֎~����A�r���I delay �֐�
void exclusive_delay_ms(uint16 time);

#
#endif


















