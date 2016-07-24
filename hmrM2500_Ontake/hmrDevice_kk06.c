#ifndef HMR_DEVICE_KK06_C_INC
#define HMR_DEVICE_KK06_C_INC 108
#
#ifndef HMR_DEVICE_KK06_INC
#	include "hmrDevice_kk06.h"
#endif

//================================CONFIG==================================
_FOSCSEL( FNOSC_FRCPLL);//Internal FRC(7.37MHz) + PLL//
_FOSC( FCKSM_CSDCMD & OSCIOFNC_ON & POSCMD_NONE ); //HS//
_FWDT( FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS1024 );//WDT 2s
_FPOR( FPWRT_PWR64 & ALTI2C2_ON);//power on timer 64ms

//=================================interrupt==============================
#define HMR_INTERRUPT_LEVEL_UART1_SEND 3
#define HMR_INTERRPUT_LEVEL_UART1_RECV 4
#define HMR_INTERRUPT_LEVEL_UART2_SEND 6
#define HMR_INTERRPUT_LEVEL_UART2_RECV 7
#define HMR_INTERRUPT_LEVEL_TIMER1 2
#define HMR_INTERRUPT_LEVEL_TIMER5 2
#define HMR_INTERRUPT_LEVEL_MAIN 1
#define HMR_INTERRUPT_LEVEL_LOCK 5
//���荞�ݏ������֐�
void hmr_interrupt_initalize(void){
	//���荞�݁@�l�X�e�B���O(���d���荞��)����(���ꂪ�Ȃ��ƃf�[�^��������݂���)
	INTCON1bits.NSTDIS = 0;
	//���荞�ݏ��ʐݒ� �D�揇�ɐ������傫�� 
	//�����͐G��ȁI�I�@stop_MainStreamInterrupt,restart/// ���ˑ����Ă���I�I
	IPC7bits.U2RXIP = HMR_INTERRPUT_LEVEL_UART2_RECV;// UART2(CAMERA) Rx
	IPC7bits.U2TXIP = HMR_INTERRUPT_LEVEL_UART2_SEND;// UART2(CAMERA) Tx
	IPC2bits.U1RXIP = HMR_INTERRPUT_LEVEL_UART1_RECV;// UART1(RF module) Rx  
	IPC3bits.U1TXIP = HMR_INTERRUPT_LEVEL_UART1_SEND;// UART1(RF module) Tx  
	IPC0bits.T1IP = HMR_INTERRUPT_LEVEL_TIMER1;	// Timer1   
	IPC7bits.T5IP = HMR_INTERRUPT_LEVEL_TIMER5;	// Timer5   
	SRbits.IPL = HMR_INTERRUPT_LEVEL_MAIN;		// Main Loop
}
//���荞�ݏI�[���֐�
void hmr_interrupt_finalize(void){}
//main���[�v�ւ̊��荞�݈ꎞ��~
void hmr_main_lock_interrupt(void){SRbits.IPL = HMR_INTERRUPT_LEVEL_LOCK;}
//main���[�v�ւ̊��荞�ݕ��A
void hmr_main_unlock_interrupt(void){SRbits.IPL = HMR_INTERRUPT_LEVEL_MAIN;}
//uart1_recv�ւ̊��荞�݈ꎞ��~
void hmr_uart1_fget_lock_interrupt(void){IPC2bits.U1RXIP= HMR_INTERRUPT_LEVEL_LOCK;}
//uart1_recv�ւ̊��荞�ݕ��A
void hmr_uart1_fget_unlock_interrupt(void){IPC2bits.U1RXIP = HMR_INTERRPUT_LEVEL_UART1_RECV;}
//uart1_send�ւ̊��荞�݈ꎞ��~
void hmr_uart1_fput_lock_interrupt(void){IPC3bits.U1TXIP = HMR_INTERRUPT_LEVEL_LOCK;}
//uart1_send�ւ̊��荞�ݕ��A
void hmr_uart1_fput_unlock_interrupt(void){IPC3bits.U1TXIP = HMR_INTERRUPT_LEVEL_UART1_SEND;}
//timer1�ւ̊��荞�݈ꎞ��~
void hmr_timer1_lock_interrupt(void){IPC0bits.T1IP = HMR_INTERRUPT_LEVEL_LOCK;}
//timer1�ւ̊��荞�ݕ��A
void hmr_timer1_unlock_interrupt(void){IPC0bits.T1IP = HMR_INTERRUPT_LEVEL_TIMER1;}
//timer5�ւ̊��荞�݈ꎞ��~
void hmr_timer4_5_lock_interrupt(void){IPC7bits.T5IP = HMR_INTERRUPT_LEVEL_LOCK;}
//timer5�ւ̊��荞�ݕ��A
void hmr_timer4_5_unlock_interrupt(void){IPC7bits.T5IP = HMR_INTERRUPT_LEVEL_TIMER5;}



