#ifndef HMR_DATA_INC
#define HMR_DATA_INC 102
#
/*===hmrData===
hmr��Com/Message�ȂǂŎg����ʐM�p�f�[�^�^�̐錾

hmrData:v1_02/130928 hmIto
	inform_destruct�@�\��odata_destruct���Ŏ��s����悤�ɕύX
hmrData:v1_01/130216 hmIto
	EDS�̈�ւ̃A�N�Z�X�ɑΉ�
		DSRPAG��odata�ɒǉ�
		construct�֐���ύX
hmrData:v1_00/130105 hmIto
	hmrCom����f�[�^�^�֘A�̐錾�̖�����
*/
// idata �̃R���X�g���N�^�A�Z�b�e�B���O�֐������
//  �P�Ƀ��b�s���O���邾���B
//�@������
#ifndef HMLIB_CSTRING_INC
#	include"hmLib_v3_04/cstring.h"
#endif
#ifndef HMR_VMC_INC
#	include"hmrVMC1.h"
#endif
#ifdef __cplusplus
using hmLib::cstring;
#endif
//�f�[�^�^�̒�`
typedef vmc1_dsize_t data_dsize_t;
typedef vmc1_did_t data_did_t;
typedef data_dsize_t(*data_dsizeFp_v)(void);
typedef void(*data_vFp_v)(void);
//��M�̃f�[�^�^
typedef struct{
	data_did_t ID;
	cstring Str;
}idata;
//���M�̃f�[�^�^
typedef struct{
	data_did_t ID;
	cstring Str;
	int DSRPAG;			//EDS�̈�Ŋm�ۂ����ꍇ�A�m�ۗ̈��PageFile���L�^���Ă���
	data_dsizeFp_v FpSendable;
	data_vFp_v FpInformDestruct;
}odata;
//�֐��Q
void idata_format(idata* Dat);
hmLib_boolian idata_is_construct(idata*Dat);
void idata_construct(idata* Dat, data_did_t ID, cstring* mStr);
void idata_destruct(idata* Dat);
void idata_move(idata* From, idata* To);

void odata_format(odata* Dat);
hmLib_boolian odata_is_construct(odata*Dat);
void odata_construct(odata* Dat, data_did_t ID, cstring* mStr, int DSRPAG,
					 data_dsizeFp_v FpSendable, data_vFp_v FpInformDestruct);
void odata_destruct(odata* Dat);
void odata_move(odata* From, odata* To);

#
#endif
