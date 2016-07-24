#ifndef HMR_DEVICE_KK09R2_C_INC
#define HMR_DEVICE_KK09R2_C_INC 102
#
#define HMLIB_NOLIB
#ifndef HMR_DEVICE_KK09R2_INC
//__TRACE(id);
#	include "hmrDevice_kk09R2.h"
#endif

//=================================interrupt==============================
#define HMR_INTERRUPT_LEVEL_UART1_SEND 3
#define HMR_INTERRPUT_LEVEL_UART1_RECV 4
#define HMR_INTERRUPT_LEVEL_UART2_SEND 6	//6
#define HMR_INTERRPUT_LEVEL_UART2_RECV 7	//7
#define HMR_INTERRUPT_LEVEL_TIMER1  2		//2
#define HMR_INTERRUPT_LEVEL_TIMER5  2		//2
#define HMR_INTERRUPT_LEVEL_MAIN 1
#define HMR_INTERRUPT_LEVEL_LOCK 5
#define HMR_INTERRUPT_LEVEL_STRONG_LOCK 7
//���荞�ݏ������֐�
void hmr_interrupt_initalize(void){
	//���荞�݁@�l�X�e�B���O(���d���荞��)����(���ꂪ�Ȃ��ƃf�[�^��������݂���)
	INTCON1bits.NSTDIS = 0; //debug 	//0�����d���荞�݋���
	// 
	// �D��x�����荞�݂̍l����
	// �@�P�A���ׂĂ̏������s���̂�CPU�ł���A����ɗD��x�Ƃ����T�O������
	//   �Q�A�e���W���[���̊��荞�ݏ���(UART�ATIMER�Ȃ�)���D��x�������Ă���
	//   �R�A�������荞�ݗv�����N���������́ACPU�̗D��x�Ɗ��荞�ݏ����̗D��x���ׂāA�D��x�̍����������s���B
	//			���̍ۂɁA�D��x�̍������荞�ݏ������s���ꍇ��CPU�̗D��x�����l�ɏオ��
	//   �S�A����ɂ���āA���d���荞�݂��A���荞�݂̗D��x����ŉ\�ɂȂ���
	//
	//���荞�݃l�X�e�B���O�֎~�����@NSTDIS = 1
	//�@�@�����̎d�g�݂Ƃ��ẮA���荞�݂�����ƁA�K��CPU�̏����D��x��MAX 7 �ɂ��Ă���
	//�@�@����đ��d���荞�݂͋N���Ȃ��I�I
	//�@�@�@�������A�����ɁACPU�̏����D��x����ؕύX�ł��Ȃ��Ȃ��Ă��܂� SRbits.IPL �͏������ݕs�ɂȂ�
	//�@�@�܂�A���C�����Ŋ��荞�݂����b�N�������Ƃ��͍H�v���K�v�ƂȂ�
	//

	//���荞�ݏ��ʐݒ� �D�揇�ɐ������傫�� 
	//�����͐G��ȁI�I�@stop_MainStreamInterrupt,restart/// ���ˑ����Ă���I�I
	IPC7bits.U2RXIP = HMR_INTERRPUT_LEVEL_UART2_RECV;// UART2(CAMERA) Rx
	__asm__("NOP");
	IPC7bits.U2TXIP = HMR_INTERRUPT_LEVEL_UART2_SEND;// UART2(CAMERA) Tx
	IPC2bits.U1RXIP = HMR_INTERRPUT_LEVEL_UART1_RECV;// UART1(RF module) Rx  
	IPC3bits.U1TXIP = HMR_INTERRUPT_LEVEL_UART1_SEND;// UART1(RF module) Tx  
	IPC0bits.T1IP = HMR_INTERRUPT_LEVEL_TIMER1;	// Timer1   
	IPC7bits.T5IP = HMR_INTERRUPT_LEVEL_TIMER5;	// Timer5   
	SRbits.IPL = HMR_INTERRUPT_LEVEL_MAIN;		// Main Loop
}

//���荞�݃��b�N�E�A�����b�N�֐�
//static unsigned char x_hmr_lock_prv=HMR_INTERRUPT_LEVEL_LOCK;
mutex hmr_interrupt_lock(void){
	mutex x_hmr_lock_prv;
	x_hmr_lock_prv = SRbits.IPL; 
	SRbits.IPL = HMR_INTERRUPT_LEVEL_LOCK;

	return x_hmr_lock_prv;
}
void hmr_interrupt_unlock(mutex x_hmr_lock_prv){
	SRbits.IPL = x_hmr_lock_prv;
}

