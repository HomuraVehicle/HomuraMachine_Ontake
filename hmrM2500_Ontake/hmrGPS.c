#ifndef HMR_GPS_C_INC
#define HMR_GPS_C_INC 3
#
#define HMLIB_NOLIB
/*===hmrGPS===
GPS�֘A���W���[��
hmrGPS_v0_01/141004 hmIto
	GPS�̓d���Ǘ��֐���ǉ�
hmrGPS_v0_00/121208 hmIto
	��{�֐��쐬
*/
#ifndef HMR_GPS_INC
#	include"hmrGPS.h"
#endif
#ifndef HMR_DEVICE_INC
#	include"hmrDevice.h"
#endif
#ifndef HMR_UTILITY_INC
#	include "hmrUtility.h"
#endif
//***************** GPS ************************************
#define SPICMD_GPSGet 0x00
#define SPICMD_GPSChange 0x01
#define SPICMD_GPSGetChannel 0x02
#define SPICMD_GPSPowerOFF 0x03
#define SPICMD_GPSPowerON 0x04

#define SPICMD_NULL 0xfe
unsigned char GPSCh=0;
struct{
	unsigned Data:1;
	unsigned SendData:1;			//�f�[�^���M���[�h
	unsigned SendData_i:1;			//�f�[�^���M���[�h��
	unsigned SwapGPS_i:1;			//GPS�����v����
	unsigned PowerGPS:1;
	unsigned PowerGPS_i:1;
}_gps_Mode={0,0,1,0};

hmLib_boolian gps_listen(cstring Str){
	//�f�[�^�T�C�Y�m�F
	if(cstring_size(&Str)==0)return hmLib_true;

	//1byte�ڂŃ��[�h�𕪂���
	switch(cstring_getc(&Str,0)){
	case 0x10:
		_gps_Mode.SendData=hmLib_true;
		_gps_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	case 0x11:
		_gps_Mode.SendData=hmLib_false;
		_gps_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	case 0x20:
		//�f�[�^�T�C�Y�m�F
		if(cstring_size(&Str)!=2)return hmLib_true;
		if(cstring_getc(&Str,1)!=GPSCh){
			_gps_Mode.SwapGPS_i=hmLib_true;
		}
		return hmLib_false;
	case 0x30:
		_gps_Mode.PowerGPS=hmLib_true;
		_gps_Mode.PowerGPS_i=hmLib_true;
		return hmLib_false;
	case 0x31:
		_gps_Mode.PowerGPS=hmLib_false;
		_gps_Mode.PowerGPS_i=hmLib_true;
		return hmLib_false;
	default:
		return hmLib_true;
	}
}
hmLib_boolian gps_talk(cstring* pStr){
	unsigned char j;
        unsigned char tmp;

	if(_gps_Mode.SendData_i){
		cstring_construct_dynamic_safe(pStr,1);
		if(_gps_Mode.SendData)cstring_putc(pStr,0,0x10);
		else cstring_putc(pStr,0,0x11);
		//���M�㎩���I�Ƀt���O�𗎂Ƃ�
		_gps_Mode.SendData_i=hmLib_false;
		return hmLib_false;
	}else if(_gps_Mode.SwapGPS_i){
		cstring_construct_dynamic_safe(pStr,1+1);
		cstring_putc(pStr,0,0x20);

		spi_selectGPS(1);
		delay_ms(1);
		spi2_write(SPICMD_GPSChange);	//�����R�}���h�𑗂�
		delay_ms(1);
		GPSCh=spi2_read(SPICMD_NULL);	//��������Ch���T����
		cstring_putc(pStr,1,GPSCh);
		spi_selectGPS(0);

		//���M�㎩���I�Ƀt���O�𗎂Ƃ�
		_gps_Mode.SwapGPS_i=hmLib_false;
		return hmLib_false;
	}else if(_gps_Mode.PowerGPS_i){
		cstring_construct_dynamic_safe(pStr,1);

		spi_selectGPS(1);
		delay_ms(1);
		if(_gps_Mode.PowerGPS){
			cstring_putc(pStr,0,0x30);
			spi2_write(SPICMD_GPSPowerON);	//�����R�}���h�𑗂�
		}else{
			cstring_putc(pStr,0,0x31);
			spi2_write(SPICMD_GPSPowerOFF);	//�����R�}���h�𑗂�
		}
		delay_ms(1);
		spi2_read(SPICMD_NULL);
		spi_selectGPS(0);

		//���M�㎩���I�Ƀt���O�𗎂Ƃ�
		_gps_Mode.PowerGPS_i=hmLib_false;
		return hmLib_false;
	}else if(_gps_Mode.Data){
		cstring_construct_dynamic_safe(pStr,1+20);
		cstring_putc(pStr,0,0x00);

		spi_selectGPS(1);
		delay_ms(1);
		//�f�[�^�擾
		spi2_write(SPICMD_GPSGet);//GPSdata�悱���R�}���h
		delay_ms(1);

		for(j=1;j<20;j++){//data��M
//			cstring_putc(pStr,j,spi2_read(SPICMD_NULL));	//�R�}���h�ȊO�̓K���Ȃ��̂𑗂�
            tmp=spi2_read(SPICMD_NULL);
 //           if(tmp)HMR_PIN_debugLED_set(0);
			cstring_putc(pStr,j,tmp);	//�R�}���h�ȊO�̓K���Ȃ��̂𑗂�
			delay_ms(1);
		}

		spi2_write(SPICMD_GPSGetChannel);//�Ƃ���GPS��CH����������
		//spi2_write(0xBB);
		delay_ms(1);
		cstring_putc(pStr,20,spi2_read(SPICMD_NULL));//�R�}���h�ȊO�̓K���Ȃ��̂𑗂�
		spi_selectGPS(0);

		_gps_Mode.Data=hmLib_false;

/*
        cstring_construct_dynamic_safe(pStr,1+20);
		cstring_putc(pStr,0,0x00);
		spi_selectGPS(1);
        delay_ms(1);
		spi2_write('@');
		tmp=spi2_read(SPICMD_NULL);
		if(tmp==0xAA)HMR_PIN_debugLED_set(0);
//		spi_selectGPS(0);
		for(j=0;j<21;++j){
			cstring_putc(pStr,j+1,tmp);
		}

		_gps_Mode.Data=hmLib_false;
*/
		return hmLib_false;
	}

	return hmLib_true;
}
void gps_setup_listen(void){return;}
void gps_setup_talk(void){
	_gps_Mode.Data=_gps_Mode.SendData;
}
void gps_task_setup_talk(void){return;}
sint16 gps_task_data(sint16 interval){
	_gps_Mode.Data=_gps_Mode.SendData;
	return interval;
}
sint16 gps_task_inform(sint16 interval){
	_gps_Mode.SendData_i=hmLib_true;
//	_gps_Mode.SwapGPS_i=hmLib_true;
	return interval;
}
#
#endif
