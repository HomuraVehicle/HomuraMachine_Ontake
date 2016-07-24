#ifndef HMR_SENSOR_INC
#define HMR_SENSOR_INC 112
#
/*
=== hmrSensor ===
�Z���T�[�֘A�֐��ɂ��Ă܂Ƃ߂�����

hmrSensor_v1_00/120925 hmIto
	�Z���T�[�֘A�̊֐����𐮗�
hmrSensor_v1_01/121006 hmIto
	Message��Battery�֐��̂ݑΉ� ������Vref_GND�͕s��
hmrSensor_v1_02/121013 hmIto
	��ʂ芮��
hmrSensor_v1_03/121014 iwahori
	AD_H2S��ADC_H2S�ɕύX
	s_byte,u_int��sint8,uint16�ɕύX
hmrSensor_v1_04/121020 hmIto
	�Ƃ肠�����G���[�C�����m�F���čX�V
	getAveADC���̊֐��Q�̃R�����g�A�E�g������
hmrSensor_v1_05/121020 iwahori
	c�t�@�C����getAveADC()��adc_getAve100()�ɕύX
hmrSensor_v1_06/121021 iwahori
	battery_message(cstring Str)�̖����� return Ans;��ǉ�
	sprite_message()����409�s��cstring_static_construct�̈���(unsigned char)(*BufPtr)��BufPtr�ɕύX
hmrSensor_v1_07/121028 iwahori
	if�̏����̒���cstring_getc(&Ans,0)��cstring_getc(&Str,0)�ɕύX
	�����x�E���C�Z���T�[�̏������֐���ǉ�
	�����x�E���C�Z���T�[�̃��b�Z�[�W�֐���kk06�ɑΉ�
hmrSensor_v1_08/121031 hmIto
	battery_message�֐��̃������j����C��
hmrSensor_v1_09/121101 hmIto
	restart_wdt��p�~�imain-loop�ɏW��j
hmrSprite_v1_10/121101 hmIto
	�J�������x�d����sprite_message2��ǉ��@�ʐM�K�񂪂܂�ŕʕ��Ȃ̂Œ���
hmrSensor_v1_11/121208 iwahori
	�����x�E���C�Z���T�[�̏���������у��b�Z�[�W�֐����C��
hmrSensor_v1_12/121222 iwahori
	Gyro�ǉ��D�Z���T�[�ύX�ɔ���Axel��Compass�̏������֐��ύX
*/
#ifndef HMCLIB_BYTEBASETYPE_INC
#	include"hmCLib_v1_00/bytebase_type.h"
#endif
#ifndef HMR_DEVICECONFIG_INC
#	include"hmrDeviceConfig.h"
#endif
#ifndef HMR_MESSAGE_INC
#	include"hmrMessage.h"
#endif
//***************** GPS ************************************
#define SPICMD_GPSGet 0x00
#define SPICMD_GPSChange 0x01
#define SPICMD_GPSGetChannel 0x02
#define SPICMD_NULL 0xfe
//***************** SpriteCmr *********************
#define SPRITE_PACKSIZE 160 //240 debug_
#define SPRITE_BUF_HEADER 4	//0x20,0x21,size[2]
#define sprite_BufferSize_LARGE 1680 //240 debug_
#define sprite_BufferSize_SMALL 160 //240 debug_
#define sprite_BufferHeader 3
#define sprite_BufferNum 2

//******************* Acc ******************
#define AXEL_ADDRESS 0x19
//****************** compass *********************
#define COMPASS_ADDRESS 0x1E
//****************** Gyro *********************
#define GYRO_ADDRESS 0x69
//**************** Battery *********************************
cstring battery_message(cstring Str);
//************************ GPS 
cstring gps_message(cstring Str);
//************************ CO2 
cstring co2_message(cstring Str);
//*********************** H2S 
cstring h2s_message(cstring Str);
//************************ Acc 
void axel_initialize(void);
cstring axel_message(cstring Str);
//************************ Thermo 
cstring thermo_message(cstring Str);
//************************ InfThermo 
cstring infthermo_message(cstring Str);
//************************ Hygro 
cstring hygro_message(cstring Str);
//************************ Compass 
void compass_initialize(void);
cstring compass_message(cstring Str);
//************************ Gyro
void gyro_initialize(void);
cstring gyro_message(cstring Str);
//************************ Get ADC Ave 100
//uint32 adc_getAve100(unsigned char mode, unsigned char ch);
//************************ SpriteCmr
cstring sprite_message(cstring Str);
cstring sprite_message2(cstring Str);
#
#endif