// �S�Ă̊��荞�݂��֎~
void hmr_interrupt_disable(void){INTCON2bits.GIE = 0;}
// �S�Ă̊��荞�݂�����
void hmr_interrupt_enable(void){INTCON2bits.GIE = 1;}

//================================CONFIG==================================
_FOSCSEL( FNOSC_FRCPLL);//Internal FRC(7.37MHz) + PLL//
_FOSC( FCKSM_CSDCMD & OSCIOFNC_ON & POSCMD_NONE ); //HS//
_FWDT( FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS1024 );//WDT 2s
_FPOR( FPWRT_PWR64 & ALTI2C1_ON & ALTI2C2_ON);//power on timer 64ms

//===========================�f�o�C�X=========================
#define HMR_SYSTEM_CLOCK 73700000

void HMR_core_initialize(void){
//=============CLOCK�ݒ�======================
	
	// Pattern 1
	// Fin = 7.37 /2 * 40 / 2 = 73.7 MHz
	CLKDIVbits.PLLPRE = 0; //N1 =  1/2
	__asm__("NOP");
	CLKDIVbits.PLLPOST = 0;// N2 = 1/2
	PLLFBD = 38; // M = 40
	

	/*
	// Pattern 2
	// Fin = 7.37 /2 * 40 /4 = 36.35 MHz
	CLKDIVbits.PLLPRE = 0; //N1 =  1/2
	__asm__("NOP");
	CLKDIVbits.PLLPOST = 1;// N2 = 1 / 2(1 + 1)
	PLLFBD = 38; // M = 40
	*/


//==============TRIS�ݒ�====================
//1��in
	TRISA=0x04C0;
	TRISB=0xCE1F;
	TRISC=0x6004;
	TRISD=0x0001;
	TRISE=0x0392;
	TRISF=0x0007;
	TRISG=0x1083;
	
//=============ADC�ݒ�=======================
//1��ADC��L���@���ӁFDataSheet�ɋL�ڂ���Ă��Ȃ��s�����ݒ�ł��邱�Ƃ�����̂Ńw�b�_�t�@�C���łǂ̃s�����ݒ�\���m�F������������
	ANSELA=0x00C0;	//A port��A6,7,9,10��AD�ݒ��
	ANSELB=0x403E;	//B port��B0����15�܂ł��ׂ�AD�ݒ��
	ANSELC=0x0004;	//C port��C1,2,3,4,13,14��AD�ݒ��
	ANSELD=0x0000;	//D port��D6,7��AD�ݒ��
	ANSELE=0x0390;	//E port��E0����9�܂�AD�ݒ��
	ANSELG=0x0000;	//G port��G6,7,8,9��AD�ݒ��
	//�|�[�gF��AD�s���͂Ȃ�

	//�N���b�N�̐ݒ�
	//ini_clock(73700000);
	set_clock(HMR_SYSTEM_CLOCK);

	//�|�X�g�X�P�[���[�ݒ�
	changeClock(MAIN_CLK_Div_4);
}

