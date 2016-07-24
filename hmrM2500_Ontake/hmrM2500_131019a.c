#ifndef HMR_M2500_INC
#define HMR_M2500_INC 200
#
#define HMLIB_NOLIB
/*
===hmrMain2500===
hmrMainPic�p�̃��C���v���O����
hmrMain2500:v2_00/130720 hmIto
	�f�o�b�O�p�̊֐��Q��R�����g�������Đ���
hmrMain2500_v1_01/130202 iwahori
	main�֐�����msg_setup_talk��msg_setup_listen��ǉ�
	listen�֐��ɓ���Ă���̂Ŏ�M�͏�肭�����Ă��邪�ԐM���Ȃ��C
	�p�P�b�g�̋�؂�����packet_talk�Ƃ�������Ă��Ȃ��̂������H
	fp_finRecvPac,com_vmc_finRecvPac�ɈϏ��H
hmrMain2500_v1_00/130112 hmIto
	hmLib_v3_03�ɑΉ�
	�S��d�ʐM�ɑΉ�
	�Z���T�[�����𕪗�����include
hmrMain2400_v1_09/121101 hmIto
	restart_wdt��main-loop�̈ꂩ���ɏW��
	�p�P�b�g��؂��HMR_COM_PACTRMNID���g�p
	��M�p�P�b�g���t���ɂȂ����ꍇ�́A�������Z�b�g
	�������Z�b�g�R�}���h�Ƃ���Message�Ɉˑ����Ȃ�(Com�ɂ͈ˑ�)�A0xFF���߂�ǉ�
	->�o�O�����������߈ꎞ�I�ɔp�~
	sprite_message2���g�p����悤�ɕύX
hmrMain2400_v1_08/121031 hmIto
	devctr�ɔ��X���[�v���̓d������p�ϐ��Ƃ��̃R�}���h��ǉ�
hmrMain2400_v1_07/121030 iwahori
	�����x��msg_resist��ǉ�
hmrMain2400_v1_06/121028 hmIto
	�X���[�v�@�\�ƃ��[�~���O�@�\�i�ʐM�f�o�C�X�؂�ւ��j������
		devctr�֐��Q��p���đ���
		��������Adevice_message�R�}���h��p���Đ��䂷��
		timer4_5��p���Ă���
			�X���[�v����10�b�Ɉ�x���荞�܂��āA���Ԍv��
			���[�~���O���̃f�o�C�X�؂�ւ��Ɏg�p
	���M�o�b�t�@�Ɋ�Â����M���荞�݊��荞�ݐ�����̗p
		���M�o�b�t�@������ɂȂ�����disable
		���M�o�b�t�@�Ƀf�[�^��ǉ�������enable
		com_out_empty�֐����g���āA���M�o�b�t�@���󂩂ǂ����m�F
hmrMain2400_v1_05/121027 hmIto
	uart�̑���M������ɍs���邱�Ƃ܂Ŋm�F
		hmrTask����邳�����Ă����̂Ŕp�~�D
		�o�b�t�@�̐��䎞�ɑ��M���荞�݂𐧌䂷��悤�ɕύX
hmrMain2400_v1_04/121020 iwahori
	initialize()����SPI slave slect�s���̏�������ǉ�
		RF module�d���I��
hmrMain2400_v1_03/121020 hmIto
	�f�o�b�O�C�����m�F���āA�R�����g�A
	�E�g���폜
	packet_message�֐����������������̂�ǉ�
hmrMain2400_v1_02/121014 iwahori
	#include "hmrSpriteFunctions.h"��ǉ�
	p_DIP1��UserConfigBit1�ɕύX
	uart1_fget_interrupt_retry_task()��uart1_fput_interrupt()�̏��Ԃ����ւ�
	���M�����l
	packet_message()��cstring Ans{1,1}��cstring Ans={1,1}
	sprite_ON��sprite_power�ɕύX
hmrMain2400_v1_01/121013 hmIto
	���炭����M�������قڊ���
	�������I�[�����K�v�Ȋ֐��̍쐬���c��
hmrMain2400_v1_00/120921 hmIto
	�o�[�W�����Ǘ��J�n
	�t�@�C���𕡐��ɕ���
*/
#include <stdlib.h>
#include "amPic30_v7.h"
#include "hmrVMC1.h"
#include "hmrCom.h"
#include "hmrMessage.h"
#include "hmrCO2.h"
#include "hmrBattery.h"
#include "hmrGPS.h"
#include "hmrDrive.h"
#include "hmrInertial.h"
#include "hmrInfraRed.h"
#include "hmrSprite.h"
#include "hmrThermo.h"
//#include "hmrDeviceManage.h"
//#include "hmrSpriteFunctions.h"
#include "hmrSpriteInterface.h"
#include "hmrDebug.h"
//VMC1
VMC1* pVMC;
//********************** �������֐�
void initialize(void){

	spi_selectGPS(0);
	spi_selectDrive(0);
	subPIC_power(0);
	i2c_power(0);
	adc_power(0);

	//�f�o�C�X������
	hmr_device_initialize();
	hmr_clock_initialize_withNormal();
	hmr_interrupt_initalize();

	if(Config_DefaultMobilePhone){
		hmr_uart1_initialize(UART1_MODULEID_MP);
	}else{
		hmr_uart1_initialize(UART1_MODULEID_RF);
	}
	hmr_uart1_power_switch(1,hmr_uart1_getModuleID());

	rs_fputc('O',Stream_VMC);
	rs_fputc('K',Stream_VMC);
	rs_fputc((unsigned char)(RCON),Stream_VMC);
	rs_fputc((unsigned char)(RCON>>8),Stream_VMC);
	rs_fputc(0x0d,Stream_VMC);
	rs_fputc(0x0a,Stream_VMC);

	hmr_uart2_initialize();
	hmr_spi2_initialize();
	hmr_i2c_initialize();
	hmr_adc_initialize();

	rs_fputc('I',Stream_VMC);
	rs_fputc('n',Stream_VMC);
	rs_fputc(0x0d,Stream_VMC);
	rs_fputc(0x0a,Stream_VMC);

	spi_selectGPS(0);
	spi_selectDrive(0);
	subPIC_power(1);
	i2c_power(1);
	adc_power(1);
 //       HMR_PIN_I2C_ACCEL0_PW_set(1);
//        HMR_PIN_ADCPW_CH0to6_set(1);

    HMR_PIN_ADCPW_CH7_set(1);
    HMR_PIN_ADCPW_CH8_set(1);

	rs_fputc('P',Stream_VMC);
	rs_fputc('w',Stream_VMC);
	rs_fputc(0x0d,Stream_VMC);
	rs_fputc(0x0a,Stream_VMC);

	inertial_initialize();

	//�ʐM�֘A�@�\�̏�����
	pVMC=com_createVMC1();
	vmc1_initialize(pVMC,(const unsigned char*)("hmr"),(const unsigned char*)("ctr"));
	com_initialize();
	msg_initialize();

	rs_fputc('C',Stream_VMC);
	rs_fputc('m',Stream_VMC);
	rs_fputc(0x0d,Stream_VMC);
	rs_fputc(0x0a,Stream_VMC);

	//�^�X�N�@�\�̏�����
//	task_initialize();

	sprite_power(1);
	sprite_initialize();

	set_interrupt_fputflag(Stream_VMC);//���荞�ݗ\��
	enable_interrupt_fget(Stream_VMC);
	enable_interrupt_fput(Stream_VMC);

	//Watch dog timer
	if(Config_DisableWatchDogTimer){//DIP��OFF�̂Ƃ�
		disable_wdt();
	}else{
		enable_wdt();
	}

}
/*void initialize(void){
	devmng_initialize();

	inertial_initialize();

	//�ʐM�֘A�@�\�̏�����
	pVMC=com_createVMC1();
	vmc1_initialize(pVMC,(const unsigned char*)("hmr"),(const unsigned char*)("ctr"));
	com_initialize();
	msg_initialize();

	rs_fputc('C',Stream_VMC);
	rs_fputc('m',Stream_VMC);
	rs_fputc(0x0d,Stream_VMC);
	rs_fputc(0x0a,Stream_VMC);

	sprite_power(1);
	sprite_initialize();


	set_interrupt_fputflag(Stream_VMC);//���荞�ݗ\��
	enable_interrupt_fget(Stream_VMC);
	enable_interrupt_fput(Stream_VMC);
}*/
void finalize(void){
	//�J�����̏I�[������
	sprite_finalize();
	
	inertial_finalize();

	//�^�X�N�@�\�̏I�[������
//	task_finalize();
	
	//�ʐM�֘A�̏I�[������
	msg_finalize();
	vmc1_finalize(pVMC);
	com_finalize();
	com_releaseVMC1(pVMC);
}
//********************** �ʐM
//��M���荞�݊֐�
void uart1_fget_interrupt(void){
	unsigned char prv;

	//�����݃t���O���N���A
	clear_interrupt_fgetflag(Stream_VMC);

	//@@@@@@ Begin Critical @@@@@@
	prv = hmr_lock_interrupt();

	//�f�[�^����M���ACom�ɏ���������
	vmc1_recv(pVMC,rs_fgetc(Stream_VMC));

	hmr_release_interrupt(prv);	
	//@@@@@@ End Critical @@@@@@

	HMR_PIN_RedLED_set(0);//debug_am

}
//���M���荞�݊֐�
void uart1_fput_interrupt(void){
	// debug +
	unsigned char prv;

	clear_interrupt_fputflag(Stream_VMC);

	//@@@@@@ Begin Critical @@@@@@
	prv = hmr_lock_interrupt();

	//���M�\�Ȃ�A���M
	rs_fputc(vmc1_send(pVMC),Stream_VMC);

	//���M�\�łȂ��Ȃ����ꍇ�́A���荞�݂�؂�
	if(!vmc1_can_send(pVMC)){
		uart1_disable_fput_interrupt();
	}

	hmr_release_interrupt(prv);
	//@@@@@@ End Critical @@@@@@

};
//********************** dEBUG
hmLib_boolian DebugFlag;
void debug_setup_talk(){DebugFlag=hmLib_true;}
void debug_setup_listen(){}
hmLib_boolian debug_listen(cstring Str){return hmLib_false;}
hmLib_boolian debug_talk(cstring* pStr){
   static unsigned char Buf[7];
	if(DebugFlag){
		DebugFlag=hmLib_false;
		cstring_construct_static(pStr,Buf,Buf+7);
		cstring_putc(pStr,0,_cstring_debug_CreateCnt);
		cstring_putc(pStr,1,_cstring_debug_StaticCnt);
		cstring_putc(pStr,2,_cstring_debug_DynamicCnt);
		cstring_putc(pStr,3,_cstring_debug_ErrorStatic);
		cstring_putc(pStr,4,_cstring_debug_ErrorDynamic);
		cstring_putc(pStr,5,_cstring_debug_ErrorPre);
		cstring_putc(pStr,6,_cstring_debug_ErrorDestruct);
		return hmLib_false;
	}
	return hmLib_true;
}
//################## MAIN ###################################
int main(void){
	unsigned char prv;

	idata IData;
	boolian IPacketMode=0;
	odata OData;
	boolian OPacketMode=0;
	sint8 debug_cnt=0;

	//������
	initialize();

	//���b�Z�[�W�o�^
	msg_regist('m',motor_setup_listen,motor_listen,motor_setup_talk,motor_talk);
	msg_regist('j',sprite_setup_listen,sprite_listen,sprite_setup_talk,sprite_talk);
	msg_regist('b',battery_setup_listen,battery_listen,battery_setup_talk,battery_talk);
	msg_regist('g',gps_setup_listen,gps_listen,gps_setup_talk,gps_talk);
	msg_regist('C',co2_setup_listen,co2_listen,co2_setup_talk,co2_talk);
//	msg_regist('S',h2s_message);
	msg_regist('t',thermo_setup_listen,thermo_listen,thermo_setup_talk,thermo_talk);
//	msg_regist('T',infrared_setup_listen,infrared_listen,infrared_setup_talk,infrared_talk);
	msg_regist('h',0,0,0,0);
	msg_regist('c',compass_setup_listen,compass_listen,compass_setup_talk,compass_talk);
	msg_regist('a',axel_setup_listen,axel_listen,axel_setup_talk,axel_talk);
	msg_regist('G',gyro_setup_listen,gyro_listen,gyro_setup_talk,gyro_talk);
msg_regist('!',debug_setup_listen,debug_listen,debug_setup_talk,debug_talk);

/*
	HMR_PIN_RedLED_set(0);
	HMR_PIN_YellowLED_set(0);
        delay_ms(500);
	HMR_PIN_RedLED_set(1);
	HMR_PIN_YellowLED_set(1);
        delay_ms(500);
	HMR_PIN_RedLED_set(0);
	HMR_PIN_YellowLED_set(0);
        delay_ms(500);
	HMR_PIN_RedLED_set(1);
	HMR_PIN_YellowLED_set(1);
        delay_ms(500);
	HMR_PIN_RedLED_set(0);
	HMR_PIN_YellowLED_set(0);

	HMR_PIN_RedLED_set(1);
*/
	HMR_PIN_DebugLED_set(0);
	HMR_PIN_Debug2LED_set(0);

	while(1){

		delay_ms(5);
		//Watch Dog Timer ���Z�b�g
		restart_wdt();
			
		//��M�\�ȃf�[�^��������΍Ăё҂�
		if(!com_in_empty()){
			//@@@@@@ Begin Critical@@@@@@
			prv = hmr_lock_interrupt();

			com_in_move_pop(&IData);
			
			hmr_release_interrupt(prv);
			//@@@@@@ End Critical@@@@@@

			if(idata_is_construct(&IData)){
				//PacketMode�ɓ����Ă��Ȃ��Ƃ�
				if(!IPacketMode){
					//Packet�������ŊJ��
					IPacketMode=hmLib_true;
					msg_setup_listen();

				}

				//Packet�I��ID�������ꍇ
				if(IData.ID==HMR_COM_PACTRMNID){
					//Packet�������ŏI��
					IPacketMode=hmLib_false;

					prv = hmr_lock_interrupt(); 
					idata_destruct(&IData);
					hmr_release_interrupt(prv); 
	
				}else{
					//���b�Z�[�W����
					prv = hmr_lock_interrupt();
					msg_listen(&IData);
					hmr_release_interrupt(prv);
				}
			}
		}

		delay_ms(5);

		//���M�҂���Packet���Ȃ��Acom�̑��M�o�b�t�@�������ς��ł��Ȃ��Ƃ�
		if(!com_isWaitSendPacket() && !com_out_full()){
			//PacketMode�ɓ����Ă��Ȃ��Ƃ�
			if(!OPacketMode){
				//Packet�������ŊJ��
				OPacketMode=hmLib_true;
				msg_setup_talk();
			}

			//���M�p�f�[�^����������
			odata_format(&OData);

			//���M�f�[�^�̎擾�Ɏ��s�����ꍇ
			prv = hmr_lock_interrupt();
			if(msg_talk(&OData)){
				//�����ō쐬����Ă���Δj��
				if(odata_is_construct(&OData))odata_destruct(&OData);
				//Packet�������ŕ���
				OPacketMode=hmLib_false;
				OData.ID=HMR_COM_PACTRMNID;
			}
			hmr_release_interrupt(prv);

			//���M�f�[�^��o�^����
			//@@@@@@ Begin Critical@@@@@@
			prv = hmr_lock_interrupt();

			com_out_move_push(&OData);

			hmr_release_interrupt(prv);
			//@@@@@@ End Critical@@@@@@
			
			
		}

		//���M���荞�݂��؂��Ă��āA�����M�\��Ԃ̂Ƃ��ɂ́A���M���荞�݂��I���ɂ���
		if(!uart1_is_enable_fput_interrupt()){
			if(vmc1_can_send(pVMC))uart1_enable_fput_interrupt();
		}

if(debug_cnt<0){
    HMR_PIN_DebugLED_set(1);
}else{
    HMR_PIN_DebugLED_set(0);
}
++debug_cnt;

	}

	finalize();

	return 0;
}
#
#endif