//===========================�f�o�C�X=========================
void hmr_device_initialize(void){
//=============CLOCK�ݒ�======================
	// Fin = 7.37 /2 * 40 / 2 = 73.7 MHz
	CLKDIVbits.PLLPRE = 0; //N1 =  1/2
	__asm__("NOP");
	CLKDIVbits.PLLPOST = 0;// N2 = 1/2
	PLLFBD = 38; // M = 40

//==============TRIS�ݒ�====================
	TRISA=0x04C0;
	TRISB=0x8F43;	//0x8F7F adc2,3,4,5��؂���
	TRISC=0x000E;
	TRISD=0x4020;
	TRISE=0x01FF;	//0x3FF adc1��؂���
	TRISF=0x0017;
	TRISG=0x008F;
	
//=============ADC�ݒ�=======================
//1��ADC��L��
	ANSELA=0x0040;
	ANSELB=0x0743;	//0x77F ch2,3,4,5��؂���
	ANSELC=0x000E;
	ANSELD=0x0000;
	ANSELE=0x01FF;	//0x03FF ch1��؂���
	ANSELG=0x0000;
	//�|�[�gF��AD�s���͂Ȃ�
}
void hmr_device_finalize(void){}

//===============================�N���b�N===============================
void hmr_clock_initialize_withNormal(void){
	//�|�X�g�X�P�[���[�ݒ�
	changeClock(MAIN_CLK_Div_1);
	//�N���b�N�̐ݒ�
	ini_clock(73700000);
}
void hmr_clock_initialize_withSlow(void){
	//�|�X�g�X�P�[���[�ݒ�
	changeClock(MAIN_CLK_Div_16);
	//�N���b�N�̐ݒ� (Post�X�P�[���[�Őݒ肵�Ă��e�����Ȃ�)
	ini_clock(73700000);
}
void hmr_clock_finalize(void){}

//===============================�ʐM�ݒ�n=============================
#define HMR_UART1_RF_BAUDRATE 9600
#define HMR_UART1_MP_BAUDRATE 38400
#define HMR_UART2_BAUDRATE 38400
#define HMR_I2C2_CLOCK I2C_CLOCK_1MHz
void hmr_uart1_initialize_withRF(void){
//____________UART1(for RF modeule)____________
	OSCCONbits.IOLOCK=0;		//pheriperal pin�ݒ�̂��߃��W�X�^�̃��b�N������
	RPINR18bits.U1RXR=0x62;		//RP98��UART1��M�s���ɐݒ� U1RXR����M�s���ݒ背�W�X�^ 0x62��RP98�̔ԍ�
	RPOR8bits.RP99R=1;			//RP99��UART1���M�s���ɐݒ� ���M�s���ݒ��1������
	RPOR14bits.RP125R=0;		//MP���̑��M�s���ݒ�������Ă���
	OSCCONbits.IOLOCK=1;		//pheriperal pin�ݒ�̂��߃��W�X�^�����b�N
	ini_UART1(HMR_UART1_RF_BAUDRATE);
}
void hmr_uart1_initialize_withMP(void){
//____________UART1(for MobilePhone modeule)____________
	OSCCONbits.IOLOCK=0;		//pheriperal pin�ݒ�̂��߃��W�X�^�̃��b�N������
	RPINR18bits.U1RXR=0x17;		//RPI23��UART1��M�s���ɐݒ� U1RXR����M�s���ݒ背�W�X�^ 0x17��RPI23�̔ԍ�
	RPOR14bits.RP125R=1;		//RP125��UART1���M�s���ɐݒ�
	RPOR8bits.RP99R=0;			//RF���̑��M�s���ݒ��0�ɖ߂��Ă���
	OSCCONbits.IOLOCK=1;		//pheriperal pin�ݒ�̂��߃��W�X�^�����b�N	
	ini_UART1(HMR_UART1_MP_BAUDRATE);
}
void hmr_uart1_finalize(void){}
void hmr_uart2_initialize(void){
//_____________UART2(for CAMERA)_______________
	OSCCONbits.IOLOCK=0;		//pheriperal pin�ݒ�̂��߃��W�X�^�̃��b�N������
	RPINR19bits.U2RXR=0x64;		//RP100��UART2��M�s���ɐݒ�
	RPOR9bits.RP101R=3;			//RP101��UART2��M�s���ɐݒ�
	OSCCONbits.IOLOCK=1;		//pheriperal pin�ݒ�̂��߃��W�X�^�����b�N	
	ini_UART2(HMR_UART2_BAUDRATE);
}
void hmr_uart2_finalize(void){}
void hmr_spi2_initialize(void){
	setup_spi2(SPI_MASTER | SPI_L_TO_H | SPI_CLK_DIV_64|SPI_CLK_DIV2_5);//MASTER, L to H(����), �N���b�N100Kbps��
}
void hmr_spi2_finalize(void){}
void hmr_i2c_initialize(void){i2c2_initialize(73700000,HMR_I2C2_CLOCK,0);}
void hmr_i2c_finalize(void){i2c2_finalize();}
//==============================AD�ϊ�==================================
void hmr_adc_initialize(void){
	setup_adc1_12bit(1);
	set_adc_sampleMode(EVERY);
	setup_adc1_time_auto();
	setup_adc1_Vref(Vref_GND);
}
void hmr_adc_finalize(void){}
#
#endif