//===============================�ʐM�ݒ�n=============================
//#define HMR_UART1_RF_BAUDRATE 9600
//#define HMR_UART1_RF_BAUDRATE_DIP4ON 38400
//#define HMR_UART1_RF_BAUDRATE_DIP4OFF 9600
//#define HMR_UART1_MP_BAUDRATE 38400
//#define HMR_UART2_BAUDRATE 38400
void HMR_uart1_initialize_withRF0_with_flowCtr(uint16 BaudRate){
//____________UART1(for RF modeule)____________
	_IOLOCK=0;		//pheriperal pin�ݒ�̂��߃��W�X�^�̃��b�N������

	_RP125R=0;			//MP����Tx�ݒ��0�ɖ߂��Ă���
	_RP126R=0;			//MP����RTS�ݒ��0�ɖ߂��Ă���
	
	_U1RXR=41;			//RPI41��UART1��M�s���ɐݒ� U1RXR����M�s���ݒ背�W�X�^ 44��RPI44�̔ԍ�(�v�f�[�^�V�[�g�Q��)
	_U1CTSR=42;			//RPI42��UART1��CTS�ɐݒ�
	_RP79R=1;			//RP100��UART1���M�s���ɐݒ� ���M�s���ݒ��1������
	_RP100R=2;			//RP79��UART1��RTS�s���ɐݒ�
	
	
	/*
	CTS,RTS�̗L�����̃���
	UxMODEbits.UEN = 0:UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLK pins controlled by PORT latches
					 1:UxTX, UxRX and UxRTS pins are enabled and used; UxCTS pin controlled by PORT latches
					 2:UxTX, UxRX, UxCTS and UxRTS pins are enabled and used
					 3:UxTX, UxRX and BCLK pins are enabled and used; UxCTS pin controlled by PORT latches
	*/
//	U1MODEbits.UEN=0x00;	//U1CTS,RTS�𖳌��ɂ���
	U1MODEbits.UEN = 0x02;	//U1CTS,RTS��L���ɂ���
	
	_IOLOCK=1;		//pheriperal pin�ݒ�̂��߃��W�X�^�����b�N
	uart1_initialize(BaudRate);
}
void HMR_uart1_initialize_withRF0_without_flowCtr(uint16 BaudRate){
	//____________UART1(for RF modeule)____________
	_IOLOCK = 0;		//pheriperal pin�ݒ�̂��߃��W�X�^�̃��b�N������

	_RP125R = 0;			//MP����Tx�ݒ��0�ɖ߂��Ă���
	_RP126R = 0;			//MP����RTS�ݒ��0�ɖ߂��Ă���

	_U1RXR = 41;			//RPI41��UART1��M�s���ɐݒ� U1RXR����M�s���ݒ背�W�X�^ 44��RPI44�̔ԍ�(�v�f�[�^�V�[�g�Q��)
	_U1CTSR = 42;			//RPI42��UART1��CTS�ɐݒ�
	_RP79R = 1;				//RP100��UART1���M�s���ɐݒ� ���M�s���ݒ��1������
	_RP100R = 2;			//RP79��UART1��RTS�s���ɐݒ�


	/*
	CTS,RTS�̗L�����̃���
	UxMODEbits.UEN = 0:UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLK pins controlled by PORT latches
	1:UxTX, UxRX and UxRTS pins are enabled and used; UxCTS pin controlled by PORT latches
	2:UxTX, UxRX, UxCTS and UxRTS pins are enabled and used
	3:UxTX, UxRX and BCLK pins are enabled and used; UxCTS pin controlled by PORT latches
	*/
	U1MODEbits.UEN=0x00;	//U1CTS,RTS�𖳌��ɂ���

	_IOLOCK = 1;		//pheriperal pin�ݒ�̂��߃��W�X�^�����b�N
	uart1_initialize(BaudRate);
}
void HMR_uart1_initialize_withRF1_with_flowCtr(uint16 BaudRate){
//____________UART1(for MobilePhone modeule)____________
	_IOLOCK=0;			//pheriperal pin�ݒ�̂��߃��W�X�^�̃��b�N������
	
	_RP79R=0;			//RF����Tx�ݒ��0�ɖ߂��Ă���
	_RP100R=0;			//RF����RTS�ݒ��0�ɖ߂��Ă���

	_U1RXR=124;			//RP124��UART1��M�s���ɐݒ� U1RXR����M�s���ݒ背�W�X�^ 0x17��RPI23�̔ԍ�
	_U1CTSR=81;		//RPI81��CTS�ɐݒ�
						// ���ӁF�g�у��W���[��MPM�ɂ�CTS�p�̐����Ȃ��ł�����K�v�����邪�A���̐���8�s���ɂȂ��I�I
						// �@�ӂ���7�s���ł悢�̂����AMPM�ł�8�s���ɂȂ��Ƃ������Ƃ炵���E�E�E�E
	_RP125R=1;			//RP125��UART1���M�s���ɐݒ�
	_RP126R=2;			//RP126��UART1��RTS�s���ɐݒ�
	
	
//	U1MODEbits.UEN=0x00;	//U1CTS,RTS�𖳌��ɂ���
	U1MODEbits.UEN = 0x02;	//U1CTS,RTS��L���ɂ���
	
	_IOLOCK=1;			//pheriperal pin�ݒ�̂��߃��W�X�^�����b�N	
	uart1_initialize(BaudRate);
}
void HMR_uart1_initialize_withRF1_without_flowCtr(uint16 BaudRate){
//____________UART1(for MobilePhone modeule)____________
	_IOLOCK=0;			//pheriperal pin�ݒ�̂��߃��W�X�^�̃��b�N������
	
	_RP79R=0;			//RF����Tx�ݒ��0�ɖ߂��Ă���
	_RP100R=0;			//RF����RTS�ݒ��0�ɖ߂��Ă���

	_U1RXR=124;			//RP124��UART1��M�s���ɐݒ� U1RXR����M�s���ݒ背�W�X�^ 0x17��RPI23�̔ԍ�
	_U1CTSR=81;		//RPI81��CTS�ɐݒ�
						// ���ӁF�g�у��W���[��MPM�ɂ�CTS�p�̐����Ȃ��ł�����K�v�����邪�A���̐���8�s���ɂȂ��I�I
						// �@�ӂ���7�s���ł悢�̂����AMPM�ł�8�s���ɂȂ��Ƃ������Ƃ炵���E�E�E�E
	_RP125R=1;			//RP125��UART1���M�s���ɐݒ�
	_RP126R=2;			//RP126��UART1��RTS�s���ɐݒ�
	
	
	U1MODEbits.UEN=0x00;	//U1CTS,RTS�𖳌��ɂ���
//	U1MODEbits.UEN = 0x02;	//U1CTS,RTS��L���ɂ���
	
	_IOLOCK=1;			//pheriperal pin�ݒ�̂��߃��W�X�^�����b�N	
	uart1_initialize(BaudRate);
}
void HMR_uart1_finalize(void){
	uart1_finalize();
}

