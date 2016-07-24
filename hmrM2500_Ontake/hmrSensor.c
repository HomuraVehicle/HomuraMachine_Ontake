#ifndef HMR_SENSOR_C_INC
#define HMR_SENSOR_C_INC 112
#
#include<stdlib.h>
#ifndef HMR_SENSOR_INC
#	include"hmrSensor.h"
#endif
#ifndef HMR_SPRITEFUNCTIONS_INC
#	include"hmrSpriteFunctions.h"
#endif
//**************** Battery *********************************
cstring battery_message(cstring Str){
	cstring Ans={0,0};
	unsigned char adcdata_l,adcdata_h,size,j;
	uint32 sumadc;

	//���������݂��Ȃ���΃G���[��Ԃ�
	if(cstring_size(&Str)==0){
		cstring_dynamic_construct(&Ans,1);
		cstring_putc(&Ans,0,0xF0);			//�G���[�R�[�h:0xF0
		return Ans;
	}

	//���[�h��0x10�̏ꍇ
	if(cstring_getc(&Str,0) == 0x10){

		size = 3*(cstring_size(&Str)-1)+1;			//���[�h�𔲂���size�v�Z
		cstring_dynamic_construct(&Ans,size);	//������

		//�ԐM�p���[�h
		cstring_putc(&Ans,0,0x10);

		//�v�����ꂽBatteryID�̌����f�[�^�Ԃ�
		for(j = 0; j < size; j++){
			//ID�� �f�[�^�擾
			sumadc = adc_get_agerage(Vref_GND,battery_id_to_adc(cstring_getc(&Str,j+1)));//ID��AN�ɕϊ����Ă����̃f�[�^��ǂݓǂ�
			adcdata_h = (unsigned char)((sumadc>>8)%256);
			adcdata_l = (unsigned char)(sumadc%256);

			//�f�[�^�}��
			cstring_putc(&Ans,3*j+1,cstring_getc(&Str,j+1));//BatteryID
			cstring_putc(&Ans,3*j+2,adcdata_l);				//LowBitsData
			cstring_putc(&Ans,3*j+3,adcdata_h);				//HighBitsData
		}
		return Ans;
	}
	return Ans;
}
//************************ GPS 
cstring gps_message(cstring Str){
	cstring Ans={0,0};
	unsigned char j;
	unsigned char* gPtr;

	spi_selectGPS(1);
	delay_ms(3);

	if(cstring_getc(&Str,0) == 0x00){//GPS�����R�}���h(0x00)
		cstring_dynamic_construct(&Ans,1);
		cstring_putc(&Ans,0,0x00);

		gPtr=malloc(2);
		spi2_write(SPICMD_GPSChange);//�����R�}���h�𑗂�
		delay_ms(1);
	}else{			//GPS�f�[�^�擾�R�}���h(0x10)
		cstring_dynamic_construct(&Ans,21);
		cstring_putc(&Ans,0,0x10);

		spi2_write(SPICMD_GPSGet);//GPSdata�悱���R�}���h
		delay_ms(1);

		//restart_wdt();
		for(j=1;j<20;j++){//data��M
			cstring_putc(&Ans,j,spi2_read(SPICMD_NULL));	//�R�}���h�ȊO�̓K���Ȃ��̂𑗂�
			delay_ms(1);
		}
		//restart_wdt();
		spi2_write(SPICMD_GPSGetChannel);//�Ƃ���GPS��CH����������
		//spi2_write(0xBB);
		delay_ms(1);
		cstring_putc(&Ans,20,spi2_read(SPICMD_NULL));//�R�}���h�ȊO�̓K���Ȃ��̂𑗂�
	}
	spi_selectGPS(0);

	return Ans;
}
//************************ CO2 
cstring co2_message(cstring Str){
	cstring Ans={0,0};
	unsigned char	adcdata_l,adcdata_h;//,c;
	uint32 sumadc;

	if(cstring_getc(&Str,0) == 0x30){//pump off
		cstring_dynamic_construct(&Ans,1);
		cstring_putc(&Ans,0,0x30);

		co2_powerPump(0);
	}else if(cstring_getc(&Str,0) == 0x31){//pump on
		cstring_dynamic_construct(&Ans,1);
		cstring_putc(&Ans,0,0x31);

		co2_powerPump(1);
	}else if(cstring_getc(&Str,0) == 0x20){//sensor off
		cstring_dynamic_construct(&Ans,1);
		cstring_putc(&Ans,0,0x20);

		co2_powerSensor(0);
	}else if(cstring_getc(&Str,0) == 0x21){//sensor on
		cstring_dynamic_construct(&Ans,1);
		cstring_putc(&Ans,0,0x21);

		co2_powerSensor(1);
	}else if(cstring_getc(&Str,0) == 0x10){//get CO2 data
		cstring_dynamic_construct(&Ans,3);
		cstring_putc(&Ans,0,0x10);
		
		// CO2�f�[�^�擾
		sumadc = adc_get_agerage(Vref_GND,ADC_CO2);
		adcdata_h= (unsigned char)((sumadc>>8)%256);
		adcdata_l = (unsigned char)(sumadc%256);

		cstring_putc(&Ans,1,adcdata_l);
		cstring_putc(&Ans,2,adcdata_h);
	}

	return Ans;
}
//*********************** H2S 
cstring h2s_message(cstring Str){
	cstring Ans={0,0};
	unsigned char	adcdata_l,adcdata_h;//,c;
	uint32 sumadc;

	if(cstring_getc(&Str,0) == 0x20){//sensor off
		cstring_dynamic_construct(&Ans,1);
		cstring_putc(&Ans,0,0x20);

		h2s_powerSensor(0);
	}else if(cstring_getc(&Str,0) == 0x21){//sensor on
		cstring_dynamic_construct(&Ans,1);
		cstring_putc(&Ans,0,0x21);

		h2s_powerSensor(1);
	}else if(cstring_getc(&Str,0) == 0x10){//get H2S data
		cstring_dynamic_construct(&Ans,3);
		cstring_putc(&Ans,0,0x10);
		
		// H2S�f�[�^�擾
		sumadc = adc_get_agerage(Vref_GND,ADC_H2S);
		adcdata_h= (unsigned char)((sumadc>>8)%256);
		adcdata_l = (unsigned char)(sumadc%256);

		cstring_putc(&Ans,1,adcdata_l);
		cstring_putc(&Ans,2,adcdata_h);
	}

	return Ans;
}
//************************ Acc 
void axel_initialize(void){
	while(1){
		if(i2c2_start(AXEL_ADDRESS,0))continue;
		//CTRL_REG1�Ȃ郌�W�X�^�ւ̏������ݗv��
		if(i2c2_write(0x20))continue;
		//CTRL_REG1�Ȃ郌�W�X�^�̏�����		�iApplication Note p.10 �̃R�s�y�j
		if(i2c2_write(0x27))continue;
		break;
	}
	i2c2_stop();
/*
	while(1){
		if(i2c2_start(AXEL_ADDRESS,0))continue;
		//CTRL_REG4�Ȃ郌�W�X�^�ւ̏������ݗv��
		if(i2c2_write(0x23))continue;
		//CTRL_REG4�Ȃ郌�W�X�^�̏�����		�iApplication Note p.10 �̃R�s�y�j
		if(i2c2_write(0x40))continue;
		break;
	}
	i2c2_stop();
	*/
}
cstring axel_message(cstring Str){
	cstring Ans={0,0};
	uint8 LowData,HighData;;

	if(cstring_getc(&Str,0) == 0x10){//�f�[�^�擾
		cstring_dynamic_construct(&Ans,7);
		cstring_putc(&Ans,0,0x10);

		//======== ACC X
		while(1){
			if(i2c2_start(AXEL_ADDRESS,0))continue;
			//x�������xLowByte�v��
			if(i2c2_write(0x28))continue;
			if (i2c2_restart(AXEL_ADDRESS,1))continue;
			break;
		}
		//x�������xLowByte��M
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(AXEL_ADDRESS,0))continue;
			//x�������xHighByte�v��
			if(i2c2_write(0x29))continue;
			if (i2c2_restart(AXEL_ADDRESS,1))continue;
			break;
		}
		//x�������xHighByte��M
		HighData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,1,LowData);
		cstring_putc(&Ans,2,HighData);

		//========= ACC Y
		while(1){
			if(i2c2_start(AXEL_ADDRESS,0))continue;
			//y�����������xLowByte�v��
			if(i2c2_write(0x2A))continue;
			if (i2c2_restart(AXEL_ADDRESS,1))continue;
			break;
		}
		//y�����������xLowByte��M
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(AXEL_ADDRESS,0))continue;
			//y�����������xHighByte�v��
			if(i2c2_write(0x2B))continue;
			if (i2c2_restart(AXEL_ADDRESS,1))continue;
			break;
		}
		HighData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,3,LowData);
		cstring_putc(&Ans,4,HighData);

		//=========== ACC Z
		while(1){
			if(i2c2_start(AXEL_ADDRESS,0))continue;
			//z�����������xLowByte�v��
			if(i2c2_write(0x2C))continue;
			if (i2c2_restart(AXEL_ADDRESS,1))continue;
			break;
		}
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(AXEL_ADDRESS,0))continue;
			//z�����������xHighByte�v��
			if(i2c2_write(0x2D))continue;
			if (i2c2_restart(AXEL_ADDRESS,1))continue;
			break;
		}
		LowData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,5,LowData);
		cstring_putc(&Ans,6,HighData);
	}

	return Ans;
}
//************************ Thermo 
cstring thermo_message(cstring Str){
	cstring Ans={0,0};
	unsigned char	adcdata_l,adcdata_h;//,c;
	uint32 sumadc;

	if(cstring_getc(&Str,0) == 0x10){//�f�[�^�擾(100�񕽋�)�R�}���h		
		cstring_dynamic_construct(&Ans,3);
		cstring_putc(&Ans,0,0x10);

		//���x�f�[�^�擾
		sumadc = adc_get_agerage(Vref_GND,ADC_Thermo);
		adcdata_h = (unsigned char)((sumadc>>8)%256);
		adcdata_l = (unsigned char)(sumadc%256);
		cstring_putc(&Ans,1,adcdata_l);
		cstring_putc(&Ans,2,adcdata_h);
	}

	return Ans;
}
//************************ InfThermo 
cstring infthermo_message(cstring Str){
	cstring Ans={0,0};
	unsigned char	adcdata_l,adcdata_h;//,c;
	uint32 sumadc;

	if(cstring_getc(&Ans,0) == 0x10){//�f�[�^�擾(100�񕽋�)�R�}���h		
		cstring_dynamic_construct(&Ans,5);
		cstring_putc(&Ans,0,0x10);

		// �O�����x�擾
		sumadc = adc_get_agerage(Vref_GND,ADC_InfThermo_thermo);
		adcdata_h = (unsigned char)((sumadc>>8)%256);
		adcdata_l = (unsigned char)(sumadc%256);

		cstring_putc(&Ans,1,adcdata_l);
		cstring_putc(&Ans,2,adcdata_h);

		//�������x�擾
		sumadc = adc_get_agerage(Vref_GND,ADC_InfThermo);
		adcdata_h = (unsigned char)((sumadc>>8)%256);
		adcdata_l = (unsigned char)(sumadc%256);

		cstring_putc(&Ans,3,adcdata_l);
		cstring_putc(&Ans,4,adcdata_h);
	}

	return Ans;
}
//************************ Hygro 
cstring hygro_message(cstring Str){
	cstring Ans={0,0};
	unsigned char	adcdata_l,adcdata_h;//,c;
	uint32 sumadc;

	if(cstring_getc(&Str,0) == 0x10){//�f�[�^�擾(100�񕽋�)�R�}���h		
		cstring_dynamic_construct(&Ans,3);
		cstring_putc(&Ans,0,0x10);

		//���x�f�[�^�擾
		sumadc = adc_get_agerage(Vref_GND,ADC_Hygro);
		adcdata_h = (unsigned char)((sumadc>>8)%256);
		adcdata_l = (unsigned char)(sumadc%256);

		cstring_putc(&Ans,1,adcdata_l);
		cstring_putc(&Ans,2,adcdata_h);
	}

	return Ans;
}
//************************ Compass 
void compass_initialize(void){
	while(1){
		if(i2c2_start(COMPASS_ADDRESS,0))continue;
		//CRA_REG_M�Ȃ郌�W�X�^�ւ̏������ݗv��
		if(i2c2_write(0x00))continue;
		//CRA_REG_M�Ȃ郌�W�X�^���������@�iApplication Note p.11�̃R�s�y�j
		if(i2c2_write(0x04))continue;
		break;
	}
	i2c2_stop();

	while(1){
		if(i2c2_start(COMPASS_ADDRESS,0))continue;
		//CRB_REG_M�Ȃ郌�W�X�^�ւ̏������ݗv��
		if(i2c2_write(0x01))continue;
		//CRB_REG_M�Ȃ郌�W�X�^���������@�iApplication Note p.11�̃R�s�y�j
		if(i2c2_write(0x01))continue;
		break;
	}
	i2c2_stop();

	while(1){
		if(i2c2_start(COMPASS_ADDRESS,0))continue;
		//CRB_REG_M�Ȃ郌�W�X�^�ւ̏������ݗv��
		if(i2c2_write(0x02))continue;
		//CRB_REG_M�Ȃ郌�W�X�^���������@�iApplication Note p.11�̃R�s�y�j
		if(i2c2_write(0x00))continue;
		break;
	}
	i2c2_stop();
}
cstring compass_message(cstring Str){
	cstring Ans={0,0};
	uint8 LowData,HighData;

	if(cstring_getc(&Str,0) == 0x10){//�f�[�^�擾
		cstring_dynamic_construct(&Ans,7);
		cstring_putc(&Ans,0,0x10);

		//===========COMPASS X
		while(1){
			if(i2c2_start(COMPASS_ADDRESS,0))continue;
			//����x����LowByte�v��
			if(i2c2_write(0x04))continue;
			if (i2c2_restart(COMPASS_ADDRESS,1))continue;
			break;
		}
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(COMPASS_ADDRESS,0))continue;
			//����x����HighByte�v��
			if(i2c2_write(0x03))continue;
			if (i2c2_restart(COMPASS_ADDRESS,1))continue;
			break;
		}
		HighData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,1,LowData);
		cstring_putc(&Ans,2,HighData);

		//=========COMPASS Y
		while(1){
			if(i2c2_start(COMPASS_ADDRESS,0))continue;
			//����y����LowByte�v��
			if(i2c2_write(0x06))continue;
			if (i2c2_restart(COMPASS_ADDRESS,1))continue;
			break;
		}
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(COMPASS_ADDRESS,0))continue;
			//����y����HighByte�v��
			if(i2c2_write(0x05))continue;
			if (i2c2_restart(COMPASS_ADDRESS,1))continue;
			break;
		}
		HighData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,3,LowData);
		cstring_putc(&Ans,4,HighData);

		//============COMPASS Z
		while(1){
			if(i2c2_start(COMPASS_ADDRESS,0))continue;
			//����z����LowByte�v��
			if(i2c2_write(0x08))continue;
			if (i2c2_restart(COMPASS_ADDRESS,1))continue;
			break;
		}
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(COMPASS_ADDRESS,0))continue;
			//����z����HighByte�v��
			if(i2c2_write(0x07))continue;
			if (i2c2_restart(COMPASS_ADDRESS,1))continue;
			break;
		}
		HighData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,5,LowData);
		cstring_putc(&Ans,6,HighData);
	}

	return Ans;
}
//************************ Gyro
void gyro_initialize(void){
	while(1){
		if(i2c2_start(GYRO_ADDRESS,0))continue;
		//CRA_REG1���W�X�^�ւ̏������ݗv��
		if(i2c2_write(0x20))continue;
		//CRA_REG1���W�X�^��������
		if(i2c2_write(0x0F))continue;
		break;
	}
	i2c2_stop();
}
cstring gyro_message(cstring Str){
	cstring Ans={0,0};
	uint8 LowData,HighData;

	if(cstring_getc(&Str,0) == 0x10){//�f�[�^�擾
		cstring_dynamic_construct(&Ans,7);
		cstring_putc(&Ans,0,0x10);

		//===========Gyro X
		while(1){
			if(i2c2_start(GYRO_ADDRESS,0))continue;
			//x����LowByte�v��
			if(i2c2_write(0x28))continue;
			if (i2c2_restart(GYRO_ADDRESS,1))continue;
			break;
		}
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(GYRO_ADDRESS,0))continue;
			//x����HighByte�v��
			if(i2c2_write(0x29))continue;
			if (i2c2_restart(GYRO_ADDRESS,1))continue;
			break;
		}
		HighData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,1,LowData);
		cstring_putc(&Ans,2,HighData);

		//=========Gyro Y
		while(1){
			if(i2c2_start(GYRO_ADDRESS,0))continue;
			//y����LowByte�v��
			if(i2c2_write(0x2A))continue;
			if (i2c2_restart(GYRO_ADDRESS,1))continue;
			break;
		}
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(GYRO_ADDRESS,0))continue;
			//y����HighByte�v��
			if(i2c2_write(0x2B))continue;
			if (i2c2_restart(GYRO_ADDRESS,1))continue;
			break;
		}
		HighData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,3,LowData);
		cstring_putc(&Ans,4,HighData);

		//============Gyro Z
		while(1){
			if(i2c2_start(GYRO_ADDRESS,0))continue;
			//z����LowByte�v��
			if(i2c2_write(0x2C))continue;
			if (i2c2_restart(GYRO_ADDRESS,1))continue;
			break;
		}
		LowData=i2c2_read(1);
		i2c2_stop();

		while(1){
			if(i2c2_start(GYRO_ADDRESS,0))continue;
			//z����HighByte�v��
			if(i2c2_write(0x2D))continue;
			if (i2c2_restart(GYRO_ADDRESS,1))continue;
			break;
		}
		HighData=i2c2_read(1);
		i2c2_stop();

		cstring_putc(&Ans,5,LowData);
		cstring_putc(&Ans,6,HighData);
	}

	return Ans;
	
}
//************************ Get ADC Ave 100
/*
uint32 adc_get_agerage(unsigned char mode, unsigned char ch){
	sint8 j = 0;
	uint32 sumadc;

	setup_adc1_Vref(mode);//MODE�ݒ�
	//restart_wdt();

	set_adc_channel(ch);//ADCCh�I�� 
	//restart_wdt();

	sumadc = 0;
	for(j=0;j<100;++j){
		sumadc += (uint16)(read_adc());
		//restart_wdt();
	}
	sumadc = sumadc/100;
	return sumadc;
}*/

