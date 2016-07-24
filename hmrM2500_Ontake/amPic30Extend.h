#ifndef AMPIC30EXTEND_INC
#define AMPIC30EXTEND_INC 101
#
/*
===amPic30Extend===
amPic30�̊g�����C�u����
	amPic30�K�v�Ǝv����֐��Q
	�t���O�Ǘ��֐��Q��ǉ�

amPic30Extend_v1_01/121201 iwahori
	I2C�֐��Q��ǉ��E�C��
amPic30Extend_v1_00/121027 hmIto
	
*/
#ifndef AMPIC30_INC
#	include"amPic30_v7.h"
#endif
//stream1�̎�M������
#define uart1_fget_interrupt(void) __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
void uart1_enable_fget_interrupt(void);
void uart1_disable_fget_interrupt(void);
boolian uart1_is_enable_fget_interrupt(void);
void uart1_set_fget_interrupt_flag(void);
void uart1_clear_fget_interrupt_flag(void);
//stream1�̑��M������
#define uart1_fput_interrupt(void) __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void)
void uart1_enable_fput_interrupt(void);
void uart1_disable_fput_interrupt(void);
boolian uart1_is_enable_fput_interrupt(void);
void uart1_set_fput_interrupt_flag(void);
void uart1_clear_fput_interrupt_flag(void);
//stream2�̎�M������
#define uart2_fget_interrupt(void) __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void)
void uart2_enable_fget_interrupt(void);
void uart2_disable_fget_interrupt(void);
boolian uart2_is_enable_fget_interrupt(void);
void uart2_set_fget_interrupt_flag(void);
void uart2_clear_fget_interrupt_flag(void);
//stream2�̑��M������
#define uart2_fput_interrupt(void) __attribute__((interrupt, no_auto_psv)) _U2TXInterrupt(void)
void uart2_enable_fput_interrupt(void);
void uart2_disable_fput_interrupt(void);
boolian uart2_is_enable_fput_interrupt(void);
void uart2_set_fput_interrupt_flag(void);
void uart2_clear_fput_interrupt_flag(void);
//timer1�̊��荞��
#define timer1_interrupt(void) __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
//timer1�p�֐�
void timer1_initialize(uint16 time);
//timer1�̊��荞�݃t���O���N���A
void timer1_clear_interrupt_flag(void);
//timer1�̊��荞�݃t���O���Z�b�g
void timer1_set_interrupt_flag(void);
//timer1�̃J�E���g�����Z�b�g
void timer1_reset(void);
//timer1�����荞�ݗL�����ǂ����̊m�F
boolian timer1_is_enable_interrupt(void);
//timer1�̊��荞�ݗL��
void timer1_enable_interrupt(void);
//timer1�̊��荞�ݖ���
void timer1_disable_interrupt(void);
//timer5�̊�����
#define timer4_5_interrupt(void) __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void)
//timer4&5�A���^�C�}�[�̏�����
void timer4_5_initialize(uint16 time);
//timer4&5�A���^�C�}�[�̏I�[��
void timer4_5_finalize();
//timer4&5�̊��荞�݃t���O���N���A
void timer4_5_clear_interrupt_flag(void);
//timer4&5�̊��荞�݃t���O���Z�b�g
void timer4_5_set_interrupt_flag(void);
//timer4&5�̃J�E���g�����Z�b�g
void timer4_5_reset(void);
//timer4&5�����荞�ݗL�����ǂ����̊m�F
boolian timer4_5_is_enable_interrupt(void);
//timer4&5�̊��荞�ݗL��
void timer4_5_enable_interrupt(void);
//timer4&5�̊��荞�ݖ���
void timer4_5_disable_interrupt(void);
//RTCC�p���荞�݊֐�
#define rtcc_interrupt(void) void __attribute__((interrupt)) _RTCCInterrupt(void)
//RTCC�p�Z�J���h�I�V���[�^�������v���Z�X
void rtcc_oscilator_initialize(void);
//RTCC�������֐� �������ݒ莞�� 
//	rtcc_initialize("121027","0","210120"); == 2012�N10��27��(��)21:01:20
void rtcc_initialize(unsigned char YMD[6],unsigned char WeekNo,unsigned char HMS[6]);
//RTCC�̊��荞�݃t���O���N���A
void rtcc_clear_interrupt_flag(void);
//RTCC�̊��荞�݃t���O���Z�b�g
void rtcc_set_interrupt_flag(void);
//RTCC�̃J�E���g�����Z�b�g
void rtcc_reset_interrupt(void);
//RTCC�����荞�ݗL�����ǂ����̊m�F
boolian rtcc_is_enable_interrupt(void);
//RTCC�̊��荞�ݗL�� ���������荞�ݎ���
//	rtcc_initialize("----","-","--3000"); == ����30�����傤�ǂɊ��荞��
void rtcc_enable_interrupt(unsigned char MD[4],unsigned char WeekNo,unsigned char HMS[6],boolian IterateNoLimitFlag_,uint8 IterateNum_);
//RTCC�̊��荞�ݖ���(�J��Ԃ���)
void rtcc_disable_interrupt(void);
//=================================I2C1======================================
#define I2C_CLOCK_100kHz 10000
#define I2C_CLOCK_400kHz 2500
#define I2C_CLOCK_1MHz 1000
//i2c1������
void i2c1_initialize(uint32 SystemClock_,uint16 I2C_ClockMode_,uint16 nsPGD_);
//I2C1�̏I�[������
void i2c1_finalize(void);
//�ʐM�J�n(IO=1����M�C0�����M)
boolian i2c1_start(uint8 slave_address,boolian IO);
//�ʐM�ĊJ(IO=1����M�C0�����M)(�I��������������IO��ύX����Ƃ��Ɏg��)
boolian i2c1_restart(uint8 slave_address,boolian IO);
//�ʐM�I��
void i2c1_stop(void);
//���M�֐�
boolian i2c1_write(uint8 c);
//��M�֐�
uint8 i2c1_read(boolian EndFlag);
//ACK���M�֐�
void _i2c1_send_ACK(void);
//NACK���M�֐�
void _i2c1_send_NACK(void);
//NACK��M�`�F�b�N�֐�(0:ACK,1:NACK)
boolian i2c1_check_NACK(void);
//��M�m�F�֐�
boolian i2c1_data_is_in(void);
//I2C�̏����҂��֐��i�����̋�����h���j
void i2c1_wait_process(void);