void HMR_uart2_initialize(uint16 BaudRate){
//_____________UART2(for CAMERA)_______________
	OSCCONbits.IOLOCK=0;		//pheriperal pin�ݒ�̂��߃��W�X�^�̃��b�N������
	RPINR19bits.U2RXR=98;		//RP98��UART2��M�s���ɐݒ�
	_RP101R=3;					//RP101��UART2���M�s���ɐݒ�


	OSCCONbits.IOLOCK=1;		//pheriperal pin�ݒ�̂��߃��W�X�^�����b�N	
	uart2_initialize(BaudRate);
}
//debug�p�@uart1��RF1�Əd������̂ŁC�ʏ�͎g���Ă͂����Ȃ�
/*
void HMR_uart3_initialize(uint16 BaudRate){
	//_____________UART2(for CAMERA)_______________
	OSCCONbits.IOLOCK = 0;		//pheriperal pin�ݒ�̂��߃��W�X�^�̃��b�N������

	//debug begin0
	RPINR27bits.U3RXR = 124;			//RP124��UART2��M�s���ɐݒ� U1RXR����M�s���ݒ背�W�X�^ 0x17��RPI23�̔ԍ�
	_RP125R = 0x1B;			//RP125��UART2���M�s���ɐݒ�
	U3MODEbits.UEN = 0x00;
	//debug end

	OSCCONbits.IOLOCK = 1;		//pheriperal pin�ݒ�̂��߃��W�X�^�����b�N	
	uart3_initialize(BaudRate); 
}
*/
void HMR_uart2_finalize(void){
	uart2_finalize();
}

void HMR_spi1_initialize(void){
	spi1_initialize(SPI_MASTER | SPI_L_TO_H | SPI_CLK_DIV_64|SPI_CLK_DIV2_5);	//MASTER, L to H(����), �N���b�N100Kbps��
}
void HMR_spi2_initialize(void) {
	spi2_initialize(SPI_MASTER | SPI_H_TO_L | SPI_CLK_DIV_64|SPI_CLK_DIV2_5);
}
void HMR_spi1_finalize(void) {}
void HMR_spi2_finalize(void) {}

#define HMR_I2C1_CLOCK I2C_CLOCK_1MHz
#define HMR_I2C2_CLOCK I2C_CLOCK_1MHz
void HMR_i2c_initialize(void){
	i2c1_initialize(HMR_SYSTEM_CLOCK,HMR_I2C1_CLOCK,0);
	//i2c2_initialize(HMR_SYSTEM_CLOCK,HMR_I2C2_CLOCK,0);
}
void HMR_i2c_finalize(void){
	i2c1_finalize();
	//i2c2_finalize();
}

//==============================AD�ϊ�==================================
void HMR_adc_initialize(void){
	setup_adc1_12bit(1);
	set_adc_sampleMode(EVERY);
	setup_adc1_time_auto();
	setup_adc1_Vref(Vref_GND);
}
void HMR_adc_finalize(void){}
//==============================Camera===================================
void HMR_PIN_CAM_PW_set(unsigned char val){
	if(val==0){
		HMR_PIN_CAM_SELECT0_TRIS_set(1);
		HMR_PIN_CAM_SELECT1_TRIS_set(1);
	}else{
		HMR_PIN_CAM_SELECT0_TRIS_set(0);
		HMR_PIN_CAM_SELECT1_TRIS_set(0);
	}
}
#
#endif