//************************ SpriteCmr
boolian sprite_AutoLight=0;
boolian sprite_SpeedMode=0;
#define sprite_powerLight(onoff) HMR_PIN_HEADLIGHT_PW_set(onoff)
/*void sprite_powerLight(boolian b){
	delay_ms(1);
	spi_selectDrive(1);
	if(b){
		//restart_wdt();
		delay_ms(1);
		spi2_read('<');
		delay_ms(1);
		spi2_read(0xFF);
		delay_ms(1);
	}else{
		//restart_wdt();
		delay_ms(1);
		spi2_read('<');
		delay_ms(1);
		spi2_read(0xFE);
		delay_ms(1);
	}
	spi_selectDrive(0);
}*/
cstring sprite_message(cstring Str){
	cstring Ans={0,0};
	static unsigned char* BufPtr = 0;
	unsigned char c1;
//	int i;
	if(sprite_AutoLight){
		if(cstring_getc(&Str,0)==0x10)sprite_powerLight(1);
		else sprite_powerLight(0);
	}

	//�G���[���Ȃ����Ƃ��m�F
	//�G���[������΃G���[�������s��
	if(sprite_getErrorFlag()){
		//���������m�ۂ��Ă����Ȃ�̂Ă�
		if(BufPtr != 0){
			free(BufPtr);
			BufPtr = 0;
		}

		//�G���[��`����
		cstring_dynamic_construct(&Ans,7);
		cstring_putc(&Ans,0,0xFF);						//Error
		cstring_putc(&Ans,1,sprite_getStatus());		// ���݂̃X�e�[�^�X
		cstring_putc(&Ans,2,sprite_getSubStatus());		//���݂̃T�u�X�e�[�^�X
		cstring_putc(&Ans,3, sprite_isError());			//���݃G���[��Ԃ��ǂ���
		cstring_putc(&Ans,4,sprite_getErrorStatus());	//�G���[���̃X�e�[�^�X
		cstring_putc(&Ans,5,sprite_getErrorSubStatus());//�G���[���̃T�u�X�e�[�^�X
		cstring_putc(&Ans,6,sprite_getErrorPosition());	//�G���[���̃|�W�V����

		//�G���[�t���O������
		sprite_clearErrorFlag(); 
	}
	//�������Z�b�g����
	else if(sprite_getStatus() == 0xF0){
		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,cstring_getc(&Str,0));
		cstring_putc(&Ans,1,0xF0);
	}
	//��������R�}���h�ʏ폈���Ɉڂ�
	//�������擾 
	else if(cstring_getc(&Str,0) == 0x00){
		cstring_dynamic_construct(&Ans,9);
		cstring_putc(&Ans,0,0x00);
		cstring_putc(&Ans,1,sprite_getStatus());		// ���݂̃X�e�[�^�X
		cstring_putc(&Ans,2,sprite_getSubStatus());		//���݂̃T�u�X�e�[�^�X
		cstring_putc(&Ans,3,sprite_isError());			//���݃G���[��Ԃ��ǂ���
		cstring_putc(&Ans,4,sprite_getErrorStatus());	//�G���[���̃X�e�[�^�X
		cstring_putc(&Ans,5,sprite_getErrorSubStatus());//�G���[���̃T�u�X�e�[�^�X
		cstring_putc(&Ans,6,sprite_getErrorPosition());	//�G���[���̃|�W�V����
		cstring_putc(&Ans,7,sprite_getRequestImageSize());//imageSize�𓾂�
		cstring_putc(&Ans,8,sprite_getAutoResetMode());	//AutoReset�𓾂� 
	}
	//�ʐ^�B�e
	else if(cstring_getc(&Str,0) == 0x10){
		//�ʐ^�B���Ă�����ԂȂ�
		if(sprite_isIdle() == 1){
			//�o�b�t�@���Z�b�g����Ă��Ȃ����
			if(sprite_is_setPicDataBuf()==0){
				//�o�b�t�@�Z�b�g
				BufPtr = malloc(SPRITE_PACKSIZE + SPRITE_BUF_HEADER);
				//�o�b�t�@��SPRITE�ɋ����Ă�����
				sprite_setPicDataBuf(&(BufPtr[SPRITE_BUF_HEADER]), SPRITE_PACKSIZE);
			}

			//PictureSize�ݒ�
			if(cstring_getc(&Str,1)>=2){
				c1=sprite_PictureSize_640_480;
			}else if(cstring_getc(&Str,1)==1){
				c1=sprite_PictureSize_320_240;
			}else{
				c1=sprite_PictureSize_160_120;
			}
			
			
			//SPRITE�Ŏʐ^�B�閽�ߑ���
			sprite_beginTakePic(c1);

			//�Ƃ肠������ԕԂ�
			cstring_dynamic_construct(&Ans,2);
			cstring_putc(&Ans,0,0x10);
			cstring_putc(&Ans,1,0x10);
		//�ʐ^�̃f�[�^���B���Ă�����ԂȂ�
		}else if(sprite_can_sizeAllPicData() == 1){
			//�Ƃ肠������ԕԂ�
			cstring_dynamic_construct(&Ans,4);
			cstring_putc(&Ans,0,0x10);
			cstring_putc(&Ans,1,0x11);
			cstring_putc(&Ans,2,(unsigned char)(sprite_sizeAllPicData()));
			cstring_putc(&Ans,3,(unsigned char)((sprite_sizeAllPicData())/256));
		}
		//����ȊO�c�����҂�
		else{
			cstring_dynamic_construct(&Ans,2);
			cstring_putc(&Ans,0,0x10);
			cstring_putc(&Ans,1,0x10);
		}
	}
	//�B�e�f�[�^�擾
	else if(cstring_getc(&Str,0) == 0x20){
		//�f�[�^���ׂĎ�M�����Ƃ�
		if(sprite_getStatus() == sprite_Status_FinishTakePic){
			cstring_dynamic_construct(&Ans,2);
			cstring_putc(&Ans,0,0x20);
			cstring_putc(&Ans,1,0x30);
		}
		//�f�[�^��M�\�Ȏ�
		else if(sprite_can_getPicData() == 1){
			//�o�b�t�@����f�[�^�����炤�̂ŁAstatic�ɍ쐬
			cstring_static_construct(&Ans,BufPtr,BufPtr+SPRITE_BUF_HEADER+sprite_sizePicData());
			cstring_putc(&Ans,0,0x20);
			cstring_putc(&Ans,1,0x21);
			//�擾�ς݃f�[�^�T�C�Y[2]
			cstring_putc(&Ans,2,(unsigned char)(sprite_sizeGotPicData()%256));
			cstring_putc(&Ans,3,(unsigned char)((sprite_sizeGotPicData()>>8)%256));

			//�f�[�^��M������`����
			sprite_getPicData();

			//���̃f�[�^�����鎞
			if(sprite_is_getAllPicData()==0){
				//�������m��//�o�b�t�@�Z�b�g
				BufPtr = malloc(SPRITE_PACKSIZE + SPRITE_BUF_HEADER);
				//�o�b�t�@��SPRITE�ɋ����Ă����� �擾�J�n
				sprite_setPicDataBuf(BufPtr +  SPRITE_BUF_HEADER, SPRITE_PACKSIZE);
			}
		}
		//�f�[�^��M�܂��s�\�̂Ƃ�
		else{
			cstring_dynamic_construct(&Ans,4);
			cstring_putc(&Ans,0,0x20);
			cstring_putc(&Ans,1,sprite_getStatus());
			//�擾�ς݃f�[�^�T�C�Y[2]
			cstring_putc(&Ans,2,(unsigned char)(sprite_sizeGotPicData()%256));
			cstring_putc(&Ans,3,(unsigned char)((sprite_sizeGotPicData()>>8)%256));
		}
	}
	//�B�e�f�[�^�擾����
	else if(cstring_getc(&Str,0) == 0x30){
		//�ʐ^�B�e���I����R�}���h
		sprite_endTakePic();

		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x30);
		cstring_putc(&Ans,1,0x00);
	}

	//�������Z�b�g�v��
	else if(cstring_getc(&Str,0) == 0x40){
		//���Z�b�g���s��
		sprite_requestCommandReset();

		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x40);
		cstring_putc(&Ans,1, sprite_getStatus());
	}
	//�G���[�������������Z�b�gON�EOFF
	else if(cstring_getc(&Str,0) == 0x50){
		//AutoResetMode�ݒ�
		sprite_setAutoResetMode(cstring_getc(&Str,1));
		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x50);
		cstring_putc(&Ans,1, sprite_getAutoResetMode());
	}
	//�������Z�b�g
	else if(cstring_getc(&Str,0) == 0xF0){
		//�������Z�b�g��������
		sprite_requestPowerReset();

		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0xF0);
		cstring_putc(&Ans,1, sprite_getStatus());
	}
	//���C�g�̐ݒ�
	else if(cstring_getc(&Str,0) == 0x60){
		sprite_AutoLight=cstring_getc(&Str,1);

		//���C�g������
		if(sprite_AutoLight==0)sprite_powerLight(0);

		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x60);
		cstring_putc(&Ans,1, sprite_AutoLight);
	}

	return Ans;
}
//static unsigned char Buffer1[sprite_BufferHeader+sprite_BufferSize_LARGE];
//static unsigned char Buffer2[sprite_BufferHeader+sprite_BufferSize_LARGE];
static unsigned char Buffer[sprite_BufferNum][sprite_BufferHeader+sprite_BufferSize_LARGE];
cstring sprite_message2(cstring Str){
	cstring Ans={0,0};
	static boolian BufferUsed=0;
	static unsigned char* BufferPtr=0;
	static unsigned char BufferNo=0;
	unsigned char c1;

	if(cstring_size(&Str)<1)return;

	//�G���[���Ȃ����Ƃ��m�F
	//�G���[������΃G���[�������s��
	if(sprite_getErrorFlag()){
		//���������m�ۂ��Ă����Ȃ�̂Ă�
//		if(BufPtr != 0){
//			free(BufPtr);
//			BufPtr = 0;
//		}

		//�G���[��`����
		cstring_dynamic_construct(&Ans,7);
		cstring_putc(&Ans,0,0xFF);						//Error
		cstring_putc(&Ans,1,sprite_getStatus());		// ���݂̃X�e�[�^�X
		cstring_putc(&Ans,2,sprite_getSubStatus());		//���݂̃T�u�X�e�[�^�X
		cstring_putc(&Ans,3, sprite_isError());			//���݃G���[��Ԃ��ǂ���
		cstring_putc(&Ans,4,sprite_getErrorStatus());	//�G���[���̃X�e�[�^�X
		cstring_putc(&Ans,5,sprite_getErrorSubStatus());//�G���[���̃T�u�X�e�[�^�X
		cstring_putc(&Ans,6,sprite_getErrorPosition());	//�G���[���̃|�W�V����

		//�G���[�t���O������
		sprite_clearErrorFlag(); 
	}
	//�������Z�b�g����
	else if(sprite_getStatus() == 0xF0){
		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,cstring_getc(&Str,0));
		cstring_putc(&Ans,1,0xF0);
	}
	//��������R�}���h�ʏ폈���Ɉڂ�
	//�������擾 
	else if(cstring_getc(&Str,0) == 0x00){
		cstring_dynamic_construct(&Ans,9);
		cstring_putc(&Ans,0,0x00);
		cstring_putc(&Ans,1,sprite_getStatus());		// ���݂̃X�e�[�^�X
		cstring_putc(&Ans,2,sprite_getSubStatus());		//���݂̃T�u�X�e�[�^�X
		cstring_putc(&Ans,3,sprite_isError());			//���݃G���[��Ԃ��ǂ���
		cstring_putc(&Ans,4,sprite_getErrorStatus());	//�G���[���̃X�e�[�^�X
		cstring_putc(&Ans,5,sprite_getErrorSubStatus());//�G���[���̃T�u�X�e�[�^�X
		cstring_putc(&Ans,6,sprite_getErrorPosition());	//�G���[���̃|�W�V����
		cstring_putc(&Ans,7,sprite_getRequestImageSize());//imageSize�𓾂�
		cstring_putc(&Ans,8,sprite_getAutoResetMode());	//AutoReset�𓾂� 
	}
	//�ʐ^�B�eor�f�[�^�v�����[�h
	else if((cstring_getc(&Str,0)&0xF0) == 0x10){
		//�ʐ^�B���Ă�����ԂŁA�B�e�v���������ꍇ
		if(sprite_isIdle() == 1 && BufferUsed==0){
			//�o�b�t�@���Z�b�g����Ă��Ȃ����
			if(sprite_is_setPicDataBuf()==0){
				//�o�b�t�@�Z�b�g
				//�o�b�t�@��SPRITE�ɋ����Ă�����
				BufferPtr=Buffer[(BufferNo++)%sprite_BufferNum];
				if(sprite_SpeedMode){
					sprite_setPicDataBuf(BufferPtr+sprite_BufferHeader, sprite_BufferSize_LARGE);
				}else{
					sprite_setPicDataBuf(BufferPtr+sprite_BufferHeader, sprite_BufferSize_SMALL);
				}
				BufferUsed=1;
			}

			//PictureSize�ݒ�
			if(cstring_getc(&Str,1)>=2){
				c1=sprite_PictureSize_640_480;
			}else if(cstring_getc(&Str,1)==1){
				c1=sprite_PictureSize_320_240;
			}else{
				c1=sprite_PictureSize_160_120;
			}
			
			if(sprite_AutoLight){
				sprite_powerLight(1);
				delay_ms(200);
			}

			//SPRITE�Ŏʐ^�B�閽�ߑ���
			sprite_beginTakePic(c1);

			//�ʐ^�f�[�^������Ă�����Ԃ܂ő҂�
			while(!sprite_can_sizeAllPicData()){
				if(sprite_isError()){
					sprite_powerLight(0);
					return;
				}
			}

			if(sprite_AutoLight){
				sprite_powerLight(0);
			}

			//�f�[�^������܂ő҂�
			while(!sprite_can_getPicData()){
				if(sprite_isError())return;
			}

			//�o�b�t�@����f�[�^�����炤�̂ŁAstatic�ɍ쐬
			msg_set_nondelete_mode();
			cstring_static_construct(&Ans,BufferPtr,BufferPtr+sprite_BufferHeader+sprite_sizePicData());
			cstring_putc(&Ans,0,(unsigned char)(0x10));
			cstring_putc(&Ans,1,(unsigned char)(sprite_sizeAllPicData()));
			cstring_putc(&Ans,2,(unsigned char)(sprite_sizeAllPicData()>>8));

			//�f�[�^��M������`����
			sprite_getPicData();
			BufferUsed=0;
			delay_ms(5);
			//���̃f�[�^�����鎞
			if(sprite_is_getAllPicData()){
				//�ʐ^�B�e���I����R�}���h
				sprite_endTakePic();
			}//���̃f�[�^���Ȃ���
			else{
				//�o�b�t�@�Z�b�g
				BufferPtr=Buffer[(BufferNo++)%sprite_BufferNum];
				if(sprite_SpeedMode){
					sprite_setPicDataBuf(BufferPtr+sprite_BufferHeader, sprite_BufferSize_LARGE);
				}else{
					sprite_setPicDataBuf(BufferPtr+sprite_BufferHeader, sprite_BufferSize_SMALL);
				}
				BufferUsed=1;
			}
		}else{
			//�f�[�^������܂ő҂�
			while(!sprite_can_getPicData()){
				if(sprite_isError())return;
			}

			//�o�b�t�@����f�[�^�����炤�̂ŁAstatic�ɍ쐬
			msg_set_nondelete_mode();
			cstring_static_construct(&Ans,BufferPtr,BufferPtr+3+sprite_sizePicData());
			cstring_putc(&Ans,0,(unsigned char)(0x11));
			cstring_putc(&Ans,1,(unsigned char)(sprite_sizeGotPicData()));
			cstring_putc(&Ans,2,(unsigned char)(sprite_sizeGotPicData()>>8));

			//�f�[�^��M������`����
			sprite_getPicData();
			BufferUsed=0;
			delay_ms(5);

			//���̃f�[�^�����鎞
			if(sprite_is_getAllPicData()){
				//�ʐ^�B�e���I����R�}���h
				sprite_endTakePic();
			}//���̃f�[�^���Ȃ���
			else{
				//�o�b�t�@�Z�b�g
				BufferPtr=Buffer[(BufferNo++)%sprite_BufferNum];
				if(sprite_SpeedMode){
					sprite_setPicDataBuf(BufferPtr+sprite_BufferHeader, sprite_BufferSize_LARGE);
				}else{
					sprite_setPicDataBuf(BufferPtr+sprite_BufferHeader, sprite_BufferSize_SMALL);
				}
				BufferUsed=1;
			}
		}
	}//�������Z�b�g�v��
	else if(cstring_getc(&Str,0) == 0x40){
		//���Z�b�g���s��
		sprite_requestCommandReset();

		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x40);
		cstring_putc(&Ans,1, sprite_getStatus());
	}//�G���[�������������Z�b�gON�EOFF
	else if(cstring_getc(&Str,0) == 0x51){
		//AutoResetMode�ݒ�
		sprite_setAutoResetMode(cstring_getc(&Str,1));
		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x51);
		cstring_putc(&Ans,1, sprite_getAutoResetMode());
	}//�������Z�b�g
	else if(cstring_getc(&Str,0) == 0x50){
		//�������Z�b�g��������
		sprite_requestPowerReset();

		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x50);
		cstring_putc(&Ans,1, sprite_getStatus());
	}//���C�g�̎���ON/OFF�̐ݒ�
	else if(cstring_getc(&Str,0) == 0x61){
		sprite_AutoLight=cstring_getc(&Str,1);

		//���C�g��������
		if(sprite_AutoLight==0)sprite_powerLight(0);

		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x61);
		cstring_putc(&Ans,1, sprite_AutoLight);
	}//���C�g��ON/OFF
	else if(cstring_getc(&Str,0) == 0x60){
		if(!sprite_AutoLight){
			sprite_powerLight(cstring_getc(&Str,1));
			cstring_dynamic_construct(&Ans,2);
			cstring_putc(&Ans,0,0x60);
			cstring_putc(&Ans,1, cstring_getc(&Str,1));
		}else{
			cstring_dynamic_construct(&Ans,2);
			cstring_putc(&Ans,0,0x60);
			cstring_putc(&Ans,1,0xFF);
		}
	}//�X�s�[�h���[�h���ǂ����̐ݒ�
	else if(cstring_getc(&Str,0)==0x70){
		sprite_SpeedMode=cstring_getc(&Str,1);
		cstring_dynamic_construct(&Ans,2);
		cstring_putc(&Ans,0,0x70);
		cstring_putc(&Ans,1, sprite_SpeedMode);
	}
	return Ans;
}
//��M���Ȃ��瑗�M����ꍇ��Message�ɓo�^���邽�߂̊֐�
//	���݂͎̂�M������ɂ̂ݑ��M���Ă���̂ŁA�s�g�p
msg_dsize_t sprite_sendable_bufdata(void){return sprite_sizePicData();}
#
#endif
