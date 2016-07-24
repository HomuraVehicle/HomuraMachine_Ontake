#ifndef AMPIC30EXTEND_C_INC
#define AMPIC30EXTEND_C_INC 101
#
#define HMLIB_NOLIB
#ifndef AMPIC30EXTEND_INC
#	include "amPic30Extend.h"
#endif

void uart1_enable_fget_interrupt(void){
	IFS0bits.U1RXIF = U1STAbits.URXDA;
	IEC0bits.U1RXIE = 1;
}
void uart1_disable_fget_interrupt(void){
	IEC0bits.U1RXIE = 0;
}
boolian uart1_is_enable_fget_interrupt(void){return IEC0bits.U1RXIE;}
void uart1_set_fget_interrupt_flag(void){
	IFS0bits.U1RXIF = 1;
}
void uart1_clear_fget_interrupt_flag(void){
	IFS0bits.U1RXIF = 0;
}
void uart1_enable_fput_interrupt(void){
	IEC0bits.U1TXIE = 1;
}
void uart1_disable_fput_interrupt(void){
	IEC0bits.U1TXIE = 0;
}
boolian uart1_is_enable_fput_interrupt(void){return IEC0bits.U1TXIE;}
void uart1_set_fput_interrupt_flag(void){
	IFS0bits.U1TXIF = 1;
}
void uart1_clear_fput_interrupt_flag(void){
	IFS0bits.U1TXIF = 0;
}
void uart2_enable_fget_interrupt(void){
	IFS1bits.U2RXIF = U2STAbits.URXDA;
	IEC1bits.U2RXIE = 1;
}
void uart2_disable_fget_interrupt(void){
	IEC1bits.U2RXIE = 0;
}
boolian uart2_is_enable_fget_interrupt(void){return IEC1bits.U2RXIE;}
void uart2_set_fget_interrupt_flag(void){
	IFS1bits.U2RXIF = 1;
}
void uart2_clear_fget_interrupt_flag(void){
	IFS1bits.U2RXIF = 0;
}
void uart2_enable_fput_interrupt(void){
	IEC1bits.U2TXIE = 1;
}
void uart2_disable_fput_interrupt(void){
	IEC1bits.U2TXIE = 0;
}
boolian uart2_is_enable_fput_interrupt(void){return IEC1bits.U2TXIE;}
void uart2_set_fput_interrupt_flag(void){
	IFS1bits.U2TXIF = 1;
}
void uart2_clear_fput_interrupt_flag(void){
	IFS1bits.U2TXIF = 0;
}
//timer1�p�֐�
void timer1_initialize(uint16 time){ini_timer1(time);}
//timer1�̊��荞�݃t���O���N���A
void timer1_clear_interrupt_flag(void){
	IFS0bits.T1IF = 0;			//T1���荞�݃t���O���N���A
}
//timer1�̊��荞�݃t���O���Z�b�g
void timer1_set_interrupt_flag(void){
	IFS0bits.T1IF = 1;			//T1���荞�݃t���O���N���A
}
//timer1�̃J�E���g�����Z�b�g
void timer1_reset(void){
	TMR1 = 0;					//T1�J�E���g���N���A
}
//timer1�����荞�ݗL�����ǂ����̊m�F
boolian timer1_is_enable_interrupt(void){return IEC0bits.T1IE!=0;}
//timer1�̊��荞�ݗL��
void timer1_enable_interrupt(void){enable_interrupt_timer1();}
//timer1�̊��荞�ݖ���
void timer1_disable_interrupt(void){disable_interrupt_timer1();}
//timer4&5�A���^�C�}�[�̏�����
void timer4_5_initialize(uint16 time){//time(ms)�i1�`10000ms�̊ԁj�Ɉ�񊄂荞��
	ini_timer4(time);
}
//timer4&5�A���^�C�}�[�̏I�[��
void timer4_5_finalize(){
	timer4_5_disable_interrupt();
	timer4_5_clear_interrupt_flag();
	timer4_5_reset();
}
//timer4&5�̊��荞�݃t���O���N���A
void timer4_5_clear_interrupt_flag(void){
	IFS1bits.T5IF = 0;			
}
//timer4&5�̊��荞�݃t���O���Z�b�g
void timer4_5_set_interrupt_flag(void){
	IFS1bits.T5IF = 1;			
}
//timer4&5�̃J�E���g�����Z�b�g
void timer4_5_reset(void){
	TMR5 = 0x00; // Clear 32-bit Timer (msw)
	TMR4 = 0x00; // Clear 32-bit Timer (lsw)
}
//timer4&5�����荞�ݗL�����ǂ����̊m�F
boolian timer4_5_is_enable_interrupt(void){return IEC1bits.T5IE!=0;}
//timer4&5�̊��荞�ݗL��
void timer4_5_enable_interrupt(void){enable_interrupt_timer4();}
//timer4&5�̊��荞�ݖ���
void timer4_5_disable_interrupt(void){disable_interrupt_timer4();}
//====================RTCC====================
#define _rtcc_lock() 		RCFGCALbits.RTCWREN=0;
#define _rtcc_active()		RCFGCALbits.RTCEN = 1;
#define	_rtcc_inactive()		RCFGCALbits.RTCEN = 0;
//#define EnableSecOsc()	 __builtin_write_OSCCONL(0x02);
void _rtcc_unlock(void){
	asm("MOV #0x55,W0");
	asm("MOV W0,NVMKEY");                      // Write the 0x55 key
	asm("MOV #0xAA,W1");
	asm("MOV W1,NVMKEY");                      // Write the 0xAA key

	RCFGCALbits.RTCWREN = 1;
}
//RTCC�p�Z�J���h�I�V���[�^�������v���Z�X
void rtcc_oscilator_initialize(void){
	__builtin_write_OSCCONL(0x02);		// EnableSecOsc
}
//RTCC�������֐� ���������荞�݊Ԋu(�b)
void rtcc_initialize(unsigned char YMD[6],unsigned char WeekNo,unsigned char HMS[6]){
	uint16 Val[4]={0,0,0,0};
	Val[3]|=(((HMS[5]-'0')&0x0F)<<0);
	Val[3]|=(((HMS[4]-'0')&0x0F)<<1);
	Val[3]|=(((HMS[3]-'0')&0x0F)<<2);
	Val[3]|=(((HMS[2]-'0')&0x0F)<<3);
	Val[2]|=(((HMS[1]-'0')&0x0F)<<0);
	Val[2]|=(((HMS[0]-'0')&0x0F)<<1);
	Val[2]|=(((WeekNo-'0')&0x0F)<<2);
	Val[1]|=(((YMD[5]-'0')&0x0F)<<0);
	Val[1]|=(((YMD[4]-'0')&0x0F)<<1);
	Val[1]|=(((YMD[3]-'0')&0x0F)<<2);
	Val[1]|=(((YMD[2]-'0')&0x0F)<<3);
	Val[0]|=(((YMD[1]-'0')&0x0F)<<0);
	Val[0]|=(((YMD[0]-'0')&0x0F)<<1);

	//===RTCC Critical Begin===
	_rtcc_unlock();								//unlock Timer Registers
	_rtcc_inactive();                                  //disable the RTCC peripheral

	/* Configure the alarm settings*/
	ALCFGRPTbits.ALRMEN = 0;					//alarm enable flag
	ALCFGRPTbits.CHIME = 0;  	 				//no rolloever of the repeat count	
	ALCFGRPTbits.AMASK = 0;                     //alarm mask configuration bits
	ALCFGRPTbits.ARPT =  0;                     //alarm repeat counter value bits 	

	RCFGCALbits.RTCOE = 0;   	//��p�s������O���ւ̃N���b�N�o�͂�L���ɂ��邩? 0:����
//	PADCFG1 = 0x0000;			//�O���ւ̏o�͂�L���ɂ����ۂ̏o�͓��e�̐ݒ�

	/* Load the initial values to the RTCC value registers*/
	RCFGCALbits.RTCPTR = 0b11;						//point to Year register
	//RTCVAL�֏������ނƎ����I��RTCVAL�̎Q�Ɛ悪�C���N�������g�����
	RTCVAL = Val[0];
	RTCVAL = Val[1];
	RTCVAL = Val[2];
	RTCVAL = Val[3];

	RCFGCALbits.CAL=0x0000;                     //�N���b�N�̌덷�̕␳ 0�Ȃ�␳����

	_rtcc_active();                                   //enable RTCC peripheral
	_rtcc_lock();                                 //lock the RTCC value registers
	//===RTCC Critical End===
}
//RTCC�̊��荞�݃t���O���N���A
void rtcc_clear_interrupt_flag(void){IFS3bits.RTCIF = 0; }
//RTCC�̊��荞�݃t���O���Z�b�g
void rtcc_set_interrupt_flag(void){IFS3bits.RTCIF = 1; }
//RTCC�̃J�E���g�����Z�b�g
void rtcc_reset_interrupt(void){IFS3bits.RTCIF = 0;}
//RTCC�����荞�ݗL�����ǂ����̊m�F
boolian rtcc_is_enable_interrupt(void){return ALCFGRPTbits.ALRMEN;}
//RTCC�̊��荞�ݗL��
void rtcc_enable_interrupt(unsigned char MD[4],unsigned char WeekNo,unsigned char HMS[6],boolian IterateNoLimitFlag_,uint8 IterateNum_){
	unsigned char MaskFlag=1;
	uint16 Val[3]={0,0,0};
	//���s���Ȃ犄�荞�݂���O���
	if(rtcc_is_enable_interrupt())return;

	MaskFlag=1;		//���b

	if(HMS[5]!='-'){
		Val[2]|=(((HMS[5]-'0')&0x0F)<<0);	
		MaskFlag=2;	//��10�b
	}
	if(HMS[4]!='-'){
		Val[2]|=(((HMS[4]-'0')&0x0F)<<1);	
		MaskFlag=3;	//����
	}
	if(HMS[3]!='-'){
		Val[2]|=(((HMS[3]-'0')&0x0F)<<2);	
		MaskFlag=4;	//��10��
	}
	if(HMS[2]!='-'){
		Val[2]|=(((HMS[2]-'0')&0x0F)<<3);	
		MaskFlag=5;	//����
	}
	if(HMS[0]!='-' || HMS[1]!='-'){
		Val[1]|=(((HMS[0]-'0')&0x0F)<<1)+(((HMS[1]-'0')&0x0F)<<0);	
		MaskFlag=6;	//����
	}
	if(WeekNo!='-'){
		Val[1]|=(((WeekNo-'0')&0x0F)<<2);
		MaskFlag=7;	//���T
	}
	if( MD[2]!='-' ||  MD[3]!='-'){
		Val[0]|=((( MD[2]-'0')&0x0F)<<1)+((( MD[3]-'0')&0x0F)<<0);	
		MaskFlag=8;	//����
	}
	if( MD[0]!='-' ||  MD[1]!='-'){
		Val[0]|=((( MD[0]-'0')&0x0F)<<3)+((( MD[1]-'0')&0x0F)<<2);
		MaskFlag=9;	//���N
	}

	//===RTCC Critical Begin===
	_rtcc_unlock();								//unlock Timer Registers
	_rtcc_inactive();                                  //disable the RTCC peripheral

	ALCFGRPTbits.ALRMPTR = 2;  	 				//Point to Month/Day register
	//ALRMVAL�֏������ނƎ����I��ALRMVAL�̎Q�Ɛ悪�C���N�������g�����
	RTCVAL = Val[0];
	RTCVAL = Val[1];
	RTCVAL = Val[2];

	ALCFGRPTbits.CHIME = IterateNoLimitFlag_;  	//�����J�Ԃ��t���O
	ALCFGRPTbits.ARPT = IterateNum_;			//�J��Ԃ���
	ALCFGRPTbits.AMASK = MaskFlag;		//�A���[���p�̎Q�ƃ}�X�N

	ALCFGRPTbits.ALRMEN = 1;

	_rtcc_active();                                   //enable RTCC peripheral
	_rtcc_lock();                                 //lock the RTCC value registers
	//===RTCC Critical End===

	rtcc_reset_interrupt();
	rtcc_clear_interrupt_flag();
}
//RTCC�̊��荞�ݖ���(�J��Ԃ���)
void rtcc_disable_interrupt(void){
	//===RTCC Critical Begin===
	_rtcc_unlock();								//unlock Timer Registers
	_rtcc_inactive();                                  //disable the RTCC peripheral
	ALCFGRPTbits.ALRMEN = 0;
	_rtcc_active();                                   //enable RTCC peripheral
	_rtcc_lock();                                 //lock the RTCC value registers
	//===RTCC Critical End===
}

