#ifndef HMR_SHT75_C_INC
#define HMR_SHT75_C_INC 1
#
#define HMLIB_NOLIB
#ifndef HMR_THERMO_INC
#	include"hmrSHT75.h"
#endif
#ifndef HMR_DEVICE_INC
#	include"hmrDevice.h"
#endif
#ifndef HMR_UTILITY_INC
#	include "hmrUtility.h"
#endif
#include "hmrI2C_SHT75.h"
//SHT75�Ƃ̒ʐM���s���ɁA���g���C�ł����
#define SHT75_I2C_RETRYNUM		3
//SHT75�̃A�h���X
#define SHT75_ADDRESS_THERMO	1
#define SHT75_ADDRESS_HUMID		2
#define SHT75_ADDRESS_CONFIG	3
#define SHT75_ADDRESS_RESET		0x0F
//SHT75�X�e�[�^�X�Ǘ��p�\����
struct{
	unsigned Data:1;				//�f�[�^�𑗐M���ɗ�����
	unsigned SendData:1;			//�f�[�^���M���[�h
	unsigned SendData_i:1;			//�f�[�^���M���[�h��
	unsigned LowAccData:1;			//�ᐸ�x�f�[�^���[�h
	unsigned LowAccData_i:1;		//�ᐸ�x�f�[�^���[�h��
	unsigned Reset_i:1;				//���Z�b�g��
}_sht75_Mode={0,0,1,0,1};
//�ʏ탂�[�h�̃f�[�^��M�p�֐�
void sht75_setup_listen(void){return;}
hmLib_boolian sht75_listen(cstring Str){
	unsigned char Cnt;

	//�f�[�^�T�C�Y�m�F
	if(cstring_size(&Str)==0)return hmLib_true;

	//1byte�ڂŃ��[�h�𕪂���
	switch(cstring_getc(&Str,0)){
	case 0x10:
		_sht75_Mode.SendData=hmLib_true;
		_sht75_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	case 0x11:
		_sht75_Mode.SendData=hmLib_false;
		_sht75_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	case 0x20:
		if(_sht75_Mode.LowAccData==hmLib_false){
			//ConfigBit��ݒ�
			for(Cnt=0;Cnt<SHT75_I2C_RETRYNUM;++Cnt){
				//�������ݗv��
				if(i2c1_start(SHT75_ADDRESS_CONFIG,0))continue;
				//ConfigBits��������
				if(i2c1_write(0x01))continue;	
				//i2c1�I��
				i2c1_stop();
				break;
			}

			if(Cnt==SHT75_I2C_RETRYNUM)return hmLib_false;
		}
		
		_sht75_Mode.LowAccData=hmLib_true;
		_sht75_Mode.LowAccData_i=hmLib_true;

		return hmLib_false;
	case 0x21:
		if(_sht75_Mode.LowAccData==hmLib_true){
			//ConfigBit��ݒ�
			for(Cnt=0;Cnt<SHT75_I2C_RETRYNUM;++Cnt){
				//�������ݗv��
				if(i2c1_start(SHT75_ADDRESS_CONFIG,0))continue;
				//ConfigBits��������
				if(i2c1_write(0x00))continue;
				//i2c1�I��
				i2c1_stop();
				break;
			}

			if(Cnt==SHT75_I2C_RETRYNUM)return hmLib_false;
		}
		
		_sht75_Mode.LowAccData=hmLib_false;
		_sht75_Mode.LowAccData_i=hmLib_true;

		return hmLib_false;
	case 0xE0:
		if(_sht75_Mode.LowAccData==hmLib_true){
			//ConfigBit��ݒ�
			for(Cnt=0;Cnt<SHT75_I2C_RETRYNUM;++Cnt){
				//�������ݗv��
				if(i2c1_start(SHT75_ADDRESS_RESET,0))continue;
				//i2c1�I��
				i2c1_stop();
				break;
			}

			if(Cnt==SHT75_I2C_RETRYNUM)return hmLib_false;
		}
		
		_sht75_Mode.Reset_i=hmLib_true;
		_sht75_Mode.LowAccData=hmLib_false;
		_sht75_Mode.LowAccData_i=hmLib_true;

		return hmLib_false;
	default:
		return hmLib_true;
	}
}
//�ʏ탂�[�h�̃f�[�^���M�p�֐�
void sht75_setup_talk(void){
	_sht75_Mode.Data=_sht75_Mode.SendData;
}
hmLib_boolian sht75_talk(cstring* pStr){
	uint8 Cnt;
	uint8 TLow,THigh,HLow,HHigh;

	//�f�[�^���M���[�h�ύX���ߎ��
	if(_sht75_Mode.SendData_i){
		cstring_construct_dynamic_safe(pStr,1);
		if(_sht75_Mode.SendData)cstring_putc(pStr,0,0x10);
		else cstring_putc(pStr,0,0x11);

		//���M�㎩���I�Ƀt���O�𗎂Ƃ�
		_sht75_Mode.SendData_i=hmLib_false;
		return hmLib_false;
	}//�����x�f�[�^���[�h�ύX���ߎ��
	else if(_sht75_Mode.LowAccData_i){
		cstring_construct_dynamic_safe(pStr,1);
		if(_sht75_Mode.LowAccData)cstring_putc(pStr,0,0x20);
		else cstring_putc(pStr,0,0x21);

		//���M�㎩���I�Ƀt���O�𗎂Ƃ�
		_sht75_Mode.LowAccData_i=hmLib_false;
		return hmLib_false;
	}//���Z�b�g���ߎ��
	else if(_sht75_Mode.Reset_i){
		cstring_construct_dynamic_safe(pStr,1);
		cstring_putc(pStr,0,0xE0);

		//���M�㎩���I�Ƀt���O�𗎂Ƃ�
		_sht75_Mode.Reset_i=hmLib_false;
		return hmLib_false;
	}//�f�[�^���M
	else if(_sht75_Mode.Data){
		hmr_i2c_finalize();
		i2cSHT75_initialize();

		//���x�f�[�^�擾
		for(Cnt=0;Cnt<SHT75_I2C_RETRYNUM;++Cnt){
			//i2c���G���[��Ԃ����ꍇ
			if(i2cSHT75_start(SHT75_ADDRESS_THERMO,1)){
				//�K��񐔈ȏゾ�ƁA�I��
				if(Cnt==SHT75_I2C_RETRYNUM){
					cstring_construct_dynamic_safe(pStr,2);
					cstring_putc(pStr,0,0xFF);
					cstring_putc(pStr,1,0x10);	//Default�ł�0�ɂ��Ă���
					_sht75_Mode.Data=hmLib_false;
					return hmLib_false;
				}
				continue;
			}
			break;
		}

		while(i2cSHT75_data_can_recv());

		//���݂�FIFOSRC�擾
		THigh=i2cSHT75_read(0);
		TLow=i2cSHT75_read(0);
		i2cSHT75_read(1);
		i2cSHT75_stop();
		

		//���x�f�[�^�擾
		for(Cnt=0;Cnt<SHT75_I2C_RETRYNUM;++Cnt){
			//i2c���G���[��Ԃ����ꍇ
			if(i2cSHT75_start(SHT75_ADDRESS_HUMID,1)){
				//�K��񐔈ȏゾ�ƁA�I��
				if(Cnt==SHT75_I2C_RETRYNUM){
					cstring_construct_dynamic_safe(pStr,2);
					cstring_putc(pStr,0,0xFF);
					cstring_putc(pStr,1,0x11);	//Default�ł�0�ɂ��Ă���
					_sht75_Mode.Data=hmLib_false;
					return hmLib_false;
				}
				continue;
			}
			break;
		}

		//���݂�FIFOSRC�擾
		HHigh=i2cSHT75_read(0);
		HLow=i2cSHT75_read(0);
		i2cSHT75_read(1);
		i2cSHT75_stop();

		hmr_i2c_initialize();

		//���x�ɉ����ăf�[�^��������
		if(_sht75_Mode.LowAccData){
			cstring_construct_dynamic_safe(pStr,4);
			cstring_putc(pStr,0,0x00);
			cstring_putc(pStr,1,TLow);
			cstring_putc(pStr,2,THigh);
			cstring_putc(pStr,3,HLow);
		}else{
			cstring_construct_dynamic_safe(pStr,5);
			cstring_putc(pStr,0,0x01);
			cstring_putc(pStr,1,TLow);
			cstring_putc(pStr,2,THigh);
			cstring_putc(pStr,3,HLow);
			cstring_putc(pStr,4,HHigh);
		}

		//���M�㎩���I�Ƀt���O�𗎂Ƃ�
		_sht75_Mode.Data=hmLib_false;
		return hmLib_false;
	}

	return hmLib_true;
}
//�^�X�N���[�h�̃f�[�^���M�p�֐�
void sht75_task_setup_talk(void){}
sint16 sht75_task_data(sint16 interval){
	_sht75_Mode.Data=_sht75_Mode.SendData;
	return interval;
}
sint16 sht75_task_inform(sint16 interval){
	_sht75_Mode.SendData_i=hmLib_true;
	_sht75_Mode.LowAccData_i=hmLib_true;
	return interval;
}
#
#endif