//���荞�ݗp�ʐM�J�n�����֐�
void i2c1_interrupt_setup_start(void);
//���荞�ݗp�ʐM�J�n�֐�
void i2c1_interrupt_do_start(uint8 slave_address,boolian IO);
//���荞�ݗp�ʐM�ĊJ�����֐�
void i2c1_interrupt_setup_restart(void);
//���荞�ݗp�ʐM�ĊJ�֐�
#define i2c1_interrupt_do_restart i2c1_interrupt_do_start
//���荞�ݗp���M�֐�
void i2c1_interrupt_write(uint8 c);
//���荞�ݗp��M�����֐�
void i2c1_interrupt_settup_read(void);
//���荞�ݗp��M�֐�
uint8 i2c1_interrupt_do_read(boolian EndFlag);
//���荞�ݗp�ʐM�I���֐�
void i2c1_interrupt_stop(void);

//================================I2C2=======================================
//i2c2������
void i2c2_initialize(uint32 SystemClock_,uint16 I2C_ClockMode_,uint16 nsPGD_);
//I2C2�̏I�[������
void i2c2_finalize(void);
//�ʐM�J�n(IO=1����M�C0�����M)
boolian i2c2_start(uint8 slave_address,boolian IO);
//�ʐM�ĊJ(IO=1����M�C0�����M)(�I��������������IO��ύX����Ƃ��Ɏg��)
boolian i2c2_restart(uint8 slave_address,boolian IO);
//�ʐM�I��
void i2c2_stop(void);
//���M�֐�
boolian i2c2_write(uint8 c);
//��M�֐�
uint8 i2c2_read(boolian EndFlag);
//ACK���M�֐�
void _i2c2_send_ACK(void);
//NACK���M�֐�
void _i2c2_send_NACK(void);
//NACK��M�`�F�b�N�֐�(0:ACK,1:NACK)
boolian i2c2_check_NACK(void);
//��M�m�F�֐�
boolian i2c2_data_is_in(void);
//I2C�̏����҂��֐��i�����̋�����h���j
void i2c2_wait_process(void);

//���荞�ݗp�ʐM�J�n�����֐�
void i2c2_interrupt_setup_start(void);
//���荞�ݗp�ʐM�J�n�֐�
void i2c2_interrupt_do_start(uint8 slave_address,boolian IO);
//���荞�ݗp�ʐM�ĊJ�����֐�
void i2c2_interrupt_setup_restart(void);
//���荞�ݗp�ʐM�ĊJ�֐�
#define i2c2_interrupt_do_restart i2c2_interrupt_do_start
//���荞�ݗp���M�֐�
void i2c2_interrupt_write(uint8 c);
//���荞�ݗp��M�����֐�
void i2c2_interrupt_settup_read(void);
//���荞�ݗp��M�֐�
uint8 i2c2_interrupt_do_read(boolian EndFlag);
//���荞�ݗp�ʐM�I���֐�
void i2c2_interrupt_stop(void);

/*
//I2C�̎g����
	while(1){
		if(i2c2_start(slave_adress,0))continue;
		if(i2c2_write('A'))continue;
		if(i2c2_write('B'))continue;
		if(i2c2_write('C'))continue;
		if (i2c2_restart(slave_adress,1))continue;
		break;
	}
	Buf[1]=i2c2_read(0);
	Buf[2]=i2c2_read(0);
	Buf[3]=i2c2_read(0);
	Buf[4]=i2c2_read(1);
	i2c2_stop();

//I2C���荞�ݎg����
void __attribute__((interrupt, no_auto_psv)) _MI2C2Interrupt(void){
	static boolian ACKFlag=0;
	static uint8 status=0;
	static uint8 SendCnt=0;
	static uint8 RecvCnt=0;
	IFS3bits.MI2C2IF = 0;		//Clear the DMA0 Interrupt Flag;
	
	if(ACKFlag){
		if(i2c2_check_NACK())status=0;
	}
	ACKFlag=0;

	switch (status){
		case 1:
			i2c2_interrupt_setup_start();
			++status;
			ACKFlag=1;
			break;
		case 2:
			i2c2_interrupt_do_start(slave_address,0);
			++status;
			ACKFlag=1;
			break;
		case 3:
			i2c2_interrupt_write(Buf[SendCnt++]);
			if(SendCnt>X)++status;
			ACKFlag=1;
			break;
		case 4:
			i2c2_interrupt_setup_restart();
			++status;
			ACKFlag=1;
			break;
		case 5:
			i2c2_interrupt_do_restart(slave_address,1);
			++status;
			ACKFlag=1;
			break;
		case 6:
			if(RecvCnt%2==0)i2c2_interrupt_settup_read();
			else Buf[RecvCnt]=i2c2_interrupt_do_read;
			++RecvCnt;
			if(RecvCnt>Y)++status;
			break;
		case 7:
			i2c2_interrupt_stop();
			status=0;
			break;
		default:
			break;
	}
}
*/


#
#endif