//======================I2C1==============================
boolian I2C1_BusyFlag;		//I2C1�������F1
void i2c1_initialize(uint32 SystemClock_,uint16 I2C_ClockMode_,uint16 nsPGD_){
	I2C1_BusyFlag=hmLib_false;
	I2C1CON=0x00;

	//Configre SCA/SDA pin as open-drain
	ODCAbits.ODCA2=0;
	ODCAbits.ODCA3=0;
	
	//�A�h���X�̃}�X�N�̗L���̐ݒ� �}�X�^�Ƃ��ē��삷��ꍇ�͏�ɃN���A
	I2C1CONbits.IPMIEN=0;
	//�N���b�N�X�g���b�`�𖳌���
	I2C1CONbits.STREN=0;
	//�X���[�u�̃A�h���X�̃r�b�g����7bit�ɂ���i�����1�ɂ����10bit�j
	I2C1CONbits.A10M=0;
	//�N���b�N�������[�X
	I2C1CONbits.SCLREL=1;

	//�{�[���[�g����I2C�N���b�N�𐶐����邽�߂̉����ݒ�
	//	BRG= ( 1/FSCL - PGD ) * FCY -1
	//	FSCL�FI2C���̃N���b�N�X�s�[�h 100k,400k,1M��3��ނ��嗬�炵��
	//		100k:1.0e-5s=10000ns
	//		400k:2.5e-6s= 2500ns
	//		  1M:1.0e-6s= 1000ns
	//	PGD : Pulse_Gobbler_Delay �f�o�C�X�ŗL�@�f�[�^�V�[�g�Q��
	//		Pic4EP512GU810:1.30e-7
	//	FCY : �V�X�e�����̃N���b�N PLL��@�������CDOZE���p�̏ꍇ��CPU�N���b�N�ƈ�v���Ȃ�
	I2C1BRG=((uint32)(I2C_ClockMode_+nsPGD_)*(SystemClock_/100000)-10000)/10000;

	//�X���[�u�̃A�h���X���������H
	I2C1ADD=0;
	//�X���[�u�̃A�h���X�}�X�N�@�\���֎~�H
	I2C1MSK=0;

	//I2C�L����
	I2C1CONbits.I2CEN=1;

//	IEC3bits.MI2C1IE = 1;
//	IFS3bits.MI2C1IF = 0;
}
//I2C1�̏I�[������
void i2c1_finalize(void){
	I2C1_BusyFlag=hmLib_false;
	I2C1CON=0x00;
}
//�ʐM�J�n(IO=1����M�C0�����M)
boolian i2c1_start(uint8 slave_address,boolian IO){
	//�ȑO�̒ʐM�����҂�
	i2c1_wait_process();

	//Busy�t���O���グ��
	I2C1_BusyFlag=1;
	//�ʐM�J�n
	i2c1_interrupt_setup_start();
	
	//�ȑO�̒ʐM�����҂�(��start�����҂�)
	i2c1_wait_process();
	
	//Busy�t���O���グ��
	I2C1_BusyFlag=1;
	//slave address��IO(write or read)�𑗐M
	i2c1_interrupt_do_start(slave_address,IO);

	//�ȑO�̒ʐM�����҂�
	i2c1_wait_process();
	return i2c1_check_NACK();
}
//�ʐM�ĊJ(IO=1����M�C0�����M)(�I��������������IO��ύX����Ƃ��Ɏg��)
boolian i2c1_restart(uint8 slave_address,boolian IO){
	//�ȑO�̒ʐM�����҂�
	i2c1_wait_process();

	//Busy�t���O���グ��
	I2C1_BusyFlag=1;
	//�ʐM�ĊJ
	i2c1_interrupt_setup_restart();

	//�ȑO�̒ʐM�����҂��i��restart�����҂��j
	i2c1_wait_process();

	//Busy�t���O���グ��
	I2C1_BusyFlag=1;
	//slave address��IO(write or read)�𑗐M
	i2c1_interrupt_do_restart(slave_address,IO);

	//�ȑO�̒ʐM�����҂�
	i2c1_wait_process();
	return i2c1_check_NACK();
}
//�ʐM�I��
void i2c1_stop(void){
	//�ȑO�̒ʐM�����҂�
	i2c1_wait_process();

	//Busy�t���O���グ��
	I2C1_BusyFlag=1;
	//�ʐM�I�������J�n
	i2c1_interrupt_stop();
}
//���M�֐�
boolian i2c1_write(uint8 c){
	//�ȑO�̒ʐM�����҂�
	i2c1_wait_process();

	//Busy�t���O���グ��
	I2C1_BusyFlag=1;
	//�o�b�t�@�ɏ�������
	i2c1_interrupt_write(c);

	//�ȑO�̒ʐM�����҂�(slave������̉����҂�)
	i2c1_wait_process();

	//ACK or NACK ��Ԃ�
	return i2c1_check_NACK(); 
}
//��M�֐�
uint8 i2c1_read(boolian EndFlag){
	//�ȑO�̒ʐM�����҂�
	i2c1_wait_process();

	//Busy�t���O���グ��
	I2C1_BusyFlag=1;
	//��Menable
	i2c1_interrupt_settup_read();

	//�ȑO�̒ʐM�����҂��i����M�҂��j
	i2c1_wait_process();

	//Busy�t���O���グ��
	I2C1_BusyFlag=1;
	//�f�[�^��M�����ACK/NACK���M
	return i2c1_interrupt_do_read(EndFlag);
}
//ACK���M�֐�
void _i2c1_send_ACK(void){
	//ACK���M���[�h�ɐݒ�
	I2C1CONbits.ACKDT=0;
	//ACK���M
	I2C1CONbits.ACKEN=1;
}
//NACK���M�֐�
void _i2c1_send_NACK(void){
	//NACK���M���[�h�ɐݒ�
	I2C1CONbits.ACKDT=1;
	//NACK���M
	I2C1CONbits.ACKEN=1;
}
//NACK��M�`�F�b�N�֐�(0:ACK,1:NACK)
boolian i2c1_check_NACK(void){
	//�ȑO�̒ʐM�����҂�
	i2c1_wait_process();	
	return I2C1STATbits.ACKSTAT;
}
//��M�m�F�֐�
boolian i2c1_data_is_in(void){return I2C1STATbits.RBF;}
//I2C�̏����҂��֐��i�����̋�����h���j
void i2c1_wait_process(void){
	if(I2C1_BusyFlag){
		//���荞�݃t���O���オ��܂ő҂i�����I�����Ɏ����I�Ɋ��荞�ݐ��������j
		while(!_MI2C1IF);
		//busy�t���O�N���A
		I2C1_BusyFlag=0;
		//���荞�݃t���O�N���A
		_MI2C1IF=0;
	}
}
//���荞�ݗp�ʐM�J�n�����֐�
void i2c1_interrupt_setup_start(void){I2C1CONbits.SEN=1;}
//���荞�ݗp�ʐM�J�n�֐�
void i2c1_interrupt_do_start(uint8 slave_address,boolian IO){I2C1TRN=((0b00000001&IO)|(slave_address<<1));};
//���荞�ݗp�ʐM�ĊJ�����֐�
void i2c1_interrupt_setup_restart(void){I2C1CONbits.RSEN=1;}
//���荞�ݗp�ʐM�I���֐�
void i2c1_interrupt_stop(void){I2C1CONbits.PEN=1;}
//���荞�ݗp���M�֐�
void i2c1_interrupt_write(uint8 c){I2C1TRN=c;}
//���荞�ݗp��M�����֐�
void i2c1_interrupt_settup_read(void){I2C1CONbits.RCEN=1;}
//���荞�ݗp��M�֐�
uint8 i2c1_interrupt_do_read(boolian EndFlag){
	//slave��ACK/NACK���M
	if(EndFlag)_i2c1_send_NACK();	//NACK���M�i��M�����I���j
	else _i2c1_send_ACK();			//ACK���M

	//��M���W�X�^��Ԃ�
	return I2C1RCV;
}


