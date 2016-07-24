#ifndef HMR_INERTIAL_C_INC
#define HMR_INERTIAL_C_INC 103
#
/*===�q�s���p���W���[���Q===
*/
#define HMLIB_NOLIB
#ifndef HMR_INERTIAL_INC
#	include"hmrInertial.h"
#endif
#ifndef HMR_DEVICE_INC
#	include"hmrDevice.h"
#endif
#ifndef HMR_UTILITY_INC
#	include "hmrUtility.h"
#endif
void inertial_initialize(void){
	axel_device_initialize();
	compass_device_initialize();
	gyro_device_initialize();	
}

void inertial_finalize(void){}
//******************* Axel ******************
struct{
	unsigned Data:1;
	unsigned SendData:1;
	unsigned SendData_i:1;
}_axel_Mode={0,0,1};
void axel_device_initialize(){
	int Cnt=0;
	//�܂��A���W�X�^���������񃊃Z�b�g
	//	��������Ă����Ȃ��ƁAPic�����Z�b�g�����ۂɐ������ċN�����Ă���Ȃ�
/*	
	//Axel&Magne#0�����Z�b�g
	for(Cnt=0;Cnt<AXEL_I2C_RETRY_NUM;++Cnt){
		if(i2c2_start(AXEL_ADDRESS,0))continue;
		//CTRL_REG���W�X�^�ւ̏������ݗv��(�����o�C�g)
		if(i2c2_write(AXEL_REG_CTRL|AXEL_REGMODE_SINGLE))continue;
		//CTRL_REG1���W�X�^ NormalMode�ɐݒ肷��
		if(i2c2_write(0x00))continue;
		i2c2_stop();
		break;
	}
*/
	//Axel&Magne#1�����Z�b�g
	for(Cnt=0;Cnt<AXEL_I2C_RETRY_NUM;++Cnt){
		if(i2c1_start(AXEL_ADDRESS,0))continue;
		//CTRL_REG���W�X�^�ւ̏������ݗv��(�����o�C�g)
		if(i2c1_write(AXEL_REG_CTRL|AXEL_REGMODE_SINGLE))continue;
		//CTRL_REG1���W�X�^ NormalMode�ɐݒ肷��
		if(i2c1_write(0x00))continue;
		i2c1_stop();
		break;
	}
	
	// ����͊��荞�݊֐��ŌĂ΂ꂤ��̂ŁA���S�������s��
	exclusive_delay_ms(500);

//=========================Axel ������========================
	//continue�̓G���[���̂�蒼��
/*	//Axel#0
	for(Cnt=0;Cnt<AXEL_I2C_RETRY_NUM;++Cnt){
		if(i2c2_start(AXEL_ADDRESS,0))continue;
		//CTRL_REG���W�X�^�ւ̏������ݗv��(�����o�C�g)
		if(i2c2_write(AXEL_REG_CTRL|AXEL_REGMODE_MULTI))continue;
		//CTRL_REG1�̏����� Normal 10Hz
		if(i2c2_write(0x27))continue;
		//CTRL_REG2�̏����� default
		if(i2c2_write(0x00))continue;
		//CTRL_REG3�̏����� default
		if(i2c2_write(0x00))continue;
		//CTRL_REG4�̏����� default
		if(i2c2_write(0x00))continue;
		//CTRL_REG5�̏����� default
		if(i2c2_write(0x00))continue;
		//CTRL_REG6�̏����� default
		if(i2c2_write(0x00))continue;

		//i2c2�I��
		i2c2_stop();
		break;
	}
*/
	//Axel#1
	for(Cnt=0;Cnt<AXEL_I2C_RETRY_NUM;++Cnt){
		if(i2c1_start(AXEL_ADDRESS,0))continue;
		//CTRL_REG���W�X�^�ւ̏������ݗv��(�����o�C�g)
		if(i2c1_write(AXEL_REG_CTRL|AXEL_REGMODE_MULTI))continue;
		//CTRL_REG1�̏����� Normal 10Hz
		if(i2c1_write(0x27))continue;
		//CTRL_REG2�̏����� default
		if(i2c1_write(0x00))continue;
		//CTRL_REG3�̏����� default
		if(i2c1_write(0x00))continue;
		//CTRL_REG4�̏����� default
		if(i2c1_write(0x00))continue;
		//CTRL_REG5�̏����� default
		if(i2c1_write(0x00))continue;
		//CTRL_REG6�̏����� default
		if(i2c1_write(0x00))continue;

		//i2c1�I��
		i2c1_stop();
		break;
	}

}
void axel_device_finalize(){}
hmLib_boolian axel_listen(cstring Str){
	switch(cstring_getc(&Str,0)){
	case 0x10://sensor on
		_axel_Mode.SendData=hmLib_true;
		_axel_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	case 0x11://sensor on
		_axel_Mode.SendData=hmLib_false;
		_axel_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	default:
		return hmLib_true;
	}
}
hmLib_boolian axel_talk(cstring* pStr){
	uint8 LowData,HighData;
        uint16 Data;
	uint8 Cnt;
	if(_axel_Mode.SendData_i){
		cstring_construct_dynamic_safe(pStr,1);
		if(_axel_Mode.SendData)cstring_putc(pStr,0,0x10);
		else cstring_putc(pStr,0,0x11);
		_axel_Mode.SendData_i=hmLib_false;
		return hmLib_false;
	}else if(_axel_Mode.Data){

		//======== ACC
		for(Cnt=0;Cnt<AXEL_I2C_RETRY_NUM;++Cnt){
			if(i2c1_start(AXEL_ADDRESS,0))continue;
			//x�������xLowByte�v��
			if(i2c1_write(AXEL_REG_DATA|AXEL_REGMODE_MULTI))continue;
			//�ǂݎ��p�ɍċN��
			if (i2c1_restart(AXEL_ADDRESS,1))continue;
			break;
		}
		if(Cnt==AXEL_I2C_RETRY_NUM){
			cstring_construct_dynamic_safe(pStr,2);
			cstring_putc(pStr,0,0xFF);
			cstring_putc(pStr,1,0x00);	//Default�ł�0�ɂ��Ă���
			_axel_Mode.Data=hmLib_false;
			return hmLib_false;
		}

		cstring_construct_dynamic_safe(pStr,7);
		cstring_putc(pStr,0,0x00);
		//x�������xLowByte��M
		LowData=i2c1_read(0);
		//x�������xHighByte��M
		HighData=i2c1_read(0);
/*
		//�������]����
        Data=HighData;
        Data<<=8;
        Data|=LowData;
        Data=-Data;
        LowData=Data&0x00FF;
        Data>>=8;
        HighData=Data&0x00FF;
*/
        //homura-y����������
		cstring_putc(pStr,3,LowData);
		cstring_putc(pStr,4,HighData);

		//y�������xLowByte��M
		LowData=i2c1_read(0);
		//y�������xHighByte��M
		HighData=i2c1_read(0);

////////////////////////////////////////
		//�������]����
        Data=HighData;
        Data<<=8;
        Data|=LowData;
        Data=-Data;
        LowData=Data&0x00FF;
        Data>>=8;
        HighData=Data&0x00FF;
////////////////////////////////////////

        //homura-x���������݁i�������]�j
		cstring_putc(pStr,1,LowData);
		cstring_putc(pStr,2,HighData);

		//z�������xLowByte��M
		LowData=i2c1_read(0);
		//z�������xHighByte��M
		HighData=i2c1_read(1);

		//�������]����
        Data=HighData;
        Data<<=8;
        Data|=LowData;
        Data=-Data;
        LowData=Data&0x00FF;
        Data>>=8;
        HighData=Data&0x00FF;

		//z����������
		cstring_putc(pStr,5,LowData);
		cstring_putc(pStr,6,HighData);

		i2c1_stop();

		_axel_Mode.Data=hmLib_false;
		return hmLib_false;
	}
	return hmLib_true;
}
void axel_setup_listen(void){return;}
void axel_setup_talk(void){
	_axel_Mode.Data=_axel_Mode.SendData;
}
void axel_task_setup_talk(void){return;}
sint16 axel_task_data(sint16 interval){
	_axel_Mode.Data=_axel_Mode.SendData;
	return interval;
}
sint16 axel_task_inform(sint16 interval){
	_axel_Mode.SendData_i=hmLib_true;
	return interval;
}
//****************** compass *********************
struct{
	unsigned Data:1;
	unsigned SendData:1;
	unsigned SendData_i:1;
}_compass_Mode={0,0,1};
void compass_device_initialize(){
	int Cnt=0;
	//=======================COMPASS ������=========================
/*	//Magne#0
	for(Cnt=0;Cnt<CMPS_I2C_RETRY_NUM;++Cnt){
		if(i2c2_start(CMPS_ADDRESS,0))continue;
		//CTRL_REG���W�X�^�ւ̏������ݗv��(�����o�C�g)
		if(i2c2_write(CMPS_REG_CTRL|CMPS_REGMODE_MULTI))continue;
		//CRA_REG_M���������@Temperatur Enable, 15Hz
		if(i2c2_write(0x10))continue;
		//CRB_REG_M���������@1.3 Gauss
		if(i2c2_write(0x20))continue;
		//MR_REG_M���������@continuous
		if(i2c2_write(0x00))continue;

		//i2c2�I��
		i2c2_stop();
		break;
	}
*/
	//Magne#1
	for(Cnt=0;Cnt<CMPS_I2C_RETRY_NUM;++Cnt){
		if(i2c1_start(CMPS_ADDRESS,0))continue;
		//CTRL_REG���W�X�^�ւ̏������ݗv��(�����o�C�g)
		if(i2c1_write(CMPS_REG_CTRL|CMPS_REGMODE_MULTI))continue;
		//CRA_REG_M���������@Temperatur Enable, 15Hz
		if(i2c1_write(0x10))continue;
		//CRB_REG_M���������@1.3 Gauss
		if(i2c1_write(0x20))continue;
		//MR_REG_M���������@continuous
		if(i2c1_write(0x00))continue;

		//i2c1�I��
		i2c1_stop();
		break;
	}
}
void compass_device_finalize(){}
hmLib_boolian compass_listen(cstring Str){
	switch(cstring_getc(&Str,0)){
	case 0x10://sensor on
		_compass_Mode.SendData=hmLib_true;
		_compass_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	case 0x11://sensor on
		_compass_Mode.SendData=hmLib_false;
		_compass_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	default:
		return hmLib_true;
	}
}
hmLib_boolian compass_talk(cstring* pStr){
    uint8 Cnt;
	uint8 LowData,HighData;
	uint16 Data;
	if(_compass_Mode.SendData_i){
		cstring_construct_dynamic_safe(pStr,1);
		if(_compass_Mode.SendData)cstring_putc(pStr,0,0x10);
		else cstring_putc(pStr,0,0x11);
		_compass_Mode.SendData_i=hmLib_false;
		return hmLib_false;
	}else if(_compass_Mode.Data){
		//======== CMPS
		for(Cnt=0;Cnt<CMPS_I2C_RETRY_NUM;++Cnt){
			if(i2c1_start(CMPS_ADDRESS,0))continue;
			//x�������xLowByte�v��
			if(i2c1_write(CMPS_REG_DATA|CMPS_REGMODE_MULTI))continue;
			//�ǂݎ��p�ɍċN��
			if (i2c1_restart(CMPS_ADDRESS,1))continue;
			break;
		}
		if(Cnt==CMPS_I2C_RETRY_NUM){
			cstring_construct_dynamic_safe(pStr,2);
			cstring_putc(pStr,0,0xFF);
			cstring_putc(pStr,1,0x00);	//Default�ł�0�ɂ��Ă���
			_compass_Mode.Data=hmLib_false;
			return hmLib_false;
		}

		cstring_construct_dynamic_safe(pStr,7);
		cstring_putc(pStr,0,0x00);

		//x�������xHighByte��M
		HighData=i2c1_read(0);
		//x�������xLowByte��M
		LowData=i2c1_read(0);

		//�������]����
        Data=HighData;
        Data<<=8;
        Data|=LowData;
        Data=-Data;
        LowData=Data&0x00FF;
        Data>>=8;
        HighData=Data&0x00FF;


		//homura-x����������
		cstring_putc(pStr,3,LowData);
		cstring_putc(pStr,4,HighData);

		//y�������xHighByte��M
		HighData=i2c1_read(0);
		//y�������xLowByte��M
		LowData=i2c1_read(0);
		//homura-z����������
		cstring_putc(pStr,5,LowData);
		cstring_putc(pStr,6,HighData);


		//z�������xHighByte��M
		HighData=i2c1_read(0);
		//z�������xLowByte��M
		LowData=i2c1_read(1);
/*
		//�������]����
        Data=HighData;
        Data<<=8;
        Data|=LowData;
        Data=-Data;
        LowData=Data&0x00FF;
        Data>>=8;
        HighData=Data&0x00FF;
*/
		//homura-x����������(�������t�]������)
		cstring_putc(pStr,1,LowData);
		cstring_putc(pStr,2,HighData);

		i2c1_stop();

		_compass_Mode.Data=hmLib_false;
		return hmLib_false;
	}
	return hmLib_true;
}
void compass_setup_listen(void){return;}
void compass_setup_talk(void){
	_compass_Mode.Data=_compass_Mode.SendData;
}
void compass_task_setup_talk(void){return;}
sint16 compass_task_data(sint16 interval){
	_compass_Mode.Data=_compass_Mode.SendData;
	return interval;
}
sint16 compass_task_inform(sint16 interval){
	_compass_Mode.SendData_i=hmLib_true;
	return interval;
}
//=====================Gyro=======================
struct{
	unsigned Data:1;
	unsigned SendData:1;
	unsigned SendData_i:1;
}_gyro_Mode={0,0,1};
void gyro_device_initialize(){
	int Cnt=0;
	//�܂��A���W�X�^���������񃊃Z�b�g
	//	��������Ă����Ȃ��ƁAPic�����Z�b�g�����ۂɐ������ċN�����Ă���Ȃ�

	//Gyro�����Z�b�g
	for(Cnt=0;Cnt<GYRO_I2C_RETRY_NUM;++Cnt){
		if(i2c1_start(GYRO_ADDRESS,0))continue;
		//CTRL_REG���W�X�^�ւ̏������ݗv��(�����o�C�g)
		if(i2c1_write(GYRO_REG_CTRL|GYRO_REGMODE_SINGLE))continue;
		//CTRL_REG1���W�X�^ NormalMode�ɐݒ肷��
		if(i2c1_write(0x00))continue;
		i2c1_stop();
		break;
	}
	
	// ���荞�݈��S�ȃf�B���C
	exclusive_delay_ms(500);
	/*
	HMR_PIN_YellowLED_set(1);
	exclusive_delay_ms(200);
	HMR_PIN_YellowLED_set(0);
	exclusive_delay_ms(400);
	HMR_PIN_YellowLED_set(1);
	exclusive_delay_ms(400);
	HMR_PIN_YellowLED_set(0);
	exclusive_delay_ms(400);
	HMR_PIN_YellowLED_set(1);
	exclusive_delay_ms(400);
	HMR_PIN_YellowLED_set(0);
	exclusive_delay_ms(400);
	HMR_PIN_YellowLED_set(1);
	*/
//=======================Gyro ������===========================
	//Default��PowerDown���[�h�Ȃ̂ŁC�܂��N������
	for(Cnt=0;Cnt<GYRO_I2C_RETRY_NUM;++Cnt){
		if(i2c1_start(GYRO_ADDRESS,0))continue;
		//CTRL_REG���W�X�^�ւ̏������ݗv��(�����o�C�g)
		if(i2c1_write(GYRO_REG_CTRL|GYRO_REGMODE_MULTI))continue;
		//CTRL_REG1���W�X�^ NormalMode�ɐݒ肷��
		if(i2c1_write(0x0F))continue;
		//CTRL_REG2���W�X�^ Default
		if(i2c1_write(0x00))continue;
		//CTRL_REG3���W�X�^ BufOverrun�Ŋ��荞��PinON
		if(i2c1_write(0x02))continue;
		//CTRL_REG4���W�X�^ Default
		if(i2c1_write(0x00))continue;
		//CTRL_REG5���W�X�^ FIFO enable
		if(i2c1_write(0x40))continue;
//		if(i2c1_write(0x00))continue;

		//i2c1�I��
		i2c1_stop();
		break;
	}
	//FIFO�o�b�t�@���[�h��ݒ肷��
	for(Cnt=0;Cnt<GYRO_I2C_RETRY_NUM;++Cnt){
		if(i2c1_start(GYRO_ADDRESS,0))continue;
		//FIFO_CTRL_REG���W�X�^�ւ̏������ݗv��
		if(i2c1_write(GYRO_REG_FIFOCTRL|GYRO_REGMODE_SINGLE))continue;
		//FIFO_CTRL_REG���W�X�^ StreamMode WTM=32�ɐݒ肷��
		if(i2c1_write(0x5F))continue;
		i2c1_stop();
		break;
	}
}
void gyro_device_finalize(){}
hmLib_boolian gyro_listen(cstring Str){
	switch(cstring_getc(&Str,0)){
	case 0x10://sensor on
		_gyro_Mode.SendData=hmLib_true;
		_gyro_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	case 0x11://sensor on
		_gyro_Mode.SendData=hmLib_false;
		_gyro_Mode.SendData_i=hmLib_true;
		return hmLib_false;
	default:
		return hmLib_true;
	}
}
hmLib_boolian gyro_talk(cstring* pStr){
	uint8 LowData,HighData;
	uint8 IsEnd;
	volatile uint8 Src;
	uint16 Cnt;
//	uint16 A;
	sint32 X,Y,Z;
	if(_gyro_Mode.SendData_i){
		cstring_construct_dynamic_safe(pStr,1);
		if(_gyro_Mode.SendData)cstring_putc(pStr,0,0x10);
		else cstring_putc(pStr,0,0x11);
		_gyro_Mode.SendData_i=hmLib_false;
		return hmLib_false;
	}else if(_gyro_Mode.Data){
		Cnt=0;
		X=0;
		Y=0;
		Z=0;
		//===========Gyro X
//		while(1){
		for(Cnt=0;Cnt<GYRO_I2C_RETRY_NUM;++Cnt){
			//FIFOSRC�擾�v��
			if(i2c1_start(GYRO_ADDRESS,0))continue;
			if(i2c1_write(GYRO_REG_FIFOSRC|GYRO_REGMODE_SINGLE))continue;
			if(i2c1_restart(GYRO_ADDRESS,1))continue;
			break;
		}
		if(Cnt==GYRO_I2C_RETRY_NUM){
			cstring_construct_dynamic_safe(pStr,2);
			cstring_putc(pStr,0,0xFF);
			cstring_putc(pStr,1,0x00);	//Default�ł�0�ɂ��Ă���
			_gyro_Mode.Data=hmLib_false;
			return hmLib_false;
		}

		//���݂�FIFOSRC�擾
		Src=i2c1_read(1);
		i2c1_stop();
			
		for(Cnt=0;Cnt<GYRO_I2C_RETRY_NUM;++Cnt){
			//Data�擾�v��
			if(i2c1_start(GYRO_ADDRESS,0))continue;
			if(i2c1_write(GYRO_REG_DATA|GYRO_REGMODE_MULTI))continue;
			if(i2c1_restart(GYRO_ADDRESS,1))continue;
			break;
		}
		if(Cnt==GYRO_I2C_RETRY_NUM){
			cstring_construct_dynamic_safe(pStr,2);
			cstring_putc(pStr,0,0xFF);
			cstring_putc(pStr,1,0x00);	//Default�ł�0�ɂ��Ă���
			_gyro_Mode.Data=hmLib_false;
			return hmLib_false;
		}

		for(Cnt=0;Cnt<gyro_bufsize(Src);++Cnt){
			if(Cnt+1==gyro_bufsize(Src))IsEnd=1;
			else IsEnd=0;

			LowData=i2c1_read(0);
			HighData=i2c1_read(0);
//			LowData=0x34;
//			HighData=0x12;
			X+=(sint16)(((uint16)(LowData)&0x00FF)|(((uint16)(HighData)<<8)&0xFF00));
			LowData=i2c1_read(0);
			HighData=i2c1_read(0);
//			LowData=0x01;
//			HighData=0x00;
			Y+=(sint16)(((uint16)(LowData)&0x00FF)|(((uint16)(HighData)<<8)&0xFF00));
			LowData=i2c1_read(0);
			HighData=i2c1_read(IsEnd);
//			HighData=i2c1_read(1);
//			LowData=0x23;
//			HighData=0x01;
			Z+=(sint16)(((uint16)(LowData)&0x00FF)|(((uint16)(HighData)<<8)&0xFF00));
//                        break;
		}
		i2c1_stop();

		cstring_construct_dynamic_safe(pStr,16);
		cstring_putc(pStr,0,0x00);
		cstring_putc(pStr,1,0x00);	//Default�ł�0�ɂ��Ă���

		cstring_putc(pStr, 2,(uint8)(Cnt&0x00FF));
		cstring_putc(pStr, 3,(uint8)((Cnt>>8)&0x00FF));

		cstring_putc(pStr, 4,(uint8)(X&0x00FF));
//		cstring_putc(pStr, 4,(uint8)(0x40));
		cstring_putc(pStr, 5,(uint8)((X>>8)&0x00FF));
		cstring_putc(pStr, 6,(uint8)((X>>16)&0x00FF));
		cstring_putc(pStr, 7,(uint8)((X>>24)&0x00FF));

		cstring_putc(pStr, 8,(uint8)(Y&0x00FF));
		cstring_putc(pStr, 9,(uint8)((Y>>8)&0x00FF));
		cstring_putc(pStr,10,(uint8)((Y>>16)&0x00FF));
		cstring_putc(pStr,11,(uint8)((Y>>24)&0x00FF));

		cstring_putc(pStr,12,(uint8)(Z&0x00FF));
		cstring_putc(pStr,13,(uint8)((Z>>8)&0x00FF));
		cstring_putc(pStr,14,(uint8)((Z>>16)&0x00FF));
		cstring_putc(pStr,15,(uint8)((Z>>24)&0x00FF));

		_gyro_Mode.Data=hmLib_false;
		return hmLib_false;
	}
	return hmLib_true;
}
void gyro_setup_listen(void){return;}
void gyro_setup_talk(void){
	_gyro_Mode.Data=_gyro_Mode.SendData;
}
void gyro_task_setup_talk(void){return;}
sint16 gyro_task_data(sint16 interval){
	_gyro_Mode.Data=_gyro_Mode.SendData;
	return interval;
}
sint16 gyro_task_inform(sint16 interval){
	_gyro_Mode.SendData_i=hmLib_true;
	return interval;
}
#
#endif