//======================I2C2==============================
boolian I2C2_BusyFlag;		//I2C2�������F1
void i2c2_initialize(uint32 SystemClock_,uint16 I2C_ClockMode_,uint16 nsPGD_){
	I2C2_BusyFlag=hmLib_false;
	I2C2CON=0x00;

	//Configre SCA/SDA pin as open-drain
	ODCAbits.ODCA2=0;
	ODCAbits.ODCA3=0;
	
	//�A�h���X�̃}�X�N�̗L���̐ݒ� �}�X�^�Ƃ��ē��삷��ꍇ�͏�ɃN���A
	I2C2CONbits.IPMIEN=0;
	//�N���b�N�X�g���b�`�𖳌���
	I2C2CONbits.STREN=0;
	//�X���[�u�̃A�h���X�̃r�b�g����7bit�ɂ���i�����1�ɂ����10bit�j
	I2C2CONbits.A10M=0;
	//�N���b�N�������[�X
	I2C2CONbits.SCLREL=1;

	//�{�[���[�g����I2C�N���b�N�𐶐����邽�߂̉����ݒ�
	//	BRG= ( 1/FSCL - PGD ) * FCY -1
	//	FSCL�FI2C���̃N���b�N�X�s�[�h 100k,400k,1M��3��ނ��嗬�炵��
	//		100k:1.0e-5s=10000ns
	//		400k:2.5e-6s= 2500ns
	//		  1M:1.0e-6s= 1000ns
	//	PGD : Pulse_Gobbler_Delay �f�o�C�X�ŗL�@�f�[�^�V�[�g�Q��
	//		Pic4EP512GU810:1.30e-7
	//	FCY : �V�X�e�����̃N���b�N PLL��@�������CDOZE���p�̏ꍇ��CPU�N���b�N�ƈ�v���Ȃ�
	I2C2BRG=((uint32)(I2C_ClockMode_+nsPGD_)*(SystemClock_/100000)-10000)/10000;

	//�X���[�u�̃A�h���X���������H
	I2C2ADD=0;
	//�X���[�u�̃A�h���X�}�X�N�@�\���֎~�H
	I2C2MSK=0;

	//I2C�L����
	I2C2CONbits.I2CEN=1;

//	IEC3bits.MI2C2IE = 1;
//	IFS3bits.MI2C2IF = 0;
}
//I2C2�̏I�[������
void i2c2_finalize(void){
	I2C2_BusyFlag=hmLib_false;
	I2C2CON=0x00;
}
//�ʐM�J�n(IO=1����M�C0�����M)
boolian i2c2_start(uint8 slave_address,boolian IO){
	//�ȑO�̒ʐM�����҂�
	i2c2_wait_process();

	//Busy�t���O���グ��
	I2C2_BusyFlag=1;
	//�ʐM�J�n
	i2c2_interrupt_setup_start();
	
	//�ȑO�̒ʐM�����҂�(��start�����҂�)
	i2c2_wait_process();
	
	//Busy�t���O���グ��
	I2C2_BusyFlag=1;
	//slave address��IO(write or read)�𑗐M
	i2c2_interrupt_do_start(slave_address,IO);

	//�ȑO�̒ʐM�����҂�
	i2c2_wait_process();
	return i2c2_check_NACK();
}
//�ʐM�ĊJ(IO=1����M�C0�����M)(�I��������������IO��ύX����Ƃ��Ɏg��)
boolian i2c2_restart(uint8 slave_address,boolian IO){
	//�ȑO�̒ʐM�����҂�
	i2c2_wait_process();

	//Busy�t���O���グ��
	I2C2_BusyFlag=1;
	//�ʐM�ĊJ
	i2c2_interrupt_setup_restart();

	//�ȑO�̒ʐM�����҂��i��restart�����҂��j
	i2c2_wait_process();

	//Busy�t���O���グ��
	I2C2_BusyFlag=1;
	//slave address��IO(write or read)�𑗐M
	i2c2_interrupt_do_restart(slave_address,IO);

	//�ȑO�̒ʐM�����҂�
	i2c2_wait_process();
	return i2c2_check_NACK();
}
//�ʐM�I��
void i2c2_stop(void){
	//�ȑO�̒ʐM�����҂�
	i2c2_wait_process();

	//Busy�t���O���グ��
	I2C2_BusyFlag=1;
	//�ʐM�I�������J�n
	i2c2_interrupt_stop();
}
//���M�֐�
boolian i2c2_write(uint8 c){
	//�ȑO�̒ʐM�����҂�
	i2c2_wait_process();

	//Busy�t���O���グ��
	I2C2_BusyFlag=1;
	//�o�b�t�@�ɏ�������
	i2c2_interrupt_write(c);

	//�ȑO�̒ʐM�����҂�(slave������̉����҂�)
	i2c2_wait_process();

	//ACK or NACK ��Ԃ�
	return i2c2_check_NACK(); 
}
//��M�֐�
uint8 i2c2_read(boolian EndFlag){
	//�ȑO�̒ʐM�����҂�
	i2c2_wait_process();

	//Busy�t���O���グ��
	I2C2_BusyFlag=1;
	//��Menable
	i2c2_interrupt_settup_read();

	//�ȑO�̒ʐM�����҂��i����M�҂��j
	i2c2_wait_process();

	//Busy�t���O���グ��
	I2C2_BusyFlag=1;
	//�f�[�^��M�����ACK/NACK���M
	return i2c2_interrupt_do_read(EndFlag);
}
//ACK���M�֐�
void _i2c2_send_ACK(void){
	//ACK���M���[�h�ɐݒ�
	I2C2CONbits.ACKDT=0;
	//ACK���M
	I2C2CONbits.ACKEN=1;
}
//NACK���M�֐�
void _i2c2_send_NACK(void){
	//NACK���M���[�h�ɐݒ�
	I2C2CONbits.ACKDT=1;
	//NACK���M
	I2C2CONbits.ACKEN=1;
}
//NACK��M�`�F�b�N�֐�(0:ACK,1:NACK)
boolian i2c2_check_NACK(void){
	//�ȑO�̒ʐM�����҂�
	i2c2_wait_process();	
	return I2C2STATbits.ACKSTAT;
}
//��M�m�F�֐�
boolian i2c2_data_is_in(void){return I2C2STATbits.RBF;}
//I2C�̏����҂��֐��i�����̋�����h���j
void i2c2_wait_process(void){
	if(I2C2_BusyFlag){
		//���荞�݃t���O���オ��܂ő҂i�����I�����Ɏ����I�Ɋ��荞�ݐ��������j
		while(!IFS3bits.MI2C2IF);
		//busy�t���O�N���A
		I2C2_BusyFlag=0;
		//���荞�݃t���O�N���A
		IFS3bits.MI2C2IF=0;
	}
}
//���荞�ݗp�ʐM�J�n�����֐�
void i2c2_interrupt_setup_start(void){I2C2CONbits.SEN=1;}
//���荞�ݗp�ʐM�J�n�֐�
void i2c2_interrupt_do_start(uint8 slave_address,boolian IO){I2C2TRN=((0b00000001&IO)|(slave_address<<1));};
//���荞�ݗp�ʐM�ĊJ�����֐�
void i2c2_interrupt_setup_restart(void){I2C2CONbits.RSEN=1;}
//���荞�ݗp�ʐM�I���֐�
void i2c2_interrupt_stop(void){I2C2CONbits.PEN=1;}
//���荞�ݗp���M�֐�
void i2c2_interrupt_write(uint8 c){I2C2TRN=c;}
//���荞�ݗp��M�����֐�
void i2c2_interrupt_settup_read(void){I2C2CONbits.RCEN=1;}
//���荞�ݗp��M�֐�
uint8 i2c2_interrupt_do_read(boolian EndFlag){
	//slave��ACK/NACK���M
	if(EndFlag)_i2c2_send_NACK();	//NACK���M�i��M�����I���j
	else _i2c2_send_ACK();			//ACK���M

	//��M���W�X�^��Ԃ�
	return I2C2RCV;
}


//I2C1�̃}�X�^�[���荞�݃t���O�FIFS1bits.MI2C1IF
//I2C1�̃}�X�^�[���荞��enable�r�b�g�FIEC1bits.MI2C1IE
//I2C2�̃}�X�^�[���荞�݃t���O�FIFS3bits.MI2C2IF
//I2C2�̃}�X�^�[���荞��enable�r�b�g�FIEC3bits.MI2C2IE

void __attribute__((interrupt, no_auto_psv)) _MI2C2Interrupt(void){
        IFS3bits.MI2C2IF = 0;		//Clear the DMA0 Interrupt Flag;

}

#
#endif
